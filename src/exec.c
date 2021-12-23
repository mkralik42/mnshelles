/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 14:48:10 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/23 13:58:33 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_lst *cmd_lst, t_data *data)
{
	g_exit_status = 0;
	if (!ft_strcmp(cmd_lst->cmd, "echo"))
		return (exec_echo(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "cd"))
		return (exec_cd(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "pwd"))
		return (exec_pwd(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "export"))
		return (exec_export(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "unset"))
		return (exec_unset(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "env"))
		return (exec_env(cmd_lst, data));
	else if (!ft_strcmp(cmd_lst->cmd, "exit"))
		exec_exit(data, cmd_lst);
	return (EXIT_FAILURE);
}

int	ft_execute(t_data *data, int exit_code, t_lst *lst, char **ch_env)
{
	if (lst && lst->builtin)
	{
		exit_code = exec_builtin(lst, data);
		ft_free_exit(data);
		free_dble_str(ch_env);
		exit(exit_code);
	}
	else if (lst && lst->path)
		exit_code = execve(lst->path, lst->arg, ch_env);
	else
	{
		error_cmd(lst->cmd);
		g_exit_status = 127;
		ft_free_exit(data);
		free_dble_str(ch_env);
		exit (g_exit_status);
	}
	return (exit_code);
}
