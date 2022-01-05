/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2022/01/05 17:15:28 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*fill_memory(char *s, t_data *d, int i, t_env *tmp)
{
	char	*nb_exit;

	nb_exit = ft_itoa(g_exit_status);
	if (d->sp->remember_mem == 0 && s[d->sp->line] != '?')
		d->sp->new[d->sp->j] = (char *)malloc((sizeof(char)
					* (d->sp->count_c - i) + ft_strlen(tmp->value) + 1));
	else if (d->sp->remember_mem == 0 && s[d->sp->line] == '?')
		d->sp->new[d->sp->j] = (char *)malloc((sizeof(char)
					* (d->sp->count_c - i) + ft_strlen(nb_exit) + 1));
	else if (d->sp->remember_mem != 0 && s[d->sp->line] != '?')
		d->sp->new[d->sp->j] = (char *)malloc(sizeof(char)
				* (d->sp->remember_mem + ft_strlen(tmp->value) + 1));
	else if (d->sp->remember_mem != 0 && s[d->sp->line] == '?')
		d->sp->new[d->sp->j] = (char *)malloc(sizeof(char)
				* (d->sp->remember_mem + ft_strlen(nb_exit) + 1));
	if (s[d->sp->line] == '?' && s[d->sp->line + 1] == 0)
		d->sp->remember_mem += ft_strlen(nb_exit);
	else
		d->sp->remember_mem += ft_strlen(tmp->value);
	return (nb_exit);
}

void	copy_var_env(char *s, t_data *d, t_env *tmp, char *nb_exit)
{
	int	i;

	i = 0;
	if (s[d->sp->line] == '?')
	{
		d->sp->line++;
		while (nb_exit[i])
		{
			d->sp->new[d->sp->j][d->sp->k] = nb_exit[i];
			d->sp->k++;
			i++;
		}
	}
	else
	{
		while (tmp->value[i])
		{
			d->sp->new[d->sp->j][d->sp->k] = tmp->value[i];
			d->sp->k++;
			i++;
		}
	}
}

void	execute_var_env(char *s, t_data *d, int i, t_env *tmp)
{
	char	*bis;
	char	*nb_exit;

	d->sp->new[d->sp->j][d->sp->k] = 0;
	bis = (char *)malloc(sizeof(char) * (ft_strlen(d->sp->new[d->sp->j]) + 1));
	ft_strcpy(bis, d->sp->new[d->sp->j]);
	free(d->sp->new[d->sp->j]);
	d->sp->new[d->sp->j] = NULL;
	nb_exit = fill_memory(s, d, i, tmp);
	i = 0;
	d->sp->k = 0;
	while (bis[i])
	{
		d->sp->new[d->sp->j][d->sp->k] = bis[i];
		d->sp->k++;
		i++;
	}
	copy_var_env(s, d, tmp, nb_exit);
	free(bis);
	free(nb_exit);
}

char	*make_change_new(char *s, t_data *d, int *i, int size_new)
{
	char	*new;
	int		j;

	j = 0;
	new = (char *)malloc(sizeof(char) * (size_new + 1));
	while (size_new > 0)
	{
		new[j] = s[d->sp->line];
		d->sp->line++;
		(*i)++;
		j++;
		size_new--;
	}
	new[j] = 0;
	return (new);
}

char	*make_change(char *s, t_data *d)
{
	char	*new;
	int		size_new;
	int		i;
	t_env	*tmp;

	size_new = 0;
	while (s[d->sp->line++] && !is_charset(s[d->sp->line])
		&& ft_isalpha_parsing(s[d->sp->line]))
		size_new++;
	d->sp->line -= size_new;
	i = 0;
	new = make_change_new(s, d, &i, size_new);
	tmp = d->env;
	while (tmp->next && !cmp_str(tmp->key, new))
		tmp = tmp->next;
	if (cmp_str(tmp->key, new) || s[d->sp->line] == '?')
		execute_var_env(s, d, i, tmp);
	else
	{
		free(new);
		return (NULL);
	}
	free(new);
	return (d->sp->new[d->sp->j]);
}
