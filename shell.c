#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "string_handler.h"
#include "BrainFuck.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define LINE_MAX 9000


void removeBubble(char** argS, int *size);
int setStream(char **argS, int size);
void doExec(char **argS, int argn, int plumber);
char** nextCmd(char ** argS, int argn, int * cont);

typedef struct{
	int *tab;
	int cursor;
	int size;
}DTab;
void AddDtab(int, DTab*);

int main(){
	bool run = true, nowait = false;
	char str[LINE_MAX];
	char** argS;
	char *RdL=NULL;
	int argn, pid;
	DTab PidAr;

	PidAr.tab=NULL;
	
	printMotd();
		
	while(run){
		
		strcpy(str, "\033[32m");	
		getcwd(str+(strlen(str)), LINE_MAX);
		strcat(str, " % ");	
		strcat(str, "\033[0m");
		RdL = readline(str);

		if(NULL == RdL){ //On sort si EOF ou erreur
			run = false;
			printf("\nBye Capt'ain\n");
		}else{
			add_history(RdL);
			strcpy(str, RdL);
			free(RdL);
			
		//	str[strlen(str)-1]='\0';  //On remove \n de la chaine récupéré
					
			argS = getArg(str, &argn);
					
			if(argn == 0); //Un peu crappy mais c'est compact

			else if(strcmp(argS[0], "cd") == 0){
				if(chdir(argS[1]))
					perror("\033[31mYarrrrrh ! \033[0m");
			
			}else if(strcmp(argS[0], "exit") == 0)
				run = false;
			else if(!strcmp(argS[0], "BF")){
				if(argn > 1)
					if(!fork()){
						Brfck(argS[1]);
						exit(0);
					}
					else
						wait(NULL);
			}
			else{
				if(!strcmp(argS[argn-1], "&")){
					 nowait=false;
					 argS[argn-1] = NULL;
					 argn --;
				}
				else
					 nowait=true;
				
				if(!(pid=fork())){
					int plumber = pipeAreNull(argS, argn);
					if(plumber){
						doExec(argS, argn, plumber);
					}
					else{
						setStream(argS, argn);
						removeBubble(argS, &argn); // setStream Crée des bulle de \0 dans argS ( A voir : appelle direct dans SetStream ? )
						execvp(argS[0], argS);
						perror("\033[31mYarrrrh ! \033[0m");
						free(argS);
						exit(EXIT_FAILURE);
					}
				}
				else{
					
					if(nowait) wait(NULL);
					else AddDtab(pid, &PidAr);
				}
			}	

			free(argS);
		}
	}

	if(PidAr.tab!=NULL)
		for(int i=0 ; i<=PidAr.cursor ; i++ ){
			kill(PidAr.tab[i], SIGTERM);
			waitpid(PidAr.tab[i], NULL, 0);
		};

	printf("\n");
	return 0;
}

void AddDtab(int pid, DTab* arr){
	
	if(arr->tab == NULL){
		arr->tab = malloc(sizeof(int));
		arr->size = 1;
		arr->cursor = 0;
	}else if(arr->cursor + 1  == arr->size){

		if(realloc(arr->tab, sizeof(int)*arr->size*2)==NULL) perror("\033[31mYarrrr !\033[0m");
		arr->size *= 2;
		arr->cursor++;
	}else{
		arr->cursor++;
	}

	arr->tab[arr->cursor] = pid;
}

void doExec(char** args, int argn, int cont){
	char** argS;

	while((argS=nextCmd(args, argn, &cont))!=NULL){
		int pipeAr[2]; 
		pipe(pipeAr);

		if(fork()){
			if(nextCmd(args, argn, &cont)!=NULL){
				dup2(pipeAr[STDIN], STDIN);
				close(pipeAr[STDOUT]); close(pipeAr[STDIN]);
				cont++;
			}
			setStream(argS, argn);
			removeBubble(args, &argn);	
			execvp(argS[0], argS);
			perror("\033[31mYarrrrh ! \033[0m");
			exit(EXIT_FAILURE);
		}
		else{
			dup2(pipeAr[STDOUT], STDOUT);
			close(pipeAr[STDIN]); close(pipeAr[STDOUT]);
		}
	}
	
	exit(EXIT_SUCCESS);

}

