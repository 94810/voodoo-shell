#include <stdio.h>
#include <string.h>
#define MEM 64000

void Brfck(const char str[]){	
	unsigned int ptr = 0; int lst = 0, loop;
	char mem[MEM]={0};

	while(lst < strlen(str)) switch(str[lst]){
		case '>' : ptr++ % MEM; lst++; break;
		case '<' : ptr-- % MEM; lst++; break;
		case '+' : mem[ptr]++; lst++; break;
		case '-' : mem[ptr]--; lst++; break;
		case '.' : putchar(mem[ptr]); lst++; break;
		case ',' : mem[ptr]=getchar(); lst++; break;
		case '[' : loop = 0;
			if(mem[ptr]) lst++;
			else do{
				lst++;
				if(str[lst]=='[') loop++; else if(str[lst]==']' && loop) loop--;
			}while(str[lst]!=']' || loop); break;
		case ']' : loop = 0;
			if(!mem[ptr]) lst++; 
			else do{	
				lst--;
				if(str[lst]==']') loop++; else if(str[lst]=='[' && loop) loop--;
			}while(str[lst]!='[' || loop); break;
		default : lst++; } }

