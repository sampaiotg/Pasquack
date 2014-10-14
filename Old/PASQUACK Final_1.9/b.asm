title listadevalores	; Nome do Programa

dosseg
.model small
.stack 100h

.data

	x1 dw 
	x2 dw 
	i dw 

.code

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
EndLabel1:
;Fim do FOR
	jmp Label0
EndLabel0: 
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
EndLabel3:
;Fim do FOR
	jmp Label2
EndLabel2: 
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
	jmp Label4
EndLabel4: 


	mov ax,4C00h
	int 21h
PROG ends
	end MAIN
