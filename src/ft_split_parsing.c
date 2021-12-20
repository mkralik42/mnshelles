/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:41:15 by lcavallu          #+#    #+#             */
/*   Updated: 2021/12/20 18:57:12 by mkralik          ###   ########.fr       */
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

int count_charset_is(const char *s, int i, int *words)
{
	if (s[i] == '<' || s[i] == '>')
	{
		while (s[i] == '<' || s[i] == '>')
			i++;
		(*words)++;
	}
	else
		while (s[i] == ' ')
			i++;
	return (i);
}

int count_charset_not_else(const char *s, int i, int *words)
{
	while (s[i] && !is_charset(s[i]) && s[i] != '"' && s[i] != '\'')
		i++;
	if (s[i] && !is_charset(s[i]))
		(*words)--;
	return (i);
}

int count_charset_not(const char *s, int i, int *words, t_data *d)
{
	if (s[i] == '"' && d->sp->s_quote == 0)
	{
		d->sp->d_quote = 1;
		i++;
		while (s[i] && s[i] != '"')
			i++;
		i++;
		if (s[i] && s[i] != ' ')
			(*words)--;
	}
	else if (s[i] == '\'' && d->sp->d_quote == 0)
	{
		d->sp->s_quote = 1;
		i++;
		while (s[i] && s[i] != '\'')
			i++;
		i++;
		if (s[i] && s[i] != ' ')
			(*words)--;
	}
	else
		i = count_charset_not_else(s, i, words);
	return (i);
}

static int   count_charset(t_data *d, const char *s)
{
	int i;
	int words;

	words = 0;
	i = 0;
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
	while (s[i])
	{
		if (is_charset(s[i]))
			i = count_charset_is(s, i, &words);
		else
		{
			i = count_charset_not(s, i, &words, d);
			words++;
		}
	}
	d->sp->s_quote = 0;
	d->sp->d_quote = 0;
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

int	count_char_i(const char *s, t_data *d, int i, int *count)
{
	if (s[i] == '"' && d->sp->s_quote == 0)
		i++;
	else if (s[d->sp->line] == '\'' && d->sp->d_quote == 0)
		i++;
	else
	{
		i++;
		(*count)++;
	}
	return (i);
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
			i = count_char_i(s, d, i, &count);
		}
	}
	else if (c == 'c')
		while (s[i] && (s[i] == '<' || s[i] == '>'))
		{
			i++;
			count++;
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
	printf("nb = %i, nb exit = %s\n", g_exit_status, nb_exit);
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

char	*make_change_new(char *s, t_data *d, int *i, int size_new)
{
	char	*new;
	int		j;

	j = 0;
	new = (char *)malloc(sizeof(char) * (size_new + 1));
	while (size_new > 0)
	{
		new[j] = s[d->sp->line];
		d->sp->line++;
		(*i)++;
		j++;
		size_new--;
	}
	new[j] = 0;
	return (new);
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
	d->sp->line -= size_new;
	i = 0;
	new = make_change_new(s, d, &i, size_new);
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
			d->sp->new[d->sp->j][d->sp->k++] = s[d->sp->line++];
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

void	split_parsing(char *s, t_data *d)
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

char	**ft_split_parsing(char *s, t_data *d)
{
	d->sp = init_sp();
	if (!d->sp)
		return (NULL);
	d->sp->j = -1;
	d->sp->count = count_charset(d, s);
	d->sp->k = 0;
	d->sp->new = (char **)malloc(sizeof(char *) * (d->sp->count + 1));
	if (!d->sp->new)
	{
		free(d->sp);
		return (NULL);
	}
	while (++d->sp->j < d->sp->count)
		split_parsing(s, d);
	d->sp->new[d->sp->j] = 0;
	return (d->sp->new);
}
