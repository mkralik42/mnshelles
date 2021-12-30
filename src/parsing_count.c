/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_count.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/30 12:17:23 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sp	*init_sp(void)
{
	t_sp	*sp;

	sp = malloc(sizeof(t_sp));
	sp->line = 0;
	sp->count = 0;
	sp->j = -1;
	sp->k = 0;
	sp->new = NULL;
	sp->s_quote = 0;
	sp->d_quote = 0;
	sp->count_c = 0;
	sp->remember_mem = 0;
	return (sp);
}

void	check_quote(char str, t_data *d)
{
	if (str == '\'' && d->sp->d_quote == 0)
	{
		if (d->sp->s_quote == 0)
			d->sp->s_quote = 1;
		else
			d->sp->s_quote = 0;
	}
	if (str == '"' && d->sp->s_quote == 0)
	{
		if (d->sp->d_quote == 0)
			d->sp->d_quote = 1;
		else
			d->sp->d_quote = 0;
	}
}

int	count_char_i(const char *s, t_data *d, int i, int *count)
{
	if (s[i] == '"' && d->sp->s_quote == 0)
		i++;
	else if (s[i] == '\'' && d->sp->d_quote == 0)
		i++;
	else
	{
		i++;
		(*count)++;
	}
	return (i);
}

size_t	count_char(const char *s, char c, t_data *d)
{
	size_t	i;
	int		count;

	init_quote(d);
	i = d->sp->line;
	count = 0;
	if (c == 'w')
	{
		while (s[i])
		{
			check_quote(s[i], d);
			if (is_charset(s[i]) && d->sp->s_quote == 0 && d->sp->d_quote == 0)
				break ;
			i = count_char_i(s, d, i, &count);
		}
	}
	else if (c == 'c')
	{
		i--;
		while (s[++i] && (s[i] == '<' || s[i] == '>'))
			count++;
	}
	init_quote(d);
	return (count);
}

int	check_chev_data_return(int what)
{
	g_exit_status = 2;
	if (what == 1)
		ft_putstr_fd("syntax error near unexpected token `<<<'\n", 2);
	else if (what == 2)
		ft_putstr_fd("syntax error near unexpected token `>>>'\n", 2);
	else if (what == 3)
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
	return (g_exit_status);
}
