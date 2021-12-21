/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_create.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:07:05 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/21 12:02:09 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst   *create_cell(char *cmd)
{
    t_lst *cell;

    cell = malloc(sizeof(t_lst));
    if (!(cell))
        return (NULL);
    cell->cmd = cmd;
    return (cell);
}

void    add_cell_parsing(t_data *d, t_lst *new)
{
    t_lst   *last;

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

t_lst   *create_new_char(t_lst *cell, char *split, char **arg, char what)
{
    if (what == 'c')    //commande
        cell->cmd = split;
    if (what == 'p')    //path
        cell->path = split;
    if (what == 'a') // arguments
        cell->arg = arg;
    return (cell);
}

t_lst   *create_new_int(t_lst *cell, char what, int file)
{
    if (what == 'i')
        cell->input = file; //input
    if (what == 'o')
        cell->output = file; // output
    if (what == 'b')
        cell->builtin = file; // builtin
    return (cell);
}

int ft_isalpha_parsing(int c)
{
    if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95)
        return (1);
    return (0);
}
