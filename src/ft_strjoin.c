/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:32:23 by mkralik           #+#    #+#             */
/*   Updated: 2021/12/20 13:54:10 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*d;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	d = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (d == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		d[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		d[i + j] = s2[j];
		j++;
	}
	d[i + j] = 0;
	return (d);
}

char	*ft_free_strjoin(char *s1, char *s2)
{
	char	*d;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	d = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (d == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		d[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		d[i + j] = s2[j];
		j++;
	}
	d[i + j] = 0;
	if (s1)
		free(s1);
	return (d);
}

int	ft_atoi(const char *str)
{
	unsigned long long	value;
	int					sign;
	size_t				i;

	value = 0;
	sign = 1;
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		value = value * 10 + str[i++] - 48;
	if (value > 9223372036854775807 && sign == 1)
		return (-1);
	else if (value > 9223372036854775807 && sign == -1)
		return (0);
	return (value * sign);
}

// static size_t	nbrlen(long n)
// {
// 	size_t	i;

// 	i = 0;
// 	if (n == 0)
// 		i++;
// 	if (n < 0)
// 	{
// 		n = -n;
// 		i++;
// 	}
// 	while (n > 0)
// 	{
// 		n /= 10;
// 		i++;
// 	}
// 	return (i);
// }

// char	*ft_itoa(int n)
// {
// 	char	*ret;
// 	long	nb;
// 	size_t	len;

// 	nb = n;
// 	len = nbrlen(n);
// 	ret = malloc(sizeof(char) * (len + 1));
// 	if (!ret)
// 		return (NULL);
// 	ret[len--] = 0;
// 	if (n == 0)
// 		ret[0] = '0';
// 	if (nb < 0)
// 	{
// 		ret[0] = '-';
// 		nb = -nb;
// 	}
// 	while (nb)
// 	{
// 		ret[len--] = (nb % 10) + 48;
// 		nb /= 10;
// 	}
// 	return (ret);
// }
