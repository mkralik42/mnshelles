/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing_pipe.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:03:18 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/29 19:30:59 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_words_p(const char *s, char c, t_data *d)
{
	size_t	words;
	int		state;

	words = 0;
	state = 1;
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	while (s && *s)
	{
		check_quote(*s, d);
		if (*s == c && d->sp->s_quote == 0 && d->sp->d_quote == 0)
			state = 1;
		else if (state == 1)
		{
			words++;
			state = 0;
		}
		s++;
	}
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	return (words);
}

static size_t	sizeof_word_p(const char *s, char c, t_data *d)
{
	size_t	i;

	i = 0;
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	while (s[i] && (s[i] != c || ((s[i] == c) && (d->sp->s_quote == 1
					|| d->sp->d_quote == 1))))
	{
		check_quote(s[i], d);
		i++;
	}
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	return (i);
}

static char	**free_tab_p(char **tab, size_t size, t_data *d)
{
	size_t	i;

	i = 0;
	free(d->sp);
	d->sp = NULL;
	while (i < size)
		free(tab[i++]);
	free(tab);
	return (0);
}

static char	**make_s_pipe(const char **s, size_t index, t_data *d, char **tab)
{
	int	i;

	i = 0;
	while (**s && (**s != '|' || ((**s == '|') && (d->sp->s_quote == 1
					|| d->sp->d_quote == 1))))
	{
		check_quote(**s, d);
		tab[index][i++] = *(*s)++;
	}
	tab[index][i] = 0;
	return (tab);
}

char	**ft_split_parsing_pipe(const char *s, char c, t_data *d)
{
	char	**tab;
	size_t	index;
	size_t	count_w;

	if (!s)
		return (NULL);
	d->sp = init_sp();
	count_w = count_words_p(s, c, d);
	tab = malloc(sizeof(char *) * (count_w + 1));
	if (!tab)
		return (NULL);
	index = -1;
	while (++index < count_w)
	{
		while (*s == c)
			s++;
		tab[index] = malloc(sizeof(char) * (sizeof_word_p(s, c, d) + 1));
		if (!tab[index])
			return (free_tab_p(tab, index, d));
		tab = make_s_pipe(&s, index, d, tab);
	}
	free(d->sp);
	d->sp = NULL;
	tab[index] = 0;
	return (tab);
}
