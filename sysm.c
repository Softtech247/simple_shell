#include "shell.h"

/**
 * sj_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int sj_history(info_tree *info)
{
	sj_printlist(info->history);
	return (0);
}

/**
 * sj_unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int sj_unset_alias(info_tree *info, char *str)
{
	char *p, c;
	int ret;

	p = sj_stricatchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = sj_del_node(&(info->alias),
		sj_get_nodeindex(info->alias, sj_node_beginwith(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * sj_set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int sj_set_alias(info_tree *info, char *str)
{
	char *p;

	p = sj_stricatchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (sj_unset_alias(info, str));

	sj_unset_alias(info, str);
	return (sj_node_add_end(&(info->alias), str, 0) == NULL);
}

/**
 * sj_print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int sj_print_alias(l_tree *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = sj_stricatchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			sj_putchar(*a);
		sj_putchar('\'');
		sj_putstr(p + 1);
		sj_putstr("'\n");
		return (0);
	}
	return (1);
}

/**
 * sj_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int sj_alias(info_tree *info)
{
	int i = 0;
	char *p = NULL;
	l_tree *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			sj_print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = sj_stricatchr(info->argv[i], '=');
		if (p)
			sj_set_alias(info, info->argv[i]);
		else
			sj_print_alias(sj_node_beginwith(info->alias, info->argv[i], '='));
	}

	return (0);
}
