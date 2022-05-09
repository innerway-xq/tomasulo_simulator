#include "reservation_station.hpp"

#include <windows.h>

void reservation_station::print(int ld_n, int add_sub_n, int mul_div_n)
{
    if (op != "L.D")
    {
        printf("%d\t%s\t%.3lf\t%.3lf\t", t, op.c_str(), vj.fvalue, vk.fvalue);
        if (qj)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
            if (qj > 0 && qj <= ld_n)
            {
                printf("Load%d\t", qj);
            }
            else if (qj > ld_n && qj <= (ld_n + add_sub_n))
            {
                printf("ADD%d\t", qj - ld_n);
            }
            else if (qj > (ld_n + add_sub_n) && qj <= (ld_n + add_sub_n + mul_div_n))
            {
                printf("MULT%d\t", qj - ld_n - add_sub_n);
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        }
        else
            printf("%d\t", qj);
        if (qk)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
            if (qk > 0 && qk <= ld_n)
            {
                printf("Load%d\t", qk);
            }
            else if (qk > ld_n && qk <= (ld_n + add_sub_n))
            {
                printf("ADD%d\t", qk - ld_n);
            }
            else if (qk > (ld_n + add_sub_n) && qk <= (ld_n + add_sub_n + mul_div_n))
            {
                printf("MULT%d\t", qk - ld_n - add_sub_n);
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        }
        else
            printf("%d\t", qk);
        printf("%d\t", a);
    }
    else
    {
        printf("%d\t%s\t%d\t%d\t", t, op.c_str(), vj.rvalue, vk.rvalue);
        if (qj)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
            if (qj > 0 && qj <= ld_n)
            {
                printf("Load%d\t", qj);
            }
            else if (qj > ld_n && qj <= (ld_n + add_sub_n))
            {
                printf("ADD%d\t", qj - ld_n);
            }
            else if (qj > (ld_n + add_sub_n) && qj <= (ld_n + add_sub_n + mul_div_n))
            {
                printf("MULT%d\t", qj - ld_n - add_sub_n);
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        }
        else
            printf("%d\t", qj);
        if (qk)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
            if (qk > 0 && qk <= ld_n)
            {
                printf("Load%d\t", qk);
            }
            else if (qk > ld_n && qk <= (ld_n + add_sub_n))
            {
                printf("ADD%d\t", qk - ld_n);
            }
            else if (qk > (ld_n + add_sub_n) && qk <= (ld_n + add_sub_n + mul_div_n))
            {
                printf("MULT%d\t", qk - ld_n - add_sub_n);
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        }
        else
            printf("%d\t", qk);
        printf("%d\t", a);
    }
    if (busy)
        printf("true\n");
    else
        printf("false\n");
}
void reservation_station::clear()
{
    op = "";
    qj = qk = 0;
    vj.rvalue = vk.rvalue = 0;
    busy = false;
    a = 0;
    t = 0;
    itn_num = 0;
}