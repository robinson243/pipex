/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:43:09 by romukena          #+#    #+#             */
/*   Updated: 2025/08/07 00:28:07 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exit_with_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	free_all_and_exit(char *pathname, char **good_path, char **args,
		int exit_code)
{
	if (pathname)
		free(pathname);
	if (good_path)
		free_tab(good_path, len_tab(good_path));
	if (args)
		free_tab(args, len_tab(args));
	exit(exit_code);
}

void	child_process_1(int infile_fd, int pipe_write_fd, char *cmd,
		char **envp)
{
	char	*pathname;
	char	**good_path;
	char	**args;

	good_path = get_paths(envp);
	args = parse_cmd(cmd);
	if (!args || !args[0] || args[0][0] == '\0' || !good_path)
	{
		ft_putstr_fd(": command not found\n", 2);
		free_all_and_exit(NULL, good_path, args, 127);
	}
	pathname = find_cmd_path(good_path, args[0]);
	if (!pathname)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_all_and_exit(NULL, good_path, args, 127);
	}
	dup2(infile_fd, STDIN_FILENO);
	close(infile_fd);
	dup2(pipe_write_fd, STDOUT_FILENO);
	close(pipe_write_fd);
	execve(pathname, args, envp);
	perror(args[0]);
	free_all_and_exit(pathname, good_path, args, 127);
}

void	child_process_2(int pipe_read_fd, int outfile_fd, char *cmd,
		char **envp)
{
	char	*pathname;
	char	**good_path;
	char	**args;

	args = parse_cmd(cmd);
	good_path = get_paths(envp);
	if (!args || !args[0] || args[0][0] == '\0' || !good_path)
	{
		ft_putstr_fd(": command not found\n", 2);
		free_all_and_exit(NULL, good_path, args, 127);
	}
	pathname = find_cmd_path(good_path, args[0]);
	if (!pathname)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_all_and_exit(NULL, good_path, args, 127);
	}
	dup2(pipe_read_fd, STDIN_FILENO);
	close(pipe_read_fd);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	execve(pathname, args, envp);
	perror(args[0]);
	free_all_and_exit(pathname, good_path, args, 127);
}
