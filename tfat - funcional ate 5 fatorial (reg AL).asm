.MODEL small
.STACK 100h
.DATA
	intro DB 'CALCULO DE FATORIAL',13,10,'$'
	text1 DB 'Insira um numero',13,10,'$'
	text2 DB 'Caractere digitado nao e digito! Tente novamente!',13,10,'$'
	text3 DB 'O resultado e:',13,10,'$'
	fator DB 0,'$'
	resultado DB 1,'$'
	BR DB 13,10,'$'
	debug DB 'eu fiz o que nao deveria pq sou fdp',13,10,'$'
	Vetor DB 0,0,0,0,0,13,10,'$'
	Tam DB 5
.CODE


;Default code
mov ax,@data
mov ds,ax

	;Mensagem de Introducao
	mov ah,9
	mov dx,OFFSET intro
	int 21h

NOVAMENTE:	
	;Insira um numero
	mov ah,9
	mov dx,OFFSET text1
	int 21h	
	;Pega valor digitado
	mov ah,1
	int 21h	
	
    call verificador ;Verificar se e digito
	cmp cl,0 ;Se == 0 Caracter nao e digito!
    je NOVAMENTE ;Nao e digito, entao digite novamente

;Iniciar FATORIAL	
	mov al,[resultado] ;al <- 1
	mov bl,[fator] ;bl <- fator
FATORIAL:
    cmp bl,0
	je FIM_FATORIAL
	mul bl ;al*bl --- resultado*fator
	dec bl ;fator--
    jmp FATORIAL 
	
FIM_FAT_ZERO:
    mov [resultado],0    	
FIM_FATORIAL:
	mov [resultado],al
	;add [resultado],30h
	mov ah,9 
	mov dx,OFFSET BR
	int 21h
	mov dx,OFFSET text3
	int 21h 
	xor ah,ah
     mov al,[Resultado]
     call int2ascii
     mov ah,9
     mov dx, OFFSET Vetor
     int 21h
	;mov dx,OFFSET resultado
	;int 21h
;Default code
mov ah,4ch
int 21h

;Funcao para verificar se valor digitado e digito
verificador PROC NEAR
	sub al,30h
	jb NAO_DIGITO
	cmp al,9
	ja NAO_DIGITO
	;Senao pulou ate aqui, quer dizer que e digito
	cmp al,0
	je FIM_FAT_ZERO 
	mov [fator],al
		
	mov cl,1
	jmp FIM_VERIFICADOR
	
NAO_DIGITO:
	mov cl,0 ;DIGITO==FALSE
	mov ah,9
	mov dx,OFFSET BR ;Quebra-Linha
	int 21h
	mov dx,OFFSET text2 ;'Caractere digitado nao e digito! Tente novamente!'
	int 21h
	
FIM_VERIFICADOR:	
	ret

ENDP



int2ascii proc near
               mov bx, 0aH ; coloca 10 em bx
INICIO_CONVERSAO:
                 div bx ; divide ax por bl --> quociente em al, resto em ah
                 call converte_e_armazena
                 cmp al,0
                 je FIM_CONVERSAO
                xor ah,ah
               jmp INICIO_CONVERSAO
FIM_CONVERSAO:
            ret
endp


converte_e_armazena proc near
        push bx
        xor ch,ch
        mov cl, [Tam]
        mov si, cx
        mov bx, OFFSET Vetor
        add ah, 30h
        mov [bx][si],ah
       dec cx
       mov [Tam],cl
       pop bx
       ret
ENDP




END