/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:20:28 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/16 18:59:22 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_sep(t_data *d, t_sep *sep)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (d->line[i])
	{
		if (d->line[i] == '|')
			sep->pipe++;
		if (d->line[i] == '&' && d->line[i + 1] == '&')
			sep->double_and++;
		if (d->line[i] == '\'' && d_quote == 0)
		{
			s_quote = 1;
			sep->simple_quo++;
		}
		if (d->line[i] == '"' && s_quote == 0)
		{
			d_quote = 1;
			sep->double_quo++;
		}
		if (d->line[i] == '<' && d->line[i + 1] != '<')
			sep->simple_raft_left++;
		if (d->line[i] == '>' && d->line[i + 1] != '>')
			sep->simple_raft_right++;
		if (d->line[i] == '<' && d->line[i + 1] == '<')
			sep->double_raft_left++;
		if (d->line[i] == '>' && d->line[i + 1] == '>')
			sep->double_raft_right++;
		i++;
	}
}

int	check_sep(t_sep *sep)
{
	if (sep->simple_quo % 2 == 1 || sep->double_quo % 2 == 1)
	{
		ft_putstr_fd("Syntax error : unmatched quote\n", 2);
		g_exit_status = 2;
		return (g_exit_status);
	}
	return (0);
}

void	init_sep(t_sep *sep)
{
	sep->pipe = 0;
	sep->double_and = 0;
    sep->simple_quo = 0;
    sep->double_quo = 0;
    sep->simple_raft_left = 0;
    sep->simple_raft_right = 0;
    sep->double_raft_left = 0;
    sep->double_raft_right = 0;
	sep->infile = NULL;
	sep->outfile = NULL;
}

t_lst	*init_cell()
{
	t_lst	*cell;

	cell = malloc(sizeof(t_lst));
	if (!cell)
		return (NULL);
	cell->cmd = NULL;
	cell->arg = NULL;
	cell->path = NULL;
	cell->input = 0;
	cell->output = 0;
	cell->builtin = 0;
	return (cell);
}

void	print_sep(t_sep *sep, t_data *d)
{
	printf("| = %i\n&& = %i\n' = %i\n\" = %i\n< = %i\n> = %i\n<< = %i\n>> = %i\ninfile = %s\noutfile = %s\n", sep->pipe, sep->double_and, sep->simple_quo, sep->double_quo, sep->simple_raft_left, sep->simple_raft_right, sep->double_raft_left, sep->double_raft_right, sep->infile, sep->outfile);
	for(int q=0; d->split[q]; q++)
		printf("d->split[%i] = %s\n", q, d->split[q]);
}

int	check_chev(t_data *d)
{
	int	i;
	int	j;

	i = 0;
	while (d->split[i])
	{
		j = 0;
		if (d->split[i][0] != '"' && d->split[i][0] != '\'')
		{
			while (d->split[i][j])
			{
				if ((d->split[i][j] == '<' && d->split[i][j + 1] == '<' && d->split[i][j + 2] == '<' && d->split[i][j + 3] == '<') || (d->split[i][j] == '<' && d->split[i][j + 1] == '>'))
				{
					ft_putstr_fd("syntax error near unexpected token `<<<'\n", 2);
					g_exit_status = 2;
					return (g_exit_status);
				}
				else if ((d->split[i][j] == '>' && d->split[i][j + 1] == '>' && d->split[i][j + 2] == '>' && d->split[i][j + 3] == '>') || (d->split[i][j] == '>' && d->split[i][j + 1] == '<'))
				{
					ft_putstr_fd("syntax error near unexpected token `>>>'\n", 2);
					g_exit_status = 2;
					return (g_exit_status);
				}
				else if (d->split[i][j] == '>' && d->split[i + 1] == NULL)
				{
					ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
					g_exit_status = 2;
					return (g_exit_status);
				}
				j++;
			}
		}
		i++;
	}
	return (0);
}

