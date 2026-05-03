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
            if (holds_alternative<bool>(condition.value) && !get<bool>(condition.value)) {
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
        case OpCode::CALL: {
            LispVal func_val = pop();
            if (!holds_alternative<LispFunction>(func_val.value)) {
                throw runtime_error("Attempted to call a non-function");
            }
            LispFunction func = get<LispFunction>(func_val.value);
            auto local_env = make_shared<Environment>(env);

            for (int i = static_cast<int>(func.params.size()) - 1; i >= 0; --i) {
                local_env->set(func.params[i], pop());
            }
            call_stack.push_back({ip, env});

            ip = func.ip - 1; 
            env = local_env;
            break;
        }
        
        case OpCode::RETURN: {
            LispVal result = pop();

            if (call_stack.empty()) throw runtime_error("Call stack underflow!");
            CallFrame frame = call_stack.back();
            call_stack.pop_back();

            ip = frame.return_ip;
            env = frame.return_env;
            push(result);
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