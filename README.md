# 42_pipex

<p align="center">
    <img src="https://github.com/alx-sch/42_pipex/assets/134595144/292c30ab-c530-4977-b4ec-863d889eca6b" alt="born2beroot_badge.png" />
</p>

Pipex mimics the functionality of the shell pipe command '` | `' : Executing `./pipex infile cmd1 cmd2 outfile` emulates the behavior of `< infile cmd1 | cmd2 > outfile`. It facilitates the connection of the standard output of one command to the standard input of another command, creating a pipeline for data flow between commands executed within separate processes.

## Features
- **Command Execution:** Utilizing the PATH environmental variable to execute commands via `execve()`.
- **Process Management:** Creating new child processes and establishing inter-process communication via `fork()`, `pipe()`, `dup2()`, and `waitpid()`.
- **Error Handling:** Ensuring robustness by implementing mechanisms to protect the program from unexpected behavior and failure, using `perror()`, `strerror()`, and `errno`.
- **Imitating Shell Behavior:** Replicating the behavior of the shell (zsh) as closely as possible.

## Command Execution

### The PATH Environmental Variable

Environmental variables, essential elements of the operating system's environment, store information utilized by various processes and applications to configure their behavior and access system resources.

For example, commands such as 'grep', 'ls', or 'cat' all reside within the system as executable files. You can check the exact path(s) to a specific command with `which grep` or `which ls` (there might be more than one location where the executable is stored). 

All possible paths where the system searches for command executables are stored within the environmental variable 'PATH'. Whenever you enter a command in the terminal, the system checks through these locations. For instance, you might have already been asked to add a directory containing a newly installed tool to the PATH variable, which would enable easy execution by simply typing in a command.

You can retrieve a list of all environmental variables and their values by calling `env` in the terminal. This list appears as follows (excerpt):

```bash
[...]
LANGUAGE=en
USER=aschenk
SHELL=/bin/zsh
[...]
PATH=/home/aschenk/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
```

Each directory in the PATH variable is separated by a colon (:) on Unix-like systems (such as Linux and macOS)




### The Execve() System Call


execve — execute with vector of environment variables


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
  ![Screenshot from 2024-03-18 13-21-44](https://github.com/alx-sch/42_pipex/assets/134595144/247f9656-74f2-46f5-bec9-a8850e817081)


#### Proccesses run parallely
- no sleep    
![Screenshot from 2024-03-18 13-29-56](https://github.com/alx-sch/42_pipex/assets/134595144/00dcb8e8-cb5a-44d9-b1f6-7801c37f1e86)


- wait 10 microseconds before calling pipeline_right()   
![Screenshot from 2024-03-18 13-30-24](https://github.com/alx-sch/42_pipex/assets/134595144/b9dfb567-bf1b-4aaa-ac21-41fbf940a3af)

- wait 100 microseconds before calling pipeline_right()
  ![Screenshot from 2024-03-18 13-31-15](https://github.com/alx-sch/42_pipex/assets/134595144/a5b59adb-5453-47d7-9da3-adfddb79aaef)


  

#### Same file as input and output
- Let's say you want to count the words in a file and store the results in another, but use the same file as input and output `< infile.txt wc -w | cat > infile.txt`, the result would be file with '0' in it -> The 'outfile' is created first as an empty file (overwriting the actual 'infile') and THEN process are initiated.

#### Parent waits for the last process to finish
- `< infile.txt | head > outfile.txt` `./pipex infile.txt yes head outfile.txt ` 

