/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmeoli <gmeoli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 17:11:31 by masebast          #+#    #+#             */
/*   Updated: 2022/10/27 11:08:00 by gmeoli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

# define TRUE 1
# define FALSE 0

int	*g_exit_status;

typedef struct s_command {
	char	*current_shell_level;
	char	*command_string;
	char	**pipe_matrix;
	char	**word_matrix;
	int		total_pipes;
	int		pipes[2];
	int		write_fd;
	int		read_fd;
	char	*operator;
}	t_command;

// ERRORS
void	ft_error(void);
void	ft_syntax_error(char c);
void	ft_command_not_found(char *str);
void	ft_arg_not_found(char *str);
void	ft_unexpected_token(void);

// UTILITY
int		ft_check_quotes(char *str);
void	ft_free_matrix(char **m);
int		ft_parsing(char *input, t_command *command_struct);
int		ft_pipe_total(char *input);
int		ft_count_pipes(char *str);
int		ft_check_equal_presence(char *string);
void	ft_remove_quotes(char *command);
void	ft_manage_signals(void);
int		ft_modify_var(char *new_couple, char **envp);
void	ft_exit_on_signal(void);

// CD UTILITY
int		ft_find_dest_len(char *path);
int		ft_find_home_len(char *path);

// PIPES
void	ft_manage_pipes(t_command *command_struct, char **envp);
char	**ft_split_pipes(const char *s, char c);

// REDIRECTIONS
int		ft_check_redirection(char **word_struct);
void	ft_redirect(t_command *command_struct, int index, char **envp);

// ECHO
int		ft_echo(t_command *command_struct, int pipe_index);
int		ft_check_quote(char *str);
void	ft_print_exit(void);
int		ft_print_double_quote(char *string, int fd);
int		ft_print_single_quote(char *string, int fd);
int		ft_print_dollar(char *str, int fd);

// BUILTINS
int		ft_pwd(void);
void	ft_exit(t_command *c_struct);
int		ft_cd(t_command *command_struct);
int		ft_env(t_command *command_struct, char **envp);
int		ft_export(t_command *command_struct, char **envp);
int		ft_unset(t_command *command_struct, char **envp);

// OTHER COMMANDS
int		ft_other_commands(t_command *command_struct, char **envp);
int		ft_recognize_command(t_command *command_struct, int pipe_index, char **envp);

// RLS
extern void	rl_replace_line(const char *text, int clear_undo);

// SIGNALS
void	ft_ctrl_c(char **envp);

#endif
