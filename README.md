# Minishell
![Language](https://img.shields.io/badge/Language-C-blue)
![42](https://img.shields.io/badge/Made%20at-42%20Heilbronn-blue?style=flat)

A simplified UNIX shell implementation in C, recreating the basic functionality of bash (version 3.2). This project is part of the 42 School curriculum and focuses on understanding process management, parsing, and UNIX system calls.

## 📖 Overview

Minishell is a lightweight shell that implements core shell features including command execution, piping, redirection, built-in commands, and environment variable handling. The shell provides an interactive command-line interface with readline support for enhanced user experience.

## ✨ Features

### Core Shell Functionality
- **Interactive command line** with readline support
- **Command execution** with PATH resolution
- **Command history** management
- **Prompt customization** with current directory display
- **Signal handling** (Ctrl+C, Ctrl+D, Ctrl+\)

### Built-in Commands
- `echo` - Display text with -n option support
- `cd` - Change directory with relative/absolute paths
- `pwd` - Print current working directory
- `export` - Set environment variables
- `unset` - Remove environment variables
- `env` - Display environment variables
- `exit` - Exit the shell with optional exit code

### Advanced Features
- **Pipes** (`|`) - Chain commands together
- **Input/Output Redirection**:
  - `<` - Input redirection
  - `>` - Output redirection
  - `>>` - Append output redirection
  - `<<` - Heredoc (here-document)
- **Variable Expansion**:
  - Environment variables (`$VAR`)
  - Exit status (`$?`)
  - Home directory expansion (`~`)

### Error Handling
- Comprehensive syntax validation
- Proper error messages and exit codes
- Memory leak prevention

## 🚀 Getting Started

### Prerequisites
- GCC compiler
- Make
- Readline library
- UNIX-like operating system (Linux/macOS)

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Castronela/minishell.git
   cd minishell
   ```

2. **Install readline library** (if not already installed):
   
   **On Ubuntu/Debian**:
   ```bash
   sudo apt-get install libreadline-dev
   ```
   
   **On macOS**:
   ```bash
   brew install readline
   ```

3. **Compile the project**:
   ```bash
   make
   ```

### Usage

**Start the shell**:
```bash
./minishell
```

**Example commands**:
```bash
# Basic commands
ls -la
echo "Hello World"
pwd

# Piping
ls | grep .c | wc -l

# Redirection
echo "test" > output.txt
cat < input.txt
echo "append" >> output.txt

# Heredoc (one line at a time)
cat << EOF
This is a heredoc
Multiple lines supported
EOF

# Variable expansion
export MY_VAR="test"
echo $MY_VAR
echo "Exit status: $?"

# Built-ins
cd /tmp
export PATH="/usr/bin:$PATH"
env | grep PATH
```

## 🛠 Build Targets

```bash
make              # Compile the project
make clean        # Remove object files
make fclean       # Remove all generated files
make re           # Rebuild everything
make norm         # Check 42 norm compliance
make valgrind     # Run with valgrind for memory debugging
```

## 📋 Technical Implementation

1. **Parser** (`src_parse/`):
   - Tokenization of input
   - Syntax validation
   - Command structure building
   - Variable expansion
   - Quote processing

2. **Executor** (`src_exe/`):
   - Process management
   - Pipe handling
   - I/O redirection
   - Built-in command execution
   - Error handling

## 🤝 Contributing

This is an educational project for 42 School. While contributions are not expected, suggestions and feedback are welcome.

### Code Standards
- Follow 42 School norm
- Maximum 25 lines per function
- Maximum 80 characters per line
- Comprehensive error handling
- Memory leak prevention

## 👥 Authors

- **dstinghe** - [GitHub](https://github.com/Castronela)
- **pamatya** - [GitHub](https://github.com/nuz8)
