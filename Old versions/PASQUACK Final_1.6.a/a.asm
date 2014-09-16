title calnumero	; Nome do Programa

dosseg
.model small
.stack 100h

.data



	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
