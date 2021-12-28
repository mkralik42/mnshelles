/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/28 13:57:33 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_charset_not_else_quote(const char *s, int i, int *words)
{
	while (s[i] && !is_charset(s[i]))
		i++;
	if (s[i] && !is_charset(s[i]))
		(*words)--;
	return (i);
}

int count_charset_not_quote(const char *s, int i, int *words, t_data *d)
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

int count_charset_quote(t_data *d, const char *s)
{
	int i;
	int words;

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

static char *ft_free_quote(char **dst)
{
	int i;

	i = 0;
	while (dst[i])
	{
		free(dst[i]);
		i++;
	}
	free(dst);
	return (NULL);
}

size_t  count_char_quote(const char *s, char c, t_data *d)
{
	size_t  i;
	int     count;

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

static char    *make_split_char_quote(char *s, t_data *d)
{
	d->sp->count_c = count_char_quote(s, 'w', d);
	d->sp->new[d->sp->j] = (char *)malloc(sizeof(char) * (d->sp->count_c + 1));
	if (!d->sp->new[d->sp->j])
		return (ft_free_quote(d->sp->new));
	d->sp->k = 0;
	while (s[d->sp->line])
	{
		check_quote(s[d->sp->line], d);
		if (is_charset(s[d->sp->line]) && d->sp->s_quote == 0
				&& d->sp->d_quote == 0)
			break ;
		//      if (s[d->sp->line] == '"' && d->sp->s_quote == 0)
		//          d->sp->line++;
		//      else if (s[d->sp->line] == '\'' && d->sp->d_quote == 0)
		//          d->sp->line++;
		else if ((s[d->sp->line] == '$' && (s[d->sp->line + 1] == '?'
						|| ft_isalpha_parsing(s[d->sp->line + 1])))
				&& d->sp->s_quote == 0)
			make_change(s, d);
		else
			d->sp->new[d->sp->j][d->sp->k++] = s[d->sp->line++];
	}
	d->sp->new[d->sp->j][d->sp->k] = 0;
	return (d->sp->new[d->sp->j]);
}

static char    *make_split_chev_quote(char *s, t_data *d)
{
	d->sp->new[d->sp->j] = (char *)malloc(sizeof(char)
			* (count_char(s, 'c', d) + 1));
	if (!d->sp->new[d->sp->j])
		return (ft_free_quote(d->sp->new));
	d->sp->k = 0;
	while (s[d->sp->line] && (s[d->sp->line] == '<' || s[d->sp->line] == '>'))
	{
		d->sp->new[d->sp->j][d->sp->k] = s[d->sp->line];
		d->sp->k++;
		d->sp->line++;
	}
	d->sp->new[d->sp->j][d->sp->k] = 0;
	return (d->sp->new[d->sp->j]);
}

static void    split_parsing_quote(char *s, t_data *d)
{
	if (is_charset(s[d->sp->line]))
	{
		if (s[d->sp->line] == '<' || s[d->sp->line] == '>')
			make_split_chev_quote(s, d);
		else
		{
			while (s[d->sp->line] == ' ')
				d->sp->line++;
			d->sp->j--;
		}
	}
	else
		make_split_char_quote(s, d);
}

char    **ft_split_parsing_quote(char *s, t_data *d)
{
	d->sp = init_sp();
	if (!d->sp)
		return (NULL);
	d->sp->j = -1;
	d->sp->count = count_charset_quote(d, s);
	d->sp->k = 0;
	d->sp->new = (char **)malloc(sizeof(char *) * (d->sp->count + 1));
	if (!d->sp->new)
	{
		free(d->sp);
		return (NULL);
	}
	while (++d->sp->j < d->sp->count)
		split_parsing_quote(s, d);
	d->sp->new[d->sp->j] = 0;
	return (d->sp->new);
}
