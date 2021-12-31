/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 17:01:08 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/31 14:40:13 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_signal_child(t_data *data)
{
	(void) data;
	signal(SIGINT, handle_sig_child);
	signal(SIGQUIT, handle_sig_child);
	signal(SIGTSTP, handle_sig_child);
}

void	init_signal(t_data *data)
{
	(void) data;
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, handle_sig);
	signal(SIGTSTP, handle_sig);
}

void	init_signal_cmd(t_data *data)
{
	(void)data;
	signal(SIGINT, handle_sig_cmd);
	signal(SIGQUIT, handle_sig_cmd);
}
