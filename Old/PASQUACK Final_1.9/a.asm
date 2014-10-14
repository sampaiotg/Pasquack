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

.code

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
Label2: 
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
	mov ax,[x]
	mov bx,ax
	push bx
	mov ax,[y]
	mov bx,ax
	pop ax
	add ax,bx
	mov ax,[x]
	mov bx,ax
	push bx
	mov ax,[y]
	pop bx
	mul ax,bx
	mov bx,ax
	push bx
	pop ax
	mov ax,[x]
	mov bx,ax
	push bx
	mov ax,[x]
	pop bx
	mul ax,bx
	mov bx,ax
	push bx
	pop ax
	mov ax,[x]
	mov bx,ax
	push bx
	mov ax,[y]
	mov bx,ax
	pop ax
	sub ax,bx
	mov ax,[x]
	mov bx,ax
	push bx
	pop ax


	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
