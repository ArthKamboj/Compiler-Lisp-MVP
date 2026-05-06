#include "lexer.h"
#include <sstream>
#include <cctype>

using namespace std;

#include "lexer.h"
#include <cctype>

using namespace std;

vector<string> tokenize(const string& code) {
    vector<string> tokens;
    string current_token = "";
    bool in_string = false;
    bool in_comment = false;

    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];

        if(in_comment) {
            if(c == '\n') {
                in_comment = false;
            }
            continue;
        }

        if (in_string) {
            current_token += c;
            if (c == '"') {
                tokens.push_back(current_token);
                current_token = "";
                in_string = false;
            }
        } 
        else {
            if(c == ';') {
                if(!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token = "";
                }
                in_comment = true;
            }
            else if (c == '"') {
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token = "";
                }
                current_token += c;
                in_string = true;
            } 
            else if (c == '(' || c == ')') {
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token = "";
                }
                tokens.push_back(string(1, c));
            } 
            else if (isspace(c)) {
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token = "";
                }
            } 
            else {
                current_token += c;
            }
        }
    }
    
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }
    
    return tokens;
}