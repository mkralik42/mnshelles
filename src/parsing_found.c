/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_found.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/21 22:56:20 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	found_cmd(t_data *d, t_lst *cell)
{
	int	i;

	i = 0;
	while (d->split[i] && cell->cmd)
	{
		if (ft_strcmp_parsing(cell->cmd, d->split[i]) != 1)
			return (i);
		i++;
	}
	return (-1);
}

char	*found_path_data(char **path, int i, char *cmd, t_lst *cell)
{
	char	*c;

	c = ft_strjoin(path[i], "/");
	cmd = ft_strjoin(c, cell->cmd);
	free(c);
	return (cmd);
}

t_lst	*found_path(t_lst *cell, t_data *d)
{
	char	**path;
	t_env	*tmp;
	int		i;
	char	*cmd;

	tmp = d->env;
	path = NULL;
	while (tmp && ft_strcmp_parsing(tmp->key, "PATH"))
		tmp = tmp->next;
	if (tmp)
		path = ft_split(tmp->value, ':');
	//if (!path)
	//	return (NULL);
	i = 0;
	if (check_if_path(cell->cmd))
		return (ft_free_double(path, NULL, cell));
	while (path && path[i])
	{
		cmd = found_path_data(path, i, cmd, cell);
		if (access(cmd, F_OK) != -1)
			return (ft_free_double(path, cmd, cell));
		free(cmd);
		i++;
	}
	if (path)
		ft_free_str(path);
	cell = create_new_char(cell, NULL, NULL, 'p');
	return (cell);
}

void	print_sep(t_sep *sep, t_data *d)
{
	printf("| = %i\n&& = %i\n' = %i\n\" = %i\n< = %i\n> = %i\n<< = %i\n>> = %i\ninfile = %s\noutfile = %s\n", sep->pipe, sep->double_and, sep->simple_quo, sep->double_quo, sep->simple_raft_left, sep->simple_raft_right, sep->double_raft_left, sep->double_raft_right, sep->infile, sep->outfile);
	for(int q=0; d->split[q]; q++)
		printf("d->split[%i] = %s\n", q, d->split[q]);
}

int	check_if_path(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i] == '/')
			return (1);
		i++;
	}
	return (0);
}
