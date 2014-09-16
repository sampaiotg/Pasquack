title listadevalores	; Nome do Programa

.model small
.stack 100h

.data
	lit0 db 'viado',10,13,'$'
	teste dw '',10,13,'$'
.code

STARTPOINT:
	mov ah,9
	mov dx,OFFSET lit0
	int 21h
	
	mov AH,0Ah
	mov DX,offset teste
	int 21h
	
	mov ah,9
	mov dx,OFFSET teste
	int 21h


	mov ax,4C00h
	int 21h
end STARTPOINT
