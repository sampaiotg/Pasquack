title listadevalores	; Nome do Programa

dosseg
.model small
.stack 100h

.data
	Vetor DB 0,0,0,0,0,13,10,'$'
	Tam DB 5
	x1 DB 0,'$'
	x2 DB 0,'$'
	i DB 0,'$'
	lit0 DB 'Entre com dois valores x1 e x2 ',13,10,'$'
	lit1 DB 'funciona ',13,10,'$'
	lit2 DB 'Loool ',13,10,'$'
	lit3 DB 'eita ',13,10,'$'
	lit4 DB 'Numeros Iguais ',13,10,'$'

.Code
main proc
	;inicializando o registrador ds
	mov ax,@data	;coloca o numero do segmento de dados em ax
	mov ds,ax		;ds inicializado com valor de ax

	mov ah,9
	mov dx,OFFSET lit0
	int 21h
	push ax
	push bx
	push cx
	call Le_prox
	mov [x1],cx
	pop cx
	pop bx
	pop ax
	push ax
	push bx
	push cx
	call Le_prox
	mov [x2],cx
	pop cx
	pop bx
	pop ax
;Inicio do IF
Label0: 
	mov ax,[x1]
	mov bx,ax
	push bx
	pop ax
	mov bx,ax
	mov ax,[x2]
	mov bx,ax
	push bx
	pop ax
	cmp bx,ax
	jae EndLabel0
;Inicio do FOR
Label1: 
	mov ax,[x1]
	mov bx,ax
	push bx
	pop ax
	mov [i],ax
	mov ax,[x2]
	mov bx,ax
	push bx
	pop ax
	cmp [i],ax
	ja EndLabel1
	mov ah,9
	mov dx,ax
	int 21h
EndLabel1:
;Fim do FOR
EndLabel0: 
;Fim do IF
	mov ah,9
	mov dx,OFFSET lit2
	int 21h
;Inicio do IF
Label2: 
	mov ax,[x1]
	mov bx,ax
	push bx
	pop ax
	mov bx,ax
	mov ax,[x2]
	mov bx,ax
	push bx
	pop ax
	cmp bx,ax
	jae EndLabel2
;Inicio do FOR
Label3: 
	mov ax,[x2]
	mov bx,ax
	push bx
	pop ax
	mov [i],ax
	mov ax,[x1]
	mov bx,ax
	push bx
	pop ax
	cmp [i],ax
	jb EndLabel3
	mov ah,9
	mov dx,ax
	int 21h
EndLabel3:
;Fim do FOR
EndLabel2: 
;Fim do IF
;Inicio do IF
Label4: 
	mov ax,[x1]
	mov bx,ax
	push bx
	pop ax
	mov bx,ax
	mov ax,[x2]
	mov bx,ax
	push bx
	pop ax
	cmp bx,ax
	jne EndLabel4
	mov ah,9
	mov dx,OFFSET lit4
	int 21h
EndLabel4: 
;Fim do IF

	jmp fim	;chamando funcao para finalizar a aplicacao

fim: mov ah,4ch	;funcao DOS para saida
     int 21h	;saindo
main endp
Le_prox proc near
Inicio:
	mov ah,01h
	int 21h
	cmp al,0dh
	je fim_proc
	jmp Verifica
Numero:
	push ax
	mov bx,00ah
	mov ax,cx
	mul bx ;ax  = ax*bx
	mov bx,ax
	pop ax
	add bx,ax
	mov cx,bx
	jmp Inicio
Verifica:
	sub al,30h
	cmp al,9
	ja fim
	cmp al,0
	jb fim
	jmp Numero
fim_proc:
	ret
Le_prox endp
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
int2ascii endp
converte_e_armazena proc near
	push bx
	push ax
	push cx
	xor ch,ch
	mov cl, [Tam]
	mov si, cx
	mov bx, OFFSET Vetor
	add ah, 30h
	mov [bx][si],ah
	dec cx
	mov [Tam],cl
	pop cx
	pop ax
	pop bx
	ret
converte_e_armazena ENDP
end main
