[![progress-banner](https://backend.codecrafters.io/progress/shell/ea2824aa-a592-49a4-ac6c-b0f1c2a2f61d)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

# Build Your Own Shell — C++

A POSIX-compliant shell built in C++ as part of the [CodeCrafters "Build Your Own Shell"](https://app.codecrafters.io/courses/shell/overview) challenge.

## Technologies



  <img src="https://skills.syvixor.com/api/icons?i=cpp,cmake,bash"/>
  <img src="https://img.shields.io/badge/GNU%20readline-8.x-A42E2B?logo=gnubash&logoColor=white" alt="readline" height="60"/>


## Features

- **REPL loop** — reads, parses, and executes commands interactively
- **Built-in commands** — `echo`, `exit`, `pwd`, `cd`, `type`, `complete`
- **External program execution** — resolves executables from `$PATH` and runs them via `fork`/`execv`
- **Quoting** — single quotes, double quotes, and backslash escaping
- **Stdout redirection** — `>` / `1>` (truncate) and `>>` / `1>>` (append) operators
- **Stderr redirection** — `2>` (truncate) and `2>>` (append) operators
- **Home directory** — `cd ~` and bare `cd` navigate to `$HOME`
- **Tab completion** — completes built-in commands, `$PATH` executables, and filesystem paths (with trailing `/` for directories)

## Project Structure

```
src/
  main.cpp      # entry point
  shell.h       # Shell class declaration
  shell.cpp     # Shell class implementation
```

## Build & Run

Requires `cmake`, a C++17-capable compiler, and `libreadline-dev`.

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
