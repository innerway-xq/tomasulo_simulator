#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include "tomasulo_sim.hpp"

using namespace std;

// 处理文件路径
void parse_path(string &x)
{
    for (int i = 0; i < x.size(); ++i)
    {
        if (x[i] == '/')
        {
            x[i] = '\\';
        }
    }
    if (x[0] != '.' || x[0] != '\\')
    {
        x = ".\\" + x;
    }
    if (x[x.size() - 1] == '\\')
        x[x.size() - 1] = '\0';
    return;
}

int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);

    tomasulo_sim ts;

    ts.reg[2].value.rvalue = 5;
    ts.reg[3].value.rvalue = 10;
    ts.mem[39].fvalue = 7.7;
    ts.mem[55].fvalue = 2.2;

    for (;;)
    {
        cout << "xqts>";
        string usr_op;
        char usr_argv[4][10];
        int usr_argc = 0;
        getline(cin, usr_op);
        if (usr_op.empty())
        {
            if (ts.iq.size() == 0)
            {
                cout << "no instructions! please load instructions." << endl;
                continue;
            }
            cin.sync();
            if (ts.step())
                ts.show();
            else
            {
                ts.show();
                ts.done_flag = true;
                cout << "DONE!" << endl;
            }
        }
        else
        {
            stringstream s(usr_op);
            while (s >> usr_argv[usr_argc])
                usr_argc++;

            if (!strcmp(usr_argv[0], "h") || !strcmp(usr_argv[0], "help"))
            {
                printf("xqts - tomasulo simulator by xq\n"
                       "Usage:\n"
                       " *only enter                     step by step\n\n"
                       "  [OPTION...]\n"
                       "  l, load [file]                 load instructions\n"
                       "  reset                          reset everything\n"
                       "  s, step                        step by step\n"
                       "  r, run                         run (pause when any keyboard hit)\n"
                       "  save [file]                    save to file\n"
                       "  set <reg/mem> [position] [value]\n"
                       "                                 set memory/register\n"
                       "  c, change [instruction] <delay/num> [integer]\n"
                       "                                 change parameters\n"
                       "  q,quit                         quit\n"
                       "  h, help                        print help\n");
            }
            else if (!strcmp(usr_argv[0], "l") || !strcmp(usr_argv[0], "load"))
            {
                string tmp = usr_argv[1];
                parse_path(tmp);
                if (ts.load_instructions(tmp.c_str()))
                    ts.show();
            }
            else if (!strcmp(usr_argv[0], "s") || !strcmp(usr_argv[0], "step"))
            {
                if (ts.iq.size() == 0)
                {
                    cout << "no instructions! please load instructions." << endl;
                    continue;
                }
                if (ts.step())
                    ts.show();
                else
                {
                    ts.show();
                    ts.done_flag = true;
                    cout << "DONE!" << endl;
                }
            }
            else if (!strcmp(usr_argv[0], "r") || !strcmp(usr_argv[0], "run"))
            {   
                if (ts.iq.size() == 0)
                {
                    cout << "no instructions! please load instructions." << endl;
                    continue;
                }
                ts.show();
                _sleep(1000);
                for (;;)
                {
                    if(kbhit()) break;
                    if (ts.iq.size() == 0)
                    {
                        cout << "no instructions! please load instructions." << endl;
                        continue;
                    }
                    if (ts.step())
                        ts.show();
                    else
                    {
                        ts.show();
                        ts.done_flag = true;
                        cout << "DONE!" << endl;
                        break;
                    }
                    _sleep(1000);
                }
            }
            else if (!strcmp(usr_argv[0], "q") || !strcmp(usr_argv[0], "quit"))
            {
                return 0;
            }
            else if (!strcmp(usr_argv[0], "save"))
            {
                string tmp = usr_argv[1];
                parse_path(tmp);
                FILE *fp = freopen(tmp.c_str(), "w", stdout);
                instructions_state_color = 0x0f;
                ts.show();
                instructions_state_color = 0xf0;
                fclose(fp);
                freopen("CON", "w", stdout);
            }
            else if (!strcmp(usr_argv[0], "reset"))
            {
                ts.reset();
                ts.show();
            }
            else if (!strcmp(usr_argv[0], "c") || !strcmp(usr_argv[0], "change"))
            {
                if (!strcmp(usr_argv[1], "L.D"))
                {
                    if (!strcmp(usr_argv[2], "delay"))
                    {
                        ts.ld_t = strtol(usr_argv[3], NULL, 10);
                    }
                    else if (!strcmp(usr_argv[2], "num"))
                    {
                        ts.ld_n = strtol(usr_argv[3], NULL, 10);
                        ts.rvs.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.rvs_ava.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, false);
                        ts.cdb.result.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.itn_num.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, 0);
                    }
                }
                else if (!strcmp(usr_argv[1], "ADD.D"))
                {
                    if (!strcmp(usr_argv[2], "delay"))
                    {
                        ts.add_t = strtol(usr_argv[3], NULL, 10);
                    }
                    else if (!strcmp(usr_argv[2], "num"))
                    {
                        ts.add_sub_n = strtol(usr_argv[3], NULL, 10);
                        ts.rvs.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.rvs_ava.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, false);
                        ts.cdb.result.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.itn_num.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, 0);
                    }
                }
                else if (!strcmp(usr_argv[1], "SUB.D"))
                {
                    if (!strcmp(usr_argv[2], "delay"))
                    {
                        ts.sub_t = strtol(usr_argv[3], NULL, 10);
                    }
                    else if (!strcmp(usr_argv[2], "num"))
                    {
                        ts.add_sub_n = strtol(usr_argv[3], NULL, 10);
                        ts.rvs.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.rvs_ava.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, false);
                        ts.cdb.result.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.itn_num.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, 0);
                    }
                }
                else if ((!strcmp(usr_argv[1], "MULT.D")) || (!strcmp(usr_argv[1], "MUL.D")))
                {
                    if (!strcmp(usr_argv[2], "delay"))
                    {
                        ts.mul_t = strtol(usr_argv[3], NULL, 10);
                    }
                    else if (!strcmp(usr_argv[2], "num"))
                    {
                        ts.mul_div_n = strtol(usr_argv[3], NULL, 10);
                        ts.rvs.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.rvs_ava.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, false);
                        ts.cdb.result.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.itn_num.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, 0);
                    }
                }
                else if (!strcmp(usr_argv[1], "DIV.D"))
                {
                    if (!strcmp(usr_argv[2], "delay"))
                    {
                        ts.div_t = strtol(usr_argv[3], NULL, 10);
                    }
                    else if (!strcmp(usr_argv[2], "num"))
                    {
                        ts.mul_div_n = strtol(usr_argv[3], NULL, 10);
                        ts.rvs.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.rvs_ava.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, false);
                        ts.cdb.result.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n);
                        ts.cdb.itn_num.resize(ts.ld_n + ts.add_sub_n + ts.mul_div_n, 0);
                    }
                }
                ts.show_parameters();
            }
            else if (!strcmp(usr_argv[0], "set")){
                if (!strcmp(usr_argv[1], "reg")){
                    int reg_tmp;
                    if(usr_argv[2][0] == 'R'){
                        reg_tmp = strtol(usr_argv[2]+1,NULL,10);
                        if(reg_tmp >= ts.reg_r_n){
                            cout<<"no such register"<<endl;
                            continue;
                        }
                        ts.reg[reg_tmp].value.rvalue = strtoll(usr_argv[3],NULL,10);
                    }
                    else if (usr_argv[2][0] == 'F'){
                        reg_tmp = strtol(usr_argv[2] + 1, NULL, 10) + ts.reg_r_n;
                        if (reg_tmp >= ts.reg_r_n + ts.reg_f_n)
                        {
                            cout << "no such register" << endl;
                            continue;
                        }
                        ts.reg[reg_tmp].value.fvalue = strtod(usr_argv[3], NULL);
                    }
                    ts.print_registers();
                }
                else if (!strcmp(usr_argv[1], "mem")){
                    int pos = strtol(usr_argv[2],NULL,10);
                    if(strstr(usr_argv[3],".")){
                        ts.mem[pos].fvalue = strtod(usr_argv[3],NULL);
                        printf("MEM[%d] = %lf\n",pos,ts.mem[pos].fvalue);
                    }
                    else{
                        ts.mem[pos].rvalue = strtoll(usr_argv[3],NULL,10);
                        printf("MEM[%d] = %lld\n",pos,ts.mem[pos].rvalue);
                    }
                }
            }
            else{
                cout<<"invalid option!"<<endl;
                continue;
            }
        }
    }

    return 0;
}