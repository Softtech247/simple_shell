#include "shell.h"

/**
 * sj_get_nodeindex - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t sj_get_nodeindex(l_tree *head, l_tree *node)
{
    size_t i = 0;

    while (head)
    {
        if (head == node)
            return i;
        head = head->next;
        i++;
    }
    return -1;
}

/**
 * sj_node_beginwith - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
l_tree *sj_node_beginwith(l_tree *node, char *prefix, char c)
{
    char *p = NULL;

    while (node)
    {
        p = sj_starts_with(node->str, prefix);
        if (p && ((c == -1) || (*p == c)))
            return node;
        node = node->next;
    }
    return NULL;
}

/**
 * sj_printlist - prints all elements of a l_tree linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t sj_printlist(const l_tree *h)
{
    size_t i = 0;

    while (h)
    {
        sj_putstr(sj_conv_num(h->num, 10, 0));
        sj_putchar(':');
        sj_putchar(' ');
        sj_putstr(h->str ? h->str : "(nil)");
        sj_putstr("\n");
        h = h->next;
        i++;
    }
    return i;
}

/**
 * sj_getlist_len - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t sj_getlist_len(const l_tree *h)
{
    size_t i = 0;

    while (h)
    {
        h = h->next;
        i++;
    }
    return i;
}

/**
 * sj_ltree_str - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **sj_ltree_str(l_tree *head)
{
    l_tree *node = head;
    size_t i = sj_getlist_len(head), j;
    char **strs;
    char *str;

    if (!head || !i)
        return NULL;
    strs = malloc(sizeof(char *) * (i + 1));
    if (!strs)
        return NULL;
    for (i = 0; node; node = node->next, i++)
    {
        str = malloc(sj_get_strlen(node->str) + 1);
        if (!str)
        {
            for (j = 0; j < i; j++)
                free(strs[j]);
            free(strs);
            return NULL;
        }

        str = sj_strcopy(str, node->str);
        strs[i] = str;
    }
    strs[i] = NULL;
    return strs;
}
