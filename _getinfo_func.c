#include "shell.h"

/**
 * sj_clear_info - initializes info_t struct
 * @info: struct address
 */
void sj_clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * sj_set_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void sj_set_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = sj_strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = sj_strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		sj_replace_alias(info);
		sj_replace_vars(info);
	}
}

/**
 * sj_free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void sj_free_info(info_t *info, int all)
{
	sj_ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			sj_free_list(&(info->env));
		if (info->history)
			sj_free_list(&(info->history));
		if (info->alias)
			sj_free_list(&(info->alias));
		sj_ffree(info->environ);
			info->environ = NULL;
		sj_bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		sj_putchar(BUF_FLUSH);
	}
}
