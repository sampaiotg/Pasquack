#include <stdio.h>
#include <stdlib.h>

#define TOTAL_TOKENS 3

#define TRUE 1
#define FALSE 0
#define INTEIRO 256
#define PROGRAM 257
#define LABEL 258
#define CONST 259
#define TYPE 260
#define VAR 261
#define FUNCTION 262
#define PROCEDURE 263
#define BEGIN 264
#define GOTO 265
#define END 266
#define IF 267
#define THEN 268
#define WHILE 269
#define DO 270
#define OR 271
char *look;
typedef struct TToken{
    int tag;
    char *lexema;
    int inteiro;
}Token;

int automato(char *syntax[100],char *look){
     //printf("pos 2=%s\n",syntax[1]);
    int i;
    char *aux_look=look;
    for(i=0;i<TOTAL_TOKENS;i++){  //Percorre lista de tokens possiveis      
        printf("%d- %s\n",i,syntax[i]);
        char *tok_look=syntax[i]; //Cria um look para percorrer a string
        
        while(strcmp(aux_look,tok_look)){
            printf("    look=%s\n",look);
            printf("tok_look=%s\n",tok_look);
            //if(strcmp(aux_look,"\0")&&strcmp(tok_look,"\0")) break;
            printf("look++\n");
            look++,tok_look++;        }
        
    } 

    return 0;
}


Token getToken(){
    Token *retToken=(Token *)malloc(sizeof(Token));
    if(look=="p"){
        retToken->tag=PROGRAM;
        retToken->lexema="program";
        retToken->inteiro=0;
        look++;
    }
    
    
    
}


int main(){
    char *syntax[100];
    syntax[0]="program";
    syntax[1]="label";
    syntax[2]="const";
    char str[50]="program";
    look=str;
    
    automato(syntax,look);
   
    
    system("pause");
    
}
