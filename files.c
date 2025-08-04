/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:43:09 by romukena          #+#    #+#             */
/*   Updated: 2025/08/04 12:17:25 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int open_infile(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
}

int open_outfile(char *filename)
{
	int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
	if (fd == -1)
		return (-1);
	return (fd);
}

/*
Prototype possible

void	child_process_1(int infile_fd, int pipe_write_fd, char *cmd, char **envp);

But de la fonction

	Rediriger l’entrée standard (stdin) vers le fichier d’entrée (infile_fd).

	Rediriger la sortie standard (stdout) vers l’extrémité écriture du pipe (pipe_write_fd).

	Fermer tous les descripteurs inutiles.

	Trouver le chemin absolu de la commande (ex : /bin/ls) à partir de cmd et envp.

	Exécuter la commande avec execve.

	En cas d’erreur, afficher un message et terminer proprement.

*/

void exit_with_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void child_process_1(int infile_fd, int pipe_write_fd, char *cmd, char **envp)
{
	char *pathname;
	char **good_path;
	char **args;

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

void child_process_2(int pipe_read_fd, int outfile_fd, char *cmd, char **envp)
{
	char *pathname;
	char **good_path;
	char **args;

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
	{
		close(infile_fd);
		exit_with_error("outfile");
	}
	if (pipe(pipe_fd) == -1)
	{
		close(infile_fd);
		close(outfile_fd);
		exit_with_error("pipe");
	}
	pid = fork();
	if (pid < 0)
	{
		close(infile_fd);
		close(outfile_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit_with_error("fork1");
	}
	if (pid == 0)
	{
		if (dup2(infile_fd, 0) == -1 || dup2(pipe_fd[1], 1) == -1)
			exit_with_error("dup2 child1");
		close(infile_fd);
		close(outfile_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		child_process_1(infile_fd, pipe_fd[1], av[2], envp);
	}
	pid2 = fork();
	if (pid2 < 0)
	{
		close(infile_fd);
		close(outfile_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit_with_error("fork2");
	}
	if (pid2 == 0)
	{
		if (dup2(pipe_fd[0], 0) == -1 || dup2(outfile_fd, 1) == -1)
			exit_with_error("dup2 child2");
		close(infile_fd);
		close(outfile_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		child_process_2(outfile_fd, pipe_fd[0], av[3], envp);
	}
	close(infile_fd);
	close(outfile_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}
