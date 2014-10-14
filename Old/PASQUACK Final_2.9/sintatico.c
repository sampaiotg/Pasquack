#include "lexico.h"
#include "gerador.h"

/*** Estrutura onde ficam as variaveis e seus tipos ***/
typedef struct var {
    int type;
    char *name;
    int var_pos;
    struct var *prox;
} variable;


/*** Prototipação de Funções ***/
void def_tipo_var(int ini,int fin,char *tipo);
int match(int tag);
int program();
int declaracoes();
int bloco();
int nome();
int constantes();
int variaveis();
int procedimentos();
int comandos();
int ident();
int lista_constantes();
int lista_ident();
int procedimento();
int comando();
int inteiro();
int parametros();
int lista_argumentos();
int expr_relacional();
int else_opc();
int expr();
int exprl();
int chamada_proc();
int op_relacional();
int termo();
int termol();
int fator();
int declaracao();
int uses();
int literal();
int tipo();
void gerLstVar();
void alocaVar(char *nome, int tipo);
void emit(char instruction[100]);
/*** Variaveis Globais ***/
char fileName[20];
char *programName;
FILE *file;
FILE *gerFile;
int flagDeclara = 0;
int n_var = 0;
int flagBloco = 0;
int flagName = 0;
int ignoraID = 0;
int tagfail;
int contVar = 0;
char *name_result;
variable *priVar;
struct Token * lastlook; ///Armazena o ultimo look antes de chamar novamente a função scan, utilizado para geração de código
char finalasm[10000]; ///Armazena o arquivo de saída
char instr[100]; ///armazena o comando que será inserido no arquivo de saída
int label_counter=0; ///armazena quantos labels existem no programa com o objetivo de cada vez gerar um rótulo diferente
int literal_counter=0; ///controla qual Literal será exibido
char litasm[10000];
int label_number=0; ///Indica o fluxo do loop, o label atual

/*** Função Principal ***/
int main() {
    ini_symbol_table();
    printf("Digite o nome do arquivo a ser compilado\n");
    scanf("%s",fileName);
    file = fopen(fileName,"r");
    lookahead = (struct Token*) malloc(sizeof(struct Token));
    lookahead = scan(file);
    priVar = (struct var*) malloc(sizeof(struct var));
    priVar->prox = NULL;
    //priVar->name
    contVar = 0;
    if(program()) {
        printf("----------Codigo correto!------------\n");
    } else {
        printf("**REAL: %10s[%i] | EXPECTED: %i\n",lookahead->lexeme,lookahead->tag,tagfail);
        printf("Codigo incorreto!");
    }
    geralit(name_result);
    dotLit(litasm,gerFile);
    fputs("\n.code\n\n",gerFile);
    dotCode(finalasm,gerFile);
    endGeneration(gerFile);
    fclose(gerFile);
    printTable();
    char str[1000];
    name_result = strtok(fileName, ".");
    sprintf(str,"Tasm %s.asm | exit\n",name_result);
    //system(str);
    sprintf(str,"Tlink %s.obj | exit\n",name_result);
    //system(str);
    //system("pause");
    //system("exit");
    return 0;
}

/*** Função que confere um programa ***/
int program() {
    if(declaracoes()) {
        if(bloco()) {
            if(match(EOF)) {
                return 1;
            } else return 0;
        } else return 0;
    } else return 0;
}

/*** Função que confere a parte de declarações ***/
int declaracoes() {
    if(nome()) {
        if(uses()) {
            if(constantes()) {
                if(variaveis()) {
                    if(procedimentos()) {
                        return 1;
                    } else return 0;
                } else return 0;
            } else return 0;
        } else return 0;
    } else return 0;
}

/*** Função que o uso de bibliotecas ***/
int uses() {
    if(match(USES)) {
        if(ident()) {
            if(match(PONTOVIRGULA)) {
                return 1;
            } else return 0;
        } else return 0;
    }
    return 1;
}

/*** Função que confere um bloco ***/
int bloco() {
	flagBloco = 1;
    if(match(BEGIN)) {
        if(comandos()) {
            if(match(END)) {
				flagBloco = 0;
                return 1;
            } else return 0;
        } else return 0;
    } else return 0;
}

