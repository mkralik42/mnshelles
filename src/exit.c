/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 17:34:26 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/20 16:56:48 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_is_digit(char *s)
{
	int	i;

	i = 0;
	if (!ft_isdigit(s[i]) && !(s[i] == '+' || s[i] == '-'))
		return (1);
	while (s[++i])
	{
		if (!ft_isdigit(s[i]))
			return (1);
	}
	return (0);
}

int	get_exit_code(t_data *data, t_lst *cmd_lst)
{
	(void) data;
	g_exit_status = 0;
	if (!cmd_lst->arg[1])
	{
		if (data->split)
				ft_free_str(data->split);
		if (data->cmd_lst)
				free_cmd_lst(data, &data->cmd_lst);
		ft_free_all(data);
		exit(g_exit_status);
	}
	if (cmd_lst->arg && cmd_lst->arg[1])
	{
		if (exit_is_digit(cmd_lst->arg[1]))
		{
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(cmd_lst->arg[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_exit_status = 2;
			exit(g_exit_status);
		}
		else
		{
			if (cmd_lst->arg[2])
			{
				ft_putstr_fd("bash : exit: too many arguments\n", 2);
				g_exit_status = 1;
			}
			else
			{
				g_exit_status = ft_atoi(cmd_lst->arg[1]) % 256;
				exit(g_exit_status);
			}
		}
	}
	return (g_exit_status);
}

int	exec_exit(t_data *data, t_lst *cmd_lst)
{
	(void) data;
	ft_putstr_fd("exit\n", 1);
	g_exit_status = get_exit_code(data, cmd_lst);
	if (data->split)
				ft_free_str(data->split);
	if (data->cmd_lst)
				free_cmd_lst(data, &data->cmd_lst);
	ft_free_all(data);
	return (g_exit_status);
}
