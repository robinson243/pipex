/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:42:52 by romukena          #+#    #+#             */
/*   Updated: 2025/08/04 15:43:03 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


void	close_files(int fd1, int fd2, int fd3, int fd4)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	if (fd3 >= 0)
		close(fd3);
	if (fd4 >= 0)
		close(fd4);
	exit_with_error("Error");
}
void	close_files_without_eror(int fd1, int fd2, int fd3, int fd4)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	if (fd3 >= 0)
		close(fd3);
	if (fd4 >= 0)
		close(fd4);
}

void	pipex(char **av, char **envp)
{
	int		infile_fd;
	int		outfile_fd;
	int		pipe_fd[2];
	pid_t	pid;
	pid_t	pid2;

	infile_fd = open(av[1], O_RDONLY);
	if (infile_fd == -1)
		exit_with_error("infile");
	outfile_fd = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == -1)
		close_files(infile_fd, -1, -1, -1);
	if (pipe(pipe_fd) == -1)
		close_files(infile_fd, outfile_fd, -1, -1);
	pid = fork();
	if (pid < 0)
		close_files(infile_fd, outfile_fd, pipe_fd[0], pipe_fd[1]);
	if (pid == 0)
	{
		close_files_without_eror(-1, outfile_fd, pipe_fd[0], -1);
		child_process_1(infile_fd, pipe_fd[1], av[2], envp);
	}
	pid2 = fork();
	if (pid2 < 0)
		close_files(infile_fd, outfile_fd, pipe_fd[0], pipe_fd[1]);
	if (pid2 == 0)
	{
		close_files_without_eror(infile_fd, -1, -1, pipe_fd[1]);
		child_process_2(pipe_fd[0], outfile_fd, av[3], envp);
	}
	close_files_without_eror(infile_fd, outfile_fd, pipe_fd[0], pipe_fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}

