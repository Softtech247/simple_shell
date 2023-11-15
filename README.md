# 0x16. Custom Shell (Simple Shell)

# By Stephen Kalu and Joy Ogumba


A custom Unix shell project developed for Holberton School.

## Learning Objectives

At the end of this project, you will be able to explain to anyone, without the help of Google:

- The original designers and implementers of the Unix operating system.
- Who wrote the first version of the UNIX shell.
- The creator of the B programming language (the direct predecessor to the C programming language).
- The significance of Ken Thompson in the development of Unix.
- How a shell works and its role in the Unix operating system.
- The concepts of pid (process ID) and ppid (parent process ID).
- How to manipulate the environment of the current process.
- The difference between a function and a system call.
- How to create processes in a Unix-like system.
- The three prototypes of the main function.
- How the shell uses the PATH variable to locate executable programs.
- How to execute another program using the execve system call.
- How to suspend the execution of a process until one of its child processes terminates.
- What EOF ("end-of-file") means in the context of input/output.

## Requirements

### General

- You are allowed to use editors like vi, vim, or emacs for your code.
- All your source code files should be compiled using gcc on Ubuntu 20.04 LTS.
- Use the following compilation flags: -Wall -Werror -Wextra -pedantic -std=gnu89.
- All your code files must end with a new line.
- Include a README.md file at the root of your project folder.
- Ensure your code follows the Betty style, and it will be checked using betty-style.pl and betty-doc.pl.
- Your custom shell should not have any memory leaks.
- Each source file should contain no more than five functions.
- Make use of include guards in your header files.
- Use system calls only when necessary.

## GitHub

- Each team should create one project repository on GitHub.
- Add your partner as a collaborator to the repository.

## Output

Unless specified otherwise, your shell must produce the exact same output as /bin/sh, including error messages. The only difference should be in the program name when printing errors (it should match your argv[0]).

Example error with /bin/sh:

\```shell
$ echo "qwerty" | /bin/sh
/bin/sh: 1: qwerty: not found
$ echo "qwerty" | /bin/../bin/sh
/bin/../bin/sh: 1: qwerty: not found
\```

The same error with your custom shell (sj_hsh):

\```shell
$ echo "qwerty" | ./sj_hsh
./sj_hsh: 1: qwerty: not found
$ echo "qwerty" | ./././sj_hsh
./././sj_hsh: 1: qwerty: not found
\```

## List of Allowed Functions and System Calls

You are allowed to use the following functions and system calls:

- access (man 2 access)
- chdir (man 2 chdir)
- close (man 2 close)
- closedir (man 3 closedir)
- execve (man 2 execve)
- exit (man 3 exit)
- _exit (man 2 _exit)
- fflush (man 3 fflush)
- fork (man 2 fork)
- free (man 3 free)
- getcwd (man 3 getcwd)
- getline (man 3 getline)
- getpid (man 2 getpid)
- isatty (man 3 isatty)
- kill (man 2 kill)
- malloc (man 3 malloc)
- open (man 2 open)
- opendir (man 3 opendir)
- perror (man 3 perror)
- read (man 2 read)
- readdir (man 3 readdir)
- signal (man 2 signal)
- stat (__xstat) (man 2 stat)
- lstat (__lxstat) (man 2 lstat)
- fstat (__fxstat) (man 2 fstat)
- strtok (man 3 strtok)
- wait (man 2 wait)
- waitpid (man 2 waitpid)
- wait3 (man 2 wait3)
- wait4 (man 2 wait4)
- write (man 2 write)

## Compilation

To compile your custom shell, use the following command:

\```shell
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o sj_hsh
\```

## Files

- README.md: A description of the project repository.
- man_1_simple_shell: The man page for the custom shell.
- AUTHORS: A file listing all individuals who have contributed content to the repository.
- main.h: The header file that contains standard headers and function prototypes used in the program.
- main.c: Initializes the program with an infinite loop by calling the prompt function.
- prompt.c: Uses the getline system call to read input from the user and runs an infinite loop with fork to maintain the command prompt.
- special_character.c: Identifies special inputs, such as a leading slash, user-typed exit or env, etc.
- string.c: Handles various string operations, including string length, writing strings, finding strings in directories, concatenating strings, etc.
- cmd.c: Finds the command entered by the user.
- execute.c: Executes the identified command.

## How to Add an Author File

To generate a list of authors in a Git repository, you can use the following Bash script:

\```shell
#!/bin/sh

git shortlog -se \\
  | perl -spe 's/^\s+\d+\s+//' \\
  | sed -e '/^CommitSyncScript.*$/d' \\
  > AUTHORS
\```



vi README.md
