#pragma once
#include <string>
#include <vector>
#include <variant>
#include <iostream>
#include "gc.h"

using namespace std;

struct LispVal;
class Environment;

using LispList = vector<LispVal>;

struct GCList : public GCObject {
    vector <LispVal> items;

    GCList() = default;
    GCList(size_t size) : items(size) {}

    template<typename Iter>
    GCList(Iter begin, Iter end) : items(begin, end) {}
};

struct LispFunction {
    vector<string> params;
    size_t ip;
    Environment* env;
};

struct LispVal {
    variant<double, bool, string, LispList, LispFunction, GCList*> value;

    LispVal() : value(false) {}
    LispVal(double n) : value(n) {}
    LispVal(bool b) : value(b) {}
    LispVal(string s) : value(s) {}
    LispVal(LispList l) : value(l) {}
    LispVal(const char* s) : value(string(s)) {} 
    LispVal(LispFunction f) : value(f) {}
    LispVal(GCList* ptr) : value(ptr) {}
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

inline void print_lisp_val(const LispVal& val){

    visit(overloaded {
        [](double n){ cout << n; },
        [](bool b){ cout << (b?"#t":"#f"); },
        [](const string& s){ cout << s; },
        [](const LispFunction& f){ cout << "<function arity " << f.params.size() << ">"; },
        [](const LispList& l){
            cout << "(";
            for(size_t i=0; i<l.size(); ++i){
                print_lisp_val(l[i]);
                if(i<l.size()-1) cout << " ";
            }
            cout << ")";
        },
        [](GCList* vec) {
            cout << "(";
            for (size_t i = 0; i < vec->items.size(); ++i) {
                print_lisp_val(vec->items[i]);
                if (i < vec->items.size() - 1) cout << " ";
            }
            cout << ")";
        }
    }, val.value);
    
}
