.section .data

lista:		.int -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	
longlista:	.int (.-lista)/4
media:		.int -1			# Aquí guardaré la media
resto:		.int -1			# Aquí guardo el resto de la división

.section .text
main:	.global main	# Inicio del programa

	mov longlista, %ecx
	mov    $lista, %ebx
	call suma
	mov %edi, %edx		# Muevo la parte más significativa de la suma, para tener el resultado en EDX:EAX
	idiv %ecx		# Divido EDX:EAX entre la longitud de la lista (conservando el signo)
	mov %eax, media		# IDIV almacena el cociente en EAX
	mov %edx, resto		# y el resto en EDX
	
	mov $1, %eax
	mov $0, %ebx
	int $0x80

suma:
	push %edx
	push %ecx
	mov $0, %eax
	mov $0, %esi
	mov $0, %edi
	mov $0, %ecx
	mov $0, %edx
	
bucle:
	mov (%ebx,%ecx,4), %eax
	cdq 
	add %eax, %esi
	adc %edx, %edi
	inc       %ecx
	cmp %ecx, (%esp)
	jne bucle

	mov %esi, %eax
	pop %ecx
	pop %edx
	ret
