#include "vm.h"
#include <stdexcept>

using namespace std;

void vm::push(const LispVal& val) {
    stack.push_back(val);
}

LispVal vm::pop() {

    if(stack.empty()) throw runtime_error("VM stack underflow");
    LispVal val = stack.back();
    stack.pop_back();
    return val;
}

double get_num(const LispVal& v) {

    if(holds_alternative<double>(v.value)){
        return get<double>(v.value);
    }
    throw runtime_error("Type Error: Expected a number");
}

LispVal vm::run(const vector<Instruction>& bytecode) {

    for(size_t ip=0; ip<bytecode.size(); ++ip){
        const Instruction& inst = bytecode[ip];

        switch (inst.op)
        {
        case OpCode::CONST:
            push(inst.operand);
            break;

        case OpCode::ADD:{
            double b=get_num(pop());
            double a=get_num(pop());
            push(LispVal(a+b));
            break;
        }
        case OpCode::SUB:{
                double b = get_num(pop());
                double a = get_num(pop());
                push(LispVal(a - b));
                break;
        } 
        case OpCode::MUL:{
            double b = get_num(pop());
            double a = get_num(pop());
            push(LispVal(a * b));
            break;
        }
        case OpCode::DIV:{
            double b = get_num(pop());
            double a = get_num(pop());
            if (b == 0) throw runtime_error("Division by zero");
            push(LispVal(a / b));
            break;
        }
        case OpCode::GET_GLOBAL:{
            string var_name = get<string>(inst.operand.value);
            push(env->get(var_name)); 
            break;
        }
        case OpCode::SET_GLOBAL:{
            string var_name = get<string>(inst.operand.value);
            LispVal val = pop();
            env->set(var_name, val);
            push(val); 
            break;
        }
        case OpCode::EQ: {
            double b = get_num(pop());
            double a = get_num(pop());
            push(LispVal(a == b));
            break;
        }
        case OpCode::LESS_THAN: {
            double b = get_num(pop());
            double a = get_num(pop());
            push(LispVal(a < b));
            break;
        }
        case OpCode::GREATER_THAN: {
            double b = get_num(pop());
            double a = get_num(pop());
            push(LispVal(a > b));
            break;
        }
        case OpCode::LESS_EQ: {
            double b = get_num(pop());
            double a = get_num(pop());
            push(LispVal(a <= b));
            break;
        }
        case OpCode::GREATER_EQ: {
            double b = get_num(pop());
            double a = get_num(pop());
            push(LispVal(a >= b));
            break;
        }
        case OpCode::NOT_EQ: {
            double b = get_num(pop());
            double a = get_num(pop());
            push(LispVal(a != b));
            break;
        }
        case OpCode::JUMP_IF_FALSE: {
            LispVal condition = pop();
            if (std::holds_alternative<bool>(condition.value) && !std::get<bool>(condition.value)) {
                // Read the offset and jump the instruction pointer forward
                double offset = get<double>(inst.operand.value);
                ip += static_cast<size_t>(offset); 
            }
            break;
        }
        case OpCode::JUMP: {
            double offset = get<double>(inst.operand.value);
            ip += static_cast<size_t>(offset);
            break;
        }
        
        default:
            break;
        }
    }

    if (!stack.empty()) {
        return pop();
    }
    return LispVal(false);
}