#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <exception>
#include <memory>
#include <fstream>
#include <sstream>

#include "lexer.h"
#include "parser.h"
#include "types.h"
#include "environment.h"
#include "compiler.h"
#include "vm.h"

using namespace std;


void execute(const string& code, shared_ptr<Environment> env, Compiler& compiler, vm& vima) {

    cout << "Executing Script: " << code << endl;

    vector<string> token_vc = tokenize(code);
    list<string> tokens(token_vc.begin(), token_vc.end());

    LispVal last_result = LispVal(false);

    while(!tokens.empty()) {
        LispVal ast = LispVal(false);
        
        try { 
            ast = parse(tokens); 
        } catch (const exception& e) { 
            cerr << "\n[PARSER CRASH] " << e.what() << endl; return; 
        }
        
        size_t start_ip = compiler.bytecode.size();
        
        try { 
            compiler.compile(ast); 
        } catch (const exception& e) { 
            cerr << "\n[COMPILER CRASH] " << e.what() << endl; return; 
        }
        
        try { 
            last_result = vima.run(compiler.bytecode, start_ip); 
        } catch (const exception& e) { 
            cerr << "\n[VM CRASH] " << e.what() << endl; return; 
        }
    }
    
    cout << "----RESULT----" << "\n";
    print_lisp_val(last_result);
    cout << "\n--------------\n";
}

void run_file(const string& filename, shared_ptr<Environment> env, Compiler& compiler, vm& virmac) {

    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'" << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();

    execute(code, env, compiler, virmac);
}

void run_repl(shared_ptr<Environment> env, Compiler& compiler, vm& virmac) {
    cout << "Micro-Lisp Interactive Prompt (Type 'exit' to quit)\n";
    string input;

    while (true) {
        cout << ">> ";
        getline(cin, input);

        if (input == "exit" || input == "quit") break;
        if (input.empty()) continue;

        try {
            execute(input, env, compiler, virmac);
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }

    }
}


int main(int argc, char* argv[]) {

    srand(static_cast<unsigned int>(time(nullptr)));

    auto global_env = make_shared<Environment>();
    Compiler master_compiler;
    vm master_vm(global_env);

    if(argc > 1) {
        run_file(argv[1], global_env, master_compiler, master_vm);
    }
    else {
        run_repl(global_env, master_compiler, master_vm);
    }

    return 0;
}