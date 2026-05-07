#include "vm.h"
#include <stdexcept>
#include <cmath>
#include <cstdlib>

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

LispVal vm::run(const vector<Instruction>& bytecode, size_t start_ip) {

    for(size_t ip=start_ip; ip<bytecode.size(); ++ip){

        // cout << "[VM TRACE] IP: " << ip << " | Opcode: " << static_cast<int>(bytecode[ip].op) << endl;

        const Instruction& inst = bytecode[ip];

        try {
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
        case OpCode::PRINT: {
            LispVal val = pop();

            print_lisp_val(val);
            cout << endl;
            push(val);
            break;
        }
        case OpCode::READ: {
            string input_str;
            if (!(std::cin >> input_str)) {
                std::cin.clear();
                throw std::runtime_error("VM FATAL ERROR: Input stream crashed.");
            }

            if(input_str == "#t") {
                push(LispVal(true));
            }
            else if(input_str == "#f") {
                push(LispVal(false));
            }
            else {
                try{
                    size_t chars_processed;
                    double val = stod(input_str, &chars_processed);

                    if(chars_processed == input_str.length()) {
                        push(LispVal(val));
                    }
                    else {
                        push(LispVal(input_str));
                    }
                }
                catch(const invalid_argument) {
                    push(LispVal(input_str));
                }

            }
            break;
        }
        case OpCode::POP_STACK: {
            pop();
            break;
        }
        case OpCode::JUMP_BACK: {
            double offset = get<double>(inst.operand.value);
            ip -= static_cast<size_t>(offset);
            break;
        }
        case OpCode::SQRT: {
            LispVal val = pop();
            if(!holds_alternative<double>(val.value)) throw runtime_error("sqrt requires a number");
            push(LispVal(sqrt(get<double>(val.value))));
            break;
        }
        case OpCode::MOD: {
            LispVal b_val = pop();
            LispVal a_val = pop();
            if(!holds_alternative<double>(a_val.value) || !holds_alternative<double>(b_val.value)){
                throw runtime_error("mod requires two numbers");
            }
            double a = get<double>(a_val.value);
            double b = get<double>(b_val.value);
            push(LispVal(fmod(a,b)));
            break;
        }
        case OpCode::RAND: {
            double r = static_cast<double>(rand())/RAND_MAX;
            push(LispVal(r));
            break;
        }
        case OpCode::STR_LEN: {
            LispVal val = pop();
            if (!holds_alternative<string>(val.value)) {
                throw runtime_error("string-length requires a string");
            }
            double len = static_cast<double>(get<string>(val.value).length());
            push(LispVal(len));
            break;
        }
        case OpCode::CONCAT: {
            LispVal b_val = pop();
            LispVal a_val = pop();
            if(!holds_alternative<string>(a_val.value) || !holds_alternative<string>(b_val.value)) {
                throw runtime_error("concat requires 2 strings");
            }
            string res = get<string>(a_val.value) + get<string>(b_val.value);
            push(LispVal(res));
            break;
        }
        case OpCode::SUBSTR: {
            LispVal len_val = pop();
            LispVal start_val = pop();
            LispVal str_val = pop();
            if(!holds_alternative<double>(len_val.value) ||
               !holds_alternative<double>(start_val.value) ||
               !holds_alternative<string>(str_val.value)) {
                throw runtime_error("substring requires (tring, start, length)");
            }
            string str = get<string>(str_val.value);
            size_t start = static_cast<size_t>(get<double>(start_val.value));
            size_t len = static_cast<size_t>(get<double>(len_val.value));

            if(start >= str.length()) {
                push(LispVal(string("")));
            }
            else {
                push(LispVal(str.substr(start, len)));
            }
            break;
        }

        default:
            break;
        }
        }
        catch(const exception& e) {
            std::cerr << "\nFATAL EXCEPTION at IP: " << ip 
                      << " | Opcode ID: " << static_cast<int>(inst.op) 
                      << " | " << e.what() << std::endl;
            throw;
        }
    }

    if (!stack.empty()) {
        return pop();
    }
    return LispVal(false);
}