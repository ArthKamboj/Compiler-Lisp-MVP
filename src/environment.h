#pragma once
#include "types.h"
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "gc.h"

using namespace std;

inline void mark_lisp_val(const LispVal& val);

class Environment : public GCObject {

    public:
        unordered_map<string, LispVal> symbol_table;
        Environment* parent;
        
        Environment(Environment* parent = nullptr) : parent(parent) {}

        void set(const string& symbol, const LispVal& val) {
            symbol_table.insert_or_assign(symbol, val);
        }

        LispVal get(const string& symbol) {
            auto it = symbol_table.find(symbol);
            if(it != symbol_table.end()){
                return it->second;
            }
            if(parent){
                return parent->get(symbol);
            }
            throw runtime_error("Unbound Symbol: "+symbol);
        }

        void mark() {
            if (marked) return;
            marked = true;

            if (parent) parent->mark();

            for(auto& pair : symbol_table) {
                mark_lisp_val(pair.second);
            }
        }
};

inline void mark_lisp_val (const LispVal& val) {
    if(holds_alternative<GCList*>(val.value)) {
        GCList* list = get<GCList*>(val.value);
        if (list && !list->marked) {
            list->marked = true;
            for (const auto& item : list->items) {
                mark_lisp_val(item);
            }
        }
    }
    else if (holds_alternative<LispFunction>(val.value)) {
            LispFunction func = get<LispFunction>(val.value);
        if (func.env) {
            func.env->mark();
        }
    }
}