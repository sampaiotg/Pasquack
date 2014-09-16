title calnumero	; Nome do Programa

dosseg
.model small
.stack 100h

.bss
	ler resb 3 ;reserva 3 bytes para a variavel ler (que le do teclado)
.data

	x dw 
	i dw 
	y dw 
	q dd 
	r dd 
	u dd 
	z dd 
	w dd 

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
	mov [x],ax
	int 21h
	mov ax,ler
	mov [y],ax
	mov ax,2
	mov bx,ax
	push bx
	pop ax
	mov bx,ax
	push bx
	pop ax
	mov [x],ax
	mov ax,1
	mov bx,ax
	push bx
	pop ax
	mov [i],ax
	mov ax,2
	mov bx,ax
	push bx
	pop ax
	mov [j],ax
;Inicio do WHILE
Label0: 
	mov ax,[i]
	mov bx,ax
	push bx
	pop ax
	mov bx,ax
	mov ax,10
	mov bx,ax
	push bx
	pop ax
	cmp bx,ax
	jae EndLabel0
	mov ax,[i]
	mov bx,ax
	push bx
	mov ax,1
	mov bx,ax
	pop ax
	add ax,bx
	mov [i],ax
	jmp Label0
EndLabel0: 
;Fim do While
;Inicio do IF
Label1: 
	mov ax,[j]
	mov bx,ax
	push bx
	pop ax
	mov bx,ax
	mov ax,5
	mov bx,ax
	push bx
	pop ax
	cmp bx,ax
	jae EndLabel1
	mov ax,3
	mov bx,ax
	push bx
	pop ax
	mov [j],ax
;Fim do IF
;Inicio do ELSE
EndLabel1: 
	mov ax,5
	mov bx,ax
	push bx
	mov ax,4
	mov bx,ax
	push bx
	mov ax,3
	mov bx,ax
	push bx
	mov ax,4
	pop bx
	mul ax,bx
	mov bx,ax
	push bx
	pop ax
	mov bx,ax
	pop ax
	add ax,bx
	pop bx
	mul ax,bx
	mov bx,ax
	push bx
	mov ax,1
	mov bx,ax
	pop ax
	sub ax,bx
	mov [j],ax
;Fim do ELSE
	mov ah,9
	mov dx,OFFSET lit1
	int 21h
	mov ax,[x]
	mov bx,ax
	push bx
	mov ax,[y]
	mov bx,ax
	pop ax
	add ax,bx
	mov dx,ax
	int 21h
	mov ah,9
	mov dx,OFFSET lit2
	int 21h
	mov ax,[x]
	mov bx,ax
	push bx
	mov ax,[y]
	pop bx
	mul ax,bx
	mov bx,ax
	push bx
	pop ax
	mov dx,ax
	int 21h
	mov ah,9
	mov dx,OFFSET lit3
	int 21h
	mov ax,[x]
	mov bx,ax
	push bx
	mov ax,[x]
	pop bx
	mul ax,bx
	mov bx,ax
	push bx
	pop ax
	mov dx,ax
	int 21h
	mov ah,9
	mov dx,OFFSET lit4
	int 21h
	mov ax,[x]
	mov bx,ax
	push bx
	mov ax,[y]
	mov bx,ax
	pop ax
	sub ax,bx
	mov dx,ax
	int 21h
	mov ah,9
	mov dx,OFFSET lit5
	int 21h
	mov ax,[x]
	mov bx,ax
	push bx
	pop ax
	mov dx,ax
	int 21h


	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
