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
    //get current working directory
    if(getcwd(dirname, MAXDIR) == NULL)
    {
		perror("getcwd() ");
		return errno;
	}
	
	//open current catalog
	DIR *dir;
    if((dir = opendir(dirname)) == NULL)
    {
		perror("opendir() ");
		return errno;
	}

    struct dirent *dirstr;
    struct stat stbuf;
    
    //get max num of links in 
    //this catalog
    
	//reading all catalog's files
    while((dirstr = readdir(dir)) != NULL)
    {
		//get stat for current catalog's file
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
	//pointer get first catalog file link
    rewinddir(dir);
	
	//reading all files with
	//diff num of links
    for(int i = max_num_of_links; i > 0; i--)
    {
        printf("====================\n");
        printf("Number of links: %d\n", i);
        //reading all catalog's files
        while((dirstr = readdir(dir)) != NULL)
        {
			//get stat for current catalog's file
            if(stat(dirstr->d_name, &stbuf) == -1)
            {
                perror("stat() ");
                return errno;
			}
			else
			{
				//print filename with current 
				//num of links in stdin
				if(i == stbuf.st_nlink)
                {
                    printf("%s\n", dirstr->d_name);
				}
			}

        }
        //pointer get first catalog file link
        rewinddir(dir);
        printf("====================\n\n");
    }
    
    //close a directory
    if(closedir(dir) == -1)
    {
		perror("closedir() ");
		return errno;
	}
	
    return 0;
}
