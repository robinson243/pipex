/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:55:50 by romukena          #+#    #+#             */
/*   Updated: 2025/08/09 18:32:54 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define STDIN_FILENO 0
# define STDOUT_FILENO 1

char	**ft_split(char *s, char *charset);
char	*ft_strjoin(char *s1, char *s2);
void	free_tab(char **tab, int k);
int		len_tab(char **tab);
/* utils.c */
int		ft_strlen(char *str);
int		ft_strncmp(char *s1, char *s2, int n);
char	*ft_strdup(char *str);
char	**parse_cmd(char *cmd);
void	ft_putstr_fd(char *s, int fd);

/* findpath.c*/
char	*find_cmd_path(char **paths, char *cmd);
char	**get_paths(char **envp);

/* files.c*/

void	child_process_1(int infile_fd, int pipe_write_fd, char *cmd,
			char **envp);
void	child_process_2(int pipe_read_fd, int outfile_fd, char *cmd,
			char **envp);
void	exit_with_error(char *msg);

/* pipex.c */
void	close_files(int fd1, int fd2, int fd3, int fd4);
void	close_files_without_eror(int fd1, int fd2, int fd3, int fd4);
int		pipex(char **av, char **envp);

/*close_files*/
void	close_files_without_eror(int fd1, int fd2, int fd3, int fd4);
void	close_files(int fd1, int fd2, int fd3, int fd4);

#endif