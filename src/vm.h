#pragma once
#include <vector>
# include "gc.h"

#include "compiler.h"
#include "environment.h"

using namespace std;

struct CallFrame {
    size_t return_ip;
    Environment* return_env;
};

class vm {

    public:
        vector<LispVal> stack;
        vector<CallFrame> call_stack;
        Environment* env;
        size_t ip=0;

        GarbageCollector& gc;
        
        vm(Environment* global_env, GarbageCollector& gc_instance) : env(global_env), gc(gc_instance) {}

        void push(const LispVal& val);
        LispVal pop();

        void mark_roots();
        
        LispVal run(const vector<Instruction>& bytecode, size_t start_ip=0);
};