#include "instruction.hpp"

#include <cstdio>
#include <windows.h>
#include <iomanip>
#include <iostream>
#include <string>

void instruction::print()
{
    if (name != "L.D"){
        printf("%d  %s   ", line_num, name.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        printf("%s", operand1.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        printf(",");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        printf("%s", operand2.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        printf(",");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        printf("%s", operand3.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        printf("\t");
    }
    else{
        printf("%d  %s     ", line_num, name.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        printf("%s", operand1.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        printf(",%s(", operand2.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        printf("%s", operand3.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        printf(")\t");
    }
    for (int i = 1; i <= state; ++i)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
        if (i == 2)
        {
            std::cout << state2_st;
            std::cout << std::right << std::setfill('_') << std::setw(8 - std::to_string(state2_st).length()) << state2_end;
            std::cout.fill(' ');
        }
        else if (i == 1)
        {
            std::cout << "___";
            std::cout << std::left << std::setfill('_') << std::setw(5) << state1_t;
            std::cout.fill(' ');
        }
        else if (i == 3)
        {
            std::cout << "___";
            std::cout << std::left << std::setfill('_') << std::setw(5) << state3_t;
            std::cout.fill(' ');
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        std::cout << std::right << "  ";
    }
    printf("\n");
}