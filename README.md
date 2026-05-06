# Micro-Lisp Compiler & Virtual Machine

A lightweight, fully Turing-complete, stack-based Lisp compiler and virtual machine written from scratch in modern C++17.

This project demonstrates the complete lifecycle of a programming language front-end and back-end: taking raw string input, building a type-safe Abstract Syntax Tree (AST), compiling it down to linear bytecode, and executing it natively on a custom stack-based Virtual Machine.

It supports interactive execution via a REPL (Read-Eval-Print Loop) and batch execution of `.lisp` script files.

## Features

### Language features

* **Primitives:** Numbers (double), Booleans (#t, #f), Strings ("text")

* **Variables & Lexical Scoping:** (define x 10) with full closure support.

* **Math & Logic:** +, -, *, /, =, <, >, <=, >=, !=

* **Control Flow:** (if condition true-branch false-branch) and (while condition body)

* **Functions:** First-class, user-defined functions via (lambda (params) body)

* **I/O:** Console output (print x) and dynamic user input (read)

* **Standard Library:** Built-in native C++ functions like sqrt, % (modulo), and rand.

### Engine Architecture

* **State-Machine Lexer:** A robust token scanner capable of handling complex string literals, nested parentheses, and whitespace-agnostic formatting.

* **Recursive Descent Parser:** Dynamically builds a 2D Abstract Syntax Tree (AST) utilizing modern C++ `std::variant` for strict, memory-safe type unions (`LispVal`).

* **Persistent Environment:** A chained `std::unordered_map` structure that resolves local and global variables, natively supporting nested scopes and Closures.

* **Bytecode Compiler:** Flattens the AST into a 1D array of machine instructions. It handles Postfix/RPN translation and utilizes backpatching to compute relative jump offsets for `if` statements and `while` loops.

* **Stack-Based Virtual Machine:** Executes instructions via a master instruction pointer (`IP`). It maintains a Runtime Stack for intermediate values and a Call Stack (`CallFrame`) to pause, branch, and seamlessly resume execution during function calls.

## Build Instructions

This project requires a C++ compiler with C++17 support (e.g., GCC 7+, Clang 5+, or MSVC 19.14+). No external libraries or dependencies are required.

To compile the project from the terminal, link all `.cpp` files together:

```bash
g++ -std=c++17 main.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp -o lisp_compiler
```

## Usage

The engine supports two modes of execution:

### 1. Interactive REPL Mode

Run the executable without any arguments to start the interactive Read-Eval-Print-Loop prompt.


```bash
./lisp_compiler
OR
lisp_compiler.exe (Windows)
```

```lisp
Micro-Lisp Interactive Prompt (Type 'exit' to quit)
>> (define r 5)
#f
>> (print (* 3.14 (* r r)))
78.5
78.5
>> exit
```

### 2. File Execution Mode

Pass a `.lisp` text file as a command-line argument to execute an entire script at once.

```bash
./lisp_compiler script.lisp 
OR
lisp_compiler.exe script.lisp (Windows)
```

## Syntax and Examples

### Variable and Math
Micro-Lisp uses prefix notation (the operator comes first).

```lisp
(define x 10)
(define y 20)
(+ x y)           ; Returns 30
(* (+ 1 2) 4)     ; Returns 12
```

### Control Flow (if/while)

```lisp
(define age 20)
(if (>= age 18) 
    (print "Adult") 
    (print "Minor"))

(define count 3)
(while (> count 0)
    (print count)
    (define count (- count 1)))
```
### Functions

Functions are first-class citizens. They can be defined, assigned to variables, and called.

```lisp
(define square (lambda (x) (* x x)))

(print (square 5))             ; Prints 25
(print (+ (square 3) (square 4))) ; Prints 25
```

### Interactive I/O

The `read` function safely captures dynamic typing from the terminal (parsing inputs securely to Numbers, Booleans, or Strings).

```lisp
(define secret 42)
(define guess 0)

(while (!= guess secret)
    (print "Enter your guess:")
    (define guess (read))
    
    (if (< guess secret)
        (print "Too low!")
        (if (> guess secret)
            (print "Too high!")
            (print "You got it!")))
)
```


### The Standard Library

Native hooks to the C++ standard library.

```lisp
(print (sqrt 144))  ; Prints 12
(print (% 10 3))    ; Prints 1
(print (rand))      ; Prints a random float between 0.0 and 1.0
```
