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

/* Constants for read/write buffer sizes */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* Constants for command chaining types */
#define CMD_NORM    0   /* Normal command */
#define CMD_OR      1   /* Logical OR chaining */
#define CMD_AND     2   /* Logical AND chaining */
#define CMD_CHAIN   3   /* Standard command chaining */

/* Constants for sj_conv_num() */
#define CONVERT_LOWERCASE   1   /* Convert to lowercase */
#define CONVERT_UNSIGNED    2   /* Convert to unsigned */

/* Flag for using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK  0

#define HIST_FILE   ".shell_history"
#define HIST_MAX    4096

extern char **environ;

/**
 * Struct for a singly linked list of strings.
 */
typedef struct liststr
{
    int num;
    char *str;
    struct liststr *next;
} l_tree;

/**
 * Struct containing arguments to pass into functions.
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
    l_tree *env;
    l_tree *history;
    l_tree *alias;
    char **environ;
    int env_changed;
    int status;

    char **cmd_buf; /* Pointer to command chain buffer for memory management */
    int cmd_buf_type; /* Type of command chaining: CMD_NORM, CMD_OR, CMD_AND, CMD_CHAIN */
    int readindex;
    int histcount;
} info_tree;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/**
 * Struct for defining built-in commands and their associated functions.
 */
typedef struct builtin
{
    char *type;
    int (*func)(info_tree *);
} builtin_table_struct;

void sj_put_string(char *);
int sj_putchar(char);
int sj_putindex(char c, int index);
int sj_putstrindex(char *str, int index);

int sj_get_strlen(char *);
int sj_strcomps(char *, char *);
char *sj_starts_with(const char *, const char *);
char *sj__strconcat(char *, char *);

char *sj_strcopy(char *, char *);
char *sj_strcast(const char *);
void sj_putstr(char *);
int sj_putchar(char);

char **sj_str2row(char *, char *);
char **sj_str2row2(char *, char);

int sj_buffree(void **);

int sj_interf(info_tree *);
int sj_delimt(char, char *);
int sj_alphacheck(int);
int sj_alpha_to_int(char *);

int sj_alpha2int_error(char *);
void sj_printf_err(info_tree *, char *);
int sj_printd(int, int);
char *sj_conv_num(long int, int, int);
void sj_rm_comment(char *);

int sj_exit(info_tree *);
int sj_cd(info_tree *);
int sj_help(info_tree *);

int sj_history(info_tree *);
int sj_alias(info_tree *);

ssize_t sj_get_value(info_tree *);
int sj_get_line_info(info_tree *, char **, size_t *);
void sj_intvalue(int);

void sj_cls_info(info_tree *);
void sj_set_info(info_tree *, char **);
void sj_ffinfo(info_tree *, int);

char *sj_getenv(info_tree *, const char *);
int sj_env(info_tree *);
int sj_set_envchr(info_tree *);
int sj_unset_envchr(info_tree *);
int sj_increment_env(info_tree *);

char **sj_get_env(info_tree *);
int sj_unset_envchr(info_tree *, char *);
int sj_set_envchr(info_tree *, char *, char *);

char *sj_get_hfile(info_tree *info);
int sj_write_hfile(info_tree *info);
int sj_read_hfile(info_tree *info);
int sj_cost_hfile(info_tree *info, char *buf, int linecount);
int sj_vamp_hfile(info_tree *info);

l_tree *sj_node_add(l_tree **, const char *, int);
l_tree *sj_node_add_end(l_tree **, const char *, int);
size_t sj_printlstr(const l_tree *);
int sj_del_node(l_tree **, unsigned int);
void sj_ls_free(l_tree **);

size_t sj_getlist_len(const l_tree *);
char **sj_ltree_str(l_tree *);
size_t sj_printlist(const l_tree *);
int sj_print_alias(l_tree *node);
int sj_set_alias(info_tree *info, char *str);
int sj_unset_alias(info_tree *info, char *str);

#endif
