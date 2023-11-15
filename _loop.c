#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int sj_hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		sj_clear_info(info);
		if (sj_interactive(info))
			sj_puts("$ ");
		sj_eputchar(BUF_FLUSH);
		r = sj_get_input(info);
		if (r != -1)
		{
			sj_set_info(info, av);
			builtin_ret = sj_find_builtin(info);
			if (builtin_ret == -1)
				sj_find_cmd(info);
		}
		else if (sj_interactive(info))
			sj_putchar('\n');
		sj_free_info(info, 0);
	}
	sj_write_history(info);
	sj_free_info(info, 1);
	if (!sj_interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * sj_find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int sj_find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", sj_myexit},
		{"env", sj_myenv},
		{"help", sj_myhelp},
		{"history", sj_myhistory},
		{"setenv", sj_mysetenv},
		{"unsetenv", sj_myunsetenv},
		{"cd", sj_mycd},
		{"alias", sj_myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (sj_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * sj_find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void sj_find_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!sj_is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = sj_find_path(info, sj_getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		sj_fork_cmd(info);
	}
	else
	{
		if ((sj_interactive(info) || sj_getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && sj_is_cmd(info, info->argv[0]))
			sj_fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			sj_print_error(info, "not found\n");
		}
	}
}

/**
 * sj_fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void sj_fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = vfork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, sj_get_environ(info)) == -1)
		{
			sj_free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				sj_print_error(info, "Permission denied\n");
		}
	}
}
