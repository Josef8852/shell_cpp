<div align="center">

[![progress-banner](https://backend.codecrafters.io/progress/shell/ea2824aa-a592-49a4-ac6c-b0f1c2a2f61d)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

<h1>🐚 Shell — Build Your Own Shell in C++</h1>

<p>A fully functional, POSIX-compliant shell written from scratch in <strong>C++23</strong>.</p>

<img src="https://skills.syvixor.com/api/icons?i=cpp,cmake,bash" alt="Tech Stack"/>

<br/>
<br/>

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-23-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/23)
[![Build System](https://img.shields.io/badge/CMake-3.13+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![readline](https://img.shields.io/badge/GNU%20readline-8.x-A42E2B?style=for-the-badge&logo=gnubash&logoColor=white)](https://tiswww.case.edu/php/chet/readline/rltop.html)
[![Platform](https://img.shields.io/badge/platform-Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)](https://kernel.org/)
[![CodeCrafters](https://img.shields.io/badge/CodeCrafters-Shell%20Challenge-orange?style=for-the-badge)](https://app.codecrafters.io/courses/shell/overview)

</div>

---

## What Is This?

This project implements a Unix shell — the kind of program you interact with every day in a terminal — entirely from scratch in C++. No shortcuts, no shell libraries: just raw `fork`, `execv`, file descriptors, and a hand-written parser.

Built as part of the [CodeCrafters "Build Your Own Shell"](https://app.codecrafters.io/courses/shell/overview) challenge, this shell handles real-world shell mechanics: quoting rules, I/O redirection, tab completion, background jobs, and more.

---

## Features

| Category | Details |
|---|---|
| **REPL** | Interactive read-eval-print loop powered by GNU readline |
| **Built-in Commands** | `echo`, `exit`, `pwd`, `cd`, `type`, `complete`, `jobs` |
| **External Programs** | Resolves executables from `$PATH` and runs them via `fork` / `execv` |
| **Quoting** | Single quotes, double quotes, and backslash (`\`) escaping |
| **Stdout Redirection** | `>` / `1>` (truncate) and `>>` / `1>>` (append) |
| **Stderr Redirection** | `2>` (truncate) and `2>>` (append) |
| **Home Directory** | `cd ~` and bare `cd` both navigate to `$HOME` |
| **Tab Completion** | Completes built-ins, `$PATH` executables, and filesystem paths (directories get a trailing `/`) |
| **Job Tracking** | `jobs` lists running background processes |
| **Custom Completions** | Register per-command completions with the `complete` built-in |

---

## Demo

```
$ ./your_program.sh

myshell> echo "Hello, World!"
Hello, World!

myshell> type echo
echo is a shell builtin

myshell> type ls
ls is /usr/bin/ls

myshell> pwd
/home/user/projects/shell

myshell> cd ~
myshell> pwd
/home/user

myshell> ls src/ 2> /dev/null
main.cpp  shell.cpp  shell.h

myshell> echo foo >> output.txt
myshell> cat output.txt
foo

myshell> jobs
[1] running   sleep 10

myshell> exit 0
```

---

## Project Structure

```
.
├── src/
│   ├── main.cpp       # Entry point — constructs and runs the Shell
│   ├── shell.h        # Shell class declaration, structs, and static state
│   └── shell.cpp      # Full Shell implementation
├── CMakeLists.txt
├── your_program.sh    # Launcher script used by CodeCrafters
└── vcpkg.json         # Dependency manifest
```

### Architecture Overview

```
Input (readline)
      │
      ▼
  tokenize()          ← handles quotes, backslash escaping, word splitting
      │
      ▼
 parseRedirects()     ← strips redirection tokens, builds ParsedCommand
      │
      ├─── built-in? ──► handleEcho / handleType / changeDirectory / …
      │
      └─── external? ──► findExecutableInPath() → fork() + execv()
                              │
                         applyRedirect()     ← dup2, open file descriptors
                         restoreRedirect()   ← restore saved fds after exec
```

---

## Build & Run

### Prerequisites

- CMake ≥ 3.13
- A C++23-capable compiler (GCC 12+ or Clang 16+)
- `libreadline-dev`

```sh
# Ubuntu / Debian
sudo apt install cmake g++ libreadline-dev
```

### Build

```sh
cmake -S . -B build
cmake --build build
```

### Run

```sh
./your_program.sh
```

---

## Submit to CodeCrafters

```sh
git add .
git commit -m "your message"
git push origin master
# or use the CLI:
codecrafters submit
```

---

## How It Works — Key Concepts

**Parsing** — the tokenizer walks the input character-by-character, respecting single-quote verbatim mode, double-quote escape mode, and bare backslash escapes before splitting on whitespace.

**Redirection** — before running any command, `applyRedirect()` saves the current stdout/stderr file descriptors with `dup`, opens the target file, and `dup2`s it into place. After the command finishes, `restoreRedirect()` puts the originals back.

**Tab completion** — a static `Completer` callback is registered with readline. On first invocation (`state == 0`) it builds a match list from built-ins, all executables in `$PATH`, and filesystem entries; subsequent calls iterate through the list.

**Job tracking** — background processes are stored in a map keyed by PID; `jobs` prints their status, and `SIGCHLD` reaping keeps the list clean.

---

<div align="center">

Built with care using C++23 · Part of the [CodeCrafters](https://codecrafters.io) challenge series

If this helped you understand shell internals, consider leaving a ⭐

</div>
