#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#define PERCENT         257
#define BEGIN           258
#define END             259
#define LPARENT         260
#define RPARENT         261
#define VAR             262
#define USES            263
#define VIRGULA         264
#define PONTOVIRGULA    265
#define THEN            266
#define EQUAL           267
#define ASSIGN          268
#define PROGRAM         269
#define LTHAN           270
#define LEQUAL          271
#define OR              272
#define GTHAN           273
#define GEQUAL          274
#define AND             275
#define NOT             276
#define MOD             277
#define PLUS            278
#define MINUS           279
#define NEQUAL          280
#define DIVIDE          281
#define MULTI           282
#define LITERAL         283
#define NUM             284
#define PONTO           285
#define ID              286
#define DOISPONTOS      287
#define INTEGER         288
#define REAL            289
#define BOOLEAN         290
#define CHAR            291
#define STRING          292
#define LONGINT         293
#define SHORTINT        294
#define IF              295
#define ELSE            296
#define WHILE           297
#define BREAK           298
#define WRITELN         299
#define READLN          230
#define TRUE            231
#define FALSE           232
#define ERRO            333
#define DO              334
#define FUNCTION        335
#define DIV             336
#define ASPAS           337
#define CONSTANTE       338
#define PROCEDURE       339
#define PRINT           340
#define TO              341
#define DOWNTO          342
#define WRITE           343
#define READKEY         344
#define FOR             345



/*** Estrutura do Token ***/
struct Token {
    int tag;
    char lexeme[20];
    char literal[100];
    int value;
};

/*** Estrutura da Tabela de Simbolos ***/
struct SymbolTable {
    struct  Token *token[1000];
    int numTokens;
    int scope;
    struct SymbolTable *prev;
};

/*** Variaveis Globais ***/
struct SymbolTable *top; /*** Pilha de Tabelas usadas pelo Sintático. ***/
struct Token *lookahead; /*** Simbolo lookahead usado pelo Lexico. ***/
int line=0;
int aspas=-1;
int scope = 0;
int tagteste=0;
char fileName[20];
FILE *file;

/*** Prototipos de Funcoes ***/
void initializeSymbolTable(char *lex, int t, int v);
void putIntoSymbolTable(struct Token *token, struct SymbolTable *symb);
struct Token *getFromSymbolTable(char *str, int vlteste);
struct Token *scan(FILE *file);
void printTable(void);

void ini_symbol_table() {
    top = (struct SymbolTable*) malloc(sizeof(struct SymbolTable));
    top->numTokens = 0;
    top->scope = scope;
    top->prev = NULL;
    /*** Inserindo na Tabela de Simbolos as Palavras Reservadas ***/
    initializeSymbolTable("var", VAR, -1);
    initializeSymbolTable("integer", INTEGER, -1);
    initializeSymbolTable("real", INTEGER, -1);
    initializeSymbolTable("begin", BEGIN, -1);
    initializeSymbolTable("end", END, -1);
    initializeSymbolTable("program", PROGRAM, -1);
    initializeSymbolTable("char", CHAR, -1);
    initializeSymbolTable("if", IF, -1);
    initializeSymbolTable("then", THEN, -1);
    initializeSymbolTable("else", ELSE, -1);
    initializeSymbolTable("div", DIV, -1);
    initializeSymbolTable("mod", MOD, -1);
    initializeSymbolTable("false", FALSE, -1);
    initializeSymbolTable("true", TRUE, -1);
    initializeSymbolTable("Boolean", BOOLEAN, -1);
    initializeSymbolTable("while", WHILE, -1);
    initializeSymbolTable("do", DO, -1);
    initializeSymbolTable("or", OR, -1);
    initializeSymbolTable("and", AND, -1);
    initializeSymbolTable("not", NOT, -1);
    initializeSymbolTable("break", BREAK, -1);
    initializeSymbolTable("readln", READLN, -1);
    initializeSymbolTable("writeln", WRITELN, -1);
    initializeSymbolTable("const", CONSTANTE, -1);
    initializeSymbolTable("procedure", PROCEDURE, -1);
    initializeSymbolTable("print", PRINT, -1);
    initializeSymbolTable("to", TO, -1);
    initializeSymbolTable("for", FOR, -1);
    initializeSymbolTable("write", WRITE, -1);
    initializeSymbolTable("downto", DOWNTO, -1);
    initializeSymbolTable("readkey", READKEY, -1);
    initializeSymbolTable(";", PONTOVIRGULA, -1);
    initializeSymbolTable(".", PONTO, -1);
    initializeSymbolTable("(", LPARENT, -1);
    initializeSymbolTable(")", RPARENT, -1);
    initializeSymbolTable(":", DOISPONTOS, -1);
    initializeSymbolTable("+", PLUS, -1);
    initializeSymbolTable("%", PERCENT, -1);
    initializeSymbolTable("-", MINUS, -1);
    initializeSymbolTable("/", DIVIDE, -1);
    initializeSymbolTable("*", MULTI, -1);
    initializeSymbolTable("uses", USES, -1);
    initializeSymbolTable("=", EQUAL, -1);
    initializeSymbolTable(":=", ASSIGN, -1);
    initializeSymbolTable("'", ASPAS, -1);
}

/*** Funcao para Inicializar e alocar os elemestos da Tabela de Simbolos ***/
void initializeSymbolTable(char *lex, int t, int v) {
    struct Token *token;
    token = (struct Token*) malloc(sizeof(struct Token));
    strcpy(token->lexeme, lex);
    token->tag = t;
    token->value = v;
    putIntoSymbolTable(token,top);
}

/*** Funcao para Inserir na Tabela de Simbolos ***/
void putIntoSymbolTable(struct Token *token, struct SymbolTable *symb) {
    symb->token[symb->numTokens] = token;
    symb->numTokens++;
}

/*** Funcao para Verificar a Tabela de Simbolos ***/
struct Token *getFromSymbolTable(char *str, int vlteste) {
    int pos = 0;
    struct Token *ret;
    struct SymbolTable *current = top;
    int found = 0;
    while(!found && current != NULL) {
        pos = 0;
        while(!found && pos < current->numTokens) {
            if((!strcmp(current->token[pos]->lexeme,str) && current->token[pos]->value == vlteste)) {
                found = 1;
                ret = current->token[pos];
                return ret;
            } else
                pos++;
        }
        if(!found)
            current = current->prev;
    }
    return NULL;
}

/*** Funcao para Realizar a Analise Lexica ***/
struct Token *scan(FILE *file) {
    char look1,look2;
    struct Token *token, *token2;
    token = (struct Token*) malloc(sizeof(struct Token));
    look1 = fgetc(file);
    while(1) {
        if(look1 == ' ' || look1 == '\t') {
            look1 = fgetc(file);
            continue;
        } else if(look1 == '\n') {
            line = line + 1;
            look1 = fgetc(file);
            continue;
        } else if(look1 == '/') {
            look1 = fgetc(file);
            if(look1 == '/') {
                do {
                    look1 = fgetc(file);
                } while(look1 != '\n');
            } else {
                fseek(file, -(sizeof(char)), SEEK_CUR);
            }
            look1 = fgetc(file);
            continue;
        }
        break;
    }
    switch(look1) {
    case '<':
        look2 = fgetc(file);
        if(look2 == '=') {
            token->tag = LEQUAL;
            strcpy(token->lexeme, "<=");
            token->value = -1;
            return token;
        } else if(look2 == '>') {
            token->tag = NEQUAL;
            strcpy(token->lexeme, "<>");
            token->value = -1;
            return token;
        } else {
            token->tag = LTHAN;
            strcpy(token->lexeme, "<");
            token->value = -1;
            fseek(file, -(sizeof(char)), SEEK_CUR);
            return token;
        }
    case '>':
        look2 = fgetc(file);
        if(look2 == '=') {
            token->tag = GEQUAL;
            strcpy(token->lexeme, ">=");
            token->value = -1;
            return token;
        } else {
            token->tag = GTHAN;
            strcpy(token->lexeme, ">");
            token->value = -1;
            fseek(file, -(sizeof(char)), SEEK_CUR);
            return token;
        }
    case '+':
        token->tag = PLUS;
        strcpy(token->lexeme, "+");
        token-> value = -1;
        return token;
    case '-':
        token->tag = MINUS;
        strcpy(token->lexeme, "-");
        token->value = -1;
        return token;
    case '/':
        token->tag = DIVIDE;
        strcpy(token->lexeme, "/");
        token->value = -1;
        return token;
    case '*':
        token->tag = MULTI;
        strcpy(token->lexeme, "*");
        token->value = -1;
        return token;
    case '=':
        token->tag = EQUAL;
        strcpy(token->lexeme, "=");
        token->value = -1;
        return token;
    case '%':
        token->tag = PERCENT;
        strcpy(token->lexeme, "percent");
        token->value = -1;
        return token;
    case '(':
        token->tag = LPARENT;
        strcpy(token->lexeme, "(");
        token->value = -1;
        return token;
    case ')':
        token->tag = RPARENT;
        strcpy(token->lexeme, ")");
        token->value = -1;
        return token;
    case '.':
        if(tagteste == EOF)
            token->tag = EOF;
        else token->tag = PONTO;
        strcpy(token->lexeme, ".");
        token->value = -1;
        return token;
    case ',':
        token->tag = VIRGULA;
        strcpy(token->lexeme, ",");
        token->value = -1;
        return token;
    case ';':
        token->tag = PONTOVIRGULA;
        strcpy(token->lexeme, ";");
        token->value = -1;
        return token;
    case '\'':
        if(aspas == -1) {
            token->tag = ASPAS;
            strcpy(token->lexeme, "'");
            token->value = -1;
            aspas = 0;
            fseek(file, -(sizeof(char)), SEEK_CUR);
            return token;
        }
        if(aspas == 0) {
            token->tag = LITERAL;
            char *buf = (char*) malloc(50*sizeof(char));
            char *ibuf;
            ibuf = buf;
            look1 = fgetc(file);
            while(look1 != '\'') {
                *buf = look1;
                look1 = fgetc(file);
                buf++;
            }
            *buf = '\0';
            buf = ibuf;
            strcpy(token->literal, buf);
            strcpy(token->lexeme,"Literal");
            token->value = -1;
            putIntoSymbolTable(token, top);
            fseek(file, -(sizeof(char)), SEEK_CUR);
            aspas = 1;
            return token;
        }
        if(aspas == 1) {
            token->tag = ASPAS;
            strcpy(token->lexeme, "'");
            token->value = -1;
            aspas = -1;
            return token;
        }
    case ':':
        look2 = fgetc(file);
        if(look2 == '=') {
            token->tag = ASSIGN;
            strcpy(token->lexeme, ":=");
            token->value = -1;
            return token;
        } else {
            token->value = -1;
            token->tag = DOISPONTOS;
            fseek(file, -(sizeof(char)), SEEK_CUR);
            strcpy(token->lexeme, ":");
            return token;
        }
    } /*** Fim do Switch ***/
    if(isdigit(look1)) {
        int v = 0;
        do {
            v = 10 * v + atoi(&look1);
            look1 = fgetc(file);
        } while(isdigit(look1));
        token->value = v;
        strcpy(token->lexeme, "Numero");
        fseek(file, -(sizeof(char)), SEEK_CUR);
        token2 = getFromSymbolTable(token->lexeme, v);
        if(token2 != NULL) {
            return token2;
        }
        token->tag = NUM;
        putIntoSymbolTable(token, top);
        return token;
    }
    if(isalpha(look1)) {
        char *buffer = (char*) malloc(50*sizeof(char));
        char *ibuffer;
        ibuffer = buffer;
        do {
            *buffer = look1;
            look1 = fgetc(file);
            buffer++;
        } while(isalpha(look1) || isdigit(look1));
        *buffer = '\0';
        if(look1 == '(') {
            token->tag = FUNCTION;
        } else {
            look1 = fgetc(file);
            if(look1 == '(') {
                token->tag = FUNCTION;
            } else {
                token->tag = ID;
            }
            fseek(file, -(sizeof(char)), SEEK_CUR);
        }
        fseek(file, -(sizeof(char)), SEEK_CUR);
        buffer = ibuffer;
        token2 = getFromSymbolTable(buffer, -1);
        if(token2 != NULL) {
            if(!(strcmp(token2->lexeme, "end"))) {
                fseek(file, -(sizeof(char)), SEEK_CUR);
                look1 = fgetc(file);
                if(look1 == '.') {
                    fseek(file, -(sizeof(char)), SEEK_CUR);
                    tagteste = EOF;
                }
            }
            if(token2->tag > 287 && token2->tag < 294) {
                fseek(file, -(sizeof(char)), SEEK_CUR);
                return token2;
            } else {
                return token2;
            }
            return token2;
        }
        strcpy(token->lexeme,buffer);
        token->value = -1;
        putIntoSymbolTable(token, top);
        fseek(file, -(sizeof(char)), SEEK_CUR);
        if((look2=fgetc(file))==';') {
            fseek(file, -(sizeof(char)), SEEK_CUR);
        }
        return token;
    }
    token->tag = look1;
    token->value = -1;
    strcpy(token->lexeme, "Nao Reconhecido");
    return token;
}

/*** Funcao para Imprimir a Tabela de Simbolos ***/
void printTable(void) {
    int pos = 0;
    struct SymbolTable *current = top;
    char *saidalex=strcat(fileName,".lex");
    file = fopen(saidalex,"w");
    fprintf(file,"#!\n#!\n#!-------------------- TABELA DE SIMBOLOS ------------------\n#!\n");
    fprintf(file,"#!    Simbolo          Tag                    Valor  \n#!\n");
    while(pos < current->numTokens) {
        if(current->token[pos]->tag == LITERAL)
            fprintf(file,"%10s %15i %30s\n",current->token[pos]->lexeme,current->token[pos]->tag, current->token[pos]->literal);
        else
            fprintf(file,"%10s %15i %30i\n",current->token[pos]->lexeme,current->token[pos]->tag, current->token[pos]->value);
        pos ++;
    }
    fclose(file);
    //system(saidalex);
}
