/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/21 19:55:35 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_free(char **dst)
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

char	*make_split_char(char *s, t_data *d)
{
	d->sp->count_c = count_char(s, 'w', d);
	d->sp->new[d->sp->j] = (char *)malloc(sizeof(char) * (d->sp->count_c + 1));
	if (!d->sp->new[d->sp->j])
		return (ft_free(d->sp->new));
	d->sp->k = 0;
	while (s[d->sp->line])
	{
		check_quote(s[d->sp->line], d);
		if (is_charset(s[d->sp->line]) && d->sp->s_quote == 0
			&& d->sp->d_quote == 0)
			break ;
		if (s[d->sp->line] == '"' && d->sp->s_quote == 0)
			d->sp->line++;
		else if (s[d->sp->line] == '\'' && d->sp->d_quote == 0)
			d->sp->line++;
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

char	*make_split_chev(char *s, t_data *d)
{
	d->sp->new[d->sp->j] = (char *)malloc(sizeof(char)
			* (count_char(s, 'c', d) + 1));
	if (!d->sp->new[d->sp->j])
		return (ft_free(d->sp->new));
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

void	split_parsing(char *s, t_data *d)
{
	if (is_charset(s[d->sp->line]))
	{
		if (s[d->sp->line] == '<' || s[d->sp->line] == '>')
			make_split_chev(s, d);
		else
		{
			while (s[d->sp->line] == ' ')
				d->sp->line++;
			d->sp->j--;
		}
	}
	else
		make_split_char(s, d);
}

char	**ft_split_parsing(char *s, t_data *d)
{
	d->sp = init_sp();
	if (!d->sp)
		return (NULL);
	d->sp->j = -1;
	d->sp->count = count_charset(d, s);
	d->sp->k = 0;
	d->sp->new = (char **)malloc(sizeof(char *) * (d->sp->count + 1));
	if (!d->sp->new)
	{
		free(d->sp);
		return (NULL);
	}
	while (++d->sp->j < d->sp->count)
		split_parsing(s, d);
	d->sp->new[d->sp->j] = 0;
	return (d->sp->new);
}
