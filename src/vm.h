#pragma once
#include <vector>
# include <memory>

#include "compiler.h"
#include "environment.h"

using namespace std;

struct CallFrame {
    size_t return_ip;
    shared_ptr<Environment> return_env;
};

class vm
{
    private:
        vector<LispVal> stack;
        vector<CallFrame> call_stack;
        shared_ptr<Environment> env;

        void push(const LispVal& val);
        LispVal pop();

    public:
        vm(shared_ptr<Environment> environment) : env(environment) {};
        LispVal run(const vector<Instruction>& bytecode, size_t start_ip=0);
};