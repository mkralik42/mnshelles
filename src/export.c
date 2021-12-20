/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 14:51:48 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/20 21:33:07 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_cell_env(char *key, char *new_value, t_env *env)
{
	t_env	*block;
	char	*to_free;

	block = env;
	while (block)
	{
		if (!ft_strcmp(block->key, key))
			break ;
		block = block->next;
	}
	if (!block->value[0])
		block->with_value = 0;
	to_free = block->value;
	block->value = ft_strdup(new_value);
	free (to_free);
	if (!block->value)
		return ;
}

int	already_there(char *arg, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, arg))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	export_w_value(t_data *data, char **arg, int i)
{
	char	**add_export;
	char	**add_env;

	add_export = ft_split_env(arg[i], '=');
	if (!already_there(add_export[0], data->export))
		add_cell(&data->export, new_cell_test(add_export[0], add_export[1], 0));
	else
		change_cell_env(add_export[0], add_export[1], data->export);

	add_env = ft_split_env(arg[i], '=');
	if (!already_there(add_env[0], data->env))
		add_cell(&data->env, new_cell_test(add_env[0], add_env[1], 0));
	else
		change_cell_env(add_env[0], add_env[1], data->env);
	free_tab(add_export, 2);
	free_tab(add_env, 2);
}

int	exec_export(t_lst *cmd_lst, t_data *data)
{
	int		i;

	i = 0;
	g_exit_status = 0;
	if (!cmd_lst->arg[1])
		print_export(data->export);
	while (cmd_lst->arg && cmd_lst->arg[++i])
	{
		if (!ft_isalpha(cmd_lst->arg[i][0]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(cmd_lst->arg[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_exit_status = 1;
		}
		else if (!ft_strnstr(cmd_lst->arg[i], "=", ft_strlen(cmd_lst->arg[i])))
		{
			if (!already_there(cmd_lst->arg[i], data->export))
				add_cell(&data->export, new_cell(ft_strdup(cmd_lst->arg[i]), ft_strdup(""), 1));
		}
		else
			export_w_value(data, cmd_lst->arg, i);
	}
	return (g_exit_status);
}
