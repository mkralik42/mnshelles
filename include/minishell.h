/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 17:48:37 by mkralik           #+#    #+#             */
/*   Updated: 2022/01/05 18:50:40 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

extern int	g_exit_status;

typedef struct s_lst
{
	char			*cmd;
	char			**arg;
	char			*path;
	int				input;
	int				output;
	int				builtin;
	struct s_lst	*next;
}	t_lst;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				with_value;
	struct s_env	*next;
}	t_env;

typedef struct s_sp
{
	int		line;
	int		count;
	int		j;
	int		k;
	char	**new;
	int		s_quote;
	int		d_quote;
	int		count_c;
	int		remember_mem;
}t_sp;

typedef struct s_data
{
	char	*prompt;
	char	*line;
	t_env	*env;
	t_env	*export;
	t_lst	*cmd_lst;
	t_sp	*sp;
	char	**split;
	char	**argo;
	int		exit_value;
	int		nb_pipe;
}		t_data;

typedef struct s_sep
{
	int		pipe;
	int		double_and;
	int		simple_quo;
	int		double_quo;
	int		simple_raft_left;
	int		simple_raft_right;
	int		double_raft_left;
	int		double_raft_right;
	char	*infile;
	char	*outfile;
}t_sep;

/*---------------------lst.c--------------------------*/

void	add_cell(t_env **env, t_env *new);
t_env	*new_cell(char *key, char *value, int with_value);
t_env	*new_cell_test(char *key, char *value, int with_value);
t_env	*ft_lstlast(t_env *lst);

/*---------------------utils.c--------------------------*/

size_t	ft_strlen(const char *str);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
char	**ft_split(const char *s, char c);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, int n);
int		cmp_str(const char *s1, const char *s2);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_free_strjoin(char *s1, char *s2);
int		ft_atoi(const char *str);
char	*ft_strdup(const char *s1);
void	ft_putstr_fd(char *s, int fd);
void	error_cmd(char *arg);
void	*ft_calloc(int count, int size);

/*---------------------split.c--------------------------*/

char	**free_tab(char **tab, size_t size);
size_t	sizeof_word(const char *s, char c);
size_t	count_words(const char *s, char c);
char	**ft_split_env(const char *s, char c);
void	free_split(char **args);

/*------------------------mini.c--------------------------*/

char	*design_prompt(t_data *data);
int		exec_builtin(t_lst *cmd_lst, t_data *data);
void	print_env(t_env *env);
void	print_export(t_env *export);
t_env	*get_env_export(char **envp);
t_data	*init_data(char **envp);
int		main(int argc, char **argv, char **envp);

/*---------------------builtin.c--------------------------*/

//export
int		already_there(char *arg, t_env *env);
int		exec_export(t_lst *cmd_lst, t_data *data);
void	change_cell_env(char *key, char *new_value, t_env *env);

//pwd
char	*get_key(char *arg, t_env *env);
int		exec_pwd(t_lst *cmd_lst, t_data *data);

//env
int		exec_env(t_lst *cmd_lst, t_data *data);
char	**get_env_to_char(t_env *env);
int		size_env(t_env *env);

//echo
int		exec_echo(t_lst *cmd_lst, t_data *data);

//cd
void	update_pwd(t_data *data, char *pwd, char *old_pwd);
char	*ft_getcwd(void);
int		use_cdpath(t_lst *cmd_lst, char *cdpath);
int		exec_cd(t_lst *cmd_lst, t_data *data);
void	cd(t_lst *cmd_lst, int *new, char *cdpath);
int		cd_simple_dash(t_data *data, int *new, char *old_pwd);
int		cd_dble_dash(t_data *data, int *new);
int		cd_too_many(char *old_pwd);
void	cd_no_file(t_lst *cmd_lst, int new);

//unset
int		exec_unset(t_lst *cmd_lst, t_data *data);
void	free_first_env(t_env **env, t_env *tmp);
void	unset(char *arg, t_env **env);
t_env	*get_prev(char *key, t_env *env);

//exit
int		exec_exit(t_data *data, t_lst *cmd_lst);
int		get_exit_code(t_data *data, t_lst *cmd_lst);
int		exit_is_digit(char *s);
void	ft_free_exit(t_data *data);

void	error_cmd(char *arg);
int		ft_pipe(t_data *data, t_lst *lst, int fd_in, int step);
int		error_catch(int test, char *file, char *msg);

//int	exec(t_data *data, t_lst *cmd_lst, t_lst *lst, char **ch_env);
int		ft_execute(t_data *data, int exit_code, t_lst *lst, char **ch_env);
void	free_before_exit(t_data *data, char **ch_env);

/*------------------------free.c--------------------------*/

void	ft_free_all(t_data *data);
void	free_env(t_env *g_env);
void	free_cmd_lst(t_data *data, t_lst **cmd_lst);
void	free_dble_str(char **str);
void	free_str(char **str);

/*---------------------parsing_count.c--------------------------*/

t_sp	*init_sp(void);
void	check_quote(char str, t_data *d);
int		count_char_i(const char *s, t_data *d, int i, int *count);
size_t	count_char(const char *s, char c, t_data *d);
int		check_chev_data_return(int what);

/*---------------------ft_split_parsing.c--------------------------*/

