#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "q1.h"
void inAndOut(struct Command *command, int *ret, int fd[][2]);
int execute(struct Command *command, int *ret, int backPid);
int main()
{
    char s[200];   
	int backPid = 0;
	// Read a string from the user
	struct Command *command;
	command = malloc(sizeof(struct Command));
	while(1)
	{
	if (backPid != 0)
	{
		int status;
		pid_t result = waitpid(backPid, &status, WNOHANG);
		if(result != 0 && result != -1)
		{	
			printf("[%d] finished\n", backPid);
			backPid = 0;
		}
	}
	char c[50];
	getcwd(c, 50);
	printf("%s$ ", c);
	fgets(s, sizeof s, stdin);
	struct Command *command;
	command = malloc(sizeof(struct Command));	
	if(strcmp(s, "\n") == 0)
		continue;
	if(strcmp(s, "exit\n") == 0)
		return 0;
	ReadCommand(s, command);
	ReadRedirectsAndBackground(command);
	if (strcmp(command->sub_commands[0].argv[0], "cd") == 0)
	{ 
		if(chdir(command->sub_commands[0].argv[1]) != 0)
		{
			printf("cd: %s: No such file or directory\n", command->sub_commands[0].argv[1]);
		}
		continue;
	}

	int ret[command->size];
	int fd[command->size - 1][2];
	int i;
	for(i = 0; i < command->size - 1; i++)
	{
		pipe(fd[i]);
	}
	for(i = 0; i < command->size; i++)
		ret[i] = 1;
	ret[0] = fork();
	for(i = 1; i < command->size; i++)
	{
		if (ret[i-1] != 0)
		{	
			ret[i] = fork();
		}
		else
			break;
	}
	inAndOut(command, ret, fd);
	
	backPid = execute(command, ret, backPid);
	}
	return 0;
}

