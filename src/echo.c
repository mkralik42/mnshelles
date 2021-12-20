/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 14:23:57 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/20 15:38:28 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dash(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] == 'n')
		{
			if (str[i + 1] == '\0')
			{
				return (1);
			}
			i++;
		}
	}
	return (0);
}

int	ft_echo(t_lst *cmd_lst, int *wrote, int *dash_found)
{
	int	i;

	i = 1;
	while (cmd_lst->arg[i])
	{
		if (is_dash(cmd_lst->arg[i]) == 1 && (*wrote) == 0)
		{
			(*dash_found) = 1;
			i++;
		}
		else
		{
			ft_putstr_fd(cmd_lst->arg[i], 1);
			if (cmd_lst->arg[i + 1])
				ft_putstr_fd(" ", 1);
			(*wrote) = 1;
			i++;
		}
	}
	return (0);
}

int	exec_echo(t_lst *cmd_lst, t_data *data)
{
	int	wrote;
	int	dash_found;

	(void) data;
	wrote = 0;
	dash_found = 0;
	if (!cmd_lst->arg)
	{
		ft_putstr_fd("\n", 1);
		return (g_exit_status);
	}
	ft_echo(cmd_lst, &wrote, &dash_found);
	if (wrote == 1 && dash_found == 0)
		ft_putstr_fd("\n", 1);
	return (g_exit_status);
}
