#pragma once
#include <string>
#include <vector>
#include <variant>
#include <iostream>

using namespace std;

struct LispVal;
using LispList = vector<LispVal>;

struct LispVal {
    variant<double, bool, string, LispList> value;

    LispVal(double n) : value(n) {}
    LispVal(bool b) : value(b) {}
    LispVal(string s) : value(s) {}
    LispVal(LispList l) : value(l) {}
    LispVal(const char* s) : value(string(s)) {} 
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void print_lisp_val(const LispVal& val){

    visit(overloaded {
        [](double n){ cout << n; },
        [](bool b){ cout << (b?"#t":"#f"); },
        [](const string& s){ cout << s; },
        [](const LispList& l){
            cout << "(";
            for(size_t i=0; i<l.size(); ++i){
                print_lisp_val(l[i]);
                if(i<l.size()-1) cout << " ";
            }
            cout << ")";
        }
    }, val.value);
    
}

int main() {

    LispList inner_list = { LispVal("*"), LispVal(2.0), LispVal(3.0) };
    LispList outer_list = { LispVal("+"), LispVal(1.0), LispList(inner_list) };

    LispVal abstract_syntax_tree = LispVal(outer_list);

    cout << "AST output: ";
    print_lisp_val(abstract_syntax_tree);
    cout << endl;

    return 0;

}