/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romukena <romukena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:41:53 by romukena          #+#    #+#             */
/*   Updated: 2025/09/20 15:05:24 by romukena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	status;

	if (argc != 5 || !*envp)
	{
		write(2, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 41);
		return (1);
	}
	status = pipex(argv, envp);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