int	check_pipe_space(char *split_pipe)
{
	int	i;
	int	not_space;

	i = 0;
	not_space = 0;
	while (split_pipe[i])
	{
		if (split_pipe[i] != ' ')
			not_space++;
		i++;
	}
	return (not_space);
}

int	check_pipe(char **split_pipe, t_sep *sep)
{
	int	i;

	i = 0;
	if (split_pipe[i])
	{
		while (i <= sep->pipe)
		{
			if (!split_pipe[i] || !check_pipe_space(split_pipe[i]))
			{
				ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
				g_exit_status = 2;
				return (g_exit_status);
			}
			i++;
		}
	}
	else
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		g_exit_status = 2;
		return (g_exit_status);
	}
	return (0);
}

t_lst	*check_infile_outfile(t_data *d, t_sep *sep, t_lst *cell)
{
	int		place_raft;

	place_raft = found_place_raft(d->split, 0);
	if (place_raft != -1)
	{
		while (place_raft != -1)
		{
			if (d->split[place_raft][0] == '<')
			{
				sep->infile = d->split[place_raft + 1];
				if (place_raft != 0)
				{
					while (d->split[place_raft - 1][0] == '-')
						place_raft -= 1;
					cell = create_new_char(cell, d->split[place_raft - 1], NULL, 'c');
				}
				else
				{
					cell = create_new_char(cell, d->split[place_raft + 2], NULL, 'c');
					ft_swap(&d->split[place_raft + 1], &d->split[place_raft + 2]);
				}
			}
			else if (d->split[place_raft][0] == '>')
			{
				sep->outfile = d->split[place_raft + 1];
				if (!cell->cmd)
					cell = create_new_char(cell, d->split[0], NULL, 'c');
			}
			place_raft = found_place_raft(d->split, place_raft + 1);
		}
	}
	else
		cell = create_new_char(cell, d->split[0], NULL, 'c');
	return (cell);
}

t_lst	*fill_in_out_file(t_data *d, t_sep *sep, t_lst *cell)
{
	int	place_raft;
	int	fd[0];

	place_raft = found_place_raft(d->split, 0);
	while (place_raft != -1)
	{
	//	if (cell->input > 0)
	//		close(cell->input);
	//	if (cell->output > 0)
	//		close(cell->output);
		if (d->split[place_raft][0] == '<' && d->split[place_raft][1] != '<')
		{
			printf("infile: |%s|\n", sep->infile);
			cell = create_new_int(cell, 'i', open(sep->infile, O_RDONLY));
		}
		else if (d->split[place_raft][0] == '>' && d->split[place_raft][1] != '>')
		{
			printf("outfile: |%s|\n", sep->outfile);
			cell = create_new_int(cell, 'o', open(sep->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644));
		}
		else if (d->split[place_raft][0] == '>' && d->split[place_raft][1] == '>')
			cell = create_new_int(cell, 'i', open(sep->infile, O_CREAT | O_WRONLY | O_APPEND, 0644));
		else if (d->split[place_raft][0] == '<' && d->split[place_raft][1] == '<')
		{
			if (cell->input > 0)
				close(cell->input);
			fd[0] = heredoc(d, d->split[place_raft + 1]);
			cell->input = fd[0];
		}
//		add_cell_parsing(d, list);
		place_raft = found_place_raft(d->split, place_raft + 1);
	}
	return (cell);
}

int	found_cmd(t_data *d, t_lst *cell)
{
	int	i;

	i = 0;
	while (d->split[i])
	{
		if (ft_strcmp_parsing(cell->cmd, d->split[i]) != 1)
			return (i);
		i++;
	}
	return (-1);
}

