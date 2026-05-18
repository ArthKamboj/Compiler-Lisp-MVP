# Micro-Lisp Compiler, Virtual Machine & Garbage Collector

Micro-Lisp is a fully functional, Turing-complete programming language featuring a custom bytecode compiler and a stack-based Virtual Machine built entirely from scratch in modern C++17.

Designed without relying on external parser generators (like Bison/Yacc) or LLVM, this project implements the complete pipeline of programming language design: from raw text scanning to garbage-collected runtime execution.

It supports interactive execution via a REPL (Read-Eval-Print Loop) and batch execution of `.lisp` script files.

## Features

* **Costum Pipeline:** Handwritten Lexer, Recursive Descent Parser, and Bytecode Compiler.

* **Stack-based VM:** High-performance execution engine utilizing 1D bytecode arrays and instruction pointer jumping.
  
* **Lexical Scoping & Closures:** True first-class functions that properly capture and retain their birthing environments.

* **Mark-and-Sweep Garbage Collector:** Fully custom memory management heap that traces root stacks and sweeps unreferenced objects, entirely replacing C++ smart pointers.
* **Self-Hosted Standard Library:** Recursive functional tools (map, filter, length) written in Micro-Lisp and loaded dynamically on boot.

* **Functions:** First-class, user-defined functions via (lambda (params) body)

* **External File Exectution:** External LISP files can be passed as argument when executing the code to execute the LISP code.

* **Interactive REPL:** A real-time Read-Eval-Print Loop with state persistence across inputs.

## Architecture

* **State-Machine Lexer:** A robust token scanner scans raw ASCII strings and tokenizes them into categorised symbols (parentheses, numbers, strings, identifiers).

* **Recursive Descent Parser:** Uses recursive descent to convert the 1D token stream into a 2D Abstract Syntax Tree (AST), validating syntax and nested logic.

* **Persistent Environment:** A chained `std::unordered_map` structure that resolves local and global variables, supporting nested scopes and Closures.

* **Bytecode Compiler:** Flattens the AST into a 1D array of machine instructions (`OpCode::ADD`, `OpCode::JUMP`, `OpCode::CALL`). It also handles Postfix/RPN translation and utilizes backpatching to compute relative jump offsets for `if` statements and `while` loops.

* **Stack-Based Virtual Machine:** Executes instructions via a master instruction pointer (`IP`). It maintains a Runtime Stack for intermediate values and a Call Stack (`CallFrame`) to pause, branch, and seamlessly resume execution during function calls.

* **Garbage Collector:** Intercepts memory allocations. When the heap reaches a threshold, it pauses the VM, traces active root variables, and safely deletes inaccessible environments and lists.

## 🛠️ Build Instructions

### Prerequisites

* This project requires a C++ compiler with C++17 support (e.g., GCC 7+, Clang 5+, or MSVC 19.14+). No external libraries or dependencies are required.

### Building the Compiler
* Clone the repository in yoour system and set current directory within the src file.

  ```bash
  git clone https://github.com/ArthKamboj/Compiler-Lisp-MVP.git
  cd Compiler-Lisp-MVP\src
  ```

* To compile the project from the terminal, link all `.cpp` files together and compile using `g++`:

  ```bash
  g++ -std=c++17 main.cpp gc.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp -o lisp_compiler
  ```

## Usage

The engine supports two modes of execution:

### 1. Interactive REPL Mode

Run the executable without any arguments to start the live interactive Read-Eval-Print-Loop prompt.


```bash
./lisp_compiler
OR
lisp_compiler.exe (Windows)
```
Example Usage:
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

## 📖 Syntax and Examples

### Variable, Math & Logic
Micro-Lisp supports a wide array of functional programming paradigms natively. it uses prefix notation (the operator comes first).

```lisp
(define x 10)
(define y 5)

(print (+ x y))    ; 15
(print (* x y))    ; 50
(print (>= x 20))  ; #f
(print (!= x y))   ; #t
```

### Control Flow (if/while)

```lisp
(define count 3)
(while (> count 0)
    (print count)
    (define count (- count 1)))

(if (> 10 5) 
    (print "Math works!") 
    (print "Math is broken!"))
```
### Functions

Functions are first-class citizens. They can be defined, assigned to variables, and called.

```lisp
(define square (lambda (x) (* x x)))

(print (square 5))             ; Prints 25
(print (+ (square 3) (square 4))) ; Prints 25
```

### Lexical Closures

Functions can return other functions and will successfully capture their parent environments.

```lisp
(define make-multiplier 
    (lambda (factor) 
        (lambda (x) (* x factor))))

(define triple (make-multiplier 3))
(print (triple 15)) ; Output: 45
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


### The Standard Library (Lists & Functional Iteration)

Using the self-hosted boot library, you can easily map and filter arrays.

```lisp
(define nums (list 1 2 3 4 5 6))

; Map: Square all numbers
(define square (lambda (x) (* x x)))
(print (map square nums)) 
; Output: (1 4 9 16 25 36)

; Filter: Keep only evens
(define is-even (lambda (x) (= (% x 2) 0)))
(print (filter is-even nums)) 
; Output: (2 4 6)
```

## Project Structure

    
| File | Responsibility |
| --- | --- |
| `main.cpp` | CLI entry point, REPL loop, and Boot Library initialization. |
| `types.h` | Defines `LispVal`, `OpCodes`, and the `std::variant` type system. |
| `lexer.h/cpp` | Tokenizes raw string input. |
| `parser.h/cpp` | Constructs the Abstract Syntax Tree. |
| `compiler.h/cpp` | Generates bytecode and handles JUMP offsets. |
| `vm.h/cpp` | The stack-based execution engine and root-tracer. |
| `environment.h` | Lexical scoping, variable tables, and closure contexts. |
| `gc.h/cpp` | The Mark-and-Sweep memory management heap. |
