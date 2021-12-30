/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/30 15:21:05 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_quote(t_data *d)
{
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
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

void	fill_sep_init(t_data *d, t_sep *sep, int i)
{
	if (d->line[i] == '<' && d->line[i + 1] != '<')
		sep->simple_raft_left++;
	if (d->line[i] == '>' && d->line[i + 1] != '>')
		sep->simple_raft_right++;
	if (d->line[i] == '<' && d->line[i + 1] == '<')
		sep->double_raft_left++;
	if (d->line[i] == '>' && d->line[i + 1] == '>')
		sep->double_raft_right++;
	if (d->line[i] == '&' && d->line[i + 1] == '&')
		sep->double_and++;
}

int	fill_sep_quote(int s_quote, int d_quote, t_sep *sep, int what)
{
	if (what == 1)
	{
		sep->double_quo++;
		d_quote = 0;
		return (d_quote);
	}
	if (what == 2)
	{
		sep->simple_quo++;
		s_quote = 0;
		return (s_quote);
	}
	if (what == 3)
	{
		s_quote = 1;
		sep->simple_quo++;
		return (s_quote);
	}
	if (what == 4)
	{
		d_quote = 1;
		sep->double_quo++;
		return (d_quote);
	}
	return (0);
}
