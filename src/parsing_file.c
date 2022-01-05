/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2022/01/05 18:49:36 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_check_close(t_lst *cell)
{
	if (cell->input)
		close(cell->input);
	if (cell->output)
		close(cell->output);
}

void	check_in_out_data(t_data *d, t_sep *sep, t_lst *cell, int place_raft)
{
	(void)sep;
	if (d->split[place_raft][0] == '<')
	{
		if (cell->cmd)
			free(cell->cmd);
		if (place_raft != 0)
			cell = create_new_char(cell, d->split[0], NULL, 'c');
		else
			cell = create_new_char(cell, d->split[place_raft + 2], NULL, 'c');
	}
	else if (d->split[place_raft][0] == '>')
	{
		if (!cell->cmd)
			cell = create_new_char(cell, d->split[0], NULL, 'c');
	}
}

t_lst	*check_infile_outfile(t_data *d, t_sep *sep, t_lst *cell, char **s_q)
{
	int	place_raft;

	place_raft = found_place_raft(s_q, 0, d);
	if (place_raft != -1)
	{
		while (place_raft != -1)
		{
			check_in_out_data(d, sep, cell, place_raft);
			place_raft = found_place_raft(s_q, place_raft + 1, d);
		}
	}
	else
		cell = create_new_char(cell, d->split[0], NULL, 'c');
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
