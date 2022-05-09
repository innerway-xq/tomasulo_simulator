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
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        printf("%s", operand1.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        printf(",");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        printf("%s", operand2.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        printf(",");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        printf("%s", operand3.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        printf("\t");
    }
    else{
        printf("%d  %s     ", line_num, name.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        printf("%s", operand1.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        printf(",%s(", operand2.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        printf("%s", operand3.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        printf(")\t");
    }
    for (int i = 1; i <= state; ++i)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
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

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        std::cout << std::right << "  ";
    }
    printf("\n");
}