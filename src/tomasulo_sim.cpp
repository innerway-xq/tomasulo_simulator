#include "tomasulo_sim.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <iomanip>

using namespace std;

extern int line_reg_n = 16;
extern const int wid_reg = 6;

tomasulo_sim::tomasulo_sim()
{
    rvs.resize(ld_n + add_sub_n + mul_div_n, reservation_station());
    reg.resize(reg_f_n + reg_r_n, register_station());
    cdb.rvs_ava.resize(ld_n + add_sub_n + mul_div_n, false);
    cdb.result.resize(ld_n + add_sub_n + mul_div_n);
    cdb.itn_num.resize(ld_n + add_sub_n + mul_div_n, 0);
}

void tomasulo_sim::reset()
{
    iq.clear();
    rvs.resize(ld_n + add_sub_n + mul_div_n, reservation_station());
    reg.resize(reg_f_n + reg_r_n, register_station());
    cdb.rvs_ava.resize(ld_n + add_sub_n + mul_div_n, false);
    cdb.result.resize(ld_n + add_sub_n + mul_div_n);
    cdb.itn_num.resize(ld_n + add_sub_n + mul_div_n, 0);
    memset(mem, 0, sizeof(mem));
    pc = 0;
    t = 0;
}

bool tomasulo_sim::load_instructions(const char *filename)
{
    ifstream fin;
    fin.open(filename, ios::in);
    if (!fin)
    {
        cout << "open " << filename << " failed" << endl;
        return false;
    }
    string temp;
    for (;;)
    {
        fin >> temp;
        if (fin.eof())
            break;
        iq.push_back(instruction());
        vector<instruction>::iterator it;
        it = iq.end() - 1;
        it->line_num = strtol(temp.c_str(), NULL, 10);
        fin >> temp;
        it->name = temp;
        fin >> temp;
        it->operand1 = strtok((char *)temp.c_str(), ",");
        if (it->name != "L.D")
        {
            it->operand2 = strtok(NULL, ",");
            it->operand3 = strtok(NULL, "\n");
        }
        else
        {
            it->operand2 = strtok(NULL, "(");
            it->operand3 = strtok(NULL, ")");
        }
    }
    fin.close();
    return true;
}

void tomasulo_sim::print_instructions()
{
    string tmp[4] = {"instructions", "issue", "execute", "write"};
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0a);
    printf("   %s\t\t%s     %s   %s\n", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str(), tmp[3].c_str());
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
    for (instruction i : iq)
    {
        i.print();
    }
}

void tomasulo_sim::print_reversation_stations()
{
    string tmp[9] = {"name", "t", "OP", "Vj", "Vk", "Qj", "Qk", "A", "Busy"};
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0a);
    for (int i = 0; i < 9; ++i)
    {
        printf("%s\t", tmp[i].c_str());
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
    printf("\n");
    for (int r = 0; r < (ld_n + add_sub_n + mul_div_n); ++r)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
        if (r >= 0 && r < ld_n)
        {
            printf("Load%d\t", r + 1);
        }
        else if (r >= ld_n && r < (ld_n + add_sub_n))
        {
            printf("ADD%d\t", r + 1 - ld_n);
        }
        else if (r >= (ld_n + add_sub_n) && r < (ld_n + add_sub_n + mul_div_n))
        {
            printf("MULT%d\t", r + 1 - ld_n - add_sub_n);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);

        rvs[r].print(ld_n, add_sub_n, mul_div_n);
    }
}

void tomasulo_sim::print_registers()
{
    for (int k = 0; k < reg_r_n / line_reg_n; k++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        for (int i = k * line_reg_n; i < (k + 1) * line_reg_n; i++)
        {
            char tmp[10];
            sprintf(tmp, "R%d\0", i);
            cout << setw(wid_reg) << tmp;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        printf("\n");
        for (int i = k * line_reg_n; i < (k + 1) * line_reg_n; i++)
        {
            char tmp[10];
            if (!reg[i].qi)
            {
                cout << setw(wid_reg) << reg[i].value.rvalue;
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
                if (reg[i].qi > 0 && reg[i].qi <= ld_n)
                {
                    sprintf(tmp, "Ld%d", reg[i].qi);
                    cout << setw(wid_reg) << tmp;
                }
                else if (reg[i].qi > ld_n && reg[i].qi <= (ld_n + add_sub_n))
                {
                    sprintf(tmp, "ADD%d", reg[i].qi - ld_n);
                    cout << setw(wid_reg) << tmp;
                }
                else if (reg[i].qi > (ld_n + add_sub_n) && reg[i].qi <= (ld_n + add_sub_n + mul_div_n))
                {
                    sprintf(tmp, "MUL%d", reg[i].qi - ld_n - add_sub_n);
                    cout << setw(4) << tmp;
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
            }
        }
        printf("\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
    for (int i = (reg_r_n / line_reg_n) * line_reg_n; i < reg_r_n; i++)
    {
        char tmp[10];
        sprintf(tmp, "R%d\0", i);
        cout << setw(wid_reg) << tmp;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
    if (reg_r_n % line_reg_n)
        printf("\n");
    for (int i = int(reg_r_n / line_reg_n) * line_reg_n; i < reg_r_n; i++)
    {
        char tmp[10];
        if (!reg[i].qi)
        {
            cout << setw(wid_reg) << reg[i].value.rvalue;
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
            if (reg[i].qi > 0 && reg[i].qi <= ld_n)
            {
                sprintf(tmp, "Ld%d", reg[i].qi);
                cout << setw(wid_reg) << tmp;
            }
            else if (reg[i].qi > ld_n && reg[i].qi <= (ld_n + add_sub_n))
            {
                sprintf(tmp, "ADD%d", reg[i].qi - ld_n);
                cout << setw(wid_reg) << tmp;
            }
            else if (reg[i].qi > (ld_n + add_sub_n) && reg[i].qi <= (ld_n + add_sub_n + mul_div_n))
            {
                sprintf(tmp, "MUL%d", reg[i].qi - ld_n - add_sub_n);
                cout << setw(4) << tmp;
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        }
    }
    if (reg_r_n % line_reg_n)
        printf("\n");

    line_reg_n *= 2;

    for (int k = 0; k < reg_f_n / line_reg_n; k++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        for (int i = k * line_reg_n; i < (k + 1) * line_reg_n; i += 2)
        {
            char tmp[10];
            sprintf(tmp, "F%d\0", i);
            cout << setw(wid_reg) << tmp;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        printf("\n");
        for (int i = reg_r_n + k * line_reg_n; i < reg_r_n + (k + 1) * line_reg_n; i += 2)
        {
            char tmp[10];
            if (!reg[i].qi)
            {
                cout << setw(wid_reg) << reg[i].value.fvalue;
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
                if (reg[i].qi > 0 && reg[i].qi <= ld_n)
                {
                    sprintf(tmp, "Ld%d", reg[i].qi);
                    cout << setw(wid_reg) << tmp;
                }
                else if (reg[i].qi > ld_n && reg[i].qi <= (ld_n + add_sub_n))
                {
                    sprintf(tmp, "ADD%d", reg[i].qi - ld_n);
                    cout << setw(wid_reg) << tmp;
                }
                else if (reg[i].qi > (ld_n + add_sub_n) && reg[i].qi <= (ld_n + add_sub_n + mul_div_n))
                {
                    sprintf(tmp, "MUL%d", reg[i].qi - ld_n - add_sub_n);
                    cout << setw(wid_reg) << tmp;
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
            }
        }
        printf("\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
    for (int i = int(reg_f_n / line_reg_n) * line_reg_n; i < reg_f_n; i += 2)
    {
        char tmp[10];
        sprintf(tmp, "F%d\0", i);
        cout << setw(wid_reg) << tmp;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
    if (reg_f_n % line_reg_n)
        printf("\n");
    for (int i = reg_r_n + int(reg_f_n / line_reg_n) * line_reg_n; i < reg_r_n + reg_f_n; i += 2)
    {
        char tmp[10];
        if (!reg[i].qi)
        {
            cout << setw(wid_reg) << reg[i].value.fvalue;
        }
        else
        {
            if (reg[i].qi > 0 && reg[i].qi <= ld_n)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
                sprintf(tmp, "Ld%d", reg[i].qi);
                cout << setw(wid_reg) << tmp;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
            }
            else if (reg[i].qi > ld_n && reg[i].qi <= (ld_n + add_sub_n))
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
                sprintf(tmp, "ADD%d", reg[i].qi - ld_n);
                cout << setw(wid_reg) << tmp;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
            }
            else if (reg[i].qi > (ld_n + add_sub_n) && reg[i].qi <= (ld_n + add_sub_n + mul_div_n))
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
                sprintf(tmp, "MUL%d", reg[i].qi - ld_n - add_sub_n);
                cout << setw(wid_reg) << tmp;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
            }
        }
    }
    if (reg_f_n % line_reg_n)
        printf("\n");
    line_reg_n /= 2;
}

void tomasulo_sim::show_parameters()
{
    string head[3] = {"instruction", "delay", "num"};
    string ins_tmp[5] = {"L.D", "ADD.D", "SUB.D", "MULT.D", "DIV.D"};
    printf("%s\t%s\t%s\n", head[0].c_str(), head[1].c_str(), head[2].c_str());
    printf("%s\t\t%d\t%d\t\n", ins_tmp[0].c_str(), ld_t, ld_n);
    printf("%s\t\t%d\t%d\t\n", ins_tmp[1].c_str(), add_t, add_sub_n);
    printf("%s\t\t%d\t%d\t\n", ins_tmp[2].c_str(), sub_t, add_sub_n);
    printf("%s\t\t%d\t%d\t\n", ins_tmp[3].c_str(), mul_t, mul_div_n);
    printf("%s\t\t%d\t%d\t\n", ins_tmp[4].c_str(), div_t, mul_div_n);
}

void tomasulo_sim::show()
{
    string dividing_line = "-------------------------------------------------------------------------------------------------\n";
    printf("clock: %d\n", t);
    printf("%s", dividing_line.c_str());
    show_parameters();
    printf("%s", dividing_line.c_str());
    print_registers();
    printf("%s", dividing_line.c_str());
    print_instructions();
    printf("%s", dividing_line.c_str());
    print_reversation_stations();
    printf("%s", dividing_line.c_str());
    return;
}

int tomasulo_sim::issue()
{
    if (pc >= iq.size())
        return -1;
    if (iq[pc].name == "L.D")
    {
        int r = 0;
        for (; r < ld_n; ++r)
        {
            if (!rvs[r].busy)
                break;
        }
        if (r == ld_n)
            return -1;
        rvs[r].op = iq[pc].name;
        int rd = strtol(iq[pc].operand1.substr(1).c_str(), NULL, 10);
        if (iq[pc].operand1[0] == 'F')
            rd += 32;
        long long imm = strtol(iq[pc].operand2.c_str(), NULL, 10);
        int rs = strtol(iq[pc].operand3.substr(1).c_str(), NULL, 10);
        if (iq[pc].operand3[0] == 'F')
            rs += 32;
        if (reg[rs].qi)
        {
            rvs[r].qj = reg[rs].qi;
        }
        else
        {
            if (r < 32)
                rvs[r].vj.rvalue = reg[rs].value.rvalue;
            else
                rvs[r].vj.fvalue = reg[rs].value.fvalue;
            rvs[r].qj = 0;
        }
        reg[rd].qi = r + 1;
        rvs[r].a = imm;
        rvs[r].t = ld_t;
        rvs[r].itn_num = pc;
        iq[rvs[r].itn_num].state = 1;
        iq[rvs[r].itn_num].state1_t = t;
        pc++;
        return r;
    }
    else if (iq[pc].name == "ADD.D" || iq[pc].name == "SUB.D")
    {
        int r = ld_n;
        for (; r < ld_n + add_sub_n; ++r)
        {
            if (!rvs[r].busy)
                break;
        }
        if (r == ld_n + add_sub_n)
            return -1;
        rvs[r].op = iq[pc].name;
        int rd = strtol(iq[pc].operand1.substr(1).c_str(), NULL, 10);
        if (iq[pc].operand1[0] == 'F')
            rd += 32;
        int rs = strtol(iq[pc].operand2.substr(1).c_str(), NULL, 10);
        if (iq[pc].operand2[0] == 'F')
            rs += 32;
        int rt = strtol(iq[pc].operand3.substr(1).c_str(), NULL, 10);
        if (iq[pc].operand3[0] == 'F')
            rt += 32;

        if (reg[rs].qi)
        {
            rvs[r].qj = reg[rs].qi;
        }
        else
        {
            if (r < 32)
                rvs[r].vj.rvalue = reg[rs].value.rvalue;
            else
                rvs[r].vj.fvalue = reg[rs].value.fvalue;
            rvs[r].qj = 0;
        }
        if (reg[rt].qi)
        {
            rvs[r].qk = reg[rt].qi;
        }
        else
        {
            if (r < 32)
                rvs[r].vk.rvalue = reg[rt].value.rvalue;
            else
                rvs[r].vk.fvalue = reg[rt].value.fvalue;
            rvs[r].qk = 0;
        }
        reg[rd].qi = r + 1;
        rvs[r].t = (iq[pc].name == "ADD.D") ? add_t : sub_t;
        rvs[r].itn_num = pc;
        iq[rvs[r].itn_num].state = 1;
        iq[rvs[r].itn_num].state1_t = t;
        pc++;
        return r;
    }
    else if (iq[pc].name == "MUL.D" || iq[pc].name == "DIV.D")
    {
        int r = ld_n + add_sub_n;
        for (; r < ld_n + add_sub_n + mul_div_n; ++r)
        {
            if (!rvs[r].busy)
                break;
        }
        if (r == ld_n + add_sub_n + mul_div_n)
            return -1;
        rvs[r].op = iq[pc].name;
        int rd = strtol(iq[pc].operand1.substr(1).c_str(), NULL, 10);
        if (iq[pc].operand1[0] == 'F')
            rd += 32;
        int rs = strtol(iq[pc].operand2.substr(1).c_str(), NULL, 10);
        if (iq[pc].operand2[0] == 'F')
            rs += 32;
        int rt = strtol(iq[pc].operand3.substr(1).c_str(), NULL, 10);
        if (iq[pc].operand3[0] == 'F')
            rt += 32;

        if (reg[rs].qi)
        {
            rvs[r].qj = reg[rs].qi;
        }
        else
        {
            if (r < 32)
                rvs[r].vj.rvalue = reg[rs].value.rvalue;
            else
                rvs[r].vj.fvalue = reg[rs].value.fvalue;
            rvs[r].qj = 0;
        }
        if (reg[rt].qi)
        {
            rvs[r].qk = reg[rt].qi;
        }
        else
        {
            if (r < 32)
                rvs[r].vk.rvalue = reg[rt].value.rvalue;
            else
                rvs[r].vk.fvalue = reg[rt].value.fvalue;
            rvs[r].qk = 0;
        }

        reg[rd].qi = r + 1;
        rvs[r].t = (iq[pc].name == "MUL.D") ? mul_t : div_t;
        rvs[r].itn_num = pc;
        iq[rvs[r].itn_num].state = 1;
        iq[rvs[r].itn_num].state1_t = t;
        pc++;
        return r;
    }
}

vector<int> tomasulo_sim::execute()
{
    vector<int> ret;
    for (int r = 0; r < ld_n; ++r)
    {
        if (!rvs[r].qj && rvs[r].busy && rvs[r].t > 0)
        {
            rvs[r].t--;
            if (rvs[r].t == ld_t - 1)
            {
                rvs[r].a = rvs[r].vj.rvalue + rvs[r].a;
                iq[rvs[r].itn_num].state2_st = t;
            }
            else if (!rvs[r].t)
            {
                ret.push_back(r);
                cdb.itn_num[r] = rvs[r].itn_num;
                cdb.result[r] = mem[rvs[r].a];
                iq[rvs[r].itn_num].state = 2;
                iq[rvs[r].itn_num].state2_end = t;
            }
        }
    }
    for (int r = ld_n; r < ld_n + add_sub_n + mul_div_n; ++r)
    {
        if (!rvs[r].qj && !rvs[r].qk && rvs[r].busy && rvs[r].t > 0)
        {
            rvs[r].t--;
            if (rvs[r].op == "ADD.D" && rvs[r].t == add_t - 1)
            {
                iq[rvs[r].itn_num].state2_st = t;
            }
            else if (rvs[r].op == "SUB.D" && rvs[r].t == sub_t - 1)
            {
                iq[rvs[r].itn_num].state2_st = t;
            }
            else if (rvs[r].op == "MUL.D" && rvs[r].t == mul_t - 1)
            {
                iq[rvs[r].itn_num].state2_st = t;
            }
            else if (rvs[r].op == "DIV.D" && rvs[r].t == div_t - 1)
            {
                iq[rvs[r].itn_num].state2_st = t;
            }
            if (!rvs[r].t)
            {
                iq[rvs[r].itn_num].state2_end = t;
                ret.push_back(r);
                cdb.itn_num[r] = rvs[r].itn_num;
                iq[rvs[r].itn_num].state = 2;
                if (rvs[r].op == "ADD.D")
                {
                    cdb.result[r].fvalue = rvs[r].vj.fvalue + rvs[r].vk.fvalue;
                }
                else if (rvs[r].op == "SUB.D")
                {
                    cdb.result[r].fvalue = rvs[r].vj.fvalue - rvs[r].vk.fvalue;
                }
                else if (rvs[r].op == "MUL.D")
                {
                    cdb.result[r].fvalue = rvs[r].vj.fvalue * rvs[r].vk.fvalue;
                }
                else if (rvs[r].op == "DIV.D")
                {
                    cdb.result[r].fvalue = rvs[r].vj.fvalue / rvs[r].vk.fvalue;
                }
            }
        }
    }
    return ret;
}

void tomasulo_sim::write_result()
{
    for (int r = 0; r < ld_n + add_sub_n + mul_div_n; ++r)
    {
        if (cdb.rvs_ava[r])
        {
            cdb.rvs_ava[r] = false;
            iq[cdb.itn_num[r]].state = 3;
            iq[cdb.itn_num[r]].state3_t = t;
            for (int i = 0; i < reg_f_n + reg_r_n; i++)
            {
                if (reg[i].qi - 1 == r)
                {
                    reg[i].value = cdb.result[r];
                    reg[i].qi = 0;
                }
            }
            for (int i = 0; i < ld_n + add_sub_n + mul_div_n; i++)
            {
                if (rvs[i].qj - 1 == r)
                {
                    rvs[i].vj = cdb.result[r];
                    rvs[i].qj = 0;
                }
                if (rvs[i].qk - 1 == r)
                {
                    rvs[i].vk = cdb.result[r];
                    rvs[i].qk = 0;
                }
            }
            rvs[r].clear();
        }
    }
}

bool tomasulo_sim::step()
{
    if (done_flag)
        return false;
    t++;
    int issue_r;
    vector<int> execute_r;
    issue_r = issue();
    // cout << "issued" << endl;
    execute_r = execute();
    // cout << "executed" << endl;
    write_result();
    // cout << "writed" << endl;
    if (issue_r != -1)
        rvs[issue_r].busy = true;
    for (int i : execute_r)
    {
        cdb.rvs_ava[i] = true;
    }
    for (instruction i : iq)
    {
        if (i.state != 3)
            return true;
    }
    return false;
}
