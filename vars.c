#include "shell.h"

/**
 * sj_chain_status - test if the current character in the buffer is a chain delimiter
 * @info: the parameter struct
 * @buffer: the character buffer
 * @position: address of the current position in the buffer
 *
 * Return: 1 if a chain delimiter, 0 otherwise
 */
int sj_chain_status(info_tree *info, char *buffer, size_t *position)
{
    size_t j = *position;

    if (buffer[j] == '|' && buffer[j + 1] == '|')
    {
        buffer[j] = 0;
        j++;
        info->cmd_buf_type = CMD_OR;
    }
    else if (buffer[j] == '&' && buffer[j + 1] == '&')
    {
        buffer[j] = 0;
        j++;
        info->cmd_buf_type = CMD_AND;
    }
    else if (buffer[j] == ';') /* found the end of this command */
    {
        buffer[j] = 0; /* replace semicolon with null */
        info->cmd_buf_type = CMD_CHAIN;
    }
    else
        return (0);
    *position = j;
    return (1);
}

/**
 * sj_get_chainvalue - checks whether we should continue chaining based on the last status
 * @info: the parameter struct
 * @buffer: the character buffer
 * @position: address of the current position in the buffer
 * @start: starting position in the buffer
 * @length: length of the buffer
 *
 * Return: Void
 */
void sj_get_chainvalue(info_tree *info, char *buffer, size_t *position, size_t start, size_t length)
{
    size_t j = *position;

    if (info->cmd_buf_type == CMD_AND)
    {
        if (info->status)
        {
            buffer[start] = 0;
            j = length;
        }
    }
    if (info->cmd_buf_type == CMD_OR)
    {
        if (!info->status)
        {
            buffer[start] = 0;
            j = length;
        }
    }

    *position = j;
}

/**
 * sj_alias_update - replaces aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int sj_alias_update(info_tree *info)
{
    int i;
    l_tree *node;
    char *new_value;

    for (i = 0; i < 10; i++)
    {
        node = sj_node_beginwith(info->alias, info->argv[0], '=');
        if (!node)
            return (0);
        free(info->argv[0]);
        new_value = sj_stricatchr(node->str, '=');
        if (!new_value)
            return (0);
        new_value = sj_strcast(new_value + 1);
        if (!new_value)
            return (0);
        info->argv[0] = new_value;
    }
    return (1);
}

/**
 * sj_vars_update - replaces variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int sj_vars_update(info_tree *info)
{
    int i = 0;
    l_tree *node;

    for (i = 0; info->argv[i]; i++)
    {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
            continue;

        if (!sj_strcomps(info->argv[i], "$?"))
        {
            sj_str_updateg(&(info->argv[i]), sj_strcast(sj_conv_num(info->status, 10, 0)));
            continue;
        }
        if (!sj_strcomps(info->argv[i], "$$"))
        {
            sj_str_updateg(&(info->argv[i]), sj_strcast(sj_conv_num(getpid(), 10, 0)));
            continue;
        }
        node = sj_node_beginwith(info->env, &info->argv[i][1], '=');
        if (node)
        {
            sj_str_updateg(&(info->argv[i]), sj_strcast(sj_stricatchr(node->str, '=') + 1));
            continue;
        }
        sj_str_updateg(&info->argv[i], sj_strcast(""));
    }
    return (0);
}

/**
 * sj_str_updateg - replaces a string
 * @old: address of the old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int sj_str_updateg(char **old, char *new)
{
    free(*old);
    *old = new;
    return (1);
}
