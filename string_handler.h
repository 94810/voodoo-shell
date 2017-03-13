#ifndef STRING_HANDLER
#define STRING_HANDLER

void tab2spc(char *str);
char** getArg(char *str, int *s);
void tabAdd(char ***str, char *c, int *size);
int pipeAreNull(char** argS, int size);
void printMotd();
#endif
