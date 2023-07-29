/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeok <jinhyeok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:14:11 by jinhyeok          #+#    #+#             */
/*   Updated: 2023/07/29 17:12:48 by jinhyeok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_mini
{
	char	**command;
	int		cmd_size;
	int		output_fd;
	int		input_fd;
	int		origin_in;
	int		origin_out;
	int		cnt;
} t_mini; 

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include "../libft/libft.h"

// jinhyeok exec
//start in exec_cmd;
void	exec_cmd(t_mini *data);
int		redirection_ready(t_mini *data);
int		is_redirection(char *str);
void	set_redirection(t_mini *data, int i);
void	file_open(t_mini *data, int i);
void	error_file(void);

void	file_create(t_mini *data, int i, int flag);
void	file_open(t_mini *data, int i);
//file util
void	set_cmd_null(t_mini *data, int start, int end);

//process
void	process_start2(t_mini *data);
void	process_start(t_mini *data);
void	child(t_mini *data, int *cur_pipe);
void	head_process(t_mini *data, int *cur_pipe);
void	tail_process(t_mini *data, int *cur_pipe);
void	exec_cmd(t_mini *data);
void	cmd_find_start(t_mini *data);

//error_msg
void	error_pipe(void);
void	error_fork(void);
void    error_file(void);

#endif