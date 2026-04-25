#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<string> tokenize(const string& input){

    string padded_input;

    for(char c : input){

        if(c=='(')
            padded_input += " ( ";
        else if(c==')')
            padded_input += " ) ";
        else
            padded_input += c;

    }

    vector<string> tokens;
    stringstream ss(padded_input);
    string token;

    while(ss >> token){
        tokens.push_back(token);
    }

    return tokens;

}

int main() {

    string code = "(+ 1 (* 2 3))";
    vector<string> tokens = tokenize(code);

    for(const auto& t : tokens){
        cout << "[" << t << "] ";
    }

    cout << endl;

    return 0;
}