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
        void set(const string& symbol, const LispVal& val){
            symbol_table.insert_or_assign(symbol, val);
        }
        LispVal get(const string& symbol){
            auto it = symbol_table.find(symbol);
            if(it != symbol_table.end()){
                return it->second;
            }
            if(outer){
                return outer->get(symbol);
            }
            throw runtime_error("Unbound Symbol: "+symbol);
        }
};