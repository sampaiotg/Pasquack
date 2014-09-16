title listadevalores	; Nome do Programa

dosseg
.model small
.stack 100h

.data

	x1 dw 
	x2 dw 
	i dw 

.code

mov ah,9
mov dx,OFFSET lit0
int 21h


	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
