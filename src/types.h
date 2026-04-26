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

inline void print_lisp_val(const LispVal& val){

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