/*** Função que confere o nome do programa ***/
int nome() {
    if(match(PROGRAM)) {
        programName = lookahead->lexeme;
        flagName = 1;
        if(ident()) {
            flagName = 0;
            if(match(PONTOVIRGULA)) {
                name_result = strtok(fileName, ".");
                strcat(name_result,".asm");
                gerFile = fopen(name_result, "wb+");
                beginGereration(programName,gerFile);
                header(gerFile);
                return 1;
            } else return 0;
        } else return 0;
    } else return 1;
}

/*** Função que confere declaração de constantes ***/
int constantes() {
    if(match(CONSTANTE)) {
        if(lista_constantes()) {
            return 1;
        } else return 0;
    } else return 1;
}

/*** Função que confere declaração de variaveis ***/
int variaveis() {
    int var_ini,var_fin;
    char *tipo_v;
    while(match(VAR)) {
        flagDeclara = 1;
        var_ini = n_var;
        if(lista_ident()) {
            var_fin = n_var;
            flagDeclara = 0;
            if(match(DOISPONTOS)) {
                tipo_v = lookahead->lexeme;
                def_tipo_var(var_ini,var_fin,tipo_v);
                if(tipo()) {
                    if(match(PONTOVIRGULA)) {
                        continue;
                    } else return 0;
                } else return 0;
            } else if(match(PONTOVIRGULA)) {
                return 1;
            } else return 0;
        } else return 0;
    }
    gerLstVar();
    return 1;
}

/*** Função que confere o tipo da variavel ***/
int tipo() {
    if(match(INTEGER)||match(STRING)   ||match(REAL)||match(BOOLEAN)||match(CHAR)  ||
            match(LONGINT)||match(SHORTINT) ||match(BYTE)||match(WORD)   ||match(SINGLE)||
            match(DOUBLE) ||match(EXTENDED) ||match(COMP)) {
        return 1;
    } else return 0;
}

/*** Função que confere um procedimento ***/
int procedimentos() {
    if(procedimento()) {
        if(match(PONTOVIRGULA)) {
            if(procedimentos()) {
                return 1;
            } else return 0;
        } else return 0;
    } else return 1;
}

/*** Função que confere os comandos ***/
int comandos() {
    if(comando()) {
        if(match(PONTOVIRGULA)) {
            if(comandos()) {
                return 1;
            } else return 0;
        } else return 1;
    } else return 1;
}

/*** Função que confere um identificador ***/
int ident() {
    if(lookahead->lexeme == programName && flagName != 1) {
        printf("erro, nome do programa não pode ser usado como Variavel");
        exit(0);
    } else if((!(exist_var(lookahead->lexeme))) && (flagBloco == 1)) {
        return 0;
    } else if(match(ID)) {
        return 1;
    } else return 0;
}

/*** Função que confere a lista de constantes ***/
int lista_constantes() {
    if(ident()) {
        if(match(EQUAL)) {
            if(inteiro()) {
                if(match(VIRGULA)) {
                    if(lista_constantes()) {
                        return 1;
                    } else return 0;
                } else return 1;
            } else return 0;
        } else return 0;
    } else return 0;
}

/*** Função que confere a lista de Identificadores ***/
int lista_ident() {
    char *nome;
    if(flagDeclara == 1) {
        nome = lookahead->lexeme;
        alocaVar(nome,0);
        n_var ++;
    }
    if(ident()) {
        if(match(VIRGULA)) {
            if(lista_ident()) {
                return 1;
            } else if(lookahead->tag==ASPAS) {
                return 1;
            } else return 0;
        } else return 1;
    } else if(lookahead->tag==ASPAS) {
        return 1;
    } else return 0;
}

/*** Função que confere um procedimento ***/
int procedimento() {
    if(match(PROCEDURE)) {
        if(ident()) {
            if(parametros()) {
                if(declaracao()) {
                    if(bloco()) {
                        return 1;
                    } else return 0;
                } else return 0;
            } else return 0;
        } else return 0;
    } else return 0;
}

