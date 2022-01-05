/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:38:51 by mkralik           #+#    #+#             */
/*   Updated: 2022/01/05 14:37:18 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
}

void	free_dble_str(char **str)
{
	int	i;

	if (!str || !*str)
		return ;
	i = -1;
	while (str && str[++i])
		free_str(&str[i]);
	free(str);
	str = NULL;
}

void	remove_from_list(t_lst *cmd_lst, t_data *data)
{
	(void)data;
	if (cmd_lst->input > 0)
		close(cmd_lst->input);
	if (cmd_lst->output > 0)
		close(cmd_lst->output);
	if (!cmd_lst)
		return ;
	if (cmd_lst->cmd)
		free(cmd_lst->cmd);
	if (cmd_lst->arg)
		ft_free_str(cmd_lst->arg);
	if (cmd_lst->path)
		free(cmd_lst->path);
	free(cmd_lst);
	cmd_lst = NULL;
}

void	free_cmd_lst(t_data *data, t_lst **cmd_lst)
{
	t_lst	*tmp;

	if (!(*cmd_lst) || !cmd_lst)
		return ;
	tmp = *cmd_lst;
	while (tmp)
	{
		if ((*cmd_lst)->next)
			*cmd_lst = (*cmd_lst)->next;
		else
			*cmd_lst = NULL;
		remove_from_list(tmp, data);
		tmp = *cmd_lst;
	}
	if (*cmd_lst)
		free(*cmd_lst);
	(*cmd_lst) = NULL;
}

void	ft_free_all(t_data *data)
{
	if (!data)
		return ;
	if (data->cmd_lst)
		free_cmd_lst(data, &data->cmd_lst);
	if (data->prompt)
		free_str(&data->prompt);
	if (data->line)
		free(data->line);
	if (data->env)
		free_env(data->env);
	if (data->export)
		free_env(data->export);
	if (data)
		free(data);
	rl_clear_history();
}
