/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:46:27 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/23 15:31:15 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_first_env(t_env **env, t_env *tmp)
{
	tmp = (*env)->next;
	if ((*env)->key)
		free((*env)->key);
	if ((*env)->value)
		free((*env)->value);
	if (*env)
		free(*env);
	*env = NULL;
	*env = tmp;
}

t_env	*get_prev(char *key, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp->next)
	{
		if (!ft_strcmp(tmp->next->key, key))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	unset(char *arg, t_env **env)
{
	t_env	*prev;
	t_env	*tmp;

	tmp = NULL;
	if (!get_key(arg, *env))
		return ;
	prev = get_prev(arg, *env);
	if (!prev)
	{
		free_first_env(env, tmp);
		return ;
	}
	tmp = prev->next->next;
	if (prev->next->value)
		free(prev->next->value);
	if (prev->next->key)
		free(prev->next->key);
	free(prev->next);
	prev->next = tmp;
}

int	error_var_name(char *line)
{
	int	i;

	i = -1;
	while (line[++i] && line[i] != '=')
	{
		if (ft_isdigit(line[0]))
			return (1);
		if ((line[i] >= '0' && line[i] <= '9')
			|| (line[i] >= 'A' && line[i] <= 'Z')
			|| (line[i] >= 'a' && line[i] <= 'z') || line[i] == '_')
			return (0);
		else
			return (1);
	}
	return (1);
}

int	exec_unset(t_lst *cmd_lst, t_data *data)
{
	int	i;

	i = 0;
	if (!cmd_lst->arg || !cmd_lst->arg[1])
		return (g_exit_status);
	while (cmd_lst->arg && cmd_lst->arg[++i])
	{
		if (error_var_name(cmd_lst->arg[i]))
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(cmd_lst->arg[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_exit_status = 1;
		}
		else
		{
			unset(cmd_lst->arg[i], &data->env);
			unset(cmd_lst->arg[i], &data->export);
		}
	}
	return (g_exit_status);
}
