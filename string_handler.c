#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_handler.h"

int pipeAreNull(char** argS, int size){
	int res=0;
	for(int i=0 ; i < size ; i++)
		if(argS[i][0]=='|'){
			argS[i]=NULL;
			res++;
		}
	return res;
}

char** getArg(char *str, int *s){
	char *c;
	int size=0, Olen = strlen(str);
	int i = 0, b=0;
	char **tab = malloc(sizeof(char*));

	tab[0] = strtok(str, " \t\r");

	while((c = strtok(NULL, " \t\r")) != NULL){
		if(c[0]=='"'){
			i = 1;
			b = strlen(c);
			c[strlen(c)]=' ';
			
			while(c[i]!='"' && c[i]!='\0')
				i++;
			if(c[i]=='"'){	
				c[i] = '\0';
				c += 1;
				tabAdd(&tab, c, &size);
				if(c-str+i >= Olen){
					strtok(c+i-1, " ");
					c=NULL;
				}
				else
					c=strtok(c+i, " \t\r");
			}else{
				c[b]='\0';
			}
		}
		if(c != NULL)
			tabAdd(&tab, c, &size);
	}
	
	tabAdd(&tab, NULL, &size);

	if(tab[0] != NULL)	
		*s = size;
	else
		*s = 0;
/*	
	for(int k=0;;k++){
		if(tab[k]!=NULL)
			printf("DEBUG : %s\n", tab[k]);
		else break;

	}	
*/	
	return tab;
}

void tabAdd(char ***str, char *c, int *size){
	if((*str = realloc(*str, sizeof(char*) * (++(*size) + 1) )) == NULL) perror("\033[31mYarrrr !\033[0m");
	(*str)[*size] = c;
}

void printMotd(){
	FILE *motd;
	char stdCol[81];	

	if((motd=fopen("motd", "r"))!=NULL){
		printf("\033[32m");
		 while(fgets(stdCol, 81, motd)!=NULL)
			 printf("%s",stdCol);
		 printf("\033[0m\n");
	}
}
