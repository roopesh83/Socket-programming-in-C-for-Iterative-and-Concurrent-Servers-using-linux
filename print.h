#include<stdio.h>
void println(char a[]){
	printf("%s\n",a);
}

void print(char a[]){
	printf("%s",a);
}

void prompt(char a[]){
	printf("%s\n>", a);
}

void clean_stdin(void)
{
    int c;
    if (!feof(stdin))
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}