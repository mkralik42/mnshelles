/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 14:48:10 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/20 21:12:33 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		exit (g_exit_status);
	}
	return (exit_code);
}
