/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeok <jinhyeok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 14:14:16 by jinhyeok          #+#    #+#             */
/*   Updated: 2023/07/29 14:12:19by jinhyeok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	leaks(void)
{
	system("leaks a.out");
}

int main(int ac, char **av, char ** envp)
{
	t_mini	*data;
	char	*str;
	char	*str1;

	str = ft_strdup("cat");
	str1 = ft_strdup("ls");
	
	data = (t_mini *)malloc(sizeof(t_mini) * 2);
	data[0].command = (char **)malloc(sizeof(char *) * 1);
	data[1].command = (char **)malloc(sizeof(char *) * 1);

	data[0].cnt = 2;
	data[0].cmd_size = 1;
	data[0].command[0] = str;

	data[1].cnt = 2;
	data[1].cmd_size = 1;
	data[1].command[0] = str1;

	printf("------ command ------\n");
	for (int i = 0, j; i < data->cnt; ++i)
	{
		printf("[ ");
		for (j = 0; j < data[i].cmd_size; ++j)
			printf("%s ", data[i].command[j]);
		printf("] ");
	}
	printf("\n---------------------\n");

	exec_cmd(data); 
	//process_start2(data);
	//cmd_find_start(data);

	free(data[0].command);
	free(data);
}

void	process_start2(t_mini *data)
{
	int		i;
	int		prev_pipe;
	int		cur_pipe[2];
	pid_t	pid;

	i = -1;
	prev_pipe = 0;
	while (++i < data->cnt)
	{
		pipe(cur_pipe);
		pid = fork();
		if (pid == 0)
		{	
			if (data[i].input_fd != 0)
			{
				dup2(data[i].input_fd, 0);
				close(data[i].input_fd);
			}
			if (prev_pipe)
			{
				dup2(prev_pipe, 0);
				close(prev_pipe);
			}
			if (data[i].cnt > 1 && data[i].cnt - 1 != i)
			{
				close(cur_pipe[0]);
				dup2(cur_pipe[1], 1);
				close(cur_pipe[1]);
			}
			if (data[i].output_fd != 0)
			{
				dup2(data[i].output_fd, 1);
				close(data[i].output_fd);
			}
			cmd_find_start(&data[i]);
		}
		else if (pid > 0)
		{
			close(cur_pipe[1]);
			if (prev_pipe)
			{
				close(prev_pipe);
			}
			if (data[i].cnt > 1)
			{
				prev_pipe = cur_pipe[0];
			}
			else
			{
				close(cur_pipe[0]);
				close(cur_pipe[1]);
			}
		}
		else
			error_fork();
	}
	while (i)
	{
		waitpid(-1, NULL, 0);
		i--;
	}
}

void	cmd_find_start(t_mini *data)
{
	int	i;

	i = -1;
	char	*joined;
	char	*cmd;
	char *temp = getenv("PATH");
	char	**split;
	split = ft_split(temp, ':');

	while (split[++i])
	{
		joined = ft_strjoin(split[i], "/");
		for (int i = 0 ; i < data->cmd_size ; i++)
		{
			if (data->command[i] != NULL)
			{
				cmd = ft_strjoin(joined, data->command[i]);
			}
		}
		if (access(cmd, X_OK) == 0)
		{
			if (execve(cmd, data[0].command, NULL) == -1)
			{
				printf("error");
			}
		}
	}
}

void	exec_cmd(t_mini *data)
{
	int	i;

	i = -1;
	while (++i < data[0].cnt)
	{
		redirection_ready(&data[i]);
	}
	process_start2(data);
}

int	redirection_ready(t_mini *data)
{
	int	i;

	i = -1;
	while (++i < data->cmd_size)
	{
		if (data->command[i] && is_redirection(data->command[i]))
		{
			set_redirection(data, i);
		}
	}
	return (0);
}

void    set_redirection(t_mini *data, int i)
{
	if (!data->command[i])
		return ;
	if (is_redirection(data->command[i]) == 1)
		file_open(data, i);
	else if (is_redirection(data->command[i]) == 2)
		file_create(data, i, 0);
	else if (is_redirection(data->command[i]) == 3)
		file_create(data, i, 1);
}

void    file_create(t_mini *data, int i, int flag)
{
	int fd;

	if (i + 1 > data->cmd_size) // this condition need to be handle in parsing part
		exit(258);
	if (data->input_fd != 0)
		close(data->input_fd);
	if (flag)
	{    
		fd = open(data->command[i + 1],  O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			error_file();
	}
	else 
	{
		fd = open(data->command[i + 1],  O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			error_file();
	}
	data->output_fd = fd;
	set_cmd_null(data, i, i + 1);
}

void    file_open(t_mini *data, int i)
{
	char    *file_name;
	int     fd;

	if (i + 1 > data->cmd_size)
		exit(258);
	if (data->input_fd != 0)
		close(data->input_fd);
	file_name = data->command[i + 1];
	fd = open (file_name, O_RDONLY);
	if (fd == -1)
		error_file();
	data->input_fd = fd;
	set_cmd_null(data, i, i + 1);
}

void	set_cmd_null(t_mini *data, int start, int end)
{
	char	*temp;

	while (start <= end)
	{
		temp = data->command[start];
		data->command[start] = NULL;
		free(temp);
		start++;
	}
}

void    error_file(void)
{
	perror("file");
	exit(128);
}

void	error_fork(void)
{
	perror("fork");
	exit(1);
}

void	error_pipe(void)
{
	perror("pipe");
	exit(1);
}

int is_redirection(char *str)
{
	// < > >> <>(?)
	if (ft_strncmp(str, "<", ft_strlen(str)) == 0)
		return (1);
	else if (ft_strncmp(str, ">", ft_strlen(str)) == 0)
		return (2);
	else if (ft_strncmp(str, ">>", ft_strlen(str)) == 0)
		return (3);
	return (0);
}