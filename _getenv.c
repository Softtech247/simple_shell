#include "shell.h"

/**
 * sj_get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **sj_get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = sj_list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * sj_unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int sj_unsetenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = sj_starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = sj_delete_node_at_index(&(info->env), i);
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
 * sj_setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int sj_setenv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(sj_strlen(var) + sj_strlen(value) + 2);
	if (!buf)
		return (1);
	sj_strcpy(buf, var);
	sj_strcat(buf, "=");
	sj_strcat(buf, value);
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
	sj_add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
