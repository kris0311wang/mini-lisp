

# Mini-Lisp Interpreter

## Introduction

This project is the course assignment for **Software Design Practice (PKU, Spring 2023-2024)**. It implements a lightweight Mini-Lisp interpreter in **C++**, following the [Mini-Lisp Specification](https://pku-software.github.io/mini-lisp-spec/). The interpreter supports basic Lisp features, multi-line code input, and simple syntax highlighting for an interactive user experience.

---

## Features

- **Core Language Support**:
  - Atoms: Numbers, symbols, and strings.
  - Lists and nested expressions.
- **Core Functions**:
  - Arithmetic: `+`, `-`, `*`, `/`.
  - Logical operations: `and`, `or`, `not`.
  - Comparisons: `=`, `<`, `>`.
- **User-defined Functions**: Supports `lambda` expressions for creating custom functions.
- **Interactive REPL**:
  - Multi-line code input.
  - Simple syntax highlighting for readability.

---

## Getting Started

### Prerequisites

- A **C++17**-compatible compiler (e.g., GCC, Clang, MSVC).
- CMake (for build management).

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/mini-lisp.git
   cd mini-lisp
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the interpreter:
   ```bash
   ./mini-lisp
   ```

---

## Usage

After starting the interpreter, you can enter Lisp expressions in the REPL. For example:

```lisp
> (+ 2 3)
5

> (define square (lambda (x) (* x x)))
square

> (square 4)
16
```

### Syntax Overview

- **Atoms**: Numbers (`42`), symbols (`x`), strings (`"hello"`).
- **Lists**: `(1 2 3)` or nested expressions like `(+ 1 (* 2 3))`.
- **User-defined Functions**:
  ```lisp
  (define func-name (lambda (args) body))
  (func-name args)
  ```

---

## Example

### Multi-line Input with Highlighting

```lisp
> (define
  square
  (lambda (x) (* x x)))
square

> (square 4)
16
```
