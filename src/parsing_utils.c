/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:07:05 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/15 18:11:51 by lcavallu         ###   ########.fr       */
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

int	ft_isalpha_parsing(int c)
{
	if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95)
		return (1);
	return (0);
}

void	ft_swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	if (!src)
		return (NULL);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

int	ft_strncmp_parsing(char *s1, char *s2, int n)
{
	int		i;
	int		d;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && i + 1 < n)
		i++;
	d = (unsigned char)s1[i] - (unsigned char)s2[i];
	if (!d || !n)
		return (0);
	else if (d < 0)
		return (-1);
	else
		return (1);
}

int ft_strcmp_parsing(char *s1, char *s2)
{
    int i;

    i = 0;
	while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return (1);
		i++;
    }
    return (0);
}


t_lst   *create_cell(char *cmd)
{
	t_lst *cell;

	cell = malloc(sizeof(t_lst));
	if (!(cell))
		return (NULL);
	cell->cmd = cmd;
	return (cell);
}

void	add_cell_parsing(t_data *d, t_lst *new)
{
    t_lst	*last;

    if (d && !(d->cmd_lst))
       	d->cmd_lst = new;
   	 else
   	 {
		last = d->cmd_lst;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
    }
}

void    print_list(t_lst *list)
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



t_lst	*create_new_char(t_lst *cell, char *split, char **arg, char what)
{
	if (what == 'c')	//commande
		cell->cmd = split;
	if (what == 'p')	//path
		cell->path = split;
	if (what == 'a') // arguments
		cell->arg = arg;
	return (cell);
}

t_lst	*create_new_int(t_lst *cell, char what, int file)
{
	if (what == 'i')
		cell->input = file; //input
	if (what == 'o')
		cell->output = file; // output
	if (what == 'b')
		cell->builtin = file; // builtin
	return (cell);
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

void	ft_free_str(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	str = NULL;
}
