title listadevalores	; Nome do Programa

dosseg
.model small
.stack 100h

.bss
	ler resb 3 ;reserva 3 bytes para a variavel ler (que le do teclado)
.data


.code

	mov ah,9
	mov dx,OFFSET lit0
	int 21h
	mov ah,3fh
	mov bx,0
	mov cx,3
	mov dx,ler
	int 21h
	mov ax,ler
	mov [x1],ax
	int 21h
	mov ax,ler
	mov [x2],ax
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
	mov dx,OFFSET lit3
	int 21h
EndLabel4: 
;Fim do IF


	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
