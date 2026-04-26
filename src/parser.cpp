#include "parser.h"
#include <stdexcept>
#include <cstdlib>

using namespace std;

bool is_num(const string& s){

    if(s.empty()) return false;
    char* end = nullptr;
    strtod(s.c_str(), &end);
    
    return end != s.c_str() && *end == '\0';
}       

LispVal parse_atom(string& token){

    if(token == "#t") return LispVal(true);
    if(token == "#f") return LispVal(false);
    if(is_num(token)) return LispVal(stod(token));
    return LispVal(token);
}

LispVal parse(list<string>& tokens){

    if(tokens.empty()){
        throw runtime_error("Unexpected EOF while reading");
    }
    
    string token = tokens.front();
    tokens.pop_front();

    if(token == "(") {
        LispList list;
        while(!tokens.empty() && tokens.front()!=")"){
            list.push_back(parse(tokens));
        }
        if(tokens.empty()){
            throw runtime_error("Syntax error: Missing ')'");
        }
        tokens.pop_front();
        return LispVal(list);
    }
    else if(token == ")"){
        throw runtime_error("Syntax error: Unexpected ')'");
    }
    else{
        return parse_atom(token);
    }
}