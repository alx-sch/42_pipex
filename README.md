# 42_pipex

<p align="center">
    <img src="https://github.com/alx-sch/42_pipex/assets/134595144/292c30ab-c530-4977-b4ec-863d889eca6b" alt="born2beroot_badge.png" />
</p>

OVERVIEW PROJECT

## Features
- **XXX:** XX
- **XXX:** XX
- **XXX:** XX

## Environmental Variables

Printing out environmental variables (provided by system)
```C
int	main(int argc, char *argv[], char *env[])
{
	int	i;

	i = 0;
	(void)argc,
	(void)argv;
	while (env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
```
The PATH environment variable contains a list of directories where executable files are located. When you execute a command in the shell, the operating system searches through the directories listed in the PATH variable to find the corresponding executable file.

Each directory in the PATH variable is separated by a colon (:) on Unix-like systems (such as Linux and macOS)
```bash
MAIL=XXX@student.42berlin.de
LANGUAGE=en
USER=XXX
[...]
PATH=/home/aschenk/bin:/home/aschenk/bin:/usr/local/sbin:[...]
```

## Pipex vs Shell

Using Z Shell (zsh).

Comparison (output shell + output pipex):

#### Invalid Input of left side
- single invalid input:
	- infile not existent
 	- infile no access
  	- invalid command
  	- invalid command option    	

	![Screenshot from 2024-03-12 13-35-28](https://github.com/alx-sch/42_pipex/assets/134595144/aad56d51-8ba1-43ff-a25d-b82fd7d2575b)

	- Note: The color-coded output signals 'success' (blue)! -> left side is handled in a separate progress (and EXIT status is not considered in parent).

- single invalid input:
	- infile not existent & invalid command   
	![left_pipe_d](https://github.com/alx-sch/42_pipex/assets/134595144/f2a96f25-2599-474d-999f-0c0834367185)

	- Note: Only the file-related issue is addressed, not the invalid command.

