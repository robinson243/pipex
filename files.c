/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:43:09 by romukena          #+#    #+#             */
/*   Updated: 2025/09/14 16:06:40 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*➜  pipex git:(main) ✗ valgrind --leak-check=full
--trace-children=yes --track-fds=yes ./pipex Makefile "sleep 5" "sleep 5" out*/

static char	*print_command_error(char *cmd)
{
	char	*sentence;
	char	*dest;

	sentence = ": command not found\n";
	dest = ft_strjoin(cmd, sentence);
	if (!dest)
		free(dest);
	return (dest);
}

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

void	child_process_1(int fd[2], int pipe[2], char *cmd, char **envp)
{
	char	*pathname;
	char	**good_path;
	char	**args;
	char	*phrase;

	good_path = get_paths(envp);
	args = parse_cmd(cmd);
	pathname = find_cmd_path(good_path, args[0]);
	if (!pathname)
	{
		phrase = print_command_error(args[0]);
		ft_putstr_fd(phrase, 2);
		free (phrase);
		close_files_without_eror(fd[0], fd[1], pipe[0], pipe[1]);
		free_all_and_exit(NULL, good_path, args, 127);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(pipe[1], STDOUT_FILENO);
	close_files_without_eror(fd[0], fd[1], pipe[0], pipe[1]);
	(execve(pathname, args, envp), perror(args[0]));
	free_all_and_exit(pathname, good_path, args, 127);
}

void	child_process_2(int fd[2], int pipe[2], char *cmd, char **envp)
{
	char	*pathname;
	char	**good_path;
	char	**args;
	char	*phrase;

	args = parse_cmd(cmd);
	good_path = get_paths(envp);
	pathname = find_cmd_path(good_path, args[0]);
	if (!pathname)
	{
		phrase = print_command_error(args[0]);
		ft_putstr_fd(phrase, 2);
		free(phrase);
		close_files_without_eror(fd[0], fd[1], pipe[0], pipe[1]);
		free_all_and_exit(NULL, good_path, args, 127);
	}
	dup2(fd[1], STDOUT_FILENO);
	dup2(pipe[0], STDIN_FILENO);
	close_files_without_eror(fd[0], fd[1], pipe[0], pipe[1]);
	(execve(pathname, args, envp), perror(args[0]));
	free_all_and_exit(pathname, good_path, args, 127);
}
