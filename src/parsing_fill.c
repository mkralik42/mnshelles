/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fill.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2022/01/05 18:49:44 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_sep(t_data *d, t_sep *sep)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (d->line[i])
	{
		if (d->line[i] == '"' && d_quote == 1)
			d_quote = fill_sep_quote(s_quote, d_quote, sep, 1);
		else if (d->line[i] == '\'' && s_quote == 1)
			s_quote = fill_sep_quote(s_quote, d_quote, sep, 2);
		else if (d->line[i] == '\'' && d_quote == 0)
			s_quote = fill_sep_quote(s_quote, d_quote, sep, 3);
		else if (d->line[i] == '"' && s_quote == 0)
			d_quote = fill_sep_quote(s_quote, d_quote, sep, 4);
		else if (d->line[i] == '|' && d_quote == 0 && s_quote == 0)
			sep->pipe++;
		fill_sep_init(d, sep, i);
		i++;
	}
	d->nb_pipe = sep->pipe;
}

int	fill_in_out_file_heredoc(int *fd, t_data *d, t_lst *cell, int p_r)
{
	if (!cell->cmd)
	{
		fd[0] = heredoc(d, d->split[p_r + 1]);
		cell->cmd = ft_strdup("test");
		cell->arg = malloc(sizeof(char *) * 2);
		cell->arg[0] = ft_strdup("test");
		cell->arg[1] = NULL;
		cell->path = NULL;
		cell->input = 3;
		cell->output = 0;
		cell->builtin = 0;
	}
	else
		fd[0] = heredoc(d, d->split[p_r + 1]);
	cell->input = fd[0];
	return (fd[0]);
}

t_lst	*fill_in_out_file(t_data *d, t_lst *cell, char **split_quote)
{
	int	p_r;
	int	fd[0];

	p_r = found_place_raft(split_quote, 0, d);
	ft_check_close(cell);
	while (p_r != -1)
	{
		if (d->split[p_r][0] == '<' && d->split[p_r][1] != '<')
			cell = create_new_int(cell, 'i', open(d->split[p_r + 1], O_RDONLY));
		else if (d->split[p_r][0] == '>' && d->split[p_r][1] != '>')
			cell = create_new_int(cell, 'o', open(d->split[p_r + 1], O_CREAT
						| O_WRONLY | O_TRUNC, 0644));
		else if (d->split[p_r][0] == '>' && d->split[p_r][1] == '>')
			cell = create_new_int(cell, 'o', open(d->split[p_r + 1], O_CREAT
						| O_WRONLY | O_APPEND, 0644));
		else if (d->split[p_r][0] == '<' && d->split[p_r][1] == '<')
		{
			if (cell->input > 0)
				close(cell->input);
			fd[0] = fill_in_out_file_heredoc(fd, d, cell, p_r);
		}
		p_r = found_place_raft(split_quote, p_r + 1, d);
	}
	return (cell);
}

int	fill_arg_data(t_data *d, int place_cmd)
{
	while (d->split[place_cmd] && d->split[place_cmd][0] != '|')
		place_cmd++;
	return (place_cmd);
}

t_lst	*fill_arg(t_data *d, t_lst *cell)
{
	int	place_cmd;
	int	i;

	i = 0;
	place_cmd = found_cmd(d, cell);
	if (place_cmd != -1)
	{
		d->argo = malloc(sizeof(char *) * (fill_arg_data(d, place_cmd) + 1));
		if (!d->argo)
			return (NULL);
		while (d->split[place_cmd] && d->split[place_cmd][0] != '|')
		{
			if (d->split[place_cmd][0] == '<' || d->split[place_cmd][0] == '>')
				break ;
			d->argo[i] = ft_strdup(d->split[place_cmd]);
			i++;
			place_cmd++;
		}
		d->argo[i] = NULL;
		cell = create_new_char(cell, NULL, d->argo, 'a');
	}
	return (cell);
}
