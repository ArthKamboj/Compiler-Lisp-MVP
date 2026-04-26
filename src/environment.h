#pragma once
#include "types.h"
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>

using namespace std;

class Environment{

    private:
        unordered_map<string, LispVal> symbol_table;
        shared_ptr<Environment> outer;

    public:
        Environment() : outer(nullptr) {}
        Environment(shared_ptr<Environment> outer_env) : outer(outer_env) {}
        void set(string& symbol, const LispVal& val){
            symbol_table[symbol] = val;
        }
        
};