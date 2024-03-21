# 42_pipex

<p align="center">
    <img src="https://github.com/alx-sch/42_pipex/assets/134595144/292c30ab-c530-4977-b4ec-863d889eca6b" alt="born2beroot_badge.png" />
</p>

Pipex mimics the functionality of the shell pipe command '` | `' : Executing `./pipex infile cmd1 cmd2 outfile` emulates the behavior of `< infile cmd1 | cmd2 > outfile`. It facilitates the connection of the standard output of one command to the standard input of another command, creating a pipeline for data flow between commands executed within separate processes.

## Features
- **[Command Execution](https://github.com/alx-sch/42_pipex/tree/main?tab=readme-ov-file#command-execution):** Utilizing the PATH environmental variable to execute commands via `execve()`.
- **[Process Management](https://github.com/alx-sch/42_pipex/tree/main?tab=readme-ov-file#creating-and-managing-mutiple-processes):** Creating new child processes and establishing inter-process communication via `fork()`, `waitpid()`, `pipe()`, and `dup2()`.
- **Error Handling:** Ensuring robustness by implementing mechanisms to protect the program from unexpected behavior and failure, using `perror()`, `strerror()`, and `errno`.
- **Imitating Shell Behavior:** Replicating the behavior of the shell (zsh) as closely as possible.

## Command Execution

#### The PATH Environmental Variable

Environmental variables are essential elements of the operating system's environment. They store information that various processes and applications utilize to configure their behavior and access system resources.

For example, common commands such as 'grep', 'ls', or 'cat' are exectuable files stored within the system. To determine the exact path(s) to a specific command, you can use `which` in bash or `where` in zsh, followed by the command name, such as `which grep` or `which ls`. 

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
You can access the list of environmental variables within your C program by including `char **envp` as the third argument to the main function, e.g.: `int main(int argc, char **argv, char **envp)`. This allows you to directly access the array of environmental variables within your program and pass it to `execve()`.

To understand how Pipex retrieves the path to a specified command, please refer to the `get_command_path()` function [here](https://github.com/alx-sch/42_pipex/blob/main/src/call_cmd.c).

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

To execute commands with input/output redirection, such as `cmd1 < infile | cmd2 > outfile`, each command execution requires a separate call to `execve()`. Since `execve()` replaces the current process with the command execution, one process per command is necessary. The creation of additional processes is achieved through `fork()`. To enable communication between these processes, `pipe()` is used, which establishes a unidirectional communication channel.

## Creating and Managing Mutiple Processes

### Fork() & Waitpid()
Creating a new process is simply done by calling `fork()`, which creates two identical copies of the program's execution environment, with one being the parent (return value of `fork()` > 0) and the other being the child (return value of `fork()` = 0).

Let's look at a simple program using `fork()`:

```C
// fork.c

#include <stdio.h> // prinft()
#include <unistd.h> // fork(), usleep()
#include <sys/types.h> // pid_t: int or long representing process ID's (PIDs)

int	main(void)
{
	pid_t	child_pid;

	printf("Before the fork!\n");
	child_pid = fork();
	printf("After the fork! Child PID: %d\n", child_pid);
	if (child_pid == 0)
		printf("Hello from the child! Child PID: %d\n", child_pid);
	else
		printf("Hello from the parent! Child PID: %d\n", child_pid);

	return (0);
}
```
![Screenshot from 2024-03-21 20-03-23](https://github.com/alx-sch/42_pipex/assets/134595144/91e40035-99ba-44c2-8e80-6531e6b19f64)

"*Before the fork!*" is printed out once, before `fork()` is called. Then, "*After the fork!*" is printed out twice: Once by the parent process and once by the child process. This is because the `fork()` call creates a new process, resulting in two separate execution paths. In the parent process, it returns the process ID (PID) of the child process (> 0), while in the child process, it returns 0. This makes it possible to execute different tasks by distinguishing between the PIDs (`if (pid == 0)` for child process tasks and `else` for parent process tasks).    


#### Introducing sleep()
Note that the parent and child processes run in parallel, meaning they execute independently and their execution order is somewhat random. While it's not straightforward to predict the exact order in which they will execute, introducing delays using functions like `usleep()` can help synchronize their behavior to some extent:

```C
// sleep_fork.c

#include <stdio.h> // prinft()
#include <unistd.h> // fork(), usleep()
#include <sys/types.h> // pid_t: int or long representing process ID's (PIDs)

int	main(void)
{
	pid_t	child_pid;

	printf("Before the fork!\n");
	child_pid = fork();
	printf("After the fork! Child PID: %d\n", child_pid);
	usleep(10); // pause execution for 10 microseconds
	if (child_pid == 0)
		printf("Hello from the child! Child PID: %d\n", child_pid);
	else
		printf("Hello from the parent! Child PID: %d\n", child_pid);

	return (0);
}
```
![Screenshot from 2024-03-21 20-10-00](https://github.com/alx-sch/42_pipex/assets/134595144/a6da9d1d-9a0d-45c2-932d-143db67456e1)
  
#### Introducing waitpid()   

A more controlled way for synchronizing the execution order can be achieved with `waitpid()`. It halts the execution until the passed process terminates, allowing the parent process to wait for the completion of a specific child process before continuing its execution. `waitpid()` can also be used to retrieve and propagate the exit status of a child process (learn more [here](https://linux.die.net/man/2/waitpid)).
```C
// waitpid_fork.c

#include <stdio.h> // prinft()
#include <unistd.h> // fork(), usleep()
#include <sys/types.h> // pid_t: int or long representing process ID's (PIDs)
#include <sys/wait.h> // waitpid()

int	main(void)
{
	pid_t	child_pid;

	printf("Before the fork!\n");
	child_pid = fork();
	printf("After the fork! Child PID: %d\n", child_pid);
	usleep(10); // pause execution for 10 microseconds
	if (child_pid == 0)
		printf("Hello from the child! Child PID: %d\n", child_pid);
	else
	{
		waitpid(child_pid, NULL, 0); // waits for the child process to finish
		printf("Hello from the parent! Child PID: %d\n", child_pid);
	}
	return (0);
}
```
![Screenshot from 2024-03-21 20-29-16](https://github.com/alx-sch/42_pipex/assets/134595144/7ba4e2cb-c024-4b52-ae9a-6d4d90fce39e)

### Pipe()
ssdsdsd
sdsds
sdsd

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

