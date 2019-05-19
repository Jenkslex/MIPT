#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define PATHNAME_LENGTH 100

/*
 * FIXIT:
 * Как только ставлю глубину больше 1 (например, 2), программа падает с segfault`ом.
 * 
 * >Вроде убрал
 */

void search_the_file (const char* path, const char* needle, int depth);

int main(int argc, char* argv[])
{
	char* start_dir = argv[1];
	char* needle    = argv[3];
	int   depth     = atoi (argv[2]);

	search_the_file  (start_dir , needle, depth);
	printf ("File not found.\n");
	
	return 0;
}

void search_the_file (const char* opened_dir, const char* needle, int depth)
{
	if(depth < 0) return;

	DIR*           current_dir  = opendir (opened_dir );
	struct dirent* current_file = readdir (current_dir);
	struct stat    buf          = {};

	char path[PATHNAME_LENGTH] = {};

	while(current_file)
	{
		if ((strcmp(current_file -> d_name, "." ) == 0) &&
		    (strcmp(current_file -> d_name, "..") == 0))
				continue;

		if(strcmp(needle, current_file -> d_name) == 0)
		{
			printf("path: %s\n", opened_dir);
			exit (0);
		}

		strcpy (path, opened_dir);
		strcat (path, "/");
		strcat (path, current_file -> d_name);
		
		stat(path, &buf);
		
		if (S_ISDIR (buf.st_mode))
			search_the_file (path, needle, depth - 1);

		current_file = readdir (current_dir);
	}

	closedir (current_dir);
}
