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
           if (holds_alternative<LispFunction>(inst.operand.value)) {
                LispFunction func = get<LispFunction>(inst.operand.value);
                func.env = this->env; 
                push(LispVal(func));
            }
            else {
                push(inst.operand);
            }
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
            LispVal b_val = pop();
            LispVal a_val = pop();
            bool is_equal = false;

            if (a_val.value.index() == b_val.value.index()) {
                if (holds_alternative<double>(a_val.value)) {
                    is_equal = (get<double>(a_val.value) == get<double>(b_val.value));
                }
                else if (holds_alternative<bool>(a_val.value)) {
                    is_equal = (get<bool>(a_val.value) == get<bool>(b_val.value));
                }
                else if (holds_alternative<string>(a_val.value)) {
                    is_equal = (get<string>(a_val.value) == get<string>(b_val.value));
                }
            }
            push(LispVal(is_equal));
            break;
        }
        case OpCode::NOT_EQ: {
            LispVal b_val = pop();
            LispVal a_val = pop();
            bool is_equal = false;

            if (a_val.value.index() == b_val.value.index()) {
                if (holds_alternative<double>(a_val.value)) {
                    is_equal = (get<double>(a_val.value) == get<double>(b_val.value));
                }
                else if (holds_alternative<bool>(a_val.value)) {
                    is_equal = (get<bool>(a_val.value) == get<bool>(b_val.value));
                }
                else if (holds_alternative<string>(a_val.value)) {
                    is_equal = (get<string>(a_val.value) == get<string>(b_val.value));
                }
            }
            push(LispVal(!is_equal));
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
        case OpCode::JUMP: {
            int offset = static_cast<int>(std::get<double>(inst.operand.value));
            ip += offset; 
            break;
        }
        case OpCode::JUMP_IF_FALSE: {
            LispVal val = pop();
            if (std::holds_alternative<bool>(val.value) && !std::get<bool>(val.value)) {
                int offset = static_cast<int>(std::get<double>(inst.operand.value));
                ip += offset;
            }
            break;
        }
        case OpCode::CALL: {
            LispVal val = pop();
            LispFunction func = std::get<LispFunction>(val.value);
            auto call_env = std::make_shared<Environment>(func.env);
            
            for (int i=func.params.size()-1; i>=0; --i) {
                call_env->set(func.params[i], pop());
            }

            CallFrame frame;
            frame.return_ip = ip;
            frame.return_env = this->env;
            call_stack.push_back(frame);
            
            this->env = call_env;
        
            ip = func.ip - 1; 
            
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
        //string
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
        //list
        case OpCode::MAKE_LIST: {
            int num_args = static_cast<int>(get<double>(inst.operand.value));
            auto new_list = make_shared<vector<LispVal>>(num_args);

            for (int i=num_args-1; i>=0; --i) {
                (*new_list)[i] = pop();
            }
            push(LispVal(new_list));
            break;
        }
        case OpCode::CAR: {
            LispVal val = pop();
            auto vec = get<shared_ptr<vector<LispVal>>>(val.value);
            if(vec->empty()) throw runtime_error("car called on first item");
            push((*vec)[0]);
            break;
        }
        case OpCode::IS_EMPTY: {
            LispVal val = pop();
            auto vec = get<shared_ptr<vector<LispVal>>>(val.value);
            push(LispVal(vec->empty()));
            break;
        }
        case OpCode::CDR: {
            LispVal val = pop();
            auto vec = get<shared_ptr<vector<LispVal>>>(val.value);
            if(vec->empty()) throw runtime_error("cdr called on empty list");
            auto new_list = make_shared<vector<LispVal>>(vec->begin()+1, vec->end());
            push(LispVal(new_list));
            break;
        }
        case OpCode::CONS: {
            LispVal list_val = pop();
            LispVal item_val = pop();
            auto vec = get<shared_ptr<vector<LispVal>>>(list_val.value);

            auto new_list = make_shared<vector<LispVal>>();
            new_list->push_back(item_val);
            new_list->insert(new_list->end(), vec->begin(), vec->end());
            push(LispVal(new_list));
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