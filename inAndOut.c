#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "q1.h"

void inAndOut(struct Command *command, int *ret, int fd[][2])
{
	int i, j, fds;
	if (command->size > 1)
	{
		if (ret[0] == 0)
		{
			if(command->stdin_redirect != NULL)
  			{
				close(0);    
				fds = open(command->stdin_redirect, O_RDONLY);
				if (fds < 0)
				{	
					fprintf(stderr, "%s: File not found", command->stdin_redirect);
					exit(1);
				} 
  			}
			close(1);
  			dup(fd[0][1]);
		}

 
		for(i = 1; i < command -> size - 1; i++)
		{
  			if (ret[i]==0)
  			{
  				close(0);
  				dup(fd[i - 1][0]);
  				close(1);
  				dup(fd[i][1]);
  			}
				close(fd[i-1][0]);
				close(fd[i-1][1]);
		}

		if(ret[command -> size - 1] == 0)
		{
  			if(command->stdout_redirect != NULL)
  			{
				close(1);
  				fds = open(command -> stdout_redirect, O_WRONLY | O_CREAT, 0660);
				if (fds < 0)
				{
					fprintf(stderr, "%s: Cannot create file\n", command -> stdout_redirect);
					exit(1);
				} 
  			}
  			close(0);
  			dup(fd[command->size - 2][0]);
		}
			close(fd[command->size -2][0]);
			close(fd[command->size -2][1]);

	}
	else 
	{
		if(ret[0] == 0)
		{
  			if (command->stdin_redirect != NULL)
  			{
				close(0);
				fds = open(command->stdin_redirect, O_RDONLY);
				if (fds < 0)
				{
					fprintf(stderr, "%s: File not found\n", command->stdin_redirect);
					exit(1);
				} 
  			}
			if(command->stdout_redirect != NULL)
  			{
				close(1);
				fds = open(command -> stdout_redirect, O_WRONLY | O_CREAT | O_APPEND, 0666);
				if (fds < 0)
				{
					fprintf(stderr, "%s: Cannot create file\n", command->stdout_redirect);
					exit(1);
				}
			}
		}
	}
}
