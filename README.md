# 42_pipex

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