t_lst	*fill_arg(t_data *d, t_lst *cell)
{
	int		place_cmd;
	int		i;
	int		tmp;

	i = 0;
	place_cmd = found_cmd(d, cell);
	if (place_cmd != -1)
	{
		tmp = place_cmd;
		while (d->split[place_cmd] && d->split[place_cmd][0] != '|')
			place_cmd++;
		d->argo = malloc(sizeof(char*) * (place_cmd + 1));
		if (!d->argo)
			return (NULL);
		place_cmd = tmp;
		while (d->split[place_cmd] && d->split[place_cmd][0] != '|')
		{
			if (d->split[place_cmd][0] == '<' || d->split[place_cmd][0] == '>')
				break;
			d->argo[i] = d->split[place_cmd];
			i++;
			place_cmd++;
		}
		d->argo[i] = NULL;
		cell = create_new_char(cell, NULL, d->argo, 'a');
		//free arg ??
	}
	return (cell);
}

t_lst	*fill_builtin(t_lst *cell)
{

	if (cmp_str(cell->cmd, "echo") || cmp_str(cell->cmd, "cd") || cmp_str(cell->cmd, "pwd") || cmp_str(cell->cmd, "export") || cmp_str(cell->cmd, "unset") || cmp_str(cell->cmd, "env") || cmp_str(cell->cmd, "exit"))
		cell = create_new_int(cell, 'b', 1);
	else
		cell = create_new_int(cell, 'b', 0);
	return (cell);
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

t_lst	*ft_free_double(char **path, char *cmd, t_lst *cell)
{
	int	i;

	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	if (cmd)
		cell = create_new_char(cell, cmd, NULL, 'p');
	else
		cell = create_new_char(cell, cell->cmd, NULL, 'p');
	return (cell);
}

t_lst	*found_path(t_lst *cell, t_data *d)
{
	char	**path;
	t_env	*tmp;
	int		i;
	char	*c;
	char	*cmd;

	tmp = d->env;
	while (tmp && ft_strcmp_parsing(tmp->key, "PATH"))
		tmp = tmp->next;
	path = ft_split(tmp->value, ':');
	if (!path)
		return (NULL);
	i = 0;
	if (check_if_path(cell->cmd))
		return (ft_free_double(path, NULL, cell));
	while (path[i])
	{
		c = ft_strjoin(path[i], "/");
		cmd = ft_strjoin(c, cell->cmd);
		free(c);
		if (access(cmd, F_OK) != -1)
			return (ft_free_double(path, cmd, cell));
		free(cmd);
		i++;
	}
	cell = create_new_char(cell, NULL, NULL, 'p');
	return (cell);
}

t_lst	*parsing(t_data *d)
{
	t_sep	sep[1];
	char	**split_pipe;
	int		i;
	t_lst	*cell;

	i = 0;
	init_sep(sep);
	fill_sep(d, sep);
	d->split = NULL;
	if (!check_sep(sep))
	{
		split_pipe = ft_split(d->line, '|');
		if (check_pipe(split_pipe, sep))
		{
			cell = init_cell();
			cell->next = NULL;
			ft_free_str(split_pipe);
			add_cell_parsing(d, cell);
			return (d->cmd_lst);
		}
		while (split_pipe[i])
		{
			d->split = ft_split_parsing(split_pipe[i], d);
			if (!check_chev(d))
			{
				cell = init_cell();
				cell = check_infile_outfile(d, sep, cell); //--> detecte la cmd quand il y a chevrons
				cell = fill_in_out_file(d, sep, cell);	//ouvrir et detecte les fichiers avec chevrons
				cell = fill_builtin(cell);
				if (cell->builtin == 0)
					cell = found_path(cell, d);
				cell = fill_arg(d, cell); // remplir les arguments
				cell->next = NULL;
				add_cell_parsing(d, cell);
//			print_sep(sep, d);
			}
			else
			{
				cell = init_cell();
				cell->next = NULL;
				free(d->sp);
				ft_free_str(split_pipe);
				add_cell_parsing(d, cell);
				return (d->cmd_lst);
			}
			i++;
			free(d->sp);
			d->sp = NULL;
		}
	ft_free_str(split_pipe);
	print_list(d->cmd_lst);
	}
	else
	{
		cell = init_cell();
		cell->next = NULL;
		add_cell_parsing(d, cell);
		return (d->cmd_lst);
	}
	return (d->cmd_lst);
}
