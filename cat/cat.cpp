// cat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

#include <fcntl.h>
#include <io.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    using namespace std::literals;

    std::cout.setf(std::ios_base::binary);
    _setmode(_fileno(stdout), _O_BINARY);

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == "--version"s)
        {
            std::cerr << "ver 1.0.03\n";
            continue;
        }
        std::ifstream ifs{ argv[i], std::ios_base::binary };
        std::cerr << "cat running\n";
        char c;
        while (!ifs.eof())
        {
            ifs.read(&c, 1);
            auto cnt = ifs.gcount();
            if (cnt == 1) {
                std::cout.write(&c, 1);
                std::cout.flush();
            }
        }
    }
}

