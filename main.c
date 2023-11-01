#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	info_tree info[] = { INFO_INIT };
	int index = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (index)
		: "r" (index));

	if (ac == 2)
	{
		index = open(av[1], O_RDONLY);
		if (index == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				sj_put_string(av[0]);
				sj_put_string(": 0: Can't open ");
				sj_put_string(av[1]);
				sj_putchar('\n');
				sj_putchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readindex = index;
	}
	sj_increment_env(info);
	sj_read_hfile(info);
	sj_hsh(info, av);
	return (EXIT_SUCCESS);
}
