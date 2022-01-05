/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2022/01/05 18:42:47 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*ft_free_double(char **path, char *cmd, t_lst *cell)
{
	if (!cmd)
	{
		if (access(cell->cmd, F_OK) != -1)
			cell = create_new_char(cell, cell->cmd, NULL, 'p');
		else
			cell = create_new_char(cell, NULL, NULL, 'p');
	}
	if (path)
		ft_free_str(path);
	if (cmd)
		cell = create_new_char(cell, cmd, NULL, 'p');
	free(cmd);
	return (cell);
}

t_lst	*ft_return(t_data *d, char **split_pipe, int i)
{
	t_lst	*cell;

	if (d->split)
		ft_free_str(d->split);
	d->split = NULL;
	cell = init_cell();
	cell->next = NULL;
	free(d->sp);
	d->sp = NULL;
	if (i != 3)
		ft_free_str(split_pipe);
	add_cell_parsing(d, cell);
	return (d->cmd_lst);
}

void	ft_fill_cell(t_data *d, t_sep *sep, char **split_quote)
{
	t_lst	*cell;

	cell = init_cell();
	cell = check_infile_outfile(d, sep, cell, split_quote);
	cell = fill_in_out_file(d, cell, split_quote);
	cell = fill_builtin(cell);
	if (cell->builtin == 0 && cell->cmd[0])
		cell = found_path(cell, d);
	cell = fill_arg(d, cell);
	cell->next = NULL;
	add_cell_parsing(d, cell);
}

int	ft_fill_split(t_data *d, t_sep *sep, char **split_pipe, int *i)
{
	int		j;
	char	**split_quote;

	j = *i;
	d->split = ft_split_parsing(split_pipe[j], d);
	if (check_chev(d, split_pipe) != 0)
		return (1);
	split_quote = ft_split_parsing_quote(split_pipe[j], d);
	ft_fill_cell(d, sep, split_quote);
	(*i)++;
	ft_free_str(split_quote);
	split_quote = NULL;
	ft_free_str(d->split);
	d->split = NULL;
	free(d->sp);
	d->sp = NULL;
	return (0);
}

t_lst	*parsing(t_data *d)
{
	t_sep	sep[1];
	char	**split_pipe;
	int		i;

	i = 0;
	init_sep(sep);
	fill_sep(d, sep);
	d->split = NULL;
	if (!check_sep(sep, d))
	{
		split_pipe = ft_split_parsing_pipe(d->line, '|', d);
		if (check_pipe(split_pipe, sep) != 0)
			return (ft_return(d, split_pipe, 1));
		while (split_pipe[i])
			if (ft_fill_split(d, sep, split_pipe, &i))
				return (ft_return(d, split_pipe, 2));
		ft_free_str(split_pipe);
		split_pipe = NULL;
	}
	else
		return (ft_return(d, NULL, 3));
	return (d->cmd_lst);
}
