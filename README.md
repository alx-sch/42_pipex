# WIP!! 42_pipex WIP!!

Code: READY AND FINAL!    
README: WIP

![Screenshot from 2024-04-21 21-10-07](https://github.com/alx-sch/42_pipex/assets/134595144/350670ec-54ee-4629-94ab-0dcb1dd92a9b)


<p align="center">
    <img src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/pipexe.png" alt="pipex_badge.png" />
</p>

Pipex mimics the functionality of the shell pipe command '` | `' by executing `./pipex infile cmd1 cmd2 outfile`, which emulates the behavior of `< infile cmd1 | cmd2 > outfile`. It facilitates the connection of the standard output of one command to the standard input of another command, creating a pipeline for data flow between commands executed within separate processes.

## Features
- **[Command Execution](https://github.com/alx-sch/42_pipex/tree/main?tab=readme-ov-file#command-execution):** Utilizing the PATH environment variable to execute commands via `execve()`.
- **[Process Management](https://github.com/alx-sch/42_pipex/tree/main?tab=readme-ov-file#creating-and-managing-mutiple-processes):** Creating child processes and establishing inter-process communication via `fork()`, `waitpid()`, `pipe()`, and `dup2()`.
- **Error Handling:** Ensuring robustness by implementing mechanisms to protect the program from unexpected behavior and failure, using `perror()`, `strerror()`, and `errno`.
- **Imitating Shell Behavior:** Replicating the behavior of the shell as closely as possible (zsh).

## Command Execution

#### The PATH Environment Variable

Environment variables are essential elements of the operating system's environment. They store information that various processes and applications utilize to configure their behavior and access system resources.

For example, common commands such as 'grep', 'ls', or 'cat' are exectuable files stored within the system. To determine the exact path(s) to a specific command, you can use `which` in bash or `where` in zsh, followed by the command name, such as `which grep` or `which ls`. 

When calling a command, the terminal shell checks the PATH environment variable. This variable contains a list of directories, delimited by colons, where the operating system searches to find the executable file corresponding to the given command.

To view a list of all environment variables and their values, you can execute the `env` command in the terminal. This command displays a list like this (excerpt):

```bash
[...]
LANGUAGE=en
USER=aschenk
SHELL=/bin/zsh
[...]
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
```

In a C program, you can access the list of environment variables by including `char **envp` as the third argument to the main function, e.g. `int main(int argc, char **argv, char **envp)`. The envp parameter is structured as an array of strings in the format "VARIABLE=value", for example `envp = {"LANGUAGE=en", "PATH=/usr/local/sbin:[...]", "[...]", NULL}`.

To understand how Pipex retrieves the path to a specified command, please refer to the `get_command_path()` function [here](https://github.com/alx-sch/42_pipex/blob/main/src/call_cmd.c).

### The Execve() System Call

So far so good – but why is it necessary to create multiple processes to execute multiple commands? Theoretically, you could save the output of a command in a variable and pass this as an input for another command, couldn't you? Such "command chaining" does work in shell scripting, e.g.:   
```bash
output_of_command1=$(< infile.txt command1) 
command2 "$output_of_command1" > outfile.txt
```
However, in C, you would use a system call from the `exec()` family for this purpose ((for more information, see [here](https://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/4-ProcessBasic/exec-family.html)). As per project requirements, Pipex uses `execve()` (***exec***ute with ***v***ector of ***e***nvironment variables): 
```C
int execve(const char *path, char **const argv, char **const envp)
```
- const char \*path: Represents the path to the command executable, e.g. `/usr/bin/ls`.
- char \*\*const argv: Represents the command arguments in a NULL-terminated char array, e.g. `{"ls", "-l", NULL}`.
- char \*\*const envp: Represents the list of environment variables.

Members of the `exec()` family behave uniquely by loading and executing a new program (the command), effectively replacing the current process when called. They do not return to the original process after successful execution. This means that once `execve()` is called successfully (not returning -1), any following code is not executed.

So, to execute commands with input/output redirection, such as `cmd1 < infile | cmd2 > outfile`, each command execution requires a separate call to `execve()`. Since `execve()` replaces the current process, one process per command is necessary. The creation of additional processes is achieved through `fork()`. To enable communication between these processes, `pipe()` is used, which establishes a unidirectional communication channel.

## Creating and Managing Mutiple Processes

### Understanding Fork() & Waitpid()
Creating a new process is simply done by calling `fork()`, which creates two identical copies of the program's execution environment, with one being the parent (return value of `fork()` > 0) and the other being the child (return value of `fork()` = 0).

Let's look at a simple program using `fork()`:

```
DISCLAIMER:
Please note that the following code examples have been selected for relevance of explaining specific system calls,
they do not directly relate to the pipex project.
```

</details>

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
	if (child_pid == 0) // Child process
		printf("Hello from the child! Child PID: %d\n", child_pid);
	else // Parent process
		printf("Hello from the parent! Child PID: %d\n", child_pid);
			
	return (0);
}
```   
 <div style="display: block;">
	<img width="600" alt="fork_flow" src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/fork_dia.png">   
 </div>

 <div style="display: block;">
	<img width="420" alt="fork_prinout" src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/fork_exec.png">  
</div>

"*Before the fork!*" is printed out once, before `fork()` is called. Then, "*After the fork!*" is printed out twice: Once by the parent process and once by the child process. This is because the `fork()` call creates a new process, resulting in two separate execution paths. In the parent process, it returns the process ID (PID) of the child process (> 0), while in the child process, it returns 0. This makes it possible to execute different tasks by distinguishing between the PIDs (`if (pid == 0)` for child process tasks and `else` for parent process tasks).


#### Introducing sleep()
Note that the parent and child processes run in parallel, meaning they execute independently and their execution order is somewhat random. While it's not straightforward to predict the exact order in which they will execute, introducing delays using functions like `sleep() / usleep()` can help synchronize their behavior to some extent:

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
	usleep(10); // Pause execution for 10 microseconds
	if (child_pid == 0) // Child process
		printf("Hello from the child! Child PID: %d\n", child_pid);
	else // Parent process
		printf("Hello from the parent! Child PID: %d\n", child_pid);

	return (0);
}
```
<div style="display: block;">
<img width="710" alt="fork_sleep_flow" src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/usleep_dia.png">
</div>

<div style="display: block;">
<img width="500" alt="fork_sleep_printout" src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/usleep_exec.png">  
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
	usleep(10); // Pause execution for 10 microseconds
	if (child_pid == 0) // Child process
		printf("Hello from the child! Child PID: %d\n", child_pid);
	else // Parent process
	{
		waitpid(child_pid, NULL, 0); // waits for the child process to finish
		printf("Hello from the parent! Child PID: %d\n", child_pid);
	}
	return (0);
}
```
<div style="display: block;">
<img width="830" alt="fork_waitpid_diagram" src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/waitpid_dia.png">  
</div>

<div style="display: block;">
<img width="500" alt="fork_waitpid_printout" src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/waitpid_exec.png">  
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
	pid_t	received_child_pid;
	char	message[] = "Hello from the child! PID:";
	char	buffer[42];

	pipe(pipe_fd); // Pipe initialization
	child_pid = fork();

	if (child_pid == 0) // Child process
	{
		close(pipe_fd[0]); // Close the read end of the pipe
		write(pipe_fd[1], message, strlen(message) + 1); // Write message to the pipe
		write(pipe_fd[1], &child_pid, sizeof(pid_t)); // Write child PID to the pipe
		close(pipe_fd[1]); // Close the write end of the pipe
	}
	else // Parent process
	{
		close(pipe_fd[1]); // Close the write end of the pipe
		printf("Here is the pareny! PID: %d\n", child_pid);
		read(pipe_fd[0], buffer, sizeof(buffer)); // Read message from pipe
		read(pipe_fd[0], &received_child_pid, sizeof(pid_t)); // Read child PID from pipe
		printf("The child says: '%s %d'\n", buffer, received_child_pid);
		close(pipe_fd[0]); // Close the read end of the pipe
	}

	return (0);
}
```
<div style="display: block;">
<img width="930" alt="pipe_diagram"  src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/pipe_dia.png">
</div>

<div style="display: block;">
<img width="400" alt="pipe_printout" src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/pipe_exec.png">
</div>

#### Introducing dup2()
```C
#include <unistd.h> // pipe(), read()
#include <stdio.h> // printf()

int	main(void)
{
	int	pipe_fd[2];
	pid_t	child_pid;
	char	buffer[42];

	pipe(pipe_fd); // Pipe initialization
	child_pid = fork();

	if (child_pid == 0) // Child process
	{
		close(pipe_fd[0]); // Close the read end of the pipe
		dup2(pipe_fd[1], 1); // Redirect stdout (fd = 0) to the write end of the pipe
		// Now stdout is redirected to the pipe, so printf will write to the pipe
		printf("Hello from the child! PID: %d", child_pid);
		close(pipe_fd[1]); // Close the original write end of the pipe
	}
	else // Parent process
	{
		close(pipe_fd[1]); // Close the write end of the pipe
		printf("Here is the parent! PID: %d\n", child_pid);
		read(pipe_fd[0], buffer, sizeof(buffer)); // Read message from pipe
		printf("The child says: '%s'\n", buffer);
		close(pipe_fd[0]); // Close the read end of the pipe
	}

	return (0);
}
```
<div style="display: block;">
<img width="930" alt="pipe_dup2_flow"  src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/dup2_dia.png">
</div>

<div style="display: block;">
<img width="400" alt="pipe_dup2_printout" src="https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/dup2_exec.png">
</div>

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
    	
	![Screenshot from 2024-03-15 11-58-13](https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/comp_1.png)

   

	- Note: The color-coded output signals 'success' (blue)! -> left side is handled in a process that does NOT report the EXIT status to the parten.
  	- Note: If multiple invalid comments: Only the file-related issue is addressed, not the invalid command -> process exits after file access fails.
  	- Note: An empty outfile.txt is created ('rw-r--r-- permissions) even if the pipe call failed. 
	![Screenshot from 2024-03-15 12-05-14](https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/comp_2.png)

 
#### Invalid Input on Right Side   
- Same as above for the left side BUT color-coded output signals 'error' (red) -> right side is handled in a process that reports EXIT status to parent
     
![Screenshot from 2024-03-15 12-14-40](https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/comp_3.png)


#### Invalid Input on Both Sides
- Same as above, error messages for both sides are printed out -> processes handling each side run parallel; having one exit does not result in the other process not being executed.   
  ![Screenshot from 2024-03-21 18-01-18](https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/comp_4.png)
  

#### Proccesses run parallely
 ![Screenshot from 2024-03-21 18-04-44](https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/comp_5.png)
 ![Screenshot from 2024-03-21 18-28-43](https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/comp_6.png)

#### Outfile overwrites Infile
- Let's say you want to count the words in a file and store the results in another, but use the same file as input and output `< infile.txt wc -w | cat > infile.txt`, the result would be file with '0' in it -> The 'outfile' is created first as an empty file (overwriting the actual 'infile') and THEN process are initiated.

#### Parent waits for the last process to finish
![Screenshot from 2024-03-21 18-10-38](https://raw.githubusercontent.com/alx-sch/42_pipex/main/.assets/comp_7.png)

- `< infile.txt yes | head > outfile.txt` `./pipex infile.txt yes head outfile.txt `

## Acknowledgements
The project badge is from [this repository](https://github.com/ayogun/42-project-badges) by Ali Ogun.

