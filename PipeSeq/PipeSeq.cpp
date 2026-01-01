
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <print>
#include <cstring>

#include <boost/algorithm/string.hpp>

#include "Windows.h"


std::vector<std::string> tokenize(std::string str, const std::string& sep = " ,")
{
    std::vector<std::string> ret;

    bool inquot = false;

    while (true)
    {
        if (str.empty()) break;
        auto p = str.find_first_of(sep);

        if (p == std::string::npos) {
            ret.push_back(str);
            break;
        }
        auto val = str.substr(0, p);
        if (!val.empty())
            ret.push_back(val);
        str = str.substr(p + 1);
    }

    return ret;
}

enum CommandType { UserInput, UserInputNE, FromFile, PipeStage, Execute, Mock, DumpError };

struct Command
{
    CommandType type;
    std::string arg1, arg2;

};

extern void dorun(const std::string& fromfile, const std::vector<std::string>& pipes);

int main(int argc, char** argv)
{
    std::ifstream ifs{ argv[1] };
    std::string line;
    std::vector<Command> commands;

    while (true)
    {
        if (!std::getline(ifs, line)) break;
        boost::trim(line);
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        std::vector<std::string> tok = tokenize(line, ";");
        if (tok.empty()) continue;

        if (tok[0] == "UserInput")
        {
            if (tok.size() != 3)
                std::cerr << "UserInput takes 2 arguments, prompt and alias\n";
            else
                commands.push_back({ UserInput, tok[1], tok[2] });
        }
        else if (tok[0] == "UserInputNE")
        {
            if (tok.size() != 3)
                std::cerr << "UserInputNE takes 2 arguments, prompt and alias\n";
            else
                commands.push_back({ UserInputNE, tok[1], tok[2] });
        }
        else if (tok[0] == "FromFile")
        {
            if (tok.size() != 2)
                std::cerr << "FromFile takes 1 argument, filename\n";
            else
                commands.push_back({ FromFile, tok[1] });
        }
        else if (tok[0] == "PipeStage")
        {
            if (tok.size() != 2)
                std::cerr << "PipeStage takes 1 argument, commandstring\n";
            else
                commands.push_back({ PipeStage, tok[1] });
        }
        else if (tok[0] == "Execute")
        {
            if (tok.size() != 1)
                std::cerr << "Execute takes 0 argument\n";
            else
                commands.push_back({ Execute });
        }
        else if (tok[0] == "Mock")
        {
            if (tok.size() != 1)
                std::cerr << "Mock takes 0 argument\n";
            else
                commands.push_back({ Mock });
        }
        else if (tok[0] == "DumpError")
        {
            if (tok.size() != 1)
                std::cerr << "DumpError takes 0 argument\n";
            else
                commands.push_back({ DumpError });
        }
        else
        {
            std::cerr << "unrecognized command; " << tok[0] << "\n";
        }
    }

    std::string fromfile;
    std::vector<std::string> pipes;
    std::vector<std::pair<std::string, std::string>> aliases;

    auto replace = [&aliases](std::string ss) -> std::string
        {
            bool did;
            while (true)
            {
                did = false;
                for (auto [s, r] : aliases)
                {
                    auto p = ss.find(s);
                    if (p != std::string::npos)
                    {
                        ss.replace(p, s.length(), r);
                        did = true;
                    }
                }
                if (!did) return ss;
            }
        };

    auto domock = [&]() -> void
        {
            std::string ss;
            if (!fromfile.empty())
            {
                ss += "cat " + fromfile;
            }
            for (auto&& p : pipes)
            {
                if (!ss.empty())
                    ss += " | ";
                ss += p;
            }
            std::cout << ss << std::endl;
        };

    HANDLE hStdin;
    DWORD mode;

    for (auto&& cmd : commands)
    {
        switch (cmd.type)
        {
        case UserInput:
            std::cout << replace(cmd.arg1);
            std::cin >> line;
            aliases.push_back({ cmd.arg2,line });
            break;
        case UserInputNE:
            std::cout << replace(cmd.arg1);

            hStdin = GetStdHandle(STD_INPUT_HANDLE);
            mode = 0;
            GetConsoleMode(hStdin, &mode);
            SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
            std::cin >> line;
            SetConsoleMode(hStdin, mode);
            aliases.push_back({ cmd.arg2,line });
            break;
        case FromFile:
            fromfile = replace(cmd.arg1);
            break;
        case PipeStage:
            pipes.push_back(replace(cmd.arg1));
            break;
        case Execute:
            dorun(fromfile, pipes);
            break;
        case Mock:
            domock();
            break;
        case DumpError:
            std::cerr << "error\n";
            break;
        }

    }
}

struct WinPipe
{
    HANDLE read;
    HANDLE write;
};

struct WinProc
{
    PROCESS_INFORMATION pi;
    HANDLE std_in, std_out, std_err;
    struct 
    {
        bool std_in, std_out, std_err;
    } ispipe;
    bool running;
};

void dorun(const std::string& fromfile, const std::vector<std::string>& pipes)
{
    HFILE of = 0;
    OFSTRUCT ofs; 
    std::memset(&ofs, 0, sizeof(OFSTRUCT));
    ofs.cBytes = sizeof(OFSTRUCT);

    if (!fromfile.empty())
    {
        of = OpenFile(fromfile.c_str(), &ofs, OF_READ);
    }

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;
    sa.lpSecurityDescriptor = 0;

    std::vector<WinPipe> win_pipes;
    int i,n = std::ssize(pipes) - 1;
    for (i = 0; i < n; ++i) {
        WinPipe wp;
        CreatePipe(&wp.read, &wp.write, &sa, 0);
        win_pipes.push_back(wp);
    }

    sa.bInheritHandle = TRUE;

    WinProc wp;
    ZeroMemory(&wp, sizeof(WinProc));

    wp.ispipe.std_err = false;
    wp.std_err = GetStdHandle(STD_ERROR_HANDLE);

    std::vector<WinProc> pis;
    for (i = 0; auto&& p : pipes)
    {
        if (i == 0)
        {
            if (of)
                wp.std_in = (HANDLE)of;
            else
                wp.std_in = GetStdHandle(STD_INPUT_HANDLE);
            wp.ispipe.std_in = false;
        }
        else {
            wp.std_in = win_pipes[i - 1].read;
            wp.ispipe.std_in = true;
        }

        if (i == n)
        {
            wp.std_out = GetStdHandle(STD_OUTPUT_HANDLE);
            wp.ispipe.std_out = false;
        }
        else {
            wp.std_out = win_pipes[i].write;
            wp.ispipe.std_out = true;
        }

        std::string fn = p;

        STARTUPINFOA si;
        memset(&si, 0, sizeof(STARTUPINFOA));
        si.cb = sizeof(STARTUPINFOA);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = wp.std_in;
        si.hStdOutput = wp.std_out;
        si.hStdError = wp.std_err;

        if (wp.ispipe.std_in)
            SetHandleInformation(wp.std_in, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
        if (wp.ispipe.std_out)
            SetHandleInformation(wp.std_out, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

        PROCESS_INFORMATION pi;
        memset(&pi, 0, sizeof(PROCESS_INFORMATION));

        BOOL ok = CreateProcessA(0, fn.data(), &sa, &sa, TRUE, NORMAL_PRIORITY_CLASS, 0, 0, &si, &wp.pi);

        //std::cout << "created " << fn << (ok ? " ok" : " failed") << std::endl;

        wp.running = true;

        if (wp.ispipe.std_in)
            SetHandleInformation(wp.std_in, HANDLE_FLAG_INHERIT, 0);
        if (wp.ispipe.std_out)
            SetHandleInformation(wp.std_out, HANDLE_FLAG_INHERIT, 0);


        pis.push_back(wp);

        ++i;
    }

    while (true)
    {
        bool all = true;
        int i, n = std::ssize(pis);
        for (i=0; i<n; ++i)
        {
            auto& pi = pis[i];
            if (pi.running ) switch (WaitForSingleObject(pi.pi.hProcess, 0))
            {
            case WAIT_OBJECT_0:
                // process has terminated...
                pi.running = false;
                //std::cout << "process " << i << " has terminated";
                if (i < std::ssize(win_pipes)) {
                    auto& wp = win_pipes[i];
                    if (wp.write) {
                        //std::cout << ", closing pipe";
                        CloseHandle(wp.write);
                        wp.write = 0;
                    }
                }
                std::cout << "\n";
                break;

            case WAIT_TIMEOUT:
            default:
                // process is still running...
                all = false;
                break;
            }
        }
        if (all) break;
        Sleep(150);
    }

    // all has terminated

    for (auto&& wp : win_pipes)
    {
        if (wp.read) CloseHandle(wp.read);
        if (wp.write) CloseHandle(wp.write);
    }

    if (of)
        CloseHandle((HANDLE)of);


    std::cout << "\nAll done.\n";
}
