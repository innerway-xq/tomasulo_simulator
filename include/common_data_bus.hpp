#ifndef COMMON_DATA_BUS_H_
#define COMMON_DATA_BUS_H_
#include <vector>
#include "value_t.hpp"
using std::vector;
class common_data_bus
{
public:
    vector<bool> rvs_ava;    //保留站是否算出结果
    vector<value_t> result;
    vector<int> itn_num;     //来源的指令行号，用来更新指令的状态
};
#endif