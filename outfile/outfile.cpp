
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include <fcntl.h>
#include <io.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    using namespace std::literals;

    std::this_thread::sleep_for(1s);

    if (argc!=2)
    {
        std::cerr << "error\n";
        return 0;
    }

    if (argv[1] == "--version"s)
    {
        std::cerr << "ver 1.0.03\n";
        return 0;
    }

    std::cin.setf(std::ios_base::binary);
    _setmode(_fileno(stdin), _O_BINARY);

    std::cerr << "outfile running\n";

    std::ofstream ofs{ argv[1], std::ios_base::binary };
    std::byte buff[128];

    while (!std::cin.eof())
    {
        //std::cin.eof
        std::cin.read((char*)buff, 128);
        auto cnt = std::cin.gcount();
        if (cnt > 0) {
            ofs.write((char*)buff, cnt);
            ofs.flush();
        }
    }

}


