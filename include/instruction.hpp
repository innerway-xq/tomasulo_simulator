#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_
#include <string>
using std::string;
extern int instructions_state_color;
class instruction
{
public:
    string name = "";
    string operand1 = "";
    string operand2 = "";
    string operand3 = "";
    int line_num = 0;
    int state = 0;
    int state1_t = 0,state3_t = 0;
    int state2_st = 0,state2_end = 0;

    void print();
    void reset();
};
#endif
