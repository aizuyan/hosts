#include "hosts.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>

void debug_files(pFileList lists);
int arrlen(char *arr[])
{
	int allsize = sizeof(arr);
	printf("%d\n", allsize);
	printf("%d => %s\n", sizeof(arr[2]), arr[2]);
}

int main(int argc, char **argv)
{
	/**init*/
	char hostsdir[PATH_LEN] = {0};
	strcat(hostsdir, PREFIX);
	//strcat(hostsdir, HOSTS_DIR);
	strcat(hostsdir, DATA_DIR);

	char nowhostsname[PATH_LEN] = {0};
	strcat(nowhostsname, NOW_HOSTS_NAME);
	strcat(nowhostsname, SUFFIX);
	//配置文件中要过滤掉的文件名称列表
	char *passhosts[] = {".", "..", nowhostsname};

	char nowhostspath[PATH_LEN] = {0};
	strcat(nowhostspath, PREFIX);
	//strcat(nowhostspath, HOSTS_DIR);
	strcat(nowhostspath, DATA_DIR);
	strcat(nowhostspath, NOW_HOSTS_NAME);
	strcat(nowhostspath, SUFFIX);

	char backuphostspath[PATH_LEN] = {0};
	strcat(backuphostspath, PREFIX);
	//strcat(backuphostspath, HOSTS_DIR);
	strcat(backuphostspath, DATA_DIR);
	strcat(backuphostspath, BACKUP_HOSTS_NAME);
	strcat(backuphostspath, SUFFIX);
	//end init
	/*
	printf("DATA_DIR: %s\n", whoami());
	printf("HOSTS_NAME: %s\n", nowhostspath);
	printf("HOSTS_NAME: %s\n", currenthostsname("/tmp/now.hosts"));
	pFileList lists = dirfiles("/tmp", passhosts);
	debug_files(lists);
	arrlen(passhosts);*/

	if(!PATH_EXISTS(backuphostspath))
	{
	//不存在备份文件的话尝试备份文件
		printf("%s => %s\n", ETC_HOSTS, backuphostspath);
		if(copyfile(ETC_HOSTS, backuphostspath))
		{
			printf("备份系统hosts配置文件失败！\n");
			return 1;
		}
		else
		{
			savenowhostsname(BACKUP_HOSTS_NAME, nowhostspath);
			printf("备份系统hosts配置文件成功！\n");
		}
	}

	int command = parse_command(argc, argv);
	if(checkargs(argc, argv, command))
	{
		helpers();
		return 1;
	}
	switch(command)
	{
		case HOSTS_NOW:
			{
				char * name = currenthostsname(nowhostspath);
				char namepath[PATH_LEN] = {0};
				strcat(namepath, PREFIX);
				//strcat(namepath, HOSTS_DIR);
				strcat(namepath, DATA_DIR);
				strcat(namepath, name);
				strcat(namepath, SUFFIX);
				FILE *fp;
				if((fp = fopen(namepath, "r")) == NULL)
				{
					printf("打开当前配置失败！");
					return 1;
				}
                char buf[BUF_LEN];
                char buf_tmp[BUF_LEN];
                int i,len;
                printf("\033[40m\033[1;32m|%*s\033[0m", 30, " ");
                printf("\033[40m\033[1;32m%-*s|\033[0m\n", 81, "当前hosts配置开始");
                printf("\033[40m\033[1;32m|\033[0m");
                printf("%*s\033[40m\033[1;32m|\033[0m\n", 105, "");
                while(fgets(buf, BUF_LEN, fp) != NULL)
                {   
                    memset(buf_tmp, 0, BUF_LEN);
					formatline(buf, buf_tmp);
                    printf("\033[40m\033[1;32m|\033[0m");
                    printf("%5s", "");
                    printf("%-*s", 100, buf_tmp);
                    printf("\033[40m\033[1;32m|\033[0m\n");
                }   
                printf("\033[40m\033[1;32m|\033[0m");
                printf("%*s\033[40m\033[1;32m|\033[0m\n", 105, "");
                printf("\033[40m\033[1;32m|%*s\033[0m", 30, " ");
                printf("\033[40m\033[1;32m%-*s|\033[0m\n", 81, "当前hosts配置结束");	
				fclose(fp);
			}
			break;
		case HOSTS_ALL:
			{
				pFileList pCursor = dirfiles(hostsdir, passhosts);
				char path[PATH_LEN] = {0};
				while(pCursor->next != NULL)
				{
					pCursor = pCursor->next;
					memset(path, 0, PATH_LEN);
					strcat(path, hostsdir);
					strcat(path, pCursor->d_name);
					//如过是当前的hosts显示颜色提示
					char * nowname = currenthostsname(nowhostspath);
					char nownametmp[PATH_LEN] = {0};
					strcat(nownametmp, nowname);
					strcat(nownametmp, SUFFIX);
					if(!strcmp(nownametmp, pCursor->d_name))
					{
                        printf("\033[42m\033[31m|%*s\033[0m", 30, " ");
						printf("\033[42m\033[31m%-*s|\033[0m\n", 86, "当前正在使用的hosts配置信息");
					}
					FILE * fp;
					if((fp = fopen(path, "r")) == NULL)
					{
						printf("打开配置文件失败！\n");
						return 1;
					}
					char buf[BUF_LEN];
					char buf_tmp[BUF_LEN];
					int i,len;
					printf("\033[40m\033[1;32m|%*s\033[0m", 30, " ");
					printf("\033[40m\033[1;32m%-*s|\033[0m\n", 75, pCursor->d_name);
					printf("\033[40m\033[1;32m|\033[0m");
					printf("%*s\033[40m\033[1;32m|\033[0m\n", 105, "");
					while(fgets(buf, BUF_LEN, fp) != NULL)
					{   
						memset(buf_tmp, 0, BUF_LEN);
						formatline(buf, buf_tmp);
						printf("\033[40m\033[1;32m|\033[0m");
						printf("%5s", "");
						printf("%-*s", 100, buf_tmp);
						printf("\033[40m\033[1;32m|\033[0m\n");
					}   
					printf("\033[40m\033[1;32m|\033[0m");
					printf("%*s\033[40m\033[1;32m|\033[0m\n", 105, "");
					printf("\033[40m\033[1;32m|%*s\033[0m", 30, " ");
					printf("\033[40m\033[1;32m%-*s|\033[0m\n", 75, pCursor->d_name);
					printf("\n\n");
					fclose(fp);
				}
			}
			break;
		case HOSTS_MAKE:
			{
                char path[PATH_LEN] = {0};
                strcat(path, hostsdir);
                strcat(path, argv[2]);
                strcat(path, SUFFIX);
                if(PATH_EXISTS(path))
                {
                    printf("该名称的hosts配置文件已经存在！\n");
                    return 1;
                }
                FILE *fp;
                if((fp = fopen(path, "w")) == NULL)
                {
                    printf("新建hosts配置文件失败！=> %s", path);
                    return 1;
                }
                fclose(fp);
				printf("新建hosts配置项%s成功！\n", argv[2]);
			}
			break;
		case HOSTS_ADD:
			{
				if(!PATH_WRITE_ABLE(ETC_HOSTS))
				{
					printf("没有系统hosts配置文件写权限！\n");
					return 1;
				}
                //验证添加参数是否正确
                char *ip = argv[2];
                char web[BUF_LEN] = {0};
                int i = 3;
                for(;i<argc;i++)
                {   
                    strcat(web, argv[i]);
					if(i<(argc -1))
                    	strcat(web, " ");
                }
                char path[PATH_LEN] = {0};
				char * nowname = currenthostsname(nowhostspath);
				strcat(path, hostsdir);
				strcat(path, nowname);
				strcat(path, SUFFIX);
                FILE *fp = NULL;
                if((fp = fopen(path, "a")) == NULL)
                {   
                    printf("追加内容到hosts配置文件失败！\n");
                    return 1;
                }
                char line[BUF_LEN];
                sprintf(line, "%s    %s\n", ip, web);
                fputs(line, fp);
                fclose(fp);
				copyfile(path, ETC_HOSTS);
			}
			break;
		case HOSTS_CHANGE:
			{
				if(!PATH_WRITE_ABLE(ETC_HOSTS))
				{
					printf("没有系统hosts配置文件写权限！\n");
					return 1;
				}
				char path[PATH_LEN] = {0};
				sprintf(path, "%s%s%s", hostsdir, argv[2], SUFFIX);
				if(!PATH_EXISTS(path))
				{
					printf("你要切换的hosts配置文件不存在！\n");
					return 1;
				}
				copyfile(path, ETC_HOSTS);
				savenowhostsname(argv[2], nowhostspath);	
				printf("切换hosts配置文件成功！\n");
			}
			break;
		case HOSTS_DEL:
			{
				if(!PATH_WRITE_ABLE(ETC_HOSTS))
				{
					printf("没有系统hosts配置文件写权限！\n");
					return 1;
				}
                char path[PATH_LEN] = {0};
				char * nowname = currenthostsname(nowhostspath);
				strcat(path, hostsdir);
				strcat(path, nowname);
				strcat(path, SUFFIX);
                char *ip = argv[2];
                char *web = argv[3];
                delhosts(ip, web, path);
				copyfile(path, ETC_HOSTS);
				printf("删除指定hosts成功！\n");
			}
			break;
		case HOSTS_NAMES:
			{
				pFileList pCursor = dirfiles(hostsdir, passhosts);
				while(pCursor->next != NULL)
				{
					pCursor = pCursor->next;
					//如过是当前的hosts显示颜色提示
					char * nowname = currenthostsname(nowhostspath);
					char nownametmp[PATH_LEN] = {0};
					strcat(nownametmp, nowname);
					strcat(nownametmp, SUFFIX);
					if(!strcmp(nownametmp, pCursor->d_name))
					{
                        printf("\033[40m\033[32m%s\033[0m\n", pCursor->d_name);
					}
					else
					{
                        printf("\033[40m\033[37m%s\033[0m\n", pCursor->d_name);
					}
				}
			}
			break;
		case HOSTS_REMOVE:
			{
				char * nowname = currenthostsname(nowhostspath);
				if(!strcmp(nowname, argv[2]))
				{
					printf("不能删除当前正在使用的hosts配置文件\n");
					return 1;
				}
				char path[PATH_LEN] = {0};
				strcat(path, hostsdir);
				strcat(path, argv[2]);
				strcat(path, SUFFIX);
				if(PATH_EXISTS(path))
				{
					if(remove(path) != 0)
					{
						printf("删除失败！\n");
						return 1;
					}
				}
				else
				{
					printf("你要删除的配置文件不存在!\n");
					return 1;
				}
			}
			break;
	}
	return 0;
}

