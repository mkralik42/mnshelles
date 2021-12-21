/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_charset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/21 15:12:59 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_charset(char s)
{
	if (s == ' ' || s == '<' || s == '>')
		return (1);
	return (0);
}

int	count_charset_is(const char *s, int i, int *words)
{
	if (s[i] == '<' || s[i] == '>')
	{
		while (s[i] == '<' || s[i] == '>')
			i++;
		(*words)++;
	}
	else
		while (s[i] == ' ')
			i++;
	return (i);
}

int	count_charset_not_else(const char *s, int i, int *words)
{
	while (s[i] && !is_charset(s[i]) && s[i] != '"' && s[i] != '\'')
		i++;
	if (s[i] && !is_charset(s[i]))
		(*words)--;
	return (i);
}

int	count_charset_not(const char *s, int i, int *words, t_data *d)
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

int	count_charset(t_data *d, const char *s)
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