/*** Função que confere um comando ***/
int comando() {
    if(match(PRINT)) {
        emit("\tmov ah,9\n");
        sprintf(instr,"\tmov dx,OFFSET lit%d\n",literal_counter);
        emit(instr);
        emit("\tint 21h\n");
        literal_counter++;
        if(match(LPARENT)) {
            if(lista_argumentos()) {
                if(match(RPARENT)) {

                    return 1;
                } else return 0;
            } else return 0;
        } else return 0;
    } else if(match(IF)) {
        emit(";Inicio do IF\n");
        label_number=label_counter; ///Indica o numero do label atual
        sprintf(instr,"Label%d: \n",label_counter);
        label_counter++;
        emit(instr);
        if(match(NOT)) return 1;
        if(expr_relacional()) {
            sprintf(instr,"%d\n",label_number);
            emit(instr);
            if(match(OR)||match(AND)) return 1;
            if(match(THEN)) {
                if(match(BEGIN)) {
                    comandos();
                    if(match(END)) {
                        if(else_opc()) {
                            emit(";Fim do ELSE\n");
                            return 1;
                        } else if(match(PONTOVIRGULA)) {
                            sprintf(instr,"EndLabel%d: \n",label_number);
                            emit(instr);
                            emit(";Fim do IF\n");
                            return 1;
                        } else return 0;
                    } else return 0;
                } else if(comando()) {
                    sprintf(instr,"EndLabel%d: \n",label_number);
                    emit(instr);
                    emit(";Fim do IF\n");
                    return 1;
                } else return 0;
            } else return 0;
        } else if(match(LPARENT)) {
            if(expr_relacional()) {
                if(match(RPARENT)) {
                    sprintf(instr,"%d\n",label_number);
                    emit(instr);
                    if(match(OR)||match(AND)) return 1;
                    if(match(THEN)) {
                        if(match(BEGIN)) {
                            comandos();
                            if(match(END)) {
                                if(else_opc()) {
                                    emit(";Fim do ELSE\n");
                                    return 1;
                                } else if(match(PONTOVIRGULA)) {
                                    sprintf(instr,"EndLabel%d: \n",label_number);
                                    emit(instr);
                                    emit(";Fim do IF\n");
                                    return 1;
                                } else return 0;
                            } else return 0;
                        } else if(comando()) {
                            sprintf(instr,"EndLabel%d: \n",label_number);
                            emit(instr);
                            emit(";Fim do IF\n");
                            return 1;
                        } else return 0;
                    } else return 0;
                } else return 0;
            } else return 0;
        } else return 0;
    } else if(ident()) {
        char id[20];
        strcpy(id,lastlook->lexeme);

        if(match(ASSIGN)) {
            if(expr()) {
                if(match(PONTOVIRGULA)) {

                    sprintf(instr,"\tmov [%s],ax\n",id);
                    emit(instr);
                    //system("pause");
                    if(comando()) {
                        if(comandos()) return 1;
                        return 1;
                    } else return 1;
                } else return 0;
            } else if(match(NUM)) {
                return 1;
            } else return 0;
        } else return 0;
    } else if(match(FOR)) {
        emit(";Inicio do FOR\n");
        label_number=label_counter; ///Indica o numero do label atual
        sprintf(instr,"Label%d: \n",label_counter);
        label_counter++;
        emit(instr);
        if(ident()) {
            char id[20];
            strcpy(id,lastlook->lexeme);
            if(match(ASSIGN)) {
                if(expr()) {
                    sprintf(instr,"\tmov [%s],ax\n",id);
                    emit(instr);
                    if(match(TO)) {
                        if(expr()) {
                            sprintf(instr,"\tcmp [%s],ax\n",id);
                            emit(instr);
                            sprintf(instr,"\tja EndLabel%d\n",label_number);
                            emit(instr);
                            if(match(DO)) {
                                if(match(BEGIN)) {
                                    if(comandos()) {
                                        if(match(END)) {
                                            if(match(PONTOVIRGULA)) {
                                                sprintf(instr,"EndLabel%d:\n",label_number);
                                                emit(instr);
                                                emit(";Fim do FOR\n");
                                                return 1;
                                            } else return 0;
                                        } else return 0;
                                    } else return 0;
                                } else if(comando()) {
                                    sprintf(instr,"EndLabel%d:\n",label_number);
                                    emit(instr);
                                    emit(";Fim do FOR\n");
                                    return 1;
                                } else return 0;
                            } else return 0;
                        } else return 0;
                    } else if(match(DOWNTO)) {
                        if(expr()) {
                            sprintf(instr,"\tcmp [%s],ax\n",id);
                            emit(instr);
                            sprintf(instr,"\tjb EndLabel%d\n",label_number);
                            emit(instr);
                            if(match(DO)) {
                                if(match(BEGIN)) {
                                    if(comandos()) {
                                        if(match(END)) {
                                            if(match(PONTOVIRGULA)) {
                                                sprintf(instr,"EndLabel%d:\n",label_number);
                                                emit(instr);
                                                emit(";Fim do FOR\n");
                                                return 1;
                                            } else return 0;
                                        } else return 0;
                                    } else return 0;
                                } else if(comando()) {
                                    sprintf(instr,"EndLabel%d:\n",label_number);
                                    emit(instr);
                                    emit(";Fim do FOR\n");
                                    return 1;
                                } else return 0;
                            } else return 0;
                        } else return 0;
                    }
                } else return 0;
            } else return 0;
        } else return 0;
    } else if(chamada_proc()) {
        return 1;
    } else if(bloco()) {
        return 1;
    } else if(match(WRITELN)||match(WRITE)) {
        emit("\tmov ah,9\n");
        if(match(LPARENT)) {
            if(literal()) {
                sprintf(instr,"\tmov dx,OFFSET lit%d\n",literal_counter);
                emit(instr);
                emit("\tint 21h\n");
                literal_counter++;
                while(match(VIRGULA)) {
                    if(expr()) {
                        emit("\tmov dx,ax\n");
                        emit("\tint 21h\n");
                        continue;
                    } else if(literal()) {
                        sprintf(instr,"\tmov dx,OFFSET lit%d\n",literal_counter);
                        emit(instr);
                        //emit("\tint 21h\n");
                        literal_counter++;
                        continue;
                    } else if(expr()) {
                        emit("\tmov dx,ax\n");
                        emit("\tint 21h\n");
                        continue;
                        return 1;
                    } else {
                        return 0;
                    }
                }
                if(match(RPARENT)) {
                    return 1;
                } else return 0;
            } else if(ident()) {
                emit("\tmov dx,ax\n");
                emit("\tint 21h\n");
                while(match(VIRGULA)) {
                    if(ident()) {
                        emit("\tmov dx,ax\n");
                        //emit("\tint 21h\n");
                        continue;
                    } else if(literal()) {
                        sprintf(instr,"\tmov dx,OFFSET lit%d\n",literal_counter);
                       // emit("\tint 21h\n");
                        literal_counter++;
                        continue;
                    } else if(expr()) {
                        emit("\tmov dx,ax\n");
                        emit("\tint 21h\n");
                        return 1;
                    } else {
                        return 0;
                    }
                }
                if(match(RPARENT)) {
                    return 1;
                } else return 0;
            } else if(match(RPARENT)) {
                return 1;
            } else return 0;
        } else return 0;
    } else if(match(READLN)) {

        emit("\tmov ah,3fh\n");
        emit("\tmov bx,0\n");
        emit("\tmov cx,3\n");
        emit("\tmov dx,07h\n");
        if(match(LPARENT)) {
            if(ident()) {
                char id[20];
                strcpy(id,lastlook->lexeme);
                if(match(VIRGULA)) {
                    emit("\tint 21h\n");
                    emit("\tmov ax,07h\n");
                    sprintf(instr,"\tmov [%s],ax\n",id);
                    emit(instr);
                    if(lista_ident()) {
                            strcpy(id,lastlook->lexeme);
                        if(match(RPARENT)) {
                            emit("\tint 21h\n");
                            emit("\tmov ax,07h\n");
                            sprintf(instr,"\tmov [%s],ax\n",id);
                            emit(instr);
                            return 1;
                        } else return 0;
                    } else return 0;
                } else if(match(RPARENT)) {
                    emit("\tint 21h\n");
                    emit("\tmov ax,ler\n");
                    sprintf(instr,"\tmov [%s],ax\n",id);
                    emit(instr);
                    return 1;
                } else return 0;
            } else return 0;
        } else return 0;

    } else if(match(BREAK)) {
        if(match(PONTOVIRGULA)) {
            sprintf(instr,"\tjmp EndLabel%d",label_number);
            emit(instr);
            emit(" ;break\n");
            return 1;
        } else return 0;
    }else if(match(READKEY)){
        if(match(PONTOVIRGULA)){
            return 1;
        }return 0;
    }else if(match(WHILE)) {
        emit(";Inicio do WHILE\n");
        label_number=label_counter; ///Indica o numero do label atual
        sprintf(instr,"Label%d: \n",label_counter);
        label_counter++;
        emit(instr);
        if(match(LPARENT)) {
            if(expr_relacional()) {
                sprintf(instr,"%d\n",label_number);
                emit(instr);
                if(match(RPARENT)) {
                    if(match(DO)) {
                        if(match(BEGIN)) {
                            comandos();
                            if(match(END)) {
                                if(match(PONTOVIRGULA)) {
                                    sprintf(instr,"\tjmp Label%d\n",label_number);
                                    emit(instr);
                                    sprintf(instr,"EndLabel%d: \n",label_number);
                                    emit(instr);
                                    emit(";Fim do While\n");
                                    //system("pause");
                                    return 1;
                                } else return 0;
                            } else return 0;
                        } else if(comando()) {
                            comandos();
                            sprintf(instr,"\tjmp Label%d\n",label_number);
                            emit(instr);
                            sprintf(instr,"EndLabel%d: \n",label_number);
                            emit(instr);
                            emit(";Fim do While\n");
                            return 1;
                        } else return 0;
                    } else return 0;
                } else return 0;
            } else return 0;
        } else if(expr_relacional()) {
            if(expr_relacional()) {
                sprintf(instr,"%d\n",label_number);
                emit(instr);
                if(match(RPARENT)) {
                    if(match(DO)) {
                        if(match(BEGIN)) {
                            comandos();
                            if(match(END)) {

                                if(match(PONTOVIRGULA)) {
                                    sprintf(instr,"\tjmp Label%d\n",label_number);
                                    emit(instr);
                                    sprintf(instr,"EndLabel%d: \n",label_number);
                                    emit(instr);
                                    emit(";Fim do While\n");
                                    //system("pause");
                                    return 1;
                                } else return 0;
                            } else return 0;
                        } else if(comando()) {
                            comandos();
                            sprintf(instr,"\tjmp Label%d\n",label_number);
                            emit(instr);
                            sprintf(instr,"EndLabel%d: \n",label_number);
                            emit(instr);
                            emit(";Fim do While\n");
                            return 1;
                        } else return 0;
                    } else return 0;
                } else return 0;
            } else return 0;
        } else return 0;
    } else return 1;
}

