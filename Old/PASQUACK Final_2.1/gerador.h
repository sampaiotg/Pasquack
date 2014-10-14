#define SYMTBL_SZ 1000
#define KWLIST_SZ 11
#define MAXTOKEN 16
/*
void asm_clear();
void asm_negative();
void asm_loadconst(int i);
void asm_loadvar(char *name);
void asm_push();
void asm_popadd();
void asm_popsub();
void asm_popmul();
void asm_popdiv();
void asm_store(char *name);
void asm_not();
void asm_popand();
void asm_popor();
void asm_popxor();
void asm_popcompare();
void asm_relop(int tag);
void asm_jmp(int label);
void asm_jmpfalse(int label);
void asm_read();
void asm_write();
void header();
void beginGereration(char *filename, FILE *destfile);
void endGeneration();
*/





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
/* carrega uma constante numérica no reg. prim. */
/*
void asm_loadconst(int i)        {
    char str[20] = "\tmov ax, ";
    char *num;
    strcat(str, itoa(i, num, 10 ));
    strcat(str, "\n");
    fputs(str,destfile);
}*/
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
/* altera reg. primário (e flags, indiretamente) conforme a comparação */
/*
void asm_relop(int tag)        {
    char *jump;
    int l1, l2;
    l1 = newlabel();
    l2 = newlabel();
    if (tag == EQUAL) jump = "je";
    if (tag == NEQUAL) jump = "jne";
    if (tag == LTHAN) jump = "jl";
    if (tag == LEQUAL) jump = "jle";
    if (tag == GEQUAL) jump = "jge";
    if (tag == GTHAN) jump = "jg";
    char str[200] = "\t";
    char num[10];
    strcat(str, jump);
    strcat(str, " L");
    strcat(str, itoa(l1, num, 10));
    strcat(str, "\n");
    strcat(str, "\txor ax, ax\n");
    strcat(str, "\tjmp L");
    strcat(str, itoa(l1, num, 10));
    strcat(str, ":\n");
    strcat(str,   "\tmov ax, -1\nL");
    strcat(str, itoa(l2, num, 10));
    strcat(str, ":\n");
    fputs(str, destfile);
}*/
/* desvio incondicional *//*
void asm_jmp(int label)        {
    char str[20] = "\tjmp L";
    char num[10];
    strcat(str, itoa(label, num, 10 ));
    strcat(str, "\n");
    fputs(str, destfile);
}*/
/* desvio se falso (0) *//*
void asm_jmpfalse(int label)        {
    char str[20] = "\tjz L";
    char num[10];
    strcat(str, itoa(label, num, 10 ));
    strcat(str, "\n");
    fputs(str, destfile);

}
 lê um valor para o registrador primário e armazena em variável */
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
    fputs(".bss\n",destfile);
    fputs("ler resb 3 ;reserva 3 bytes para a variavel ler (que le do teclado)\n",destfile);
    fputs(".data\n\n", destfile);
}

void dotData(FILE *destfile, char *var,char type[]) {
    strcat(var, type);
    fputs(var,destfile);
}

/* epílogo da rotina principal */
void endGeneration(FILE *destfile)        {
    fputs("\n\n\tmov ax,4C00h\n", destfile);
    fputs("\tint 21h\n", destfile);
    fputs("PROG ends\n", destfile);
    fputs("\tend MAIN\n", destfile);
    fclose(destfile);
}