char** nextCmd(char** argS, int argn, int * cont){

	char** res=NULL;
	int index=0, pass=0;

//	printf("INPUT ENV : Args %X, cont %d, argn %d\n", argS, *cont, argn);

	pass = *cont;
		
	if(pass<0){
		res = NULL; 
	}else if(pass==0){
		res = argS;
	}else{

		while(pass && index < argn){
				
			if(argS[index]!=NULL){ // On ne trouve pas de pipe	
				index++;
			}
			else{
				index++; 
				pass--;  //On trouve un pipe on a trouver la cmd
			}
		}
		
		res = argS + index;
	}
	
/*	if(res!=NULL)
		printf("OUT ENV : Arg %X, res %s, *cont %d\n", argS, res[0], *cont);
	else
		printf("Res NULL\n");
*/
	*cont = *cont - 1 ;
	
	return res;
}

void removeBubble(char** argS, int *size){ //Faudarait le mod pour les pipes mais bon en soit c'est pas prioritaire
	
	char* tab[*size];
	int InT=0, InA=0; 
	

	for(InA=0 ; InA < *size ; InA++){
		if(argS[InA]!=NULL){
			if(argS[InA][0]!='\0'){
				tab[InT] = argS[InA];
				InT++;
			}
		}else{
			tab[InT] = argS[InA];
			InT++;
		}
	}

	
	*size = InT;
		
	argS[InT] = NULL;
	InT --;


	for(; InT >=0 ; InT--)
		argS[InT] = tab[InT];


	return;
}

int setStream(char **argS, int size){
	int i, res, fd;	

	for(i=0 ; i < size-1; i++){ // On scan pas le dernier si y a une redirection en dernier c'est forcément une erreur (Peut-être l'indiquer a l'usr ?)
	
		if(argS[i] == NULL) break; //Ok c'est pas ce qu'il y a de plus propre mais je mod plus tard ( c'est pour les pipes )

		if(!strcmp(argS[i], ">")){
			fd = open(argS[i+1], O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
			dup2(fd, STDOUT);
			close(fd);
			argS[i][0]='\0';
			argS[i+1][0]='\0';
		
		}else if(!strcmp(argS[i], "<")){
			fd = open(argS[i+1], O_RDONLY);
			dup2(fd, STDIN);
			close(fd);
			argS[i][0]='\0';
			argS[i+1][0]='\0';

		}else if(!strcmp(argS[i], "2>")){
			fd = open(argS[i+1], O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
			dup2(fd, STDERR);
			close(fd);
			argS[i][0]='\0';
			argS[i+1][0]='\0';

		}else if(!strcmp(argS[i], ">>")){
			fd = open(argS[i+1], O_CREAT|O_APPEND|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
			dup2(fd, STDOUT);
			close(fd);
			argS[i][0]='\0';
			argS[i+1][0]='\0';

		}else if(!strcmp(argS[i], "2>>")){
                        fd = open(argS[i+1], O_CREAT|O_APPEND|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	                dup2(fd, STDERR);
			close(fd);
		  	argS[i][0]='\0';
			argS[i+1][0]='\0';
			
		}

	}
	if(argS[i]!=NULL){
		if(!strcmp(argS[i], ">") || !strcmp(argS[i], "<") || !strcmp(argS[i], "2>") || !strcmp(argS[i], ">>") || !strcmp(argS[i], "2>>"))
			 printf("\033[31mWarning : Redirection can't be last cmd element (Forgoten file name ?)\n\033[0m") ; // Je la passe juste en Warning 
	}
	return 0; 
}





