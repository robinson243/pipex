/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:42:52 by romukena          #+#    #+#             */
/*   Updated: 2025/08/04 17:36:11 by romukena         ###   ########.fr       */
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

void	handle_child1(int fd[2], int pipe_fd[2],
		char **av, char **envp)
{
	close_files_without_eror(-1, fd[1], pipe_fd[0], -1);
	child_process_1(fd[0], pipe_fd[1], av[2], envp);
}

void	handle_child2(int fd[2], int pipe_fd[2],
		char **av, char **envp)
{
	close_files_without_eror(-1, fd[0], -1, pipe_fd[1]);
	child_process_2(pipe_fd[0], fd[1], av[3], envp);
}


void	pipex(char **av, char **envp)
{
	int		fd[2];
	int		pipe_fd[2];
	pid_t	pid;
	pid_t	pid2;

	fd[0] = open(av[1], O_RDONLY);
	fd[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd[0] < 0 || fd[1] < 0 || pipe(pipe_fd) < 0)
		exit_with_error("infile");
	pid = fork();
	if (pid < 0)
		close_files(fd[0], fd[1], pipe_fd[0], pipe_fd[1]);
	if (pid == 0)
		handle_child1(fd, pipe_fd, av, envp);
	pid2 = fork();
	if (pid2 < 0)
		close_files(fd[0], fd[1], pipe_fd[0], pipe_fd[1]);
	if (pid2 == 0)
		handle_child2(fd, pipe_fd, av, envp);
	close_files_without_eror(fd[0], fd[1], pipe_fd[0], pipe_fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}

