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


void execute(const string& code, shared_ptr<Environment> env) {

    cout << "Executing: " << code << endl;

    vector<string> token_vc = tokenize(code);
    list<string> tokens(token_vc.begin(), token_vc.end());
    LispVal ast = parse(tokens);

    Compiler compiler;
    compiler.compile(ast);

    vm virmac(env);
    LispVal result = virmac.run(compiler.bytecode);

    cout << "----RESULT----" << "\n";
    print_lisp_val(result);
    cout << "\n--------------\n";
}



int main() {

    try{

        cout << "----Micro-Lisp Compiler and VM----" << "\n\n";

        auto global_env = make_shared<Environment>();

        execute("(define radius 5)", global_env);
        execute("(define pi 3.14159)", global_env);
        execute("(* pi (* radius radius))", global_env);

    }
    catch(const exception e){

        cerr << "Compilation Error: " << e.what() << endl;
    }

    return 0;
}