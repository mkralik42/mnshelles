/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 14:48:10 by mkralik           #+#    #+#             */
/*   Updated: 2022/01/05 18:33:11 by mkralik          ###   ########.fr       */
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
	else if (!ft_strcmp(cmd_lst->cmd, "exit") && !data->nb_pipe)
		exec_exit(data, cmd_lst);
	return (EXIT_FAILURE);
}

void	free_before_exit(t_data *data, char **ch_env)
{
	ft_free_exit(data);
	free_dble_str(ch_env);
}

int	ft_execute(t_data *data, int exit_code, t_lst *lst, char **ch_env)
{
	if (lst && lst->builtin)
	{
		exit_code = exec_builtin(lst, data);
		free_before_exit(data, ch_env);
		exit(exit_code);
	}
	else if (lst && lst->path)
	{
		exit_code = execve(lst->path, lst->arg, ch_env);
		if (exit_code == -1)
		{
			free_before_exit(data, ch_env);
			g_exit_status = 126;
			exit(g_exit_status);
		}
	}
	else
	{
		error_cmd(lst->cmd);
		g_exit_status = 127;
		free_before_exit(data, ch_env);
		exit (g_exit_status);
	}
	return (exit_code);
}
