/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:52:36 by romukena          #+#    #+#             */
/*   Updated: 2025/07/23 22:00:30 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_split(char *s, char *charset);

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *str)
{
	int		i;
	char	*dest;

	i = 0;
	dest = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!dest)
		return (NULL);
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return dest;
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;
	
	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
	{
		if (i == n - 1)
			break;
		i++;
	}
	return (s1[i] - s2[i]);
}

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

/* int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    
    char **paths = get_paths(envp);
    if (!paths)
    {
        printf("PATH not found\n");
        return 1;
    }

    for (int i = 0; paths[i] != NULL; i++)
    {
        printf("Path[%d]: %s\n", i, paths[i]);
    }

    // ici tu devrais libérer paths si alloué dynamiquement

    return 0;
} */
