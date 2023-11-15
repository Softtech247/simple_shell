#include "shell.h"

/**
 * sj_exit - Exits the shell
 * @custom_info: Structure containing potential arguments.
 * Return: Exits with a given exit status (0) if custom_info->argv[0] != "exit"
 */
int sj_exit(info_tree *custom_info) {
    int status;

    if (custom_info->argv[1]) {  /* Check for exit argument */
        status = sj_alpha2int_error(custom_info->argv[1]);
        if (status == -1) {
            custom_info->status = 2;
            sj_printf_err(custom_info, "Illegal number: ");
            sj_put_string(custom_info->argv[1]);
            sj_putchar('\n');
            return 1;
        }
        custom_info->err_num = sj_alpha2int_error(custom_info->argv[1]);
        return -2;
    }
    custom_info->err_num = -1;
    return -2;
}

/**
 * sj_cd - Changes the current directory of the process
 * @custom_info: Structure containing potential arguments.
 * Return: Always 0
 */
int sj_cd(info_tree *custom_info) {
    char *current_dir, *custom_dir, buffer[1024];
    int custom_chdir_ret;

    current_dir = getcwd(buffer, 1024);
    if (!current_dir)
        sj_putstr("TODO: >>getcwd failure emsg here<<\n");
    if (!custom_info->argv[1]) {
        custom_dir = sj_getenv(custom_info, "HOME=");
        if (!custom_dir)
            custom_chdir_ret = /* TODO: what should this be? */
                chdir((custom_dir = sj_getenv(custom_info, "PWD=")) ? custom_dir : "/");
        else
            custom_chdir_ret = chdir(custom_dir);
    } else if (sj_strcomps(custom_info->argv[1], "-") == 0) {
        if (!sj_getenv(custom_info, "OLDPWD=")) {
            sj_putstr(current_dir);
            sj_putchar('\n');
            return 1;
        }
        sj_putstr(sj_getenv(custom_info, "OLDPWD=")), sj_putchar('\n');
        custom_chdir_ret = /* TODO: what should this be? */
            chdir((custom_dir = sj_getenv(custom_info, "OLDPWD=")) ? custom_dir : "/");
    } else
        custom_chdir_ret = chdir(custom_info->argv[1]);
    if (custom_chdir_ret == -1) {
        sj_printf_err(custom_info, "can't cd to ");
        sj_put_string(custom_info->argv[1]), sj_putchar('\n');
    } else {
        sj_set_envchr(custom_info, "OLDPWD", sj_getenv(custom_info, "PWD="));
        sj_set_envchr(custom_info, "PWD", getcwd(buffer, 1024));
    }
    return 0;
}

/**
 * sj_help - Displays help information
 * @custom_info: Structure containing potential arguments.
 * Return: Always 0
 */
int sj_help(info_tree *custom_info) {
    char **arrays;

    arrays = custom_info->argv;
    sj_putstr("Help function is not yet implemented.\n");
    if (0)
        sj_putstr(*arrays); /* Temporary workaround */
    return 0;
}
