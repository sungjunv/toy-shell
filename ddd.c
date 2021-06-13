#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 

#define MAX_LEN_LINE    100

void *prompt(char cBuf[]) 
{
	time rawtime;
	struct tm *timeinfo;
	char hBuf[MAX], uBuf[MAX], dBuf[MAX];
	char *now;
	void *ret;

     time(&rawtime); // passed time from Jan 1, 1970
     timeinfo = localtime(&rawtime); // translate rawtime to struct tm type
     now = asctime(timeinfo); // convert to string format
 
     now[strlen(now)-1] = 0;

       gethostname(hBuf, MAX); // save hostname to hBuf
	getlogin_r(uBuf, MAX); // save username to uBuf
	getcwd(dBuf, MAX); // save directoryname to dBuf
 
	printf("[%s]%s@%s(%s)$ ", now, hBuf, uBuf, dBuf); // print out prompt
 
	// get command string and save return value to ret pointer variable.
	// if fgets fails, it returns NULL
	
	ret = fgets(cBuf, MAX, stdin);
 
	// convert '\n' to null charactor so that
	// it can be recognized as string
	if(cBuf[strlen(cBuf)-1] == '\n')
		cBuf[strlen(cBuf)-1] = 0;
 
	return ret;
}

int main(void)
{

    
    char cBuf[MAX];
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    
    while (true) {
    
    
        char *s;
        int len;
        
        printf("MyShell >> ");
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);
      
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    
    }
    waitpid(pid, &status, 0);
	}
    return 0;
}
