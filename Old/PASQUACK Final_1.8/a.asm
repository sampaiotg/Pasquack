title calnumero	; Nome do Programa

dosseg
.model small
.stack 100h

.data

	x dw 
	i dw 
	y dw 
	q dd 
	r dd 
	u dd 
	z dd 
	w dd 


	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
