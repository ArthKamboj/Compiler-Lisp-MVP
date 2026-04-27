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
    SET_GLOBAL
};

