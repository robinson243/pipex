/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:42:52 by romukena          #+#    #+#             */
/*   Updated: 2025/08/10 18:37:57 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_child1(int fd[2], int pipe_fd[2], char **av, char **envp)
{
	close_files_without_eror(-1, fd[1], pipe_fd[0], -1);
	if (fd[0] < 0)
		exit(1);
	child_process_1(fd[0], pipe_fd[1], av[2], envp);
}

void	handle_child2(int fd[2], int pipe_fd[2], char **av, char **envp)
{
	close_files_without_eror(-1, fd[0], -1, pipe_fd[1]);
	if (fd[1] < 0)
		exit(1);
	child_process_2(pipe_fd[0], fd[1], av[3], envp);
}

int	open_multi_files(int fd[2], char **av)
{
	fd[0] = open(av[1], O_RDONLY);
	if (fd[0] < 0)
		perror(av[1]);
	fd[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd[1] < 0)
		perror(av[4]);
	return (0);
}

int	pipex(char **av, char **envp)
{
	int		fd[2];
	int		pipe_fd[2];
	pid_t	pid[2];
	int		status;

	if (open_multi_files(fd, av) != 0)
		return (1);
	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	pid[0] = fork();
	if (pid[0] < 0)
		return (perror("fork"), 1);
	if (pid[0] == 0)
		handle_child1(fd, pipe_fd, av, envp);
	pid[1] = fork();
	if (pid[1] < 0)
		return (perror("fork"), 1);
	if (pid[1] == 0)
		handle_child2(fd, pipe_fd, av, envp);
	close_files_without_eror(fd[0], fd[1], pipe_fd[0], pipe_fd[1]);
	waitpid(pid[1], &status, 0);
	waitpid(pid[0], NULL, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
