#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* toem_shloop.c */
int sj_hsh(info_t *, char **);
int sj_find_builtin(info_t *);
void sj_find_cmd(info_t *);
void sj_fork_cmd(info_t *);

/* toem_parser.c */
int sj_is_cmd(info_t *, char *);
char *sj_dup_chars(char *, int, int);
char *sj_find_path(info_t *, char *, char *);

/* loophsh.c */
int sj_loophsh(char **);

/* toem_errors.c */
void sj_eputs(char *);
int sj_eputchar(char);
int sj_putfd(char c, int fd);
int sj_putsfd(char *str, int fd);

/* toem_string.c */
int sj_strlen(char *);
int sj_strcmp(char *, char *);
char *sj_starts_with(const char *, const char *);
char *sj_strcat(char *, char *);

/* toem_string1.c */
char *sj_strcpy(char *, char *);
char *sj_strdup(const char *);
void sj_puts(char *);
int sj_putchar(char);

/* toem_exits.c */
char *sj_strncpy(char *, char *, int);
char *sj_strncat(char *, char *, int);
char *sj_strchr(char *, char);

/* toem_tokenizer.c */
char **sj_strtow(char *, char *);
char **sj_strtow2(char *, char);

/* toem_realloc.c */
char *sj_memset(char *, char, unsigned int);
void sj_ffree(char **);
void *sj_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int sj_bfree(void **);

/* toem_atoi.c */
int sj_interactive(info_t *);
int sj_is_delim(char, char *);
int sj_isalpha(int);
int sj_atoi(char *);

/* toem_errors1.c */
int sj_erratoi(char *);
void sj_print_error(info_t *, char *);
int sj_print_d(int, int);
char *sj_convert_number(long int, int, int);
void sj_remove_comments(char *);

/* toem_builtin.c */
int sj_myexit(info_t *);
int sj_mycd(info_t *);
int sj_myhelp(info_t *);

/* toem_builtin1.c */
int sj_myhistory(info_t *);
int sj_myalias(info_t *);

/*toem_getline.c */
ssize_t sj_get_input(info_t *);
int sj_getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void sj_clear_info(info_t *);
void sj_set_info(info_t *, char **);
void sj_free_info(info_t *, int);

/* toem_environ.c */
char *sj_getenv(info_t *, const char *);
int sj_myenv(info_t *);
int sj_mysetenv(info_t *);
int sj_myunsetenv(info_t *);
int sj_populate_env_list(info_t *);

/* toem_getenv.c */
char **sj_get_environ(info_t *);
int sj_unsetenv(info_t *, char *);
int sj_setenv(info_t *, char *, char *);

/* toem_history.c */
char *sj_get_history_file(info_t *info);
int sj_write_history(info_t *info);
int sj_read_history(info_t *info);
int sj_build_history_list(info_t *info, char *buf, int linecount);
int sj_renumber_history(info_t *info);

/* toem_lists.c */
list_t *sj_add_node(list_t **, const char *, int);
list_t *sj_add_node_end(list_t **, const char *, int);
size_t sj_print_list_str(const list_t *);
int sj_delete_node_at_index(list_t **, unsigned int);
void sj_free_list(list_t **);

/* toem_lists1.c */
size_t sj_list_len(const list_t *);
char **sj_list_to_strings(list_t *);
size_t sj_print_list(const list_t *);
list_t *sj_node_starts_with(list_t *, char *, char);
ssize_t sj_get_node_index(list_t *, list_t *);

/* toem_vars.c */
int sj_is_chain(info_t *, char *, size_t *);
void sj_check_chain(info_t *, char *, size_t *, size_t, size_t);
int sj_replace_alias(info_t *);
int sj_replace_vars(info_t *);
int sj_replace_string(char **, char *);

#endif