/*** Função que confere um literal ***/
int literal() {
    if(match(ASPAS)) {
        if(match(LITERAL)) {
            if(match(ASPAS)) {
                return 1;
            } else return 0;
        } else return 0;
    } else if(match(283)) {
        return 1;
    } else {
        return 0;
    }
}

/*** Função que confere um inteiro ***/
int inteiro() {
    if(match(NUM)) {
        return 1;
    } else {
        return 0;
    }
}

/*** Função que confere um parametro ***/
int parametros() {
    if(match((LPARENT))) {
        if(lista_ident()) {
            if(match(RPARENT)) {
                return 1;
            } else return 0;
        } else return 0;
    } else return 1;
}

/*** Função que confere a lista de argumentos ***/
int lista_argumentos() {
    if(expr()) {
        if(match(VIRGULA)) {
            if(lista_argumentos()) {
                return 1;
            }
        } else return 1;
    }
    return 0;
}

/*** Função que confere uma expressao relacional ***/
int expr_relacional() {
    if(expr()) {
        emit("\tmov bx,ax\n");
        if(match(EQUAL)) {
            if(expr()) {
                emit("\tcmp bx,ax\n");
                emit("\tjne EndLabel");
                return 1;
            } else return 0;
        } else if(match(NEQUAL)) {
            if(expr()) {
                emit("\tcmp bx,ax\n");
                emit("\tje EndLabel");
                return 1;
            } else return 0;
        } else if(match(LEQUAL)) {
            if(expr()) {
                emit("\tcmp bx,ax\n");
                emit("\tja EndLabel");
                return 1;
            } else return 0;
        } else if(match(GEQUAL)) {
            if(expr()) {
                emit("\tcmp bx,ax\n");
                emit("\tjb EndLabel");
                return 1;
            } else return 0;
        } else if(match(LTHAN)) {
            if(expr()) {
                emit("\tcmp bx,ax\n");
                emit("\tjae EndLabel");
                return 1;
            } else return 0;
        } else if(match(GTHAN)) {
            if(expr()) {
                emit("\tcmp bx,ax\n");
                emit("\tjbe EndLabel");
                return 1;
            } else return 0;
        } else return 0;
    } else return 0;
}

