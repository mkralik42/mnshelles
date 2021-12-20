/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:40:36 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/20 21:25:19 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_env	*new_cell(char *key, char *value, int with_value)
{
	t_env	*cell;

	cell = malloc(sizeof(t_env));
	if (!cell)
		return (NULL);
	cell->key = key;
	cell->value = value;
	cell->with_value = with_value;
	cell->next = NULL;
	return (cell);
}

t_env	*new_cell_test(char *key, char *value, int with_value)
{
	t_env	*cell;

	cell = malloc(sizeof(t_env));
	if (!cell)
		return (NULL);
	cell->key = ft_strdup(key);
	cell->value = ft_strdup(value);
	cell->with_value = with_value;
	cell->next = NULL;
	return (cell);
}

void	add_cell(t_env **env, t_env *new)
{
	t_env	*last;

	if (!*env)
		*env = new;
	else
	{
		last = ft_lstlast(*env);
		last->next = new;
	}
}
