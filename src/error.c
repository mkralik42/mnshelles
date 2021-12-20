/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 22:37:57 by paulguign         #+#    #+#             */
/*   Updated: 2021/12/20 20:11:40 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
