#include "shell.h"

/**
 * sj_myexit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int sj_myexit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = sj_erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			sj_print_error(info, "Illegal number: ");
			sj_eputs(info->argv[1]);
			sj_eputchar('\n');
			return (1);
		}
		info->err_num = sj_erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int sj_mycd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		sj_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = sj_getenv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = sj_getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (sj_strcmp(info->argv[1], "-") == 0)
	{
		if (!sj_getenv(info, "OLDPWD="))
		{
			sj_puts(s);
			sj_putchar('\n');
			return (1);
		}
		sj_puts(sj_getenv(info, "OLDPWD=")), sj_putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = sj_getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		sj_print_error(info, "can't cd to ");
		sj_eputs(info->argv[1]), sj_eputchar('\n');
	}
	else
	{
		sj_setenv(info, "OLDPWD", sj_getenv(info, "PWD="));
		sj_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int sj_myhelp(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	sj_puts("help call works. Function not yet implemented \n");
	if (0)
		sj_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
