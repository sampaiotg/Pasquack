title listadevalores	; Nome do Programa

dosseg
.model small
.stack 100h

.data

	x1 dw 
	x2 dw 
	i dw 


	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