/*** Função que confere o comando else ***/
int else_opc() {
    emit(";Fim do IF\n");
    emit(";Inicio do ELSE\n");
    sprintf(instr,"EndLabel%d: \n",label_counter-1);
    emit(instr);
    if(match(ELSE)) {
        if(match(BEGIN)) {
            comandos();
            if(match(END)) {
                if(match(PONTOVIRGULA)) {
                    return 1;
                } else return 0;
            } else return 0;
        } else if(comando()) {
            return 1;
        } else return 0;
    } else return 0;
}

/*** Função que confere uma expressao ***/
int expr() {
    if(termo()) {
        emit("\tpush bx\n");
        if(exprl()) {
            return 1;
        } else {
            return 1;
        }
    } else return 0;
}

/*** Função que confere a expressao linha(usado pra remover recursão a esquerda) ***/
int exprl() {
    if(match(PLUS)) {
        if(termo()) {

            if(exprl()) {
                emit("\tadd ax,bx\n");
                return 1;
            } else return 0;
        } else return 0;
    } else if(match(MINUS)) {
        if(termo()) {

            if(exprl()) {
                emit("\tsub ax,bx\n");
                return 1;
            } else return 0;
        } else return 0;
    } else {
        emit("\tpop ax\n");
        return 1;
    }
}

