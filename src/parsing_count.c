/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_count.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/21 11:52:27 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sp    *init_sp()
{
    t_sp    *sp;

    sp = malloc(sizeof(t_sp));
    sp->line = 0;
    sp->count = 0;
    sp->j = -1;
    sp->k = 0;
    sp->new = NULL;
    sp->s_quote = 0;
    sp->d_quote = 0;
    sp->count_c = 0;
    sp->remember_mem = 0;
    return (sp);
}

void    check_quote(char str, t_data *d)
{
    if (str == '\'' && d->sp->d_quote == 0)
    {
        if (d->sp->s_quote == 0)
            d->sp->s_quote = 1;
        else
            d->sp->s_quote = 0;
    }
    if (str == '"' && d->sp->s_quote == 0)
    {
        if (d->sp->d_quote == 0)
            d->sp->d_quote = 1;
        else
            d->sp->d_quote = 0;
    }
}

int count_char_i(const char *s, t_data *d, int i, int *count)
{
    if (s[i] == '"' && d->sp->s_quote == 0)
        i++;
    else if (s[d->sp->line] == '\'' && d->sp->d_quote == 0)
        i++;
    else
    {
        i++;
        (*count)++;
    }
    return (i);
}

size_t   count_char(const char *s, char c, t_data *d)
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
                break;
            i = count_char_i(s, d, i, &count);
        }
    }
    else if (c == 'c')
        while (s[i] && (s[i] == '<' || s[i] == '>'))
        {
            i++;
            count++;
        }
    d->sp->s_quote = 0;
    d->sp->d_quote = 0;
    return (count);
}

