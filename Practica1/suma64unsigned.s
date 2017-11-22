.section .data

lista0:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000	# Declaro 4 listas de longitud 8
lista1:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
lista2:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
lista3:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
	
#lista0:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
#lista1:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
#lista2:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
#lista3:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff

#lista0:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
#lista1:		.int 0, 0, 0, 0, 0, 0, 0, 0
#lista2:		.int 0, 0, 0, 0, 0, 0, 0, 0
#lista3:		.int 0, 0, 0, 0, 0, 0, 0, 0
	
	
longlista:	.int 8		# Logitud de cada lista
resultado:	.quad -1	# Aquí guardaré el resultado, necesito más de 32 bits (un quad tiene 64)

.section .text
main:	.global main 	# Inicio del programa

	mov $0, %eax		# Inicializo EAX a 0, en el guardaré la parte menos significativa del resultado
	mov $0, %edx		# Inicializo EDX a 0, en el guardaré la parte más significativa del resultado
	mov $lista0, %ebx	# Guardo la primera de las 4 listas en EBX
	mov longlista, %ecx	# Guardo la longitud de la lista en ECX
	call suma		# Sumo los elementos de la primera lista y almaceno el resultado en EDX:EAX
	mov $lista1, %ebx	# Guardo la segunda lista en EBX
	call suma		# Sumo los elementos de la segunda lista a lo que tenía en EDX:EAX
	mov $lista2, %ebx	# Guardo la tercera lista en EBX
	call suma		# Sumo los elementos de la tercera lista a lo que tenía en EDX:EAX
	mov $lista3, %ebx	# Guardo la última lista en EBX
	call suma		# Sumo los elementos de la última lista a lo que tenía en EDX:EAX
	mov %eax, resultado	# Muevo EAX a la parte menos significativa de resultado
	mov %edx, resultado+4	# Muevo EDX a la parte más significativa de resultado

	mov $1, %eax 	# Voy a llamar a exit
	mov $0, %ebx	
	int $0x80	# Llamada a exit

suma:
	push %esi		# Guardo el contenido de ESI en la pila, voy a usarlo como contador
	mov $0, %esi		# Lo inicializo a 0 
	
bucle:
	add (%ebx,%esi,4), %eax		# Sumo el elemento de la lista a EAX
	adc $0, %edx			# Sumo el acarreo (si lo hay) en EDX
	inc       %esi			# Incremento el contador del bucle
	cmp  %esi, %ecx			# Comparo con la longitud de la lista (en ECX) para ver si he llegado al final de la misma
	jne bucle			# Si no he llegado al final, hago otra iteración del bucle

	pop %esi			# Devuelvo a ESI su valor original
	ret
