/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavallu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/16 16:03:37 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sp	*init_sp()
{
	t_sp	*sp;

	sp = malloc(sizeof(t_sp));
	sp->line = 0;
	sp->count = 0;
	sp->j = -1;
	sp->k = 0;
	sp->new = NULL;
	sp->s_quote = 0;
	sp->d_quote = 0;
	sp->count_c = 0;
	sp->remember_mem = 0;
	return (sp);
}

int is_charset(char s)
{
	if (s == ' ' || s == '<' || s == '>')
		return (1);
	return (0);
}

static int   count_charset(const char *s)
{
	int	i;
	int	words;
	int	s_quote;
	int	d_quote;

	words = 0;
	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (s[i])
	{  
		if (is_charset(s[i]))
		{
			if (s[i] == '<' || s[i] == '>')
			{
				while (s[i] == '<' || s[i] == '>')
					i++;
				words++;
			}
			else
				while (s[i] == ' ')
					i++;
		}
		else
		{
			if (s[i] == '"' && s_quote == 0)
			{
				d_quote = 1;
				i++;
				while (s[i] && s[i] != '"')
					i++;
				i++;
				if (s[i] && s[i] != ' ')
					words--;
			}
			else if (s[i] == '\'' && d_quote == 0)
			{
				s_quote = 1;
				i++;
				while (s[i] && s[i] != '\'')
					i++;
				i++;
				if (s[i] && s[i] != ' ')
					words--;
			}
			else
			{
				while (s[i] && !is_charset(s[i]) && s[i] != '"' && s[i] != '\'')
					i++;
				if (s[i] && !is_charset(s[i]))
					words--;
			}
			words++;
		}
	}
	return (words);
}

void    check_quote(char str, t_data *d)
{
	if (str == '\'' && d->sp->d_quote == 0)
	{
		if (d->sp->s_quote == 0)
			d->sp->s_quote = 1;
		else
			d->sp->s_quote = 0;
	}
	if (str == '"' && d->sp->s_quote == 0)
	{
		if (d->sp->d_quote == 0)
			d->sp->d_quote = 1;
		else
			d->sp->d_quote = 0;
	}
}

static size_t   count_char(const char *s, char c, t_data *d)
{
	size_t  i;
	int		count;

	i = d->sp->line;
	count = 0;
	if (c == 'w')
	{
		while (s[i])
		{
			check_quote(s[i], d);
			if (is_charset(s[i]) && d->sp->s_quote == 0 && d->sp->d_quote == 0)
				break;
			if (s[i] == '"' && d->sp->s_quote == 0)
				i++;
			else if (s[d->sp->line] == '\'' && d->sp->d_quote == 0)
				i++;
			else
			{
				i++;
				count++;
			}
		}
	}
	else if (c == 'c')
		while (s[i] && (s[i] == '<' || s[i] == '>'))
		{
			count++;
			i++;
		}
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	return (count);
}

static char *ft_free(char **dst)
{
	int i;

	i = 0;
	while (dst[i])
	{
		free(dst[i]);
		i++;
	}
	free(dst);
	return (NULL);
}

char	*fill_memory(char *s, t_data *d, int i, t_env *tmp)
{
	char	*nb_exit;

	nb_exit = ft_itoa(g_exit_status);
	if (d->sp->remember_mem == 0 && s[d->sp->line] != '?')
		d->sp->new[d->sp->j] = (char *)malloc((sizeof(char) * (d->sp->count_c - i) + ft_strlen(tmp->value) + 1));
	else if (d->sp->remember_mem == 0 && s[d->sp->line] == '?')
		d->sp->new[d->sp->j] = (char *)malloc((sizeof(char) * (d->sp->count_c - i) + ft_strlen(nb_exit) + 1));
	else if (d->sp->remember_mem != 0 && s[d->sp->line] != '?')
		d->sp->new[d->sp->j] = (char *)malloc(sizeof(char) * (d->sp->remember_mem + ft_strlen(tmp->value) + 1));
	else if (d->sp->remember_mem != 0 && s[d->sp->line] == '?')
		d->sp->new[d->sp->j] = (char *)malloc(sizeof(char) * (d->sp->remember_mem + ft_strlen(nb_exit) + 1));
	if (s[d->sp->line] == '?')
		d->sp->remember_mem += ft_strlen(nb_exit);
	else
		d->sp->remember_mem += ft_strlen(tmp->value);	
	return (nb_exit);
}

void	copy_var_env(char *s, t_data *d, t_env *tmp, char *nb_exit)
{
	int		i;

	i = 0;
	if (s[d->sp->line] == '?')
	{
		d->sp->line++;
		while(nb_exit[i])
		{
			d->sp->new[d->sp->j][d->sp->k] = nb_exit[i];
			d->sp->k++;
			i++;
		}
	}
	else
	{
		while (tmp->value[i])
		{
			d->sp->new[d->sp->j][d->sp->k] = tmp->value[i];
			d->sp->k++;
			i++;
		}
	}
}

void	execute_var_env(char *s, t_data *d, int i, t_env *tmp)
{
	char	*bis;
	char    *nb_exit;

	d->sp->new[d->sp->j][d->sp->k] = 0;
	bis = (char *)malloc(sizeof(char) * (ft_strlen(d->sp->new[d->sp->j]) + 1));
	ft_strcpy(bis, d->sp->new[d->sp->j]);
	free(d->sp->new[d->sp->j]);
	d->sp->new[d->sp->j] = NULL;
	nb_exit = fill_memory(s, d, i, tmp);
	i = 0;
	d->sp->k = 0;
	while (bis[i])
	{
		d->sp->new[d->sp->j][d->sp->k] = bis[i];
		d->sp->k++;
		i++;
	}
	copy_var_env(s, d, tmp, nb_exit);
	free(bis);
	free(nb_exit);
}

char *make_change(char *s, t_data *d)
{
	char	*new;
	int		size_new;
	int		i;
	t_env	*tmp;

	size_new = 0;
	while (s[d->sp->line++] && !is_charset(s[d->sp->line]) && ft_isalpha_parsing(s[d->sp->line]))
		size_new++;
	new = (char *)malloc(sizeof(char) * (size_new + 1));
	d->sp->line -= size_new;
	i = 0;
	while (size_new > 0)
	{
		new[i] = s[d->sp->line];
		d->sp->line++;
		i++;
		size_new--;
	}
	new[i] = 0;
	tmp = d->env;
	while (tmp->next && !cmp_str(tmp->key, new))
		tmp = tmp->next;
	if (cmp_str(tmp->key, new) || s[d->sp->line] == '?')
		execute_var_env(s, d, i, tmp);	
	else
		return (NULL); //free new?
	free(new);
	return (d->sp->new[d->sp->j]);
}

static char	*make_split_char(char *s, t_data *d)
{
	d->sp->count_c = count_char(s, 'w', d);
	d->sp->new[d->sp->j] = (char *)malloc(sizeof(char) * (d->sp->count_c + 1));
	if (!d->sp->new[d->sp->j])
		return (ft_free(d->sp->new));
	d->sp->k = 0;
	while (s[d->sp->line])
	{
		check_quote(s[d->sp->line], d);
		if (is_charset(s[d->sp->line]) && d->sp->s_quote == 0 && d->sp->d_quote == 0)
			break;
		if (s[d->sp->line] == '"' && d->sp->s_quote == 0)
			d->sp->line++;
		else if (s[d->sp->line] == '\'' && d->sp->d_quote == 0)
			d->sp->line++;
		else if ((s[d->sp->line] == '$' && (s[d->sp->line + 1] == '?' || ft_isalpha_parsing(s[d->sp->line + 1]))) && d->sp->s_quote == 0)
			make_change(s, d);
		else
		{
			d->sp->new[d->sp->j][d->sp->k] = s[d->sp->line];
			d->sp->k++;
			d->sp->line++;
		}
	}
	d->sp->new[d->sp->j][d->sp->k] = 0;
	return (d->sp->new[d->sp->j]);
}

static char	*make_split_chev(char *s, t_data *d)
{
	d->sp->new[d->sp->j] = (char *)malloc(sizeof(char) * (count_char(s, 'c', d) + 1));
	if (!d->sp->new[d->sp->j])
		return (ft_free(d->sp->new));
	d->sp->k = 0;
	while (s[d->sp->line] && (s[d->sp->line] == '<' || s[d->sp->line] == '>'))
	{
		d->sp->new[d->sp->j][d->sp->k] = s[d->sp->line];
		d->sp->k++;
		d->sp->line++;
	}
	d->sp->new[d->sp->j][d->sp->k] = 0;
	return (d->sp->new[d->sp->j]);
}

char	**ft_split_parsing(char *s, t_data *d)
{
	d->sp = init_sp();
	if (!d->sp)
		return (NULL);
	d->sp->j = -1;
	d->sp->count = count_charset(s);
	d->sp->k = 0;
	d->sp->new = (char **)malloc(sizeof(char *) * (d->sp->count + 1));
	if (!d->sp->new)
	{
		free(d->sp);
		return (NULL);
	}
	while (++d->sp->j < d->sp->count)
	{
		if (is_charset(s[d->sp->line]))
		{
			if (s[d->sp->line] == '<' || s[d->sp->line] == '>')
				make_split_chev(s, d);
			else
			{
				while (s[d->sp->line] == ' ')
					d->sp->line++;
				d->sp->j--;
			}
		}
		else
			make_split_char(s, d);
	}
	d->sp->new[d->sp->j] = 0;
	return (d->sp->new);
}
