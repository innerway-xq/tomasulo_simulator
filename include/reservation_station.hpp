#ifndef RESERVATION_STATION_H_
#define RESERVATION_STATION_H_
#include <string>
#include "value_t.hpp"
using std::string;
class reservation_station
{
public:
    string op = "";
    int qj = 0, qk = 0;
    value_t vj, vk;
    bool busy = false;
    long long a = 0;

    int t = 0;       //剩余时间
    int itn_num = 0; //对应指令行号

    void print(int ld_n, int add_sub_n, int mul_div_n);
    void clear();
};
#endif