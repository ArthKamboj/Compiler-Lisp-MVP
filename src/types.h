#pragma once
#include <string>
#include <vector>
#include <variant>
#include <iostream>

using namespace std;

struct LispVal;
using LispList = vector<LispVal>;

struct LispVal {
    variant<double, bool, string, LispVal> value;

    LispVal(double n) : value(n) {}
    LispVal(bool b) : value(b) {}
    LispVal(string s) : value(s) {}
    LispVal(LispList l) : value(l) {}
    LispVal(const char* s) : value(string(s)) {} 
};

