/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_handle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 19:48:09 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/20 19:07:05 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_dble_dash(t_data *data, int *new)
{
	if (get_key("HOME", data->env))
		*new = chdir(get_key("HOME", data->env));
	else
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		g_exit_status = 1;
		return (g_exit_status);
	}
	return (0);
}

int	cd_simple_dash(t_data *data, int *new, char *old_pwd)
{
	if (get_key("OLDPWD", data->env))
	{
		ft_putstr_fd(get_key("OLDPWD", data->env), 1);
		ft_putstr_fd("\n", 1);
		*new = chdir(get_key("OLDPWD", data->env));
	}
	else
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		g_exit_status = 1;
		if (old_pwd)
			free(old_pwd);
		return (g_exit_status);
	}
	return (0);
}

void	cd(t_lst *cmd_lst, int *new, char *cdpath)
{
	if (cdpath && cmd_lst->arg[1] && cmd_lst->arg[1][0] != '/'
		&& ft_strcmp(cmd_lst->arg[1], ".") && ft_strcmp(cmd_lst->arg[1], "..")
		&& cdpath[ft_strlen(cdpath) - 1] == '/')
		*new = use_cdpath(cmd_lst, cdpath);
	else
		*new = chdir(cmd_lst->arg[1]);
}

void	cd_no_file(t_lst *cmd_lst, int new)
{
	if (new == -1)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(cmd_lst->arg[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_exit_status = 1;
	}
}

int	cd_too_many(void)
{
	ft_putstr_fd("cd: too many arguments\n", 2);
	g_exit_status = 1;
	return (g_exit_status);
}
