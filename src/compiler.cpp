#include "compiler.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void Compiler::compile(const LispVal& ast){

    visit(overloaded {

        [this](double n) {
            bytecode.push_back(Instruction(OpCode::CONST, LispVal(n)));
        },
        [this](bool b) {
            bytecode.push_back(Instruction(OpCode::CONST, LispVal(b)));
        },
        [this](const string& s) {
            bytecode.push_back(Instruction(OpCode::GET_GLOBAL, LispVal(s)));
        },
        [this](const LispList& l) {
            if (l.empty()) return;
            auto op_val = l[0].value;
            
            if (!holds_alternative<string>(op_val)) {
                throw runtime_error("First element of list must be a symbol/function");
            }
            string op = get<string>(op_val);

            if (op == "define") {
                if (l.size() != 3) throw runtime_error("define requires exactly 2 arguments");
                string var_name = get<string>(l[1].value);
                compile(l[2]); 
                bytecode.push_back(Instruction(OpCode::SET_GLOBAL, LispVal(var_name)));
                return;
            }

            for(size_t i=1; i<l.size(); i++){
                compile(l[i]);
            }

            if (op == "+") bytecode.push_back(Instruction(OpCode::ADD));
            else if (op == "-") bytecode.push_back(Instruction(OpCode::SUB));
            else if (op == "*") bytecode.push_back(Instruction(OpCode::MUL));
            else if (op == "/") bytecode.push_back(Instruction(OpCode::DIV));
            else throw runtime_error("Unknown operator: " + op);
        }

    }, ast.value);

}

void Compiler::disassemble(){

}