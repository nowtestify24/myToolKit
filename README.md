MyToolkit.x
Overview
MyToolkit.x is a comprehensive suite of utilities designed for file management, data processing, and I/O management. Built using C++ in Unix environments, this toolkit aims to provide efficient and user-friendly tools for various system operations.

Important Notes
File Input Redirection: Redirecting a file for input during execution causes an infinite loop.

Command Execution: Commands must be typed individually. This may be due to the lack of Control-D signal handling.

Control-D Signal Handling: Control-D signal handling is not implemented in this version.

Features
Executable Files: All executables work as intended. Executables such as a.out or those named test require ./a.out or ./test to function properly.

Single and Multiple Piping: Single piping is its own case. Multiple pipes are supported, with a recommended maximum of four pipes.

Input/Output Redirection: Supported commands for input/output redirection include:

command1 < test1.txt

command1 > test1.txt

command1 < test1.txt > test2.txt

No other I/O redirection styles are implemented.

Command Usage
Space Separation: Ensure all commands are separated by spaces, such as command1 -args or in cases of piping: ls | sort | wc or I/O redirect: command1 > test.txt.

Implementation Details
Fork/Execvp Usage: The program uses fork and execvp to create new processes and run them within the program rather than creating new external ones. This approach was deemed practical and provided a useful learning experience with fork and execvp.

Special Commands: Implemented special commands include mycd, mypwd, and myexit using the same fork/execvp approach.
