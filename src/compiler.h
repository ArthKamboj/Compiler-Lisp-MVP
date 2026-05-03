#pragma once
#include "types.h"
#include <vector>
#include <string>

using namespace std;

enum class OpCode{
    CONST,
    ADD,
    SUB,
    MUL,
    DIV,
    GET_GLOBAL,
    SET_GLOBAL,
    EQ,
    LESS_THAN,
    GREATER_THAN
};

struct  Instruction{
    
    OpCode op;
    LispVal operand;
    Instruction (OpCode o) : op(o), operand(false) {}
    Instruction (OpCode o, LispVal val) : op(o), operand(val) {}
};

class Compiler{

    public:
        vector<Instruction> bytecode;
        void compile(const LispVal& ast);
        void disassemble();
};