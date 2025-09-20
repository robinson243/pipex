/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:42:52 by romukena          #+#    #+#             */
/*   Updated: 2025/09/20 16:42:26 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_multi_files(int fd[2], char **av)
{
	fd[0] = -1;
	fd[1] = -1;
	fd[0] = open(av[1], O_RDONLY);
	if (fd[0] < 0)
	{
		perror(av[1]);
		fd[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd[1] < 0)
		{
			perror(av[4]);
			return (1);
		}
		return (1);
	}
	fd[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd[1] < 0)
	{
		perror(av[4]);
		return (1);
	}
	return (0);
}

static void	init_fds(int fd[2], int pfd[2])
{
	fd[0] = -1;
	fd[1] = -1;
	pfd[0] = -1;
	pfd[1] = -1;
}

int	pipex(char **av, char **envp)
{
	int		fd[2];
	int		pfd[2];
	pid_t	pid;
	int		status;

	init_fds(fd, pfd);
	if (open_multi_files(fd, av) != 0)
		return (close_files_without_eror(fd[0], fd[1], pfd[0], pfd[1]), 1);
	if (pipe(pfd) < 0)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		child_process_1(fd, pfd, av[2], envp);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		child_process_2(fd, pfd, av[3], envp);
	close_files_without_eror(fd[0], fd[1], pfd[0], pfd[1]);
	waitpid(pid, &status, 0);
	waitpid(pid, &status, 0);
	return (1);
}
