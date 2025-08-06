/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   findpath.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:52:36 by romukena          #+#    #+#             */
/*   Updated: 2025/08/07 00:37:49 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_paths(char **envp)
{
	int		i;
	char	*path;
	char	**tab;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			if (!path)
				return (NULL);
			break ;
		}
		i++;
	}
	if (!path)
		return (NULL);
	tab = ft_split(path, ":");
	free(path);
	if (!tab)
		return (NULL);
	return (tab);
}

char	*try_access(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*find_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*path;

	if (!cmd || !paths)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (try_access(cmd));
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
#include <stdlib.h>

char	*find_cmd_path(char **paths, char *cmd); // prototype de ta fonction

int	main(int argc, char **argv, char **envp)
{
	char	**paths;
	char	*result;
	int		i;

	i = 0;
	// Récupérer PATH depuis envp
	while (envp[i] && strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
	{
		printf("PATH not found in envp\n");
		return (1);
	}
	// Split après "PATH="
	paths = ft_split(envp[i] + 5, ":");
	if (!paths)
		return (1);
	if (argc < 2)
	{
		printf("Usage: ./a.out <cmd>\n");
		return (1);
	}
	result = find_cmd_path(paths, argv[1]);
	if (result)
	{
		printf("Found: %s\n", result);
		free(result);
	}
	else
		printf("Command not found\n");
	// N’oublie pas de free paths !
	return (0);
}

 */
