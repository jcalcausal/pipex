/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalcausa <jalcausa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 23:26:28 by jalcausa          #+#    #+#             */
/*   Updated: 2025/01/16 00:07:02 by jalcausa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*read_from_fd(int fd, char *buffer, char *buffer_remain)
{
	char	*tmp;
	ssize_t	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (NULL);
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!buffer_remain)
			buffer_remain = ft_strdup("");
		tmp = buffer_remain;
		buffer_remain = ft_strjoin(tmp, buffer);
		free (tmp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (buffer_remain);
}

static char	*trim_line(char *buffer)
{
	char	*buffer_remain;
	int		i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		++i;
	if (!buffer[i] || !buffer[i + 1])
		return (NULL);
	buffer_remain = ft_substr(buffer, i + 1, ft_strlen(buffer) - i);
	if (!buffer_remain)
		return (NULL);
	buffer[i + 1] = '\0';
	return (buffer_remain);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	static char	*buffer_remain;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (0);
	line = read_from_fd(fd, buffer, buffer_remain);
	free(buffer);
	if (!line)
	{
		free(buffer_remain);
		buffer_remain = NULL;
		return (NULL);
	}
	buffer_remain = trim_line(line);
	return (line);
}
/*
int main() {
    int fd;
    char *next_line;
    int count;

    count = 0;
    fd = open("prueba1.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir archivo");
        return 1;
    }

    while ((next_line = get_next_line(fd)) != NULL) {
        count++;
        printf("[%d]: %s", count, next_line);
        free(next_line);
    }

    if (close(fd) == -1) {
        perror("Error al cerrar archivo");
        return 1;
    }

    return 0;
}
*/
