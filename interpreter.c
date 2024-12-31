//Thomas Buhler, 261116169
//Zi Yu Guan, 261101399
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/stat.h>
#include "shellmemory.h"
#include "shell.h"

int MAX_ARGS_SIZE = 7;

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int help();
int quit();
int set(char* var, char* value);
int echo(char* string);
int my_ls();
int my_mkdir(char* new_dir);
int my_touch(char* file_name);
int my_cd(char* cd_dir);
int my_cat(char* file_name);
int print(char* var);
int run(char* script);
int badcommandFileDoesNotExist();

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;

	if ( args_size < 1){
		return badcommand();
	}

	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size < 3 || args_size > MAX_ARGS_SIZE) {
			printf("Bad command: set\n");
			return 1;
		}
		char buffer[1000];
		strcpy(buffer, command_args[2]);
		if (args_size>3){
			int j;
			for (j=3; j<args_size; j++){
				strcat(buffer, " ");
				strcat(buffer, command_args[j]);
			}
		}
		return set(command_args[1], buffer);
	
	} else if (strcmp(command_args[0], "echo")==0) {
		//echo
		if (args_size != 2) return badcommand();	
		return echo(command_args[1]);
		
	} else if (strcmp(command_args[0], "my_ls")==0) {
		//my_ls
		if (args_size != 1) return badcommand();	
		return my_ls();
		
	} else if (strcmp(command_args[0], "my_mkdir")==0) {
		//my_mkdir
		if (args_size != 2) return badcommand();	
		return my_mkdir(command_args[1]);

	} else if (strcmp(command_args[0], "my_touch")==0) {
		//my_touch
		if (args_size != 2) return badcommand();	
		return my_touch(command_args[1]);
		
	} else if (strcmp(command_args[0], "my_cd")==0){
		//my_cd
		if (args_size != 2) return badcommand();
		return my_cd(command_args[1]);
		
	} else if (strcmp(command_args[0], "my_cat")==0) {
		//my_cat
		if (args_size != 2) return badcommand();
		return my_cat(command_args[1]);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
 help                   Displays all the commands\n \
 quit                   Exits / terminates the shell with “Bye!”\n \
 set VAR STRING         Assigns a value to shell memory\n \
 print VAR              Displays the STRING assigned to VAR\n \
 run SCRIPT.TXT         Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

//set command
int set(char* var, char* value){
	mem_set_value(var, value);
	return 0;
}
//echo command
int echo(char* string){
	if (string[0] == '$') {
		char tempVar[100];
		strcpy(tempVar, string + 1);
		if (strcmp(mem_get_value(tempVar), "Variable does not exist") != 0){
			printf("%s\n", mem_get_value(tempVar));
		}
	} else printf("%s\n", string);
	return 0;
}
//my_ls command
int my_ls(){
	system("ls");
	return 0;
}
//mk_mydir command
int my_mkdir(char* new_dir){
	char tempArray[106];
	strcpy(tempArray, "mkdir ");
	strcat(tempArray, new_dir);
	system(tempArray);
	return 0;
}
//my_touch command
int my_touch(char* file_name){
	FILE *temp=fopen(file_name, "w");
	fclose(temp);
	return 0;
}
//my_cd command
int my_cd(char* cd_dir){
	if (chdir(cd_dir) != 0){
		printf("Bad command: my_cd\n");
		return 1;
	}
	return 0;
}
//my_cat command
int my_cat(char* file_name){
	FILE *file_pointer =fopen(file_name, "r");
	if (file_pointer==NULL){
		printf("Bad command: my_cat\n");
		fclose(file_pointer);
		return 1;
	}
	char content[100];
	while (fgets(content, 100, file_pointer)) {
		printf("%s", content); 
	}
	fclose(file_pointer);
	return 0;
}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	fgets(line,999,p);
	while(1){
		errCode = parseInput(line);	// which calls interpreter()
		memset(line, 0, sizeof(line));

		if(feof(p)){
			break;
		}
		fgets(line,999,p);
	}

    fclose(p);

	return errCode;
}
