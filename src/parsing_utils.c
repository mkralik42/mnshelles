/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:07:05 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/30 19:05:46 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_nbrlen(long n)
{
	int	i;

	i = 1;
	if (n < 0)
	{
		n = -n;
		i++;
	}
	while (n >= 10)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*c;
	long	nb;
	size_t	j;

	nb = n;
	j = ft_nbrlen(nb);
	c = (char *)malloc(sizeof(char) * j + 1);
	if (!c)
		return (NULL);
	c[j] = 0;
	if (n == 0)
		c[0] = '0';
	if (nb < 0)
	{
		c[0] = '-';
		nb = -nb;
	}
	while (nb)
	{
		c[--j] = (nb % 10) + 48;
		nb = nb / 10;
	}
	return (c);
}

void	ft_swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	found_place_raft(char **split, int i, t_data *d)
{
	int	j;

	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	while (split[i])
	{
		j = 0;
		while (split[i][j])
		{
			check_quote(split[i][j], d);
			if ((split[i][j] == '<' || split[i][j] == '>')
				&& d->sp->d_quote == 0 && d->sp->s_quote == 0)
				return (i);
			j++;
		}
		i++;
	}
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	return (-1);
}
