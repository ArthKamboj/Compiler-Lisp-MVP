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

    string code = "(+ (* 2 3 4) (- (* 10 5) (/ 100 2) (* 3 3)) (/ (+ (* 7 8) (- 50 20) (* 2 2 2)) 5) (* (+ 1 2 3 4 5) (- 20 10 5) (/ 90 3)) (- (+ (* 6 6) (/ 144 12) (* 2 5)) (* 3 7)) (/ (* (+ 8 2) (- 15 5) (* 3 3)) (+ 5 5)) (* (- 100 50 25) (+ 4 4 4) (/ 60 2)))";
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