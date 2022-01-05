/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:58:26 by mkralik           #+#    #+#             */
/*   Updated: 2022/01/05 18:24:38 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	use_cdpath(t_lst *cmd_lst, char *cdpath)
{
	char	*path;
	int		new;

	path = ft_strjoin(cdpath, cmd_lst->arg[1]);
	new = chdir(path);
	if (new == -1)
	{
		free(path);
		return (chdir(cmd_lst->arg[1]));
	}
	ft_putstr_fd(path, 1);
	ft_putstr_fd("\n", 1);
	free(path);
	return (new);
}

void	update_pwd(t_data *data, char *pwd, char *old_pwd)
{
	if (get_key("PWD", data->env))
	{
		change_cell_env("PWD", pwd, data->env);
		change_cell_env("PWD", pwd, data->export);
	}
	if (get_key("OLDPWD", data->env))
	{
		change_cell_env("OLDPWD", old_pwd, data->env);
		change_cell_env("OLDPWD", old_pwd, data->export);
	}
}

char	*ft_getcwd(void)
{
	char	*cwd;
	int		i;

	i = 1;
	cwd = getcwd(NULL, i++);
	while (errno)
	{
		errno = 0;
		free(cwd);
		cwd = getcwd(NULL, i++);
		if (errno == 2)
			break ;
	}
	return (cwd);
}

int	exec_cd(t_lst *cmd_lst, t_data *data)
{
	char	*old_pwd;
	char	*pwd;
	char	*cdpath;
	int		new;

	new = 0;
	cdpath = get_key("CDPATH", data->env);
	old_pwd = ft_getcwd();
	if (cmd_lst->arg && cmd_lst->arg[1] && cmd_lst->arg[2])
		return (cd_too_many(old_pwd));
	else if (!cmd_lst->arg[1] || !ft_strcmp(cmd_lst->arg[1], "--"))
		cd_dble_dash(data, &new);
	else if (!ft_strcmp(cmd_lst->arg[1], "-"))
		cd_simple_dash(data, &new, old_pwd);
	else
		cd(cmd_lst, &new, cdpath);
	pwd = ft_getcwd();
	if (new == 0)
		update_pwd(data, pwd, old_pwd);
	cd_no_file(cmd_lst, new);
	free_str(&pwd);
	free_str(&old_pwd);
	return (g_exit_status);
}
