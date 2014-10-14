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


	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
