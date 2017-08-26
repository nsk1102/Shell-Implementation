#include <stdio.h>
#include <stdlib.h>
#include "q1.h"
int execute(struct Command *command, int *ret, int backPid)
{
	int i;		
	for(i = 0; i < command->size; i++)
	{
		if (ret[i] == 0)
		{
			execvp(command->sub_commands[i].argv[0], command->sub_commands[i].argv);
			fprintf(stderr, "%s: Command not found\n", command->sub_commands[i].argv[0]);
		}
	}
	if(!command->background)
		waitpid(ret[command->size - 1], NULL, 0);
	else
	{
		printf("[%d]\n", ret[command->size -1]);
		backPid = ret[command->size -1];
	}
	return backPid;
}
