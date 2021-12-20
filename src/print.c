/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:50:31 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/14 18:50:56 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putchar_fd('=', 1);
		ft_putstr_fd(tmp->value, 1);
		ft_putchar_fd('\n', 1);
		tmp = tmp->next;
	}
}

void	print_export(t_env *export)
{
	t_env	*tmp;

	tmp = export;
	while (tmp)
	{
		ft_putstr_fd("export ", 1);
		ft_putstr_fd(tmp->key, 1);
		if (!tmp->with_value)
		{
			ft_putstr_fd("=", 1);
			ft_putchar_fd('"', 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putchar_fd('"', 1);
		}
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
}
