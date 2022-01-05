/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 22:37:57 by paulguign         #+#    #+#             */
/*   Updated: 2022/01/05 18:35:57 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_cmd(char *arg)
{
	if (!arg[0])
	{
		arg = (char *)malloc(sizeof(char));
		if (error_catch(!arg, NULL, "Malloc failed"))
			exit (1);
		*arg = 0;
	}
	error_catch(1, arg, "command not found");
	if (!arg[0])
		free(arg);
}

int	error_catch(int test, char *file, char *msg)
{
	char	*str;
	char	*tmp;

	if (test)
	{
		if (file)
		{
			tmp = ft_strjoin("bash: ", file);
			str = ft_strjoin(tmp, ": ");
			free(tmp);
			tmp = ft_strjoin(str, msg);
			free(str);
		}
		else
			tmp = ft_strjoin("bash: ", msg);
		str = ft_strjoin(tmp, "\n");
		free(tmp);
		if (error_catch(!str, NULL, "Malloc failed"))
			return (1);
		ft_putstr_fd(str, 2);
		free(str);
	}
	return (test);
}
