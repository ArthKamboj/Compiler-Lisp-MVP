#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <exception>

#include "lexer.h"
#include "parser.h"
#include "types.h"
#include "environment.h"

using namespace std;



int main() {

    string code = "(+ x 3)";
    cout << "Input code: " << code << "\n";

    try{

        vector<string> token_vec = tokenize(code);
        list<string> tokens(token_vec.begin(), token_vec.end());
        
        LispVal ast = parse(tokens);

        cout << "AST parseed successfully: ";
        print_lisp_val(ast);
        cout << "\n\n";

        cout << "-----------Testing Env-----------" << endl;

        auto global_env = make_shared<Environment>();

        cout << "Define variable x as 10" << endl;
        global_env->set("x", LispVal(10.0));

        LispVal fetched_x = global_env->get("x");
        cout << "Fetched x: ";
        print_lisp_val(fetched_x);
        cout << endl;
        
        cout << "Fetch y: " << endl;
        global_env->get("y");

    }
    catch(const exception e){

        cerr << "Compilation Error: " << e.what() << endl;
    }

    return 0;
}