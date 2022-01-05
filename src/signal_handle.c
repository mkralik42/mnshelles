/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:41:17 by mkralik           #+#    #+#             */
/*   Updated: 2022/01/05 14:29:31 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signal_sigint(void)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void	ft_signal_stdin(void)
{
	printf("\n");
	g_exit_status = 130;
}

void	handle_sig(int signum)
{
	if (signum == SIGINT)
		ft_signal_sigint();
	else if (signum == SIGQUIT)
		write(1, "\b\b  \b\b", 6);
}

void	handle_sig_child(int signum)
{
	if (signum == SIGINT)
		ft_signal_stdin();
	else if (signum == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		g_exit_status = 131;
	}
}

void	handle_sig_cmd(int signum)
{
	if (signum == SIGINT)
		printf("\n");
	else if (signum == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		g_exit_status = 131;
	}
}
