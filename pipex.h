/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:55:50 by romukena          #+#    #+#             */
/*   Updated: 2025/08/04 15:28:52 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>

char	**ft_split(char *s, char *charset);
char	*ft_strjoin(char *s1, char *s2);
/* utils.c */
int		ft_strlen(char *str);
int		ft_strncmp(char *s1, char *s2, int n);
char	*ft_strdup(char *str);
char	**parse_cmd(char *cmd);

/* findpath.c*/
char	*find_cmd_path(char **paths, char *cmd);
char	**get_paths(char **envp);

/* files.c*/

void	child_process_1(int infile_fd, int pipe_write_fd,
		char *cmd, char **envp);
void	child_process_2(int pipe_read_fd, int outfile_fd,
		char *cmd, char **envp);
#endif