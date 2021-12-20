/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 17:44:41 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/16 17:59:30 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	boucle_get_next_line(char **line, int i)
{
	char	*tmp;

	tmp = NULL;
	if (i)
		i = ft_strlen(*line);
	if (i != 0)
	{
		tmp = *line;
		*line = ft_strjoin(*line, "\n");
		free (tmp);
	}
	return (0);
}

void	get_line(char *buff, char **line)
{
	char *tmp;

	tmp = NULL;
	buff[1] = 0;
	if (buff[0] != '\n')
	{
		tmp = *line;
		*line = ft_strjoin(*line, buff);
		free (tmp);
	}
}

char	*get_next_line(t_data *data, char **line)
{
	char	buff[2];
	int		ret;
	int		i;

	i = 0;
	(void) data;
	boucle_get_next_line(line, i);
	ret = 1;
	buff[0] = 0;
	ft_putstr_fd("> ", 1);
	while (ret > 0 && buff[0] != '\n')
	{
		ret = read(STDIN_FILENO, buff, 1);
		if (ret == -1)
			break;
		get_line(buff, line);
		i++;
	}
	*line = ft_free_strjoin(*line, "\n");
	return (*line);
}

int	heredoc(t_data *data, char *eof)
{
	char	*line;
	int		fd[2];

	line = ft_strdup("");
	if (error_catch(pipe(fd) < 0, NULL, strerror(errno)))
	{
		//free
		return (1);
	}
	while (1)
	{
		line = get_next_line(data, &line); //recupere la ligne
		// printf("-   🖕   %s-\n", line);
		if (ft_strncmp(line, eof, ft_strlen(eof)) == 0 && line[ft_strlen(eof)] == '\n') // si eof on break
			break;
		ft_putstr_fd(line, fd[1]); //ecrire ligne dans fd[1] entree du pipe
		free(line);
		line = ft_strdup("");
	}
	free(line);
	close(fd[1]);
	return (fd[0]);
}