// myls.c ... my very own "ls" implementation

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
# include <bsd/string.h>
#endif
#include <sysexits.h>
#include <unistd.h>

#define MAXDIRNAME 256
#define MAXFNAME 256
#define MAXNAME 24

char *rwxmode (mode_t, char *);
char *username (uid_t, char *);
char *groupname (gid_t, char *);

int main (int argc, char *argv[])
{
	// string buffers for various names
	char uname[MAXNAME+1];
	char gname[MAXNAME+1];
	char mode[MAXNAME+1];

	// collect the directory name, with "." as default
	char dirname[MAXDIRNAME] = ".";
	if (argc >= 2)
		strlcpy (dirname, argv[1], MAXDIRNAME);

	// check that the name really is a directory
	struct stat info;
	if (stat (dirname, &info) < 0)
		err (EX_OSERR, "%s", dirname);

	if (! S_ISDIR (info.st_mode)) {
		errno = ENOTDIR;
		err (EX_DATAERR, "%s", dirname);
	}

	// open the directory to start reading
	DIR *df;
	df = opendir(dirname);
	
	// read directory entries
	struct dirent *entry;
	entry = readdir(df);
	
	while (entry != NULL) {

	    if (entry->d_name[0] == '.') {
            entry = readdir(df);
            continue;
        }

	    lstat(entry->d_name, &info);

        switch (entry->d_type) {
        case DT_DIR:  printf("d"); break;
        case DT_LNK:  printf("l"); break;
        case DT_REG:  printf("-"); break;
        default:       printf("?"); break;
        }
	    
		printf (
			"%s  %-8.8s %-8.8s %8lld  %s\n",
			rwxmode (info.st_mode, mode),
			username (info.st_uid, uname),
			groupname(info.st_gid, gname),
			(long long) info.st_size,
			entry->d_name
		);
		
	    entry = readdir(df);
	}

	// finish up
	closedir(df);

	return EXIT_SUCCESS;
}

// convert octal mode to -rwxrwxrwx string
char *rwxmode (mode_t mode, char *str)
{
    strcpy(str, "\0");
    if ((mode & S_IRUSR) == S_IRUSR) {
        strcat(str, "r");
    } else {
        strcat(str, "-");
    }

    if ((mode & S_IWUSR) == S_IWUSR) {
        strcat(str, "w");
    } else { 
        strcat(str, "-");
    }

    if ((mode & S_IXUSR) == S_IXUSR) {
        strcat(str, "x");
    } else { 
        strcat(str, "-");
    }

    if ((mode & S_IRGRP) == S_IRGRP) {
        strcat(str, "r");
    } else {
        strcat(str, "-");
    }

    if ((mode & S_IWGRP) == S_IWGRP) {
        strcat(str, "w");
    } else { 
        strcat(str, "-");
    }

    if ((mode & S_IXGRP) == S_IXGRP) {
        strcat(str, "x");
    } else { 
        strcat(str, "-");
    }

    if ((mode & S_IROTH) == S_IROTH) {
        strcat(str, "r");
    } else { 
        strcat(str, "-");
    }

    if ((mode & S_IWOTH) == S_IWOTH) {
        strcat(str, "w");
    } else { 
        strcat(str, "-");
    }

    if ((mode & S_IXOTH) == S_IXOTH) {
        strcat(str, "x");
    } else {
        strcat(str, "-");
    }
        
	return str;
}

// convert user id to user name
char *username (uid_t uid, char *name)
{
	struct passwd *uinfo = getpwuid (uid);
	if (uinfo != NULL)
		snprintf (name, MAXNAME, "%s", uinfo->pw_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) uid);
	return name;
}

// convert group id to group name
char *groupname (gid_t gid, char *name)
{
	struct group *ginfo = getgrgid (gid);
	if (ginfo != NULL)
		snprintf (name, MAXNAME, "%s", ginfo->gr_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) gid);
	return name;
}
