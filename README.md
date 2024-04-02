# WIP!! 42_pipex WIP!!

<p align="center">
    <img src="https://github.com/alx-sch/42_pipex/assets/134595144/292c30ab-c530-4977-b4ec-863d889eca6b" alt="born2beroot_badge.png" />
</p>

Pipex mimics the functionality of the shell pipe command '` | `' by executing `./pipex infile cmd1 cmd2 outfile`, which emulates the behavior of `< infile cmd1 | cmd2 > outfile`. It facilitates the connection of the standard output of one command to the standard input of another command, creating a pipeline for data flow between commands executed within separate processes.

## Features
- **[Command Execution](https://github.com/alx-sch/42_pipex/tree/main?tab=readme-ov-file#command-execution):** Utilizing the PATH environmental variable to execute commands via `execve()`.
- **[Process Management](https://github.com/alx-sch/42_pipex/tree/main?tab=readme-ov-file#creating-and-managing-mutiple-processes):** Creating new child processes and establishing inter-process communication via `fork()`, `waitpid()`, `pipe()`, and `dup2()`.
- **Error Handling:** Ensuring robustness by implementing mechanisms to protect the program from unexpected behavior and failure, using `perror()`, `strerror()`, and `errno`.
- **Imitating Shell Behavior:** Replicating the behavior of the shell (zsh) as closely as possible.

## Command Execution

#### The PATH Environmental Variable

Environmental variables are essential elements of the operating system's environment. They store information that various processes and applications utilize to configure their behavior and access system resources.

For example, common commands such as 'grep', 'ls', or 'cat' are exectuable files stored within the system. To determine the exact path(s) to a specific command, you can use `which` in bash or `where` in zsh, followed by the command name, such as `which grep` or `which ls`. 

When calling a command, the terminal shell checks the PATH environment variable. This variable contains a list of directories, delimited by colons, where the operating system searches to find the executable file corresponding to the given command.

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

So far so good – but why is it necessary to create multiple processes to execute multiple commands? Theoretically, you could save the output of a command in a variable and pass this as an input for another command, couldn't you? Such "command chaining" does work in shell scripting, e.g.:   
```bash
output_of_command1=$(< infile.txt command1) 
command2 "$output_of_command1" > outfile.txt
```
However, in C, you would use the system call `execve()` for this purpose (***exec***ute with ***v***ector of ***e***nvironment variables): 
```C
int execve(const char *path, char **const argv, char **const envp)
```
- const char \*path: Represents the path to the command executable, e.g. `/usr/bin/ls`.
- char \*\*const argv: Represents the command arguments in a NULL-terminated char array, e.g. `{"ls", "-l", NULL}`.
- char \*\*const envp: Represents the list of environmental variables.

`execve()` behaves uniquely by loading and executing a new program (the command), effectively replacing the current process when called. It does not return to the original process after successful execution. This means that once `execve()` is called successfully (not returning -1), any code after the `execve()` call is not executed.

So, to execute commands with input/output redirection, such as `cmd1 < infile | cmd2 > outfile`, each command execution requires a separate call to `execve()`. Since `execve()` replaces the current process, one process per command is necessary. The creation of additional processes is achieved through `fork()`. To enable communication between these processes, `pipe()` is used, which establishes a unidirectional communication channel.

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
 <div style="display: block;">
	<img width="600" alt="fork_flow" src="https://github.com/alx-sch/42_pipex/assets/134595144/8ab2944e-ab9c-4109-831e-7643ac78b17b">   
 </div>

 <div style="display: block;">
	<img width="500" alt="fork_printout" src="https://github.com/alx-sch/42_pipex/assets/134595144/91e40035-99ba-44c2-8e80-6531e6b19f64">  
</div>

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
<div style="display: block;">
<img width="710" alt="fork_sleep_flow" src="https://github.com/alx-sch/42_pipex/assets/134595144/819b6dc3-d43c-46e0-846a-0f71edf4bae6">
</div>

<div style="display: block;">
<img width="500" alt="fork_sleep_printout" src="https://github.com/alx-sch/42_pipex/assets/134595144/a6da9d1d-9a0d-45c2-932d-143db67456e1">  
</div>

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
<div style="display: block;">
<img width="830" alt="fork_sleep_printout" src="https://github.com/alx-sch/42_pipex/assets/134595144/af27ff0b-f249-40d6-886f-cfb8f3d9d6d7">  
</div>

<div style="display: block;">
<img width="500" alt="fork_sleep_printout" src="https://github.com/alx-sch/42_pipex/assets/134595144/7ba4e2cb-c024-4b52-ae9a-6d4d90fce39e">  
</div>

### Pipe()
```C
#include <unistd.h> // pipe(), read(), write()
#include <stdio.h> // printf()
#include <string.h> // strlen()

int	main(void)
{
	int	pipe_fd[2];
	pid_t	child_pid;
	char	message[] = "Hello from the child!";
	char	buffer[42];

	pipe(pipe_fd); // Pipe initialization
	child_pid = fork();

	if (child_pid == 0) // Child process
	{
		close(pipe_fd[0]); // Close the read end of the pipe
		write(pipe_fd[1], message, strlen(message) + 1);
		close(pipe_fd[1]); // Close the write end of the pipe
	}
	else // Parent process
	{
		close(pipe_fd[1]); // Close the write end of the pipe
		read(pipe_fd[0], buffer, sizeof(buffer));
		printf("The child says: %s\n", buffer);
		close(pipe_fd[0]); // Close the read end of the pipe
	}

	return (0);
}
```

```C
#include <unistd.h> // pipe(), read(), write()
#include <stdio.h> // printf()

int	main(void)
{
	int		pipe_fd[2];
	pid_t	child_pid;
	char	buffer[42];

	pipe(pipe_fd); // Pipe initialization
	child_pid = fork();

	if (child_pid == 0) // Child process
	{
		close(pipe_fd[0]); // Close the read end of the pipe
		dup2(pipe_fd[1], 1); // Redirect stdout (fd = 0) to the write end of the pipe
		close(pipe_fd[1]); // Close the write end of the pipe
		printf("Hello from the child! PID: %d\n", child_pid); // Prints message into the pipe (as stdout is redirected)
	}
	else // Parent process
	{
		close(pipe_fd[1]); // Close the write end of the pipe
		read(pipe_fd[0], buffer, sizeof(buffer));
		close(pipe_fd[0]); // Close the read end of the pipe
		printf("The child says: %s", buffer); //
	}

	return (0);
}
```

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

