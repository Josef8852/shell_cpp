[![progress-banner](https://backend.codecrafters.io/progress/shell/ea2824aa-a592-49a4-ac6c-b0f1c2a2f61d)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

# Build Your Own Shell — C++

A POSIX-compliant shell built in C++ as part of the [CodeCrafters "Build Your Own Shell"](https://app.codecrafters.io/courses/shell/overview) challenge.

## Features

- **REPL loop** — reads, parses, and executes commands interactively
- **Built-in commands** — `echo`, `exit`, `pwd`, `cd`, `type`
- **External program execution** — resolves executables from `$PATH` and runs them via `fork`/`execv`
- **Quoting** — single quotes, double quotes, and backslash escaping
- **Stdout redirection** — `>` and `1>` operators
- **Home directory** — `cd ~` and bare `cd` navigate to `$HOME`

## Project Structure

```
src/
  main.cpp      # entry point
  shell.h       # Shell class declaration
  shell.cpp     # Shell class implementation
```

## Build & Run

Requires `cmake` and a C++17-capable compiler.

```sh
# Build
cmake -S . -B build && cmake --build build

# Run
./your_program.sh
```

## Submit to CodeCrafters

```sh
codecrafters submit
```
