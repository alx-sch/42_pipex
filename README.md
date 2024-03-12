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

#### Invalid Input on Left Side
- single invalid input:
	- infile not existent
 	- infile no access
  	- invalid command
  	- invalid command option
  	- infile not existent & invalid command    	

  ![left_pipe](https://github.com/alx-sch/42_pipex/assets/134595144/b64c8772-8e1c-4acd-9d68-8450d86f0934)
   ![Screenshot from 2024-03-12 15-40-20](https://github.com/alx-sch/42_pipex/assets/134595144/cd8219f5-eb17-4cf0-a390-9588adde7a93)

	- Note: The color-coded output signals 'success' (blue)! -> left side is handled in a process that does NOT report the EXIT status to the parten.
 	- Note: An empty outfile.txt is created.
  	- Note: If multiple invalid comments: Only the file-related issue is addressed, not the invalid command -> process exits after file access fails.
 
#### Invalid Input on Right Side   
- Same as above for the left side BUT color-coded output signals 'error' (red) -> right side is handled in a process that reports EXIT status to partent.    
	-![right_pipe](https://github.com/alx-sch/42_pipex/assets/134595144/831aabca-d00b-4afe-97f6-25865f59fe61)    

#### Invalid Input on Both Sides
- Same as above, error messages for both sides are printed out -> processes handling each side run paralelly; having one exit does not result in the other process not being executed.
  ![double](https://github.com/alx-sch/42_pipex/assets/134595144/569798fb-55fe-4a8b-9073-52d10bc36ea3)

#### Right process waits for the left one 
- The first pipex call does not incl a waitpid for the 'left process' before the 'right process' starts
- The second pipex call incl. a waitpid for the 'left process' before the 'right process' starts
- -> An upstream process waits for the downstream one to finish.
![first_no_waitpid_second_w_waitpid](https://github.com/alx-sch/42_pipex/assets/134595144/ca798d7e-d7b8-45f2-9d71-a2fc1d0fd0b8)

#### Same file as input and output
- Let's say you want to count the words in a file and store the results in another, but use the same file as input and output `< infile.txt wc -c | cat > infile.txt`, the result would be file with '0' in it -> The 'outfile' is created first as an empty file (overwriting the actual 'infile') and THEN process are initiated.

