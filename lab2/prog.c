#include <sys/stat.h>           //for struct stat
#include <dirent.h>             //for dir operations
#include <unistd.h>             //for getcwd
#include <stdio.h>              //for printf
#include <errno.h>				//for perror

enum
{
    MAXDIR = 100,
};

int main(int argc, char *argv[])
{
    char dirname[MAXDIR];
    int max_num_of_links = 0;
    if(getcwd(dirname, MAXDIR) == NULL)
    {
		perror("getcwd() ");
		return errno;
	}
	
	DIR *dir;
    if((dir = opendir(dirname)) == NULL)
    {
		perror("opendir() ");
		return errno;
	}

    struct dirent *dirstr;
    struct stat stbuf;

    while((dirstr = readdir(dir)) != NULL)
    {
        if(stat(dirstr->d_name, &stbuf) == -1)
        {
            perror("stat() ");
            return errno;
		}
		else
		{
			if(max_num_of_links < stbuf.st_nlink)
            {
                max_num_of_links = stbuf.st_nlink;
			}
		}
	}

    rewinddir(dir);

    for(int i = max_num_of_links; i > 0; i--)
    {
        printf("====================\n");
        printf("Number of links: %d\n", i);
        while((dirstr = readdir(dir)) != NULL)
        {
            if(stat(dirstr->d_name, &stbuf) == -1)
            {
                perror("stat() ");
                return errno;
			}
			else
			{
				if(i == stbuf.st_nlink)
                {
                    printf("%s\n", dirstr->d_name);
				}
			}

        }
        rewinddir(dir);
        printf("====================\n\n");
    }
    return 0;
}
