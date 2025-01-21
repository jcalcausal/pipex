/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalcausa <jalcausa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:32:12 by jalcausa          #+#    #+#             */
/*   Updated: 2025/01/21 22:59:55 by jalcausa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**all_paths;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
    	return (NULL);
	all_paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (all_paths[i])
	{
		path = ft_strjoin(ft_strjoin(all_paths[i], "/"), cmd);
		if (access(path, X_OK) == 0)
		{
			free_split(all_paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_split(all_paths);
	return (NULL);
}

void	execute_process(char *cmd, char **envp)
{
	char	*path;
	char	**cmd_splitted;

	cmd_splitted = ft_split(cmd, ' ');
	path = get_path(cmd_splitted[0], envp);
	if (path == NULL)
	{
		free_split(cmd_splitted);
		ft_printf("error\n");
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd_splitted, envp) == -1)
	{
		ft_printf("error\n");
		free(path);
		free_split(cmd_splitted);
		exit(EXIT_FAILURE);
	}
}

static void	child_process(char **argv, int *pipe_fd, char **envp)
{
	int		infile_fd;

	close(pipe_fd[0]);
	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
	{
		ft_printf("error\n");
		close (pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1
		|| dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		ft_printf("error\n");
		close (pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	close(infile_fd);
	close (pipe_fd[1]);
	execute_process(argv[2], envp);
}

static void	parent_process(char **argv, int *pipe_fd, char **envp)
{
	int		outfile_fd;

	close(pipe_fd[1]);
	outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1
		|| dup2(outfile_fd, STDOUT_FILENO) == -1)
	{
		ft_printf("error\n");
		close(pipe_fd[0]);
		exit(EXIT_FAILURE);
	}
	close(outfile_fd);
	close(pipe_fd[0]);
	execute_process(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	ppid;
	int		pipe_fd[2];

	if (argc != 5 || !envp[0])
	{
		ft_printf("error\n");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipe_fd) == -1)
	{
		ft_printf("error\n");
		exit(EXIT_FAILURE);
	}
	ppid = fork();
	if (ppid == -1)
	{
		ft_printf("error\n");
		exit(EXIT_FAILURE);
	}
	else if (ppid == 0)
		child_process(argv, pipe_fd, envp);
	wait(0);
	parent_process(argv, pipe_fd, envp);
	return (0);
}
