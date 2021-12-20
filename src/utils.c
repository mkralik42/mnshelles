/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 14:00:01 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/20 14:44:23 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	while (s && *s)
		write(fd, s++, 1);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

void	*ft_memset(void *b, int c, int len)
{
	unsigned char	*f;

	f = b;
	while (len--)
		*f++ = c;
	return (b);
}

void	*ft_calloc(int count, int size)
{
	void	*p;

	p = malloc(size * count);
	if (!p)
		return (NULL);
	ft_memset(p, 0, size * count);
	return (p);
}