/*** Função que confere uma chamada de procedimentos ***/
int chamada_proc() {
    //printf("CHAMADA_PROC\n");
    if(ident()) {
        if(lista_argumentos()) {
            return 1;
        } else return 1;
    } else return 0;
}

/*** Função que confere um operador relacional ***/
int op_relacional() {
    if(match(EQUAL)) {
        return 1;
    } else if(match(NEQUAL)) {
        return 1;
    } else if(match(LEQUAL)) {
        return 1;
    } else if(match(GEQUAL)) {
        return 1;
    } else if(match(LTHAN)) {
        return 1;
    } else if(match(GTHAN)) {
        return 1;
    } else return 0;
}

/*** Função que confere um termo ***/
int termo() {
    if(fator()) {
        emit("\tmov bx,ax\n");
        if(termol()) {
            return 1;
        } else return 0;
    } else return 0;
}

/*** Função que confere um termo linha(usado pra remover recursão a esquerda) ***/
int termol() {
    if(match(MULTI)) {
        emit("\tpush bx\n");
        if(fator()) {
            if(termol()) {
                emit("\tpop bx\n");
                emit("\tmul ax\n");
                emit("\tmov bx,ax\n");
                return 1;
            } else return 1;
        } else return 0;
    } else if(match(DIVIDE)||match(DIV)||match(MOD)) {
        emit("\tpush bx\n");
        if(fator()) {

            if(termol()) {
                emit("\tpop bx\n");
                emit("\tdiv ax\n");
                emit("\tmov bx,ax\n");

                return 1;
            } else return 1;
        } else return 0;
    } else {
        return 1;
    }
}

/*** Função que confere um fator ***/
int fator() {
    if(match(LPARENT)) {
        if(expr()) {
            if(match(RPARENT)) {
                return 1;
            } else return 0;
        }
    } else if(ident()) {
        sprintf(instr,"\tmov ax,[%s]\n",lastlook->lexeme);
        emit(instr);
        return 1;
    } else if(inteiro()) {
        sprintf(instr,"\tmov ax,%d\n",lastlook->value);
        emit(instr);
        return 1;
    }
    return 0;
}

/*** Função que confere uma declaração ***/
int declaracao() {
    if(constantes()) {
        if(variaveis()) {
            return 1;
        } else return 0;
    }
    return 0;
}

/*** Função de comparação do token recebido pelo lexo com o token possivel ***/
int match(int tag) {
    //printf("**REAL: %10s[%i] | EXPECTED: %i\n",lookahead->lexeme,lookahead->tag,tag);
    if(tag==lookahead->tag) {
        lastlook=lookahead;
        printf("Encontrado %s\n",lookahead->lexeme);
        lookahead = (struct Token*) malloc(sizeof(struct Token));
        lookahead = scan(file);
        return 1;
    } else {
        //printf("Lexema nao encontrado!\n");
        tagfail = tag;
        return 0;
    }
}

/*** Função que chama o gerador para gerar o codigo das variaveis ***/
void gerLstVar() {
    variable *aux;
    aux = priVar;
    do {
        char saux[100];
        strcpy (saux,"\t");
        strcat(saux,aux->name);
        if(aux->type == 1) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 2) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 3) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 4) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 5) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 6) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 7) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 8) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 9) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 10) {
            dotData(gerFile,saux," DW 0,'$'\n");
        } else if(aux->type == 11) {
            dotData(gerFile,saux," DW0,'$'\n");
        }
        free(saux);
        aux = aux->prox;
    } while(aux != NULL);
}

/*** Função que aloca e preenche as variavies ***/
void alocaVar(char *nome, int tipo) {
    variable *aux,*novo;
    aux = priVar;
    if(contVar == 0) {
        priVar->name = nome;
        priVar->type = tipo;
        priVar->var_pos = n_var;
        contVar++;
    } else {
        while(aux->prox!=NULL) {
            aux=aux->prox;
        }
        novo = (struct var*) malloc(sizeof(struct var));
        novo->name = nome;
        novo->type = tipo;
        novo->prox = NULL;
        novo->var_pos = n_var;
        aux->prox = novo;
        contVar++;
    }
}
void def_tipo_var(int ini,int fin,char *tipo) {
    variable *aux = priVar;
    int tipo_aux;
    if(!strcmp(tipo,"shortint")) {
        tipo_aux = 1;
    } else if(!strcmp(tipo,"integer")) {
        tipo_aux = 2;
    } else if(!strcmp(tipo,"longint")) {
        tipo_aux = 3;
    } else if(!strcmp(tipo,"real")) {
        tipo_aux = 4;
    } else if(!strcmp(tipo,"char")) {
        tipo_aux = 5;
    } else if(!strcmp(tipo,"byte")) {
        tipo_aux = 6;
    } else if(!strcmp(tipo,"word")) {
        tipo_aux = 7;
    } else if(!strcmp(tipo,"single")) {
        tipo_aux = 8;
    } else if(!strcmp(tipo,"double")) {
        tipo_aux = 9;
    } else if(!strcmp(tipo,"extended")) {
        tipo_aux = 10;
    } else if(!strcmp(tipo,"comp")) {
        tipo_aux = 11;
    }
    while(aux->var_pos < ini) {
        aux = aux->prox;
    }
    while(aux!=NULL) {
        aux->type =tipo_aux;
        aux = aux->prox;
    }
}

int exist_var(char *var_nome) {
    variable *aux = priVar;
    if(var_nome == programName) {
        return 1;
    }
    do {
        if(var_nome == aux->name) {
            return 1;
        }
        aux = aux->prox;
    } while(aux!=NULL);
    return 0;
}

void emit(char instruction[100]) {
    //printf(instruction);
    strcat(finalasm,instruction);
}

void emit_lit(char instruction[100]) {
    //printf(instruction);
    strcat(litasm,instruction);
}