char *whoami()
{
	struct passwd *pwd;
	if((pwd = getpwuid(getuid())) == NULL)
			return "Unknown user";
	return pwd->pw_name;
}

pFileList dirfiles(char const *dir_path, char *passhosts[])
{
	DIR *dir;
	struct dirent *pinfo = NULL;
	if((dir = opendir(dir_path)) == NULL)
	{
		return NULL;
	}
	pFileList pHead = (pFileList)malloc(sizeof(FileList));
	pHead->next = NULL;
	pFileList pTail = pHead;
	while((pinfo = readdir(dir)) != NULL)
	{
		//过滤掉非普通文件
		if(pinfo->d_type != DT_REG)
			continue;
		if(passfile(pinfo->d_name, passhosts, 3) == TRUE)
			continue;
		pFileList pNew = (pFileList)malloc(sizeof(FileList));
		pNew->d_name = pinfo->d_name;
		pNew->next = NULL;
		pTail->next = pNew;
		pTail = pTail->next;
		pinfo = NULL;
	}
	return pHead;
}

static int copyfile(char const *src, char const *dest)
{
	FILE *srcfp,*destfp;
	if((srcfp = fopen(src, "r")) == NULL)
		return 1;
	if((destfp = fopen(dest, "w")) == NULL)
		return 1;
	char buf[BUF_LEN];
	while(fgets(buf, BUF_LEN, srcfp) != NULL)
	{
		fputs(buf, destfp);
	}
	fclose(srcfp);
	fclose(destfp);
	return 0;
}

