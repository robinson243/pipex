/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 16:00:07 by romukena          #+#    #+#             */
/*   Updated: 2025/07/23 21:41:08 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int is_charset(char c, char *charset)
{
    int i;

    i = 0;
    while (charset[i])
    {
        if (charset[i] == c)
            return (1);
        i++;
    }
    return 0;
}

static int count_word(char *str, char *charset)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        while (str[i] && is_charset(str[i], charset))
            i++;
        if (str[i])
            count++;
        while (str[i] && !is_charset(str[i], charset))
            i++;
    }
    return count;
}

static char	*ft_strndup(char *s, int start, int end)
{
	int	i;
	char	*dest;

	i = 0;
	dest = malloc(sizeof(char) * (end - start) + 1);
	if (!dest)
		return (NULL);
	while (start + i < end)
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
}
char	**ft_split(char *s, char *charset)
{
	int	i;
	int	j;
	int	k;
	char	**tab;

	i = 0;
	k = 0;
	tab = malloc(sizeof(char *) * count_word(s, charset) + 1);
	if (!tab)
		return (NULL);
	while (s[i])
	{
		while (s[i] && is_charset(s[i], charset))
			i++;
		j = i;
		while (s[i] && !is_charset(s[i], charset))
			i++;
		tab[k] = ft_strndup(s, j , i);
		if (!tab[k])
			free_tab(tab);
		k++;
	}
	tab[k] = NULL;
	return (tab);
}

/* int main()
{
    char str[] = "Hello, world! Welcome to C programming.";
    char charset[] = " ";

    char **result = ft_split(str, charset);
    int i = 0;

    // Vérification si l'allocation a réussi.
    if (!result)
    {
        printf("Erreur d'allocation mémoire\n");
        return 1;
    }

    // Afficher les mots séparés
    while (result[i])
    {
        printf("Mot %d : %s\n", i + 1, result[i]);
        i++;
    }

    // Si tout va bien, on libère la mémoire allouée
    i = 0;
    while (result[i])
    {
        free(result[i]);  // Libérer chaque mot
        i++;
    }
    free(result);  // Libérer le tableau de mots

    return 0;
} */