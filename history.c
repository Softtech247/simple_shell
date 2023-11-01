#include "shell.h"

/**
 * sj_read_hfile - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int sj_read_hfile(info_tree *info)
{
    int i, last = 0, linecount = 0;
    ssize_t index, rdlen, fsize = 0;
    struct stat st;
    char *buf = NULL, *filename = sj_get_hfile(info);

    if (!filename)
        return (0);

    index = open(filename, O_RDONLY);
    free(filename);
    if (index == -1)
        return (0);
    if (!fstat(index, &st))
        fsize = st.st_size;
    if (fsize < 2)
        return (0);
    buf = malloc(sizeof(char) * (fsize + 1));
    if (!buf)
        return (0);
    rdlen = read(index, buf, fsize);
    buf[fsize] = 0;
    if (rdlen <= 0)
        return (free(buf), 0);
    close(index);
    for (i = 0; i < fsize; i++)
        if (buf[i] == '\n')
        {
            buf[i] = 0;
            sj_cost_hfile(info, buf + last, linecount++);
            last = i + 1;
        }
    if (last != i)
        sj_cost_hfile(info, buf + last, linecount++);
    free(buf);
    info->histcount = linecount;
    while (info->histcount-- >= HIST_MAX)
        sj_del_node(&(info->history), 0);
    sj_vamp_hfile(info);
    return (info->histcount);
}

/**
 * sj_write_hfile - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int sj_write_hfile(info_tree *info)
{
    ssize_t index;
    char *filename = sj_get_hfile(info);
    l_tree *node = NULL;

    if (!filename)
        return (-1);

    index = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);
    if (index == -1)
        return (-1);
    for (node = info->history; node; node = node->next)
    {
        sj_putstrindex(node->str, index);
        sj_putindex('\n', index);
    }
    sj_putindex(BUF_FLUSH, index);
    close(index);
    return (1);
}

/**
 * sj_cost_hfile - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int sj_cost_hfile(info_tree *info, char *buf, int linecount)
{
    l_tree *node = NULL;

    if (info->history)
        node = info->history;
    sj_node_add_end(&node, buf, linecount);

    if (!info->history)
        info->history = node;
    return (0);
}

/**
 * sj_get_hfile - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containing history file
 */
char *sj_get_hfile(info_tree *info)
{
    char *buf, *dir;

    dir = sj_getenv(info, "HOME=");
    if (!dir)
        return (NULL);
    buf = malloc(sizeof(char) * (sj_get_strlen(dir) + sj_get_strlen(HIST_FILE) + 2));
    if (!buf)
        return (NULL);
    buf[0] = 0;
    sj_strcopy(buf, dir);
    sj_strconcat(buf, "/");
    sj_strconcat(buf, HIST_FILE);
    return (buf);
}

/**
 * sj_vamp_hfile - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int sj_vamp_hfile(info_tree *info)
{
    l_tree *node = info->history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }
    return (info->histcount = i);
}