char *currenthostsname(char const *nowhostspath)
{
	char *path = (char *)malloc(PATH_LEN);
	FILE *nowhostsfp;
	if((nowhostsfp = fopen(nowhostspath, "r")) == NULL)
		return NULL;
	fgets(path, PATH_LEN, nowhostsfp);
	fclose(nowhostsfp);
	return path;
}

int savenowhostsname(char const * name, char const *path)
{
	FILE *fp;
	if((fp = fopen(path, "w")) == NULL)
		return 1;
	fputs(name, fp);
	fclose(fp);
	return 0;
}

int delhosts(char const *ip, char const *web, char const *path)
{
	char path_tmp[PATH_LEN] = {0};
	sprintf(path_tmp, "%s%s", path, "_tmp");
	FILE *fp,*fp_tmp;
    if((fp_tmp = fopen(path_tmp, "w+")) == NULL)
    {   
        printf("打开临时hosts配置文件失败！\n");
        return 1;
    }
    if((fp = fopen(path, "r")) == NULL)
    {   
        printf("打开当前hosts配置文件失败！");
        return 1;
    }
	char buf[BUF_LEN] = {0};
	int i,j,len,flag;
	char item[BUF_LEN] = {0};
	char buf_tmp[BUF_LEN] = {0};
	while(fgets(buf, BUF_LEN, fp) != NULL)
	{
		i = 0;
		j = 0;
		flag = 0;
		len = strlen(buf);
		memset(item, 0, BUF_LEN);
		memset(buf_tmp, 0, BUF_LEN);
		while(i < len)
		{
            if(buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
            {   
                item[j] = buf[i];
            }
			else
			{
				item[j] = '\0';
				if(item[0] != '\0')
				{   
					if(!flag)
					{   
						if(strcmp(item, ip))
						{   
							strcpy(buf_tmp, buf);
							break;
						}
						else
						{   
							if(web == NULL)
							{   
								buf_tmp[0] = '\0';
								break;
							}
							else
							{
								strcat(buf_tmp, item);
								strcat(buf_tmp, " ");
								flag = 1;
							}
						}
					}
					else
					{   
						if(strcmp(web, item))
						{
							strcat(buf_tmp, item);
							strcat(buf_tmp, " ");
						}
					}
				}
				j = -1;
				memset(item, 0 ,BUF_LEN);
			}
			if(i == (len - 1))
				strcat(buf_tmp, "\n");
			i++;
			j++;
		}
		fputs(buf_tmp, fp_tmp);
	}
	fclose(fp);
	fclose(fp_tmp);
	copyfile(path_tmp, path);
	remove(path_tmp);
	return 0;
}

int parse_command(int argc, char **argv)
{   
    if(argc == 1)
        return HOSTS_NOW;
    char *command = argv[1];
    if(!strcmp(command, "all"))
        return HOSTS_ALL;
    else if(!strcmp(command, "change"))
        return HOSTS_CHANGE;
    else if(!strcmp(command, "add"))
        return HOSTS_ADD;
    else if(!strcmp(command, "del"))
        return HOSTS_DEL;
    else if(!strcmp(command, "make"))
        return HOSTS_MAKE;
    else if(!strcmp(command, "names"))
        return HOSTS_NAMES;
    else if(!strcmp(command, "remove"))
        return HOSTS_REMOVE;
    return 0;
}
//校验参数正确性
int checkargs(int argc, char **argv, int command)
{
	switch(command)
	{
		case HOSTS_NOW:
			break;
		case HOSTS_ALL:
		case HOSTS_NAMES:
			{
				if(argc > 2)
					return 1;
			}
			break;
		case HOSTS_CHANGE:
			{
				if(argc >3)
					return 1;
			}
			break;
		case HOSTS_ADD:
			{
				if(argc < 4)
					return 1;
			}
		case HOSTS_DEL:
			{
				if(argc <3)
					return 1;
			}
			break;
		case HOSTS_MAKE:
			{
				if(argc != 3)
				{
					return 1;
				}
			}
			break;
		case HOSTS_REMOVE:
			{
				if(argc != 3)
				{
					return 1;
				}
			}
			break;
		default:
			{
				return 1;
			}
	}
	return 0;
}

void debug_files(pFileList lists)
{
	pFileList pCursor = lists;
	while(pCursor->next != NULL)
	{
		pCursor = pCursor->next;
		printf("filename => %s\n", pCursor->d_name);
	}
}

static int passfile(char const *filename, char *pass[], int pass_len)
{
	int i;
	for(i=0;i<pass_len;i++)
	{
		if(!strcmp(filename, pass[i]))
			return TRUE;
	}
	return FALSE;
}

//格式化hosts中的每一行数据
void formatline(char *buf, char *buf_tmp)
{
	int i = 0, len;
	len = strlen(buf);
	while(i <= len)
	{   
		if(buf[i] == '\n' || buf[i] == '\0')
		{   
			i++;
			continue;
		}   
		if(buf[i] == '\t')
		{   
			buf_tmp[i] = ' ';
		}   
		else
		{   
			buf_tmp[i] = buf[i];
		}   
		i++;
	}   
	buf_tmp[i] = '\0';
}

int accesswriteable(char const *path)
{
    if(access(path, F_OK))
    {
        return -1;
    }
    FILE *fp = fopen(path, "a");
    if(fp == NULL)
    {
        return -1;
    }
    fclose(fp);
    return 0;
}

void helpers()
{
	HelpList helpers[] = 
	{
		{"none arg", "display current hosts info"},
		{"all", "display all hosts infos recored"},
		{"names", "display all hosts names recored"},
		{"make XX", "create a hosts recored named XX"},
		{"checkout XX", "change current hosts to recored XX"},
		{"add XX XX ...", "add info to current hosts record, the first XX is a ip, other is website"},
		{"del XX [XX ...]", "del info from current hosts record, the first is ip, other is website"}
	};
	HelpList *helpersP = NULL;
	int i = 0;
	printf("Usage: [options] ...\n");
	printf("Options:\n");
	for(;i<7;i++)
	{
		helpersP = &helpers[i];
		printf("  --");
		printf("%-*s",25, helpersP->name);
		printf("%-*s",100, helpersP->desc);
		printf("\n");
	}
}
