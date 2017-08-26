#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SUB_COMMANDS 5
#define MAX_ARGS 10

struct SubCommand
{
		char *line;
		char *argv[MAX_ARGS];
};

struct Command
{
		struct SubCommand sub_commands[MAX_SUB_COMMANDS];
		int num_sub_commands;
		char *stdin_redirect;
		char *stdout_redirect;
		int background;
		int size;
};

void PrintArgs(char **argv);
void ReadArgs(char *in, char **argv, int size); 
void ReadCommand(char *line, struct Command *command);
void PrintCommand(struct Command *command);
void ReadRedirectsAndBackground(struct Command *command);

/*int main()
{
	char s[200];	   
	     // Read a string from the user
	printf("Enter a string: ");
	fgets(s, sizeof s, stdin);

	struct Command *command;
	command = malloc(sizeof(struct Command));

	ReadCommand(s, command);
	ReadRedirectsAndBackground(command);
	PrintCommand(command);
	free(command);
}
*/
void ReadRedirectsAndBackground(struct Command *command)
{
	int i = 4;
	char *background = "&";
	char *in = "<";
	char *out = ">";
	command -> background = 0;
	while (i > 0 && command->sub_commands[i].line == NULL) 
	{	
		i--;
	}
	command->size = i+1;
	int j = 8;
	while(command->sub_commands[i].argv[j] == NULL && j > 0)
	{
		j--;
	}
	while(command->sub_commands[i].argv[j] != NULL && j >= 0)
	{
		if(strcmp(command->sub_commands[i].argv[j], background) == 0)
		{
			command->background = 1;
			command->sub_commands[i].argv[j] = NULL;
		}
		else if(strcmp(command->sub_commands[i].argv[j], out) == 0)
		{
			command->stdout_redirect = command->sub_commands[i].argv[j + 1];
			command->sub_commands[i].argv[j] = NULL;
			command->sub_commands[i].argv[j + 1] = NULL;
		}
		else if(strcmp(command->sub_commands[i].argv[j], in) == 0)
		{
			command->stdin_redirect = command->sub_commands[i].argv[j + 1];
			command->sub_commands[i].argv[j] = NULL;
			command->sub_commands[i].argv[j + 1] = NULL;
		}
		j--;
	}
}

void ReadArgs(char *in, char **argv, int size)
{
	int count = 0;
	char *s;
	s = strdup(in);
	argv[0] = strtok(s, " \n");
	while(count < size-1 && argv[count] != NULL)
	{
		argv[++count] = strtok(NULL, " \n");
	}
	argv[count] = NULL;
}

void PrintArgs(char **argv)
{
	int i=0;
	while(argv[i] != NULL)
	{
		printf("argv[%d] = '%s'\n", i, argv[i]);
		i++;
	}
}

void ReadCommand(char *line, struct Command *command)
{
   	int count = 0;
	int i;
	char *s;
	s = strdup(line);
   	command->sub_commands[count++].line = strtok(s, "|\n");
	while(count < MAX_SUB_COMMANDS)
	{   
		command->sub_commands[count].line = strtok(NULL, "|\n");
		if(command->sub_commands[count].line == NULL)
            break;
		count++;
	}

	for (i = 0; i < count; i++)
	{
		ReadArgs(command->sub_commands[i].line, command->sub_commands[i].argv, MAX_ARGS);
	}
}

void PrintCommand(struct Command *command)
{
	int i=0;
 	while(command->sub_commands[i].line != NULL)
	{
		printf("Command %d:\n", i);
        PrintArgs(command->sub_commands[i].argv);
		i++;
	}
	printf("\nRedirect stdin: %s\n", command->stdin_redirect);
	printf("Redirect stdout: %s\n", command->stdout_redirect);
	if(command->background == 1)
		printf("Background: yes\n");
	else
		printf("Background: no\n");
}
