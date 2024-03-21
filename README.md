# 42_pipex

<p align="center">
    <img src="https://github.com/alx-sch/42_pipex/assets/134595144/292c30ab-c530-4977-b4ec-863d889eca6b" alt="born2beroot_badge.png" />
</p>

Pipex mimics the functionality of the shell pipe command '` | `' : Executing `./pipex infile cmd1 cmd2 outfile` emulates the behavior of `< infile cmd1 | cmd2 > outfile`. It facilitates the connection of the standard output of one command to the standard input of another command, creating a pipeline for data flow between commands executed within separate processes.

## Features
- **[Command Execution](https://github.com/alx-sch/42_pipex/edit/main/README.md#command-execution):** Utilizing the PATH environmental variable to execute commands via `execve()`.
- **Process Management:** Creating new child processes and establishing inter-process communication via `fork()`, `pipe()`, `dup2()`, and `waitpid()`.
- **Error Handling:** Ensuring robustness by implementing mechanisms to protect the program from unexpected behavior and failure, using `perror()`, `strerror()`, and `errno`.
- **Imitating Shell Behavior:** Replicating the behavior of the shell (zsh) as closely as possible.

## Command Execution

### The PATH Environmental Variable

Environmental variables, essential elements of the operating system's environment, store information utilized by various processes and applications to configure their behavior and access system resources.

For example, common commands such as 'grep', 'ls', or 'cat' are exectuable files stored within the system. To determine the exact path(s) to a specific command, you can use `which` in bash or `which` in zsh, followed by the command name, such as `which grep` or `which ls` (there might be more than one location where the executable is stored).

When calling a command, the terminal shell checks the PATH environment variable. This variable contains a list of directories where the operating system searches to find the executable file corresponding to the given command.

To view a list of all environmental variables and their values, you can execute the `env` command in the terminal. This command displays a list like this (excerpt):

```bash
[...]
LANGUAGE=en
USER=aschenk
SHELL=/bin/zsh
[...]
PATH=/home/aschenk/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
```
You can access the list of environmental variables within your C program by including `char **envp` as the third argument to the main function, e.g.: `int main(int argc, char **argv, char **envp)`. This allows you to directly access the array of environmental variables from within your program and to pass it to `execve()`.

To understand how Pipex retrieves the path to a specified command, please refer to the [`get_command_path()` function](https://github.com/alx-sch/42_pipex/blob/main/src/call_cmd.c).

### The Execve() System Call

So far so good – but why is it necessary to create multiple processes to execute multiple commands? Theoretically, you could save the output of a command in a variable and pass this as an input for another command, couldn't you? Such "command chaining" works in shell scripting, e.g.:   
```bash
output_of_command1=$(command1)
command2 "$output_of_command1"
```
However, in C, you would use the system call `execve()` for this purpose (***exec***ute with ***v***ector of ***e***nvironment variables): 
```C
int execve(const char *path, char **const argv, char **const envp)
```
- **const char \*path:** Represents the path to the command executable, e.g. `/usr/bin/ls`.
- **char \*\*const argv:** : Represents the command arguments in a NULL-terminated char array, e.g. `{"ls", "-l", NULL}`.
- **char \*\*const envp:** Represents the list of environmental variables.

`execve()` behaves uniquely by replacing the current process upon executing the command and does not return to the original process after successful execution. This means that once `execve()` is called successfully (not returning -1), any code after the `execve()` call is not executed.

To execute commands with input/output redirection, such as `cmd1 < infile | cmd2 > outfile`, at least two commands need to be processed. This entails utilizing the `execve()` system call multiple times, as each invocation of `execve()` replaces the current process image with the command execution process. The management of multiple processes is achieved through the `fork()` system call, which creates child processes. Inter-process communication is established using `pipe()`, allowing data to flow from the output of one command to the input of another. 

## Pipe() & Fork(): Creating and Managing Child Processes




## Pipex vs Shell

Using Z Shell (zsh).

Comparison (output shell + output pipex):

#### Invalid Input on Left Side
- single invalid input:
	- infile not existent
 	- infile no access
  	- invalid command
  	- invalid command option
  	- infile not existent & invalid command
    	
	![Screenshot from 2024-03-15 11-58-13](https://github.com/alx-sch/42_pipex/assets/134595144/adc7bef6-7b0e-43c0-81de-69c66dac90ce)

   

	- Note: The color-coded output signals 'success' (blue)! -> left side is handled in a process that does NOT report the EXIT status to the parten.
  	- Note: If multiple invalid comments: Only the file-related issue is addressed, not the invalid command -> process exits after file access fails.
  	- Note: An empty outfile.txt is created ('rw-r--r-- permissions) even if the pipe call failed. 
	![Screenshot from 2024-03-15 12-05-14](https://github.com/alx-sch/42_pipex/assets/134595144/55bae8f7-b962-491f-86bc-416e8d88535f)

 
#### Invalid Input on Right Side   
- Same as above for the left side BUT color-coded output signals 'error' (red) -> right side is handled in a process that reports EXIT status to partent.    
	![Screenshot from 2024-03-15 12-14-40](https://github.com/alx-sch/42_pipex/assets/134595144/e65c2b17-c7b5-4b8e-a6fa-527e6a63d1f2)


#### Invalid Input on Both Sides
- Same as above, error messages for both sides are printed out -> processes handling each side run paralelly; having one exit does not result in the other process not being executed.
  ![Screenshot from 2024-03-21 18-01-18](https://github.com/alx-sch/42_pipex/assets/134595144/bcc7a7cd-c3f1-44e1-b260-ce3f0f896be2)



#### Proccesses run parallely
 ![Screenshot from 2024-03-21 18-04-44](https://github.com/alx-sch/42_pipex/assets/134595144/4714b3ff-dd0a-429b-a8e5-b55797dfd5e9)
 ![Screenshot from 2024-03-21 18-28-43](https://github.com/alx-sch/42_pipex/assets/134595144/90b97a9b-a680-4337-84a6-94b823ca19f2)

![Screenshot from 2024-03-21 18-10-38](https://github.com/alx-sch/42_pipex/assets/134595144/076bb83e-27a2-448a-93b5-4a92783c69b3)

#### Outfile overwrites Infile
- Let's say you want to count the words in a file and store the results in another, but use the same file as input and output `< infile.txt wc -w | cat > infile.txt`, the result would be file with '0' in it -> The 'outfile' is created first as an empty file (overwriting the actual 'infile') and THEN process are initiated.

#### Parent waits for the last process to finish
- `< infile.txt | head > outfile.txt` `./pipex infile.txt yes head outfile.txt ` 

