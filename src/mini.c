/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 15:50:42 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/21 19:51:45 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

char	*design_prompt(t_data *data)
{
	char	*user;

	user = get_key("USER", data->env);
	user = ft_strjoin("\e[34;1m", user);
	user = ft_free_strjoin(user, "\e[0m: ❤️ ");
	user = ft_free_strjoin(user, "\e[92;7m");
	user = ft_free_strjoin(user, "minishell");
	user = ft_free_strjoin(user, "\e[0m > ");
	return (user);
}

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

t_env	*get_env_export(char **envp)
{
	t_env	*block;
	int		i;
	char	**cell;
	int		SHLVL;

	i = 0;
	block = NULL;
	while (envp[i])
	{
		cell = ft_split_env(envp[i], '=');
		if (!ft_strcmp(cell[0], "SHLVL"))
		{
			SHLVL = ft_atoi(cell[1]) + 1;
			free(cell[1]);
			cell[1] = ft_itoa(SHLVL);
		}
		add_cell(&block, new_cell(cell[0], cell[1], 0));
		free(cell);
		i++;
	}
	return (block);
}

t_data	*init_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->prompt = NULL;
	data->line = NULL;
	data->env = get_env_export(envp);
	data->export = get_env_export(envp);
	data->sp = NULL;
	data->cmd_lst = NULL;
	data->split = NULL;
	data->argo = NULL;
	init_signal(data);

	return (data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*d;

	(void) argv;
	if (*envp == NULL)
	{
		ft_putstr_fd("Basic environment variables are missing\n", 2);
		exit (1);
	}
	if (argc != 1)
		exit(EXIT_FAILURE);
	d = init_data(envp);
	if (!d)
		return (1);
	d->prompt = design_prompt(d);
	while (1)
	{
		init_signal(d);
		d->line = readline(d->prompt);
		if (!d->line)
		{
			printf("exit\n");
			break ;
		}
		if (d->line[0])
		{
			add_history(d->line);
			d->cmd_lst = parsing(d);
			if (d->cmd_lst->cmd)
			{
				g_exit_status = ft_pipe(d, d->cmd_lst, -1, 1);
			}
			if (d->split)
				ft_free_str(d->split);
			if (d->cmd_lst)
				free_cmd_lst(d, &d->cmd_lst);
			if (d->line)
				free(d->line);
		}
	}
	ft_free_all(d);
	return (0);
}
