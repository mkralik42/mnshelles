/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/29 19:36:43 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_free_quote(char **dst)
{
	int	i;

	i = 0;
	while (dst[i])
	{
		free(dst[i]);
		i++;
	}
	free(dst);
	return (NULL);
}

static char	*make_split_char_quote(char *s, t_data *d)
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

static char	*make_split_chev_quote(char *s, t_data *d)
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

static void	split_parsing_quote(char *s, t_data *d)
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

char	**ft_split_parsing_quote(char *s, t_data *d)
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
