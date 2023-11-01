#include "shell.h"

/**
 * sj_cls_info - initializes info_tree struct
 * @info: struct address
 */
void sj_cls_info(info_tree *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * sj_set_info - initializes info_tree struct
 * @info: struct address
 * @av: argument vector
 */
void sj_set_info(info_tree *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = sj_str2row(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = sj_strcast(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		sj_alias_update(info);
		sj_vars_update(info);
	}
}

/**
 * sj_ffinfo - frees info_tree struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void sj_ffinfo(info_tree *info, int all)
{
	sj_freefind(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			sj_ls_free(&(info->env));
		if (info->history)
			sj_ls_free(&(info->history));
		if (info->alias)
			sj_ls_free(&(info->alias));
		sj_freefind(info->environ);
			info->environ = NULL;
		sj_buffree((void **)info->cmd_buf);
		if (info->readindex > 2)
			close(info->readindex);
		sj_putchar(BUF_FLUSH);
	}
}
