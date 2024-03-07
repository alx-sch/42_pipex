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

```bash
MAIL=XXX@student.42berlin.de
LANGUAGE=en
USER=XXX
XDG_SESSION_TYPE=x11
SHLVL=2
HOME=/home/XXX
OLDPWD=/home/XXX/Documents/repos/pipex
```
