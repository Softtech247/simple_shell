#include "shell.h"

/**
 * sj_search_bultin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit()
 */
int sj_search_bultin(info_tree *info)
{
    int i, built_in_ret = -1;
    builtin_table_struct builtintbl[] = {
        {"exit", sj_exit},
        {"env", sj_env},
        {"help", sj_help},
        {"history", sj_history},
        {"setenv", sj_set_envchr},
        {"unsetenv", sj_unset_envchr},
        {"cd", sj_cd},
        {"alias", sj_alias},
        {NULL, NULL}
    };

    for (i = 0; builtintbl[i].type; i++)
        if (sj_strcomps(info->argv[0], builtintbl[i].type) == 0)
        {
            info->line_count++;
            built_in_ret = builtintbl[i].func(info);
            break;
        }
    return built_in_ret;
}

/**
 * sj_cmd_search - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void sj_cmd_search(info_tree *info)
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
        if (!sj_delimt(info->arg[i], " \t\n"))
            k++;
    if (!k)
        return;

    path = sj_lookup_path(info, sj_getenv(info, "PATH="), info->argv[0]);
    if (path)
    {
        info->path = path;
        sj_cmd_clone(info);
    }
    else
    {
        if ((sj_interf(info) || sj_getenv(info, "PATH=")
            || info->argv[0][0] == '/') && sj_cmd_checker(info, info->argv[0]))
            sj_cmd_clone(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            sj_printf_err(info, "not found\n");
        }
    }
}

/**
 * sj_cmd_clone - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void sj_cmd_clone(info_tree *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        /* TODO: PUT ERROR FUNCTION */
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, sj_get_env(info)) == -1)
        {
            sj_ffinfo(info, 1);
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
                sj_printf_err(info, "Permission denied\n");
        }
    }
}

/**
 * sj_hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int sj_hsh(info_tree *info, char **av)
{
    ssize_t r = 0;
    int builtin_ret = 0;

    while (r != -1 && builtin_ret != -2)
    {
        sj_cls_info(info);
        if (sj_interf(info))
            sj_putstr("$ ");
        sj_putchar(BUF_FLUSH);
        r = sj_get_value(info);
        if (r != -1)
        {
            sj_set_info(info, av);
            builtin_ret = sj_search_bultin(info);
            if (builtin_ret == -1)
                sj_cmd_search(info);
        }
        else if (sj_interf(info))
            sj_putchar('\n');
        sj_ffinfo(info, 0);
    }
    sj_write_hfile(info);
    sj_ffinfo(info, 1);
    if (!sj_interf(info) && info->status)
        exit(info->status);
    if (builtin_ret == -2)
    {
        if (info->err_num == -1)
            exit(info->status);
        exit(info->err_num);
    }
    return builtin_ret;
}
