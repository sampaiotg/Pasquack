title listadevalores	; Nome do Programa

dosseg
.model small
.stack 100h

.data
	Vetor DB 0,0,0,0,0,13,10,'$'
	Tam DB 5
	x1 DB 0,'$'
	x2 DB 0,'$'
	i DB 0,'$'
	lit0 DB 'Entre com dois valores x1 e x2 ',13,10,'$'

.Code
main proc
	;inicializando o registrador ds
	mov ax,@data	;coloca o numero do segmento de dados em ax
	mov ds,ax		;ds inicializado com valor de ax

	mov ah,9
	mov dx,OFFSET lit0
	int 21h

	jmp fim	;chamando funcao para finalizar a aplicacao

fim: mov ah,4ch	;funcao DOS para saida
     int 21h	;saindo
main endp
end main
