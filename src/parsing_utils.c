/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:07:05 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/21 15:44:52 by lcavallu         ###   ########.fr       */
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

void	print_list(t_lst *list)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (list)
	{
		printf("-----------------------------------\n");
		printf("| pipe = %d                        \n", i);
		printf("| cmd = %s            \n", list->cmd);
		if (list->arg)
		{
			printf("| arg = ");
			j = 0;
			while (list->arg[j])
			{
				printf("%s--", list->arg[j]);
				j++;
			}
		}
		printf("\n");
		printf("| path = %s			  \n", list->path);
		printf("| input = %i		  \n", list->input);
		printf("| output = %i		  \n", list->output);
		printf("| builtin = %i			\n", list->builtin);
		printf("-----------------------------------\n");
		list = list->next;
		i++;
	}
}

int	found_place_raft(char **split, int i)
{
	int	j;

	while (split[i])
	{
		j = 0;
		while (split[i][j])
		{
			if (split[i][j] == '<' || split[i][j] == '>')
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}
