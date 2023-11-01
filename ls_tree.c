#include "shell.h"

/**
 * sj_ls_free - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void sj_ls_free(l_tree **head_ptr)
{
    l_tree *node, *next_node, *head;

    if (!head_ptr || !*head_ptr)
        return;
    head = *head_ptr;
    node = head;
    while (node)
    {
        next_node = node->next;
        free(node->str);
        free(node);
        node = next_node;
    }
    *head_ptr = NULL;
}

/**
 * sj_node_add - adds a node to the start of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
l_tree *sj_node_add(l_tree **head, const char *str, int num)
{
    l_tree *new_head;

    if (!head)
        return (NULL);
    new_head = malloc(sizeof(l_tree));
    if (!new_head)
        return (NULL);
    sj_setmemory((void *)new_head, 0, sizeof(l_tree));
    new_head->num = num;
    if (str)
    {
        new_head->str = sj_strcast(str);
        if (!new_head->str)
        {
            free(new_head);
            return (NULL);
        }
    }
    new_head->next = *head;
    *head = new_head;
    return (new_head);
}

/**
 * sj_printlstr - prints only the str element of a l_tree linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t sj_printlstr(const l_tree *h)
{
    size_t i = 0;

    while (h)
    {
        sj_putstr(h->str ? h->str : "(nil)");
        sj_putstr("\n");
        h = h->next;
        i++;
    }
    return (i);
}

/**
 * sj_node_add_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
l_tree *sj_node_add_end(l_tree **head, const char *str, int num)
{
    l_tree *new_node, *node;

    if (!head)
        return (NULL);

    node = *head;
    new_node = malloc(sizeof(l_tree));
    if (!new_node)
        return (NULL);
    sj_setmemory((void *)new_node, 0, sizeof(l_tree));
    new_node->num = num;
    if (str)
    {
        new_node->str = sj_strcast(str);
        if (!new_node->str)
        {
            free(new_node);
            return (NULL);
        }
    }
    if (node)
    {
        while (node->next)
            node = node->next;
        node->next = new_node;
    }
    else
        *head = new_node;
    return (new_node);
}

/**
 * sj_del_node - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int sj_del_node(l_tree **head, unsigned int index)
{
    l_tree *node, *prev_node;
    unsigned int i = 0;

    if (!head || !*head)
        return (0);

    if (!index)
    {
        node = *head;
        *head = (*head)->next;
        free(node->str);
        free(node);
        return (1);
    }
    node = *head;
    while (node)
    {
        if (i == index)
        {
            prev_node->next = node->next;
            free(node->str);
            free(node);
            return (1);
        }
        i++;
        prev_node = node;
        node = node->next;
    }
    return (0);
}
