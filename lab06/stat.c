// stat.c ... example of using the stat() system call
// borrowed from the GNU Glibc stat(2) manual entry

#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sysexits.h>

static void show_info (const char *left, char *right);

int main (int argc, char *argv[])
{
	struct stat sb;

	if (argc != 2)
		errx (EX_USAGE, "usage: %s <pathname>", argv[0]);

	if (stat (argv[1], &sb) == -1)
		err (EX_OSERR, "%s", argv[1]);

	switch (sb.st_mode & S_IFMT) {
	case S_IFBLK:  show_info ("File type", "block device"); break;
	case S_IFCHR:  show_info ("File type", "character device"); break;
	case S_IFDIR:  show_info ("File type", "directory"); break;
	case S_IFIFO:  show_info ("File type", "FIFO (named pipe)"); break;
	case S_IFLNK:  show_info ("File type", "symlink"); break;
	case S_IFREG:  show_info ("File type", "regular file"); break;
	case S_IFSOCK: show_info ("File type", "named socket"); break;
	default:       show_info ("File type", "unknown?"); break;
	}

#define N 64
	char buf[N];

	snprintf (buf, N, "%ld", sb.st_ino);
	show_info ("Inode number", buf);

	snprintf (buf, N, "%07lo (octal)", (unsigned long) sb.st_mode);
	show_info ("File mode", buf);

	snprintf (buf, N, "%ld", (long) sb.st_nlink);
	show_info ("Link count", buf);

	snprintf (buf, N, "uid=%ld   gid=%ld",
		(long) sb.st_uid, (long) sb.st_gid);
	show_info ("Ownership", buf);

	snprintf (buf, N, "%ld bytes", (long) sb.st_blksize);
	show_info ("Preferred I/O block size", buf);

	snprintf (buf, N, "%lld bytes", (long long) sb.st_size);
	show_info ("File size", buf);

	snprintf (buf, N, "%lld blocks", (long long) sb.st_blocks);
	show_info ("Blocks allocated", buf);

	show_info ("Last status change",     ctime (&sb.st_ctime));
	show_info ("Last file access",       ctime (&sb.st_atime));
	show_info ("Last file modification", ctime (&sb.st_mtime));

	return EXIT_SUCCESS;
}

static void
show_info (const char *left, char *right)
{
	// ctime(3) insists on giving us a newline; trim it
	right = strsep (&right, "\n");
	printf ("%24s: %s\n", left, right);
}
