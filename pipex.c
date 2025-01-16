/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalcausa <jalcausa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:32:12 by jalcausa          #+#    #+#             */
/*   Updated: 2025/01/16 12:46:58 by jalcausa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../libft/libft.h"

int	main(int argc, char **argv, char **envp)
{
	pid_t	ppid;
	int		pipe_fd[2];

	if (argc != 5)
	{
		ft_printf("Incorrect number of arguments");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipe_fd) == -1)
	{
		ft_printf("Error creating the pipe");
		exit(EXIT_FAILURE);
	}
	ppid = fork();
	if (ppid == -1)
	{
		ft_printf("Error in the fork");
		exit(EXIT_FAILURE);
	}
	else if (ppid == 0)
	{
		child_process(argv, pipe_fd, envp);
	}
	else
	{
		wait(0);
		parent_process(argv, pipe_fd, envp);
	}
	return (0);
}
