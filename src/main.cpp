#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <exception>
#include <memory>

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
        
        // 1. Test the Parser
        try { 
            ast = parse(tokens); 
        } catch (const exception& e) { 
            cerr << "\n[PARSER CRASH] " << e.what() << endl; return; 
        }
        
        size_t start_ip = compiler.bytecode.size();
        
        // 2. Test the Compiler
        try { 
            compiler.compile(ast); 
        } catch (const exception& e) { 
            cerr << "\n[COMPILER CRASH] " << e.what() << endl; return; 
        }
        
        // 3. Test the VM
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



int main() {

    try{

        cout << "----Micro-Lisp Compiler and VM----" << "\n\n";

        auto global_env = make_shared<Environment>();

        Compiler master_compiler;
        vm master_vm(global_env);

        string game_script = R"(
            (define secret 42)
            (define guess 0)
            
            (while (!= guess secret)
                (print "Enter_your_guess:")
                (define guess (read))
                
                (if (< guess secret)
                    (print "Too_low!")
                    (if (> guess secret)
                        (print "Too_high!")
                        (print "You_got_it!")))
            )
        )";

        execute(game_script, global_env, master_compiler, master_vm);
    }
    catch(const exception e){

        cerr << "Compilation Error: " << e.what() << endl;
    }

    return 0;
}