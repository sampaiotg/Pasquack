#define SYMTBL_SZ 1000
#define KWLIST_SZ 11
#define MAXTOKEN 16

FILE * destfile;

int lblcount; /* indica o rótulo atual */
/* tabela de símbolos */
char *symtbl[SYMTBL_SZ];
int nsym;
/* número de entradas atuais na tabela de símbolos */
/* códigos e lista de palavras-chave */
char kwcode[KWLIST_SZ+1] = "ileweRWvbep";
char *kwlist[KWLIST_SZ] = {"IF", "ELSE", "WHILE", "READ", "WRITE", "VAR", "BEGIN", "END", "PROGRAM"};
char look; /* O caracter lido "antecipadamente" (lookahead) */
char token; /* código do token atual */
char value[MAXTOKEN+1]; /* texto do token atual */

int newlabel()        {
    return lblcount++;
}

void asm_clear()        {
    fputs("\txor ax, ax\n",destfile);
}

/* negativa o reg. primário */
void asm_negative()        {
    fputs("\tneg ax\n",destfile);
}

/* carrega uma variável no reg. prim. */
void asm_loadvar(char *name)        {
    char str[30] = "\tmov ax, word ptr";
    strcat(str, name);
    strcat(str, "\n");
    fputs(str,destfile);
}
/* coloca reg. prim. na pilha */
void asm_push()        {
    fputs("\tpush ax\n",destfile);
}

/* adiciona o topo da pilha ao reg. prim. */
void asm_popadd()        {
    fputs("\tpop bx\n",destfile);
    fputs("\tadd ax, bx\n",destfile);
}

/* subtrai o reg. prim. do topo da pilha */
void asm_popsub()        {
    fputs("\tpop bx\n",destfile);
    fputs("\tsub ax, bx\n",destfile);
    fputs("\tneg ax\n",destfile);
}

/* multiplica o topo da pilha pelo reg. prim. */
void asm_popmul()        {
    fputs("\tpop bx\n",destfile);
    fputs("\tmul bx\n",destfile);
}

/* divide o topo da pilha pelo reg. prim. */
void asm_popdiv()        {
    fputs("\tpop bx\n",destfile);
    fputs("\txchg ax, bx\n",destfile);
    fputs("\tcwd\n",destfile);
    fputs("\tdiv bx\n",destfile);
}

/* armazena reg. prim. em variável */
void asm_store(char *name)        {
    char str[30] =     "\tmov word ptr ";
    strcat(str, name);
    strcat(str, ", ax\n");
    fputs(str,destfile);
}

/* inverte reg. prim. */
void asm_not()        {
    fputs("\tnot ax\n",destfile);
}

/* "E" do topo da pilha com reg. prim. */
void asm_popand()        {
    fputs("\tpop bx\n",destfile);
    fputs("\tand ax, bx\n",destfile);
}                /* "OU" do topo da pilha com reg. prim. */
void asm_popor()        {
    fputs("\tpop bx\n",destfile);
    fputs("\tor ax, bx\n",destfile);
}

/* "OU-exclusivo" do topo da pilha com reg. prim. */
void asm_popxor()        {
    fputs("\tpop bx\n",destfile);
    fputs("\txor ax, bx\n",destfile);
}

/* compara topo da pilha com reg. prim. */
void asm_popcompare()        {
    fputs("\tpop bx\n",destfile);
    fputs("\tcmp bx, ax\n",destfile);
}

/* lê um valor para o registrador primário e armazena em variável */
void asm_read()        {
    fputs("\tcall READ\n",destfile);
    asm_store(value);
}

/* mostra valor do reg. primário */
void asm_write()        {
    fputs("\tcall WRITE\n",destfile);
}

/*** Função para Gerar o Título do Programa no Arquivo Alvo ***/
void beginGereration(char *title, FILE *destfile) {
    char titleCode[26] = "title ";
    strcat(titleCode, title); /*** Concatenado title com o nome do Programa ***/
    strcat(titleCode, "\t; Nome do Programa\n\n");
    fputs(titleCode, destfile);
}

/*** Função para Gerar o Cabeçalho do Código Assembly ***/
void header(FILE *destfile) {
    fputs("dosseg\n", destfile);
    fputs(".model small\n", destfile);
    fputs(".stack 100h\n\n", destfile);
    fputs(".data\n", destfile);
    fputs("\tVetor DB 0,0,0,0,0,13,10,'$'\n",destfile);
	fputs("\tTam DB 5\n",destfile);

}

/*** Função para Gerar o Cabeçalho de Variáveis (.Data) ***/
void dotData(FILE *destfile, char *var,char type[]) {
    strcat(var, type);
    fputs(var,destfile);
}

/*** Função para Gerar o Cabeçalho de Código (.Code) e o Código do Programa na Linguagem Alvo ***/
void dotCode(char *code,FILE *destfile) {
    fputs("\n.Code\n", destfile);
    fputs("main proc\n", destfile);
    fputs("\t;inicializando o registrador ds\n", destfile);
    fputs("\tmov ax,@data\t;coloca o numero do segmento de dados em ax\n", destfile);
    fputs("\tmov ds,ax\t\t;ds inicializado com valor de ax\n\n", destfile);
    fputs(code,destfile);
}

/*** Função para Gerar a Finalização da Geração de Código ***/
void endGeneration(FILE *destfile) {
    fputs("\n\tjmp fim\t;chamando funcao para finalizar a aplicacao\n", destfile);
    fputs("\nfim: mov ah,4ch\t;funcao DOS para saida\n", destfile);
    fputs("     int 21h\t;saindo", destfile);
    fputs("\nmain endp\n", destfile);

}

void dotLit(char *code,FILE *destfile) {
    fputs(code, destfile);
}