char	*make_split_chev(char *s, t_data *d);
char	*make_split_char(char *s, t_data *d);
void	split_parsing(char *s, t_data *d);
char	**ft_split_parsing(char *s, t_data *d);

/*---------------------parsing_charset.c--------------------------*/

int		is_charset(char s);
int		count_charset_is(const char *s, int i, int *words);
int		count_charset_not_else(const char *s, int i, int *words);
int		count_charset_not(const char *s, int i, int *words, t_data *d);
int		count_charset(t_data *d, const char *s);

/*---------------------parsing_var_env.c--------------------*/

char	*fill_memory(char *s, t_data *d, int i, t_env *tmp);
void	copy_var_env(char *s, t_data *d, t_env *tmp, char *nb_exit);
char	*make_change_new(char *s, t_data *d, int *i, int size_new);
void	execute_var_env(char *s, t_data *d, int i, t_env *tmp);
char	*make_change(char *s, t_data *d);

/*---------------------parsing_utils.c--------------------*/

void	print_list(t_lst *list);
t_lst	*create_new(char *split, char **arg, char what, int file);
int		found_place_raft(char **split, int i, t_data *d);
void	ft_swap(char **a, char **b);
char	*ft_itoa(int n);

/*---------------------parsing_utils_str.c--------------------*/

int		ft_strcmp_parsing(char *s1, char *s2);
int		ft_strncmp_parsing(char *s1, char *s2, int n);
void	ft_free_str(char **str);
char	*ft_strcpy(char *dest, char *src);

/*---------------------parsing_utils_create.c--------------------*/

t_lst	*create_cell(char *cmd);
void	add_cell_parsing(t_data *d, t_lst *new);
t_lst	*create_new_char(t_lst *cell, char *split, char **arg, char what);
t_lst	*create_new_int(t_lst *cell, char what, int file);
int		ft_isalpha_parsing(int c);

/*---------------------parsing.c--------------------------*/

t_lst	*ft_free_double(char **path, char *cmd, t_lst *cell);
t_lst	*ft_return(t_data *d, char **split_pipe, int i);
void	ft_fill_cell(t_data *d, t_sep *sep, char **split_quote);
int		ft_fill_split(t_data *d, t_sep *sep, char **split_pipe, int *i);
t_lst	*parsing(t_data *d);

/*---------------------parsing_file.c--------------------------*/

void	ft_check_close(t_lst *cell);
void	check_in_out_data(t_data *d, t_sep *sep, t_lst *cell, int place_raft);
t_lst	*check_infile_outfile(t_data *d, t_sep *sep, t_lst *cell, char **s_q);
t_lst	*fill_builtin(t_lst *cell);

/*---------------------parsing_found.c--------------------------*/

int		found_cmd(t_data *d, t_lst *cell);
char	*found_path_data(char **path, int i, char *cmd, t_lst *cell);
t_lst	*found_path(t_lst *cell, t_data *d);
void	print_sep(t_sep *sep, t_data *d);
int		check_if_path(char *argv);

/*---------------------parsing_fill.c--------------------------*/

void	fill_sep(t_data *d, t_sep *sep);
t_lst	*fill_in_out_file(t_data *d, t_lst *cell, char **split_quote);
t_lst	*fill_arg(t_data *d, t_lst *cell);
int		fill_arg_data(t_data *d, int place_cmd);

/*---------------------parsing_check.c--------------------------*/

int		check_sep(t_sep *sep, t_data *d);
int		check_chev_data(char **split_pipe, int i, int j, t_data *d);
int		check_chev(t_data *d, char **split_pipe);
int		check_pipe_space(char *split_pipe);
int		check_pipe(char **split_pipe, t_sep *sep);

/*---------------------parsing_init.c--------------------------*/

void	init_quote(t_data *d);
void	init_sep(t_sep *sep);
t_lst	*init_cell(void);
void	fill_sep_init(t_data *d, t_sep *sep, int i);
int		fill_sep_quote(int s_quote, int d_quote, t_sep *sep, int what);

/*---------------------ft_split_parsing_pipes.c--------------------------*/

char	**ft_split_parsing_pipe(const char *s, char c, t_data *d);

/*---------------------ft_split_parsing_quote.c--------------------------*/

char	**ft_split_parsing_quote(char *split_pipe, t_data *d);

/*---------------------ft_split_parsing_quote_bis.c--------------------------*/

int		count_charset_not_else_quote(const char *s, int i, int *words);
int		count_charset_not_quote(const char *s, int i, int *words, t_data *d);
int		count_charset_quote(t_data *d, const char *s);
size_t	count_char_quote(const char *s, char c, t_data *d);

/*---------------------signal.c---------------------------*/

void	init_signal(t_data *data);
void	init_signal_child(t_data *data);
void	init_signal_cmd(t_data *data);
void	handle_sig(int signum);
void	handle_sig_child(int signum);
void	handle_sig_cmd(int signum);
void	ft_signal_sigint(void);
void	ft_signal_stdin(void);

/*--------------------heredoc.c---------------------------*/

int		heredoc(t_data *data, char *eof);
char	*get_next_line(t_data *data, char **line);
void	get_line(char *buff, char **line);
int		boucle_get_next_line(char **line, int i);

#endif
