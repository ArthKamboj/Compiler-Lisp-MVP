#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <exception>

#include "lexer.h"
#include "parser.h"
#include "types.h"
// #include "environment.h"
#include "compiler.h"

using namespace std;



int main() {

    string code = "(+ 10 (* 2 5))";
    cout << "Input code: " << code << "\n";

    try{

        vector<string> token_vec = tokenize(code);
        list<string> tokens(token_vec.begin(), token_vec.end());
        
        LispVal ast = parse(tokens);

        cout << "AST parseed successfully: ";
        print_lisp_val(ast);
        cout << "\n\n";

        cout << "-----------Bytecode Compilation-----------" << endl;

        Compiler compiler;
        compiler.compile(ast);
        compiler.disassemble();

    }
    catch(const exception e){

        cerr << "Compilation Error: " << e.what() << endl;
    }

    return 0;
}