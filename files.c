/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:43:09 by romukena          #+#    #+#             */
/*   Updated: 2025/08/04 15:42:48 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	open_outfile(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
	if (fd == -1)
		return (-1);
	return (fd);
}

void	exit_with_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	child_process_1(int infile_fd, int pipe_write_fd,
		char *cmd, char **envp)
{
	char	*pathname;
	char	**good_path;
	char	**args;

	dup2(infile_fd, 0);
	close(infile_fd);
	dup2(pipe_write_fd, 1);
	close(pipe_write_fd);
	good_path = get_paths(envp);
	args = parse_cmd(cmd);
	pathname = find_cmd_path(good_path, args[0]);
	if (!pathname)
	{
		perror("Command not found");
		exit(1);
	}
	execve(pathname, args, envp);
	perror("execve failed");
	exit(1);
}

void	child_process_2(int pipe_read_fd, int outfile_fd,
		char *cmd, char **envp)
{
	char	*pathname;
	char	**good_path;
	char	**args;

	dup2(pipe_read_fd, 0);
	close(pipe_read_fd);
	dup2(outfile_fd, 1);
	close(outfile_fd);
	args = parse_cmd(cmd);
	good_path = get_paths(envp);
	pathname = find_cmd_path(good_path, args[0]);
	if (!pathname)
	{
		perror("Command not found");
		exit(1);
	}
	execve(pathname, args, envp);
	perror("execve failed");
	exit(1);
}
