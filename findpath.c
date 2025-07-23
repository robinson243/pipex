/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   findpath.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:52:36 by romukena          #+#    #+#             */
/*   Updated: 2025/07/23 23:21:51 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char **get_paths(char **envp)
{
	int		i;
	char	**tab;
	char	*path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			if (!path)
				return (NULL);
		}
		i++;
	}
	tab = ft_split(path, ":");
	if (!tab)
		return (NULL);
	return (tab);
}

char	*find_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*path_to_test;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		path_to_test = ft_strjoin(tmp, cmd);
		if (!path_to_test)
			return (NULL);
		if (access(path_to_test, X_OK))
			return (path_to_test);
		i++;
	}
	return (0);
}
/* 
#include <stdio.h>
#include <stdlib.h>

char *find_cmd_path(char **paths, char *cmd); // prototype de ta fonction

int	main(int argc, char **argv, char **envp)
{
	char	**paths;
	char	*result;
	int		i = 0;

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