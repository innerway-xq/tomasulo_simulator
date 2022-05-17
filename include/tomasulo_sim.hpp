#ifndef TOMASULO_SIM_H_
#define TOMASULO_SIM_H_

#include "instruction.hpp"
#include "common_data_bus.hpp"
#include "reservation_station.hpp"
#include "register_station.hpp"

class tomasulo_sim{
public:
    vector<instruction> iq;
    vector<reservation_station> rvs;
    common_data_bus cdb;
    vector<register_station> reg;
    value_t mem[10000];

    int pc = 0;
    int t=0;//时钟
    bool done_flag = false;

    int ld_t = 2;
    int add_t = 3;
    int sub_t = 3;
    int div_t = 41;
    int mul_t = 11;
    int ld_n = 2;
    int add_sub_n = 3;
    int mul_div_n = 2;
    int reg_f_n = 32;
    int reg_r_n = 32;

    tomasulo_sim();
    //重置
    void reset();
    //载入命令
    bool load_instructions(const char *filename);
    //打印命令状态
    void print_instructions();
    //打印保留站
    void print_reservation_stations();
    //打印寄存器
    void print_registers();
    //打印参数
    void show_parameters();
    
    void show();
    //流出
    //返回值：流出指令所用的保留站号，若无法流出则返回-1
    int issue();
    //执行
    //返回值：此时钟cdb上变为可用的保留站号
    vector<int> execute();
    //写结果
    void write_result();
    //单步前进
    //返回值：true-程序未结束，false-程序已结束
    bool step();
};
#endif