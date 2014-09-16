title listadevalores	; Nome do Programa

.model small
.stack 100h

.data

	x1 DW 0,'$'
	x2 DW 0,'$'
	i DW 0,'$'
	lit0 DB 'Entre com dois valores x1 e x2 ',13,10,'$'
	lit1 DB 'funciona ',13,10,'$'
	lit2 DB 'Loool ',13,10,'$'
	lit3 DB 'eita ',13,10,'$'
	lit4 DB 'Numeros Iguais ',13,10,'$'

.code

STARTPOINT:
	mov ah,9
	mov dx,OFFSET lit0
	int 21h
	mov ah,3fh
	mov bx,0
	mov cx,3
	mov dx,07h
	int 21h
	mov ax,07h
	mov [x1],ax
	int 21h
	mov ax,07h
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


	mov ax,4C00h
	int 21h
end STARTPOINT
