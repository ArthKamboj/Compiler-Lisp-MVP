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

    cout << "Executing: " << code << endl;

    vector<string> token_vc = tokenize(code);
    list<string> tokens(token_vc.begin(), token_vc.end());
    LispVal ast = parse(tokens);

    size_t start_ip = compiler.bytecode.size();

    compiler.compile(ast);

    vm virmac(env);
    LispVal result = virmac.run(compiler.bytecode, start_ip);

    cout << "----RESULT----" << "\n";
    print_lisp_val(result);
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
                (print "Enter your guess:")
                (define guess (read))
                
                (if (< guess secret)
                    (print "Too low!")
                    (if (> guess secret)
                        (print "Too high!")
                        (print "You got it!")))
            )
        )";

        execute(game_script, global_env, master_compiler, master_vm);
    }
    catch(const exception e){

        cerr << "Compilation Error: " << e.what() << endl;
    }

    return 0;
}