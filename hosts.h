#ifndef _HOSTS_MANAGER_H
#define _HOSTS_MANAGER_H

#define TRUE 1
#define FALSE 0
#define HOSTS_NOW 1
#define HOSTS_ALL 2
#define HOSTS_CHANGE 3
#define HOSTS_ADD 4
#define HOSTS_DEL 5
#define HOSTS_MAKE 6
#define HOSTS_NAMES 7
#define HOSTS_REMOVE 8

#ifndef PREFIX
#define PREFIX "/usr/local/"
#endif

#define HOSTS_DIR "hosts/"

#define DATA_DIR "data/"

#define SUFFIX ".hosts"

#define NOW_HOSTS_NAME "now"

#define BACKUP_HOSTS_NAME "backup"

#define ETC_HOSTS "/etc/hosts"

#define DATA_NOW_HOSTS_PATH DATA_DIR now SUFFIX

#define PATH_EXISTS(path) (access(path, F_OK) == 0)

#define PATH_WRITE_ABLE(path) (access(path, W_OK) == 0)

#define BUF_LEN 1024

#define PATH_LEN 256

typedef struct FileList {
	struct FileList *next;
	char *d_name;
}FileList, *pFileList;

typedef struct HelpList {
	char *name;
	char *desc;
}HelpList;

char *whoami();
pFileList dirfiles(char const *, char *[]);
static int passfile(char const *, char *[], int);
//备份文件A到文件B
static int copyfile(char const *, char const *);
//获取当前的hosts配置名称
char *currenthostsname(char const *);
int savenowhostsname(char const *, char const *);
int parse_command(int, char **);
int checkargs(int, char **, int);
void formatline(char *, char *);
int delhosts(char const *, char const *, char const *);
void helpers();



#endif	/* _HOSTS_MANAGER_H */
