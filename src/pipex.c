/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 18:40:18 by mkralik           #+#    #+#             */
/*   Updated: 2022/01/05 18:40:22 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_pipe_exec(t_data *data, t_lst *lst, int *fd)
{
	char	**ch_env;

	ch_env = NULL;
	if (error_catch(dup2(fd[0], STDIN_FILENO) < 0, NULL, strerror(errno)))
	{
		g_exit_status = 1;
		ft_free_all(data);
		exit (g_exit_status);
	}
	if (error_catch(dup2(fd[1], STDOUT_FILENO) < 0, NULL, strerror(errno)))
	{
		g_exit_status = 1;
		ft_free_all(data);
		exit (g_exit_status);
	}
	close(fd[0]);
	close(fd[1]);
	ch_env = get_env_to_char(data->env);
	ft_execute(data, 0, lst, ch_env);
	free_dble_str(ch_env);
	return (g_exit_status);
}

void	pipe_child(t_data *data, int *fd, t_lst *lst, int fd_in)
{
	init_signal_child(data);
	close(fd[0]);
	if (fd_in == -1 && lst->input == 0)
		fd[0] = open("/dev/stdin", O_RDONLY);
	else if (lst->input == 0)
		fd[0] = fd_in;
	else
	{
		fd[0] = lst->input;
		if (fd_in > 0)
			close(fd_in);
	}
	if (lst->output == 0 && !lst->next)
	{
		close(fd[1]);
		fd[1] = open("/dev/stdout", O_WRONLY);
	}
	else if (lst->output != 0)
	{
		close(fd[1]);
		fd[1] = lst->output;
	}
	ft_pipe_exec(data, lst, fd);
}

int	handle_pipe(int *fd, int *pid)
{
	if (error_catch(pipe(fd) < 0, NULL, strerror(errno)))
	{
		close(fd[0]);
		close(fd[1]);
		return (1);
	}
	*pid = fork();
	if (error_catch(*pid < 0, NULL, strerror(errno)))
	{
		close(fd[0]);
		close(fd[1]);
		return (1);
	}
	return (0);
}

void	handle_ret(int pid, int status, t_lst *lst, int *ret)
{
	if (lst->output)
		close(lst->output);
	if (lst->input)
		close(lst->input);
	waitpid(pid, &status, WUNTRACED);
	if (!lst->next && WIFEXITED(status))
		*ret = WEXITSTATUS(status);
	else if (!lst->next && WIFSIGNALED(status))
	{
		*ret = WTERMSIG(status);
		if (*ret == 3)
			*ret = 131;
		if (*ret == 2)
			*ret = 130;
	}
}

int	ft_pipe(t_data *data, t_lst *lst, int fd_in, int step)
{
	int	fd[2];
	int	pid;
	int	status;
	int	ret;

	init_signal_cmd(data);
	status = 0;
	if (lst && !lst->next && lst->builtin
		&& step == 1 && ft_strcmp(lst->cmd, "echo"))
	{
		ret = exec_builtin(lst, data);
		return (ret);
	}
	if (handle_pipe(fd, &pid) == 1)
		return (1);
	if (pid == 0)
		pipe_child(data, fd, lst, fd_in);
	close(fd[1]);
	if (fd_in > 0)
		close(fd_in);
	if (lst->next)
		ret = ft_pipe(data, lst->next, fd[0], step + 1);
	close(fd[0]);
	handle_ret(pid, status, lst, &ret);
	return (ret);
}
