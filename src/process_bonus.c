// close read end of pipe as not needed
// replace stdin fd with infile_fd
// redirect stdout fd with write end of pipe
void	child_process(char *argv[], char *env[], int *pipe_fd)
{
	int	infile_fd;

	infile_fd = open_file(argv[1], READ_MODE);
	if (infile_fd == -1)
		perror_and_exit("Failed to open input file");
	if (close(p_fd[0]) == -1)
		perror_and_exit("Failed to close read end of pipe");
	if (dub2(infile_fd, STDIN_FILENO) == -1)
		perror_and_exit("Failed to redirect stdin to input file");
	if (dub2(pipe_fd[1], STDOUT_FILENO) == -1)
		perror_and_exit("Failed to redirect stdout to write end of the pipe");
	execute(argv[2], env);
}

void	parent_process(char *argv[], char *env[], int *pipe_fd)
{
	int	outfile_fd;

	outfile_fd = open_file(argv[4], WRITE_MODE);
	if (outfile_fd == -1)
		perror_and_exit("Failed to open output file");
	if (close(p_fd[1]) == -1)
		perror_and_exit("Failed to close write end of pipe");
	if (dub2(outfile_fd, STDOUT_FILENO) == -1)
		perror_and_exit("Failed to redirect stdout to output file");
	if (dub2(pipe_fd[0], STDIN_FILENO) == -1)
		perror_and_exit("Failed to redirect stdin to read end of the pipe");
	execute(argv[3], env);
}
