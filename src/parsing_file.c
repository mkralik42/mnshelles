/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/29 18:00:29 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_in_out_data(t_data *d, t_sep *sep, t_lst *cell, int place_raft)
{
	(void)sep;
	if (d->split[place_raft][0] == '<')
	{
		if (place_raft != 0)
		{
			while (d->split[place_raft - 1][0] == '-')
				place_raft -= 1;
			cell = create_new_char(cell, d->split[place_raft - 1], NULL, 'c');
		}
		else
		{
			cell = create_new_char(cell, d->split[place_raft + 2], NULL, 'c');
	//		ft_swap(&d->split[place_raft + 1], &d->split[place_raft + 2]);
		}
	}
	else if (d->split[place_raft][0] == '>')
	{
		if (!cell->cmd)
			cell = create_new_char(cell, d->split[0], NULL, 'c');
	}
}

t_lst	*check_infile_outfile(t_data *d, t_sep *sep, t_lst *cell, char **split_quote)
{
	int	place_raft;

	place_raft = found_place_raft(split_quote, 0, d);
	if (place_raft != -1)
	{
		while (place_raft != -1)
		{
			check_in_out_data(d, sep, cell, place_raft);
			place_raft = found_place_raft(split_quote, place_raft + 1, d);
		}
	}
	else
		cell = create_new_char(cell, d->split[0], NULL, 'c');
	return (cell);
}

void	init_sep(t_sep *sep)
{
	sep->pipe = 0;
	sep->double_and = 0;
	sep->simple_quo = 0;
	sep->double_quo = 0;
	sep->simple_raft_left = 0;
	sep->simple_raft_right = 0;
	sep->double_raft_left = 0;
	sep->double_raft_right = 0;
	sep->infile = NULL;
	sep->outfile = NULL;
}

t_lst	*init_cell(void)
{
	t_lst	*cell;

	cell = malloc(sizeof(t_lst));
	if (!cell)
		return (NULL);
	cell->cmd = NULL;
	cell->arg = NULL;
	cell->path = NULL;
	cell->input = 0;
	cell->output = 0;
	cell->builtin = 0;
	return (cell);
}

t_lst	*fill_builtin(t_lst *cell)
{
	if (cmp_str(cell->cmd, "echo") || cmp_str(cell->cmd, "cd")
		|| cmp_str(cell->cmd, "pwd") || cmp_str(cell->cmd, "export")
		|| cmp_str(cell->cmd, "unset") || cmp_str(cell->cmd, "env")
		|| cmp_str(cell->cmd, "exit"))
		cell = create_new_int(cell, 'b', 1);
	else
		cell = create_new_int(cell, 'b', 0);
	return (cell);
}
