#include "lexer.h"
#include <sstream>

using namespace std;

vector<std::string> tokenize(const string& input) {
    string padded_input;
    
    for (char c : input) {
        if (c == '(') {
            padded_input += " ( ";
        } else if (c == ')') {
            padded_input += " ) ";
        } else {
            padded_input += c;
        }
    }

    vector<std::string> tokens;
    stringstream ss(padded_input);
    string token;
    
    while (ss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}