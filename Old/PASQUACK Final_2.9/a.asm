title calnumero	; Nome do Programa

.model small
.stack 100h

.data

	x DW 0,'$'
	i DW 0,'$'
	j DW 0,'$'
	y DW 0,'$'
	q DW 0,'$'
	r DW 0,'$'
	u DW 0,'$'
	z DW 0,'$'
	w DW 0,'$'
	lit0 DB 'Entre com a porra dos dois numeros X e Y sua anta ',13,10,'$'
	lit1 DB 'X + Y = ',13,10,'$'
	lit2 DB 'X * Y^2= ',13,10,'$'
	lit3 DB 'X^2 = ',13,10,'$'
	lit4 DB 'Seno de (X - Y)= ',13,10,'$'
	lit5 DB 'Valor Absoluto de X= ',13,10,'$'

.code

STARTPOINT:
	;inicializando o registrador ds
	mov ax,@data	;coloca o numero do segmento de dados em ax
	mov ds,ax		;ds inicializado com valor de ax

	mov ah,9
	mov dx,OFFSET lit0
	int 21h
	mov ah,3fh
	mov bx,0
	mov cx,3
	mov dx,07h
	int 21h
	mov ax,07h
	mov [x],ax
	int 21h
	mov ax,07h
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
	jmp EndLabel0 ;break
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
	mul ax
	mov bx,ax
	push bx
	pop ax
	mov bx,ax
	pop ax
	add ax,bx
	pop bx
	mul ax
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
	mul ax
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
	mul ax
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
end STARTPOINT
