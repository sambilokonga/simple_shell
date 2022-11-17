#ifndef _MAIN_H_
#define _MAIN_H_

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

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

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
int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);

/* toem_parser.c */
int is_cmd(info_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int interactive(info_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int _erratoi(char *);
void print_error(info_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int _myexit(info_t *);
int _mycd(info_t *);
int _myhelp(info_t *);

/* toem_builtin1.c */
int _myhistory(info_t *);
int _myalias(info_t *);

/*toem_getline.c */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* toem_environ.c */
char *_getenv(info_t *, const char *);
int _myenv(info_t *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int populate_env_list(info_t *);

/* toem_getenv.c */
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* toem_history.c */
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* toem_lists.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* toem_lists1.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);
#endif /* _MAIN_H_ */

#ifndef  _MAIN_H_
#define  _MAIN_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

#define BUFSIZE 1024
#define TOK_BUFSIZE 128
#define TOK_DELIM " \t\r\n\a"

/* Points to an array of pointers to strings called the "environment" */
extern char **environ;


/**
 * struct data - struct that contains all relevant data on runtime
 * @av: argument vector
 * @input: command line written by the user
 * @args: tokens of the command line
 * @status: last status of the shell
 * @counter: lines counter
  * @_environ: environment variable
 * @pid: process ID of the shell
 */
typedef struct data
{
        char **av;
        char *input;
        char **args;
        int status;
        int counter;
        char **_environ;
        char *pid;
} data_shell;

/**
 * struct sep_list_s - single linked list
 * @separator: ; | &
 * @next: next node
 * Description: single linked list to store separators
 */
typedef struct sep_list_s
{
        char separator;
        struct sep_list_s *next;
} sep_list;

/**
 * struct line_list_s - single linked list
* @line: command line
 * @next: next node
 * Description: single linked list to store command lines
 */
typedef struct line_list_s
{
        char *line;
        struct line_list_s *next;
} line_list;

/**
 * struct r_var_list - single linked list
 * @len_var: length of the variable
 * @val: value of the variable
 * @len_val: length of the value
 * @next: next node
 * Description: single linked list to store variables
 */
typedef struct r_var_list
{
        int len_var;
        char *val;
        int len_val;
        struct r_var_list *next;
} r_var;

/**
 * struct builtin_s - Builtin struct for command args.
 * @name: The name of the command builtin i.e cd, exit, env
 * @f: data type pointer function.
 */
typedef struct builtin_s
{
        char *name;
        int (*f)(data_shell *datash);
} builtin_t;

/* aux_lists.c */
sep_list *add_sep_node_end(sep_list **head, char sep);
void free_sep_list(sep_list **head);
line_list *add_line_node_end(line_list **head, char *line);
void free_line_list(line_list **head);

/* aux_lists2.c */
r_var *add_rvar_node(r_var **head, int lvar, char *var, int lval);
void free_rvar_list(r_var **head);

/* aux_str functions */
char *_strcat(char *dest, const char *src);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);
char *_strchr(char *s, char c);
int _strspn(char *s, char *accept);

/* aux_mem.c */
void _memcpy(void *newptr, const void *ptr, unsigned int size);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char **_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size);

/* aux_str2.c */
char *_strdup(const char *s);
int _strlen(const char *s);
int cmp_chars(char str[], const char *delim);
char *_strtok(char str[], const char *delim);
int _isdigit(const char *s);

/* aux_str3.c */
void rev_string(char *s);

/* check_syntax_error.c */
int repeated_char(char *input, int i);
int error_sep_op(char *input, int i, char last);
int first_char(char *input, int *i);
void print_syntax_error(data_shell *datash, char *input, int i, int bool);
int check_syntax_error(data_shell *datash, char *input);

/* shell_loop.c */
char *without_comment(char *in);
void shell_loop(data_shell *datash);

/* read_line.c */
char *read_line(int *i_eof);

/* split.c */
char *swap_char(char *input, int bool);
void add_nodes(sep_list **head_s, line_list **head_l, char *input);
void go_next(sep_list **list_s, line_list **list_l, data_shell *datash);
int split_commands(data_shell *datash, char *input);
char **split_line(char *input);

/* rep_var.c */
void check_env(r_var **h, char *in, data_shell *data);
int check_vars(r_var **h, char *in, char *st, data_shell *data);
char *replaced_input(r_var **head, char *input, char *new_input, int nlen);
char *rep_var(char *input, data_shell *datash);

/* get_line.c */
void bring_line(char **lineptr, size_t *n, char *buffer, size_t j);
ssize_t get_line(char **lineptr, size_t *n, FILE *stream);

/* exec_line */
int exec_line(data_shell *datash);

/* cmd_exec.c */
int is_cdir(char *path, int *i);
char *_which(char *cmd, char **_environ);
int is_executable(data_shell *datash);
int check_error_cmd(char *dir, data_shell *datash);
int cmd_exec(data_shell *datash);

/* env1.c */
char *_getenv(const char *name, char **_environ);
int _env(data_shell *datash);

/* env2.c */
char *copy_info(char *name, char *value);
void set_env(char *name, char *value, data_shell *datash);
int _setenv(data_shell *datash);
int _unsetenv(data_shell *datash);

/* cd.c */
void cd_dot(data_shell *datash);
void cd_to(data_shell *datash);
void cd_previous(data_shell *datash);
void cd_to_home(data_shell *datash);

/* cd_shell.c */
int cd_shell(data_shell *datash);

/* get_builtin */
int (*get_builtin(char *cmd))(data_shell *datash);

/* _exit.c */
int exit_shell(data_shell *datash);

/* aux_stdlib.c */
int get_len(int n);
char *aux_itoa(int n);
int _atoi(char *s);

/* aux_error1.c */
char *strcat_cd(data_shell *, char *, char *, char *);
char *error_get_cd(data_shell *datash);
char *error_not_found(data_shell *datash);
char *error_exit_shell(data_shell *datash);

/* aux_error2.c */
char *error_get_alias(char **args);
char *error_env(data_shell *datash);
char *error_syntax(char **args);
char *error_permission(char **args);
char *error_path_126(data_shell *datash);


/* get_error.c */
int get_error(data_shell *datash, int eval);

/* get_sigint.c */
void get_sigint(int sig);

/* aux_help.c */
void aux_help_env(void);
void aux_help_setenv(void);
void aux_help_unsetenv(void);
void aux_help_general(void);
void aux_help_exit(void);

/* aux_help2.c */
void aux_help(void);
void aux_help_alias(void);
void aux_help_cd(void);

/* get_help.c */
int get_help(data_shell *datash);

#endif
