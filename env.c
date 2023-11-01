#include "shell.h"

/**
 * sj_get_env - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **sj_get_env(info_tree *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = sj_ltree_str(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * sj_unset_envchr - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int sj_unset_envchr(info_tree *info, char *var)
{
	l_tree *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = sj_starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = sj_del_node(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * sj_set_envchr - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int sj_set_envchr(info_tree *info, char *var, char *value)
{
	char *buf = NULL;
	l_tree *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(sj_get_strlen(var) + sj_get_strlen(value) + 2);
	if (!buf)
		return (1);
	sj_strcopy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = sj_starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	sj_node_add_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
