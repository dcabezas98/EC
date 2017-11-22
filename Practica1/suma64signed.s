.section .data

# Declaro una única lista de longitud <= 32
lista:		.int 0xF0000000, 0xE0000000, 0xE0000000, 0xD0000000, 0xF0000000, 0xE0000000, 0xE0000000, 0xD0000000, 0xF0000000, 0xE0000000, 0xE0000000, 0xD0000000, 0xF0000000, 0xE0000000, 0xE0000000, 0xD0000000, 0xF0000000, 0xE0000000, 0xE0000000, 0xD0000000, 0xF0000000, 0xE0000000, 0xE0000000, 0xD0000000, 0xF0000000, 0xE0000000, 0xE0000000, 0xD0000000, 0xF0000000, 0xE0000000, 0xE0000000, 0xD0000000
#lista:		.int 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000, 0x70000000
#lista:		.int 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000, 0xFC000000
#lista:		.int 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000
#lista:		.int -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
#lista:		.int 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff
#lista:		.int -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10 
	
longlista:	.int (.-lista)/4	# Logitud de la lista
resultado:	.quad -1	

.section .text
main:	.global main

	mov longlista, %ecx
	mov    $lista, %ebx	# Muevo la lista a EBX
	call suma		# Sumo los elementos de la lista y almaceno el resultado en EDI:EAX
        mov %edi, resultado+4	# Muevo EDI a la parte más significativa de resultado
	mov %eax, resultado	# Muevo EAX a la parte menos significativa de resultado
	
	mov $1, %eax
	mov $0, %ebx
	int $0x80

suma:
	push %edx		# Almaceno en la pila el valor de EDX
	push %ecx		# Almaceno en la pila el valor de ECX, voy a usarlo como contador
	mov $0, %eax		# Inicializo los registros que voy a usar a 0
	mov $0, %esi
	mov $0, %edi
	mov $0, %ecx
	mov $0, %edx
	
bucle:
	mov (%ebx,%ecx,4), %eax		# Muevo el elemento de la lista a EAX
	cdq 				# Amplío EAX en signo usando EDX
	add %eax, %esi			# Sumo el contenido de EAX a ESI
	adc %edx, %edi			# Sumo la ampliación en de EAX (en EDX) y el acarreo a EDI
	inc       %ecx
	cmp %ecx, (%esp)
	jne bucle

	mov %esi, %eax	# Muevo la parte menos significativa del resultado a EAX
	pop %ecx	# Devuelvo a ECX su valor
	pop %edx	# Devuelvo a EDX su valor
	ret
