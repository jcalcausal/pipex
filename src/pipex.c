/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalcausa <jalcausa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:32:12 by jalcausa          #+#    #+#             */
/*   Updated: 2025/01/16 14:04:28 by jalcausa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../libft/libft.h"

static char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**all_paths;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	all_paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (all_paths[i])
	{
		path = ft_strjoin(ft_strjoin(all_paths[i], "/"), cmd);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = 0;
	while (all_paths[i])
		free(all_paths[i++]);
	free(all_paths);
	return (NULL);
}

void	execute_process(char *cmd, char **envp)
{
	char	*path;
	char	**cmd_splitted;
	int		i;

	i = 0;
	cmd_splitted = ft_split(cmd, ' ');
	path = get_path(cmd_splitted[0], envp);
	if (path == NULL)
	{
		while (cmd_splitted[i])
			free(cmd_splitted[i++]);
		free(cmd_splitted);
		exit(EXIT_FAILURE);
	}
	execve(path, cmd_splitted, envp);
}

static void	child_process(char **argv, int *pipe_fd, char **envp)
{
	int		infile_fd;

	close(pipe_fd[0]);
	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
	{
		ft_printf("Error in the infile\n");
		exit(EXIT_FAILURE);
	}
	dup2(infile_fd, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	execute_process(argv[2], envp);
	close (pipe_fd[1]);
}

static void	parent_process(char **argv, int *pipe_fd, char **envp)
{
	int		outfile_fd;

	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(outfile_fd, STDOUT_FILENO);
	execute_process(argv[3], envp);
	close(pipe_fd[0]);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	ppid;
	int		pipe_fd[2];

	if (argc != 5)
	{
		ft_printf("Incorrect number of arguments\n");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipe_fd) == -1)
	{
		ft_printf("Error creating the pipe\n");
		exit(EXIT_FAILURE);
	}
	ppid = fork();
	if (ppid == -1)
	{
		ft_printf("Error in the fork\n");
		exit(EXIT_FAILURE);
	}
	else if (ppid == 0)
		child_process(argv, pipe_fd, envp);
	wait(0);
	parent_process(argv, pipe_fd, envp);
	return (0);
}
