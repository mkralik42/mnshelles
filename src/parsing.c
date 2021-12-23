/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/23 15:51:32 by mkralik          ###   ########.fr       */
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

	cell = init_cell();
	cell->next = NULL;
	free(d->sp);
	d->sp = NULL;
	if (i != 3)
		ft_free_str(split_pipe);
	add_cell_parsing(d, cell);
	return (d->cmd_lst);
}

void	ft_fill_cell(t_data *d, t_sep *sep)
{
	t_lst	*cell;

	cell = init_cell();
	cell = check_infile_outfile(d, sep, cell);
	cell = fill_in_out_file(d, sep, cell);
	cell = fill_builtin(cell);
	if (cell->builtin == 0)
		cell = found_path(cell, d);
	cell = fill_arg(d, cell);
	cell->next = NULL;
	add_cell_parsing(d, cell);
//	print_sep(sep, d);
}

int	ft_fill_split(t_data *d, t_sep *sep, char **split_pipe, int *i)
{
	int	j;

	j = *i;
	d->split = ft_split_parsing(split_pipe[j], d);
	if (!check_chev(d))
		ft_fill_cell(d, sep);
	else
	{
		ft_free_str(d->split);
		return (1);
	}
	(*i)++;
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
		split_pipe = ft_split(d->line, '|');
		if (check_pipe(split_pipe, sep))
			return (ft_return(d, split_pipe, 1));
		while (split_pipe[i])
			if (ft_fill_split(d, sep, split_pipe, &i))
				return (ft_return(d, split_pipe, 2));
		ft_free_str(split_pipe);
		split_pipe = NULL;
		print_list(d->cmd_lst);
	}
	else
		return (ft_return(d, NULL, 3));
	return (d->cmd_lst);
}
