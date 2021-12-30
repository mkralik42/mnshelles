/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing_quote_bis.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/30 11:40:47 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_charset_not_else_quote(const char *s, int i, int *words)
{
	while (s[i] && !is_charset(s[i]))
		i++;
	if (s[i] && !is_charset(s[i]))
		(*words)--;
	return (i);
}

int	count_charset_not_quote(const char *s, int i, int *words, t_data *d)
{
	if (s[i] == '"' && d->sp->s_quote == 0)
	{
		d->sp->d_quote = 1;
		i++;
		while (s[i] && s[i] != '"')
			i++;
		i++;
		if (s[i] && s[i] != ' ')
			(*words)--;
	}
	else if (s[i] == '\'' && d->sp->d_quote == 0)
	{
		d->sp->s_quote = 1;
		i++;
		while (s[i] && s[i] != '\'')
			i++;
		i++;
		if (s[i] && s[i] != ' ')
			(*words)--;
	}
	else
		i = count_charset_not_else(s, i, words);
	return (i);
}

int	count_charset_quote(t_data *d, const char *s)
{
	int	i;
	int	words;

	words = 0;
	i = 0;
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	while (s[i])
	{
		if (is_charset(s[i]))
			i = count_charset_is(s, i, &words);
		else
		{
			i = count_charset_not(s, i, &words, d);
			words++;
		}
	}
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	return (words);
}

size_t	count_char_quote(const char *s, char c, t_data *d)
{
	size_t	i;
	int		count;

	i = d->sp->line;
	count = 0;
	if (c == 'w')
	{
		while (s[i])
		{
			check_quote(s[i], d);
			if (is_charset(s[i]) && d->sp->s_quote == 0 && d->sp->d_quote == 0)
				break ;
			i++;
			count++;
		}
	}
	else if (c == 'c')
	{
		i--;
		while (s[++i] && (s[i] == '<' || s[i] == '>'))
			count++;
	}
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	return (count);
}
