#include "vm.h"
#include <stdexcept>

using namespace std;

LispVal vm::pop() {

    if(stack.empty()) throw runtime_error("VM stack underflow");
    LispVal val = stack.back();
    stack.pop_back();
    return val;
}