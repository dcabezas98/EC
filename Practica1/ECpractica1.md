## Estructura de Computadores: Preguntas Práctica 1

##### David Cabezas Berrido

###### Doble Grado en Ingeniería Informática y Matemáticas 

### Sesión de depuración saludo.s

**1)**

Con `list`, localizamos la línea que contiene la orden `mov
longsaludo, %edx`, colocamos un `breakpoint` en dicha línea y lanzamos
la ejecución con `run`. Ejecutamos la instrucción con `stepi` y
procedemos a consultar el valor del registro con `print %edx`.

El valor de `EDX` tras ejecutar `mov longsaludo, %edx` es 28 (en
decimal) o 0x1c. Se trata de la longitud de la cadena saludo,
que contiene el valor *"Hola a todos!\\nHello, World!\\n"*.

Coincide con los valores obtenidos al ejecutar la orden `info
registers`.

**2)**

De la misma forma que antes, hemos obtenido que el valor del registro
`ECX` tras ejecutar `mov $saludo, %ecx` es 134516887 (= 0x8049097).
Este valor corresponde a la dirección de memoria de la variable *saludo*.

**3)**

Cambiamos `saludo.s` y volvemos a ensamplarlo y enlazarlo. Depuramos
el nuevo ejecutable y obtenemos que el nuevo valor de `ECX` tras
ejecutar `mov saludo, $ecx` es 1634496328 (= 0x616c6f48).

Esto se debe a que transforma los caracteres del string a su valor en
ASCII en hexadecimal hasta completar los 32 bits.

**4)**

Cada caracter de la variable *saludo* ocupa un byte. Podemos comprobar
con `x/32cb &saludo` que se almacena cada uno en una dirección de
memoria, por lo que *saludo* ocupa 28 direcciones de memoria (tiene 28
caracteres). Y justo a continuación, se almacena un byte para
*longsaludo*, que podemos comprobar con `x/4cb &longsaludo`.
Por lo que la sección de datos del programa ocupa 29 bytes.

\newpage

**5)**

Volcado como entero hexadecimal:

```
	(gdb) x /1xb &longsaludo
	0x80490b3:	0x1c
```

Volcado como 4 bytes hex:

	(gdb) x /4xb &longsaludo
	0x80490b3:	0x1c	0x00	0x00	0x00

*longsaludo* ocupa la dirección 0x80490b3. El byte en la primera posición
es *00011100*, el menos significativo, por lo que podemos deducir que los
procesadores de la línea *x86* usan el criterio del extremo menor
(little-endian), en el que los bits más significativos son los de la 
derecha.

**6)**

Colocando un `breakpoint` en la lína correspondiente y con la orden 
`disassemble` podemos ver que la instrucción `mov $1, %ebx` ocupa 5 
posiciones de memoria, desde la 0x08048079 hasta la 0x0804807d, ya 
que la siguiente instrucción empieza en la 0x0804807e.

```
 => 0x08048079 <+5>:	mov    $0x1,%ebx
	0x0804807e <+10>:	mov    $0x8049097,%ecx
```

**7)**

Eliminando la primera instrucción `int 0x80` el programa finaliza
normalmente pero no imprime el resultado en pantalla.

```
	(gdb) run
	Starting program: /home/dcabezas/dgiim/ec/practica1/saludo 
	[Inferior 1 (process 3982) exited normally]
```

En cambio, eliminando la segunda instrucción `int 0x80` el programa
imprime el resultado en pantalla, pero produce un *Segmentation fault*
en lugar de salir normalmente.

```
	(gdb) run
	Starting program: /home/dcabezas/dgiim/ec/practica1/saludo 
	Hola a todos!
	Hello, World!

	Program received signal SIGSEGV, Segmentation fault.
	0x08048096 in ?? ()
```

**8)**

El número de llamada del sistema ***READ*** en el kernel Linux 32bits es
el 3. Esto puede comprobarse con `cat /usr/include/asm/unistd_32.h`,
donde podemos encontrar la siguiente línea:

	#define __NR_read 3


### Sesión de depuración suma.s

**1)**

`EAX` contiene el valor 37 (= 0x25) justo antes de ejecutar `RET`.
Lo he comprobado con un `break` en la línea correspondiente y la orden
`info reg`. Pero puedo razonarse a partir de que es la suma de todos
los elementos de la lista (1, 2, 10, 1, 2, 0b10, 1, 2, 0x10). Donde 0b10
es 2 en decimal y 0x10 es 16.

`(.-lista)` corresponde al número de bytes que ocupa lista, como cada entero
ocupa 4B (32b) y tiene 9 enteros, `(.-lista)` tiene el valor 36.
`(.-lista)/4` tiene el valor 9 (longitud de la lista), podemos comprobarlo
con un `print` de la variable *longlista*.

**2)**

Se obtiene el valor -3 (= 0xfffffffd), esto se debe a que 0xffffffff corresponde
al valor -1 (está en complemento a 2) y lo sumamos 3 veces. 

**3)**

En `ddd`, pasando el cursor sobre una variable podemos ver su dirección de memoria 
asignada, lo hacemos con las etiquetas de funciones suma y bucle.

0x8048095 <suma>
0x80480a0 <bucle>

**4)**

`EIP` contine una dirección de memoria de instrucción, que va avanzando 
a medida que el procesador va ejecutando el programa.
`ESP` es el puntero de pila.

**5)**

Antes de ejecutar `CALL`, `ESP` contiene la dirección 0xffffd0e0.
Antes de ejecutar `RET`, `ESP` contiene la dirección 0xffffd0dc.

La diferencia entre ambos valores es 4, el tamaño de pila usado para
almacenar la dirección de retorno de la función. Este tamaño es necesario
entre la llamada y el retorno de la función (entre `CALL` y `RET`).

**6)**

La instrucción `CALL` modifica los registros:
`EAX`, donde guarda el resultado de las sumas.
`ESP`, donde almacena la dirección de retorno para `RET`.
`EDX`, que actúa como contador.
`EIP`, que almacena direcciones de las instrucciones a ejecutar.

**7)**

La instrucción `RET` modifica los registros:
`ESP`, porque vuelve a la dirección de retorno guardada en la pila.
`EIP`, su modificación es necesaria para el avance del programa.

**8)**

Colocando diversos breakpoints y usando volcados de memoria, he podido 
comprobar que primero apunta a la dirección 0xffffd020, que contiene el 
valor 0x00000001, al ejecutar `CALL`, apunta a 0xffffd01c, donde se 
guarda el valor 0x08048084. Tras `push %edx`, pasa a apuntar a 0xffffd018,
donde está el valor 0x00000000. Luego se ejecuta `pop %edx` y pasa a 
apuntar a 0xffffd01c de nuevo, con el valor 0x08048084. Y después de `RET`
apunta a 0xffffd020, con el valor 0x00000001.

**9)**

`mov $0, %edx` ocupa 5 posiciones de memoria, desde la 0x0804809b hasta la
0x080480a0. Puede verse en el desensamblado de *ddd* o utilizando `objdump`. Del mismo modo podemos ver que `inc %edx` ocupa únicamente la posición
0x080480a3. 

Códigos máquina (en *Machine Code Window* de *ddd* o con `objdump`):

	0x0804809b <suma+6>:   ba 00 00 00 00       	mov    $0x0,%edx

	0x080480a3 <bucle+3>:  42                   	inc    %edx

**10)**

Que el no se volvería de la subrutina al programa principal, por lo que 
el programa terminaría con un *Segmentation fault*.

```
	(gdb) run
	Starting program: /home/dcabezas/dgiim/ec/practica1/suma 

	Program received signal SIGSEGV, Segmentation fault.
	0x080480a9 in ?? ()
```

### Sumar N enteros *sin* signo de 32bits en una plataforma de 32btis sin perder precisión (N≈32)

He creado 4 listas de 8 enteros cada una, se pueden sumar hasta 32 enteros,
para sumar menos, se pueden dejar el resto de valores a 0.

El programa llama 4 veces a la función suma, en cada una, el registro `%ebx`
contiene una lista distinta. Estas se van sumando y el resultado se va 
almacenando en `%eax`, mientras que los acarreos se suman a `%edx`.

Cuando terminan las operaciones, se guardan la suma en la parte menos significativa
de la variable `resultado`, de 64 bits, y los acarreos en la más significativa.

```assembly
.section .data

lista0:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000	   # Declaro 4 listas de longitud 8
lista1:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
lista2:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
lista3:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
	
#lista0:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
#lista1:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
#lista2:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
#lista3:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	
longlista:	.int 8		# Logitud de cada lista
resultado:	.quad -1	# Aquí guardaré el resultado, necesito más de 32 bits (un quad tiene 64)

.section .text
main:	.global main

	mov $0, %eax		# Inicializo EAX a 0, en el guardaré la parte menos significativa del resultado
	mov $0, %edx		# Inicializo EDX a 0, en el guardaré la parte más significativa del resultado
	mov $lista0, %ebx	# Guardo la primera de las 4 listas en EBX
	mov longlista, %ecx	# Guardo la longitud de la lista en ECX
	call suma		    # Sumo los elementos de la primera lista y almaceno el resultado en EDX:EAX
	mov $lista1, %ebx	# Guardo la segunda lista en EBX
	call suma		    # Sumo los elementos de la segunda lista a lo que tenía en EDX:EAX
	mov $lista2, %ebx	# Guardo la tercera lista en EBX
	call suma		    # Sumo los elementos de la tercera lista a lo que tenía en EDX:EAX
	mov $lista3, %ebx	# Guardo la última lista en EBX
	call suma		    # Sumo los elementos de la última lista a lo que tenía en EDX:EAX
	mov %eax, resultado	    # Muevo EAX a la parte menos significativa de resultado
	mov %edx, resultado+4	# Muevo EDX a la parte más significativa de resultado

	mov $1, %eax    # Voy a llamar a exit
	mov $0, %ebx
	int $0x80       # Llamada a exit

suma:
	push %esi		    # Guardo el contenido de ESI en la pila, voy a usarlo como contador
	mov $0, %esi		# Lo inicializo a 0 
	
bucle:
	add (%ebx,%esi,4), %eax		# Sumo el elemento de la lista a EAX
	adc $0, %edx			    # Sumo el acarreo (si lo hay) en EDX
	inc       %esi			    # Incremento el contador del bucle
	cmp  %esi, %ecx			    # Comparo con la longitud de la lista (en ECX) para ver si he llegado al final de la misma
	jne bucle			        # Si no he llegado al final, hago otra iteración del bucle

	pop %esi			# Devuelvo a ESI su valor original
	ret
```

### Cuestiones sobre suma64signed.s

**1)**

He dado el valor 0xffffffff (el máximo representable con 32 bits) a cada uno de
los 32 enteros de la lista, 4294967295 ≈ 4.29E+9 en decimal. Con un volcado de memoria,
he podido comprobar que la variable resultado toma el valor de 0x1fffffffe0 
(137438953440 ≈ 1.37E+11 en decimal).

Para almacenar este número, son necesarios 37 bits. Durante las sumas se producen
31 acarreos.

**2)**

2^32/32 = 2^32/2^5 = 2^27 = 134217728 (= 0x8000000 en hexadecimal).
Dando ese valor a todos los elementos de la lista, se obtiene como resultado
el valor 0x100000000 = 4294967296 = 2^32. El acarreo se produce en la última suma.

**3)**

He obtenido el resultado 0x780000000 = 32212254720. Cada ciclo suma 0xf0000000,
por lo que se produce un acarreo al sumar el primer elemento de cada ciclo a 
partir del segundo, para un total de 7 acarreos.

### Sumar N enteros con signo de 32bits en una plataforma de 32bits

Esta vez, he creado una única lista de longitud 32, por lo que el programa
llama una única vez a la función suma. Esta usa varios registros, `%ecx` es
usado como contador del bucle, pero contiene el valor de la longitud de la
lista, así que he pasado su valor a la pila con `push` y usado `(%esp)` para
acceder a ese valor.

El algoritmo consiste en mover el entero de la lista a `%eax`, este es extendido
en signo con `%edx` con la función `cdq`. después se suma el valor de `%eax`
a `%esi` y se suma `%edx` a `%edi` teniendo en cuenta el acarreo generado
anteriormente (orden `adc`), al terminar la lista. Se mueve la parte menos 
significativa del resultado de `%esi` a `%eax`, y finalmente se mueven las 
los valores de las partes menos significativa (`%eax`) y más significativa
(`%edi`) a la variable de 64 bits *resultado*.

```assembly
.section .data

# Declaro una única lista de longitud <= 32
lista:		.int 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff
	
longlista:	.int (.-lista)/4	# Logitud de la lista
resultado:	.quad -1	

.section .text
main:	.global main

	mov longlista, %ecx
	mov    $lista, %ebx	   # Muevo la lista a EBX
	call suma		       # Sumo los elementos de la lista y almaceno el resultado en EDI:EAX
    mov %edi, resultado+4    # Muevo EDI a la parte más significativa de resultado
	mov %eax, resultado	     # Muevo EAX a la parte menos significativa de resultado
	
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
	cdq 				        # Amplío EAX en signo usando EDX
	add %eax, %esi			# Sumo el contenido de EAX a ESI
	adc %edx, %edi			# Sumo la ampliación en de EAX (en EDX) y el acarreo a EDI
	inc       %ecx
	cmp %ecx, (%esp)
	jne bucle

	mov %esi, %eax	# Muevo la parte menos significativa del resultado a EAX
	pop %ecx	# Devuelvo a ECX su valor
	pop %edx	# Devuelvo a EDX su valor
	ret
```

### Cuestiones sobre suma64signed.s

**1)**

El máximo entero positivo que puede representarse con 32 bits es el 0x7fffffff (2147483647 ≈ 2.15E+9). Sumando 32 veces ese valor se obtiene 0xfffffffe0,
68719476704 ≈ 6.87E+10 en decimal.

**2)**

El menor valor negativo representable con 32 bits es 0x80000000, correspondiente
a -2147483648 ≈ -2.15E+9 en decimal (se respresenta en complemento a 2). Sumando
32 ese valor se obtiene 0xfffffff000000000, -68719476736 ≈ -6.87E+10 en decimal. 

**3)**

2^31/32 = 2^31/2^5 = 2^26 = 67108864 (= 0x400000 en hexadecimal). 
Dando ese valor a todos los elementos de la lista, se obtiene como resultado
el valor 0x80000000 =  = 2^31. El acarreo no llega a producirse.

**4)**

El registro `%eax` se extiende a `%edx` conservando su signo, por lo que el
bit que indica que un número es negativo no forma parte de los 32 bits de `%eax`,
sino que es el bit más a la derecha de `%edx`. Luego para lograr el acarreo
debemos conseguir que los números sumen 2^32, al igual que antes. El valor
requerido es 1879048192 ≈ 1.87E+9 (= 0x7000000 en hexadecimal) y el 
resultado es 0xe00000000 = 60129542144 ≈ 6.01E+10.

**5)**

Para obtener -2^31 (0x80000000 en complemento a 2) como resultado de la suma,
debe sumarse 32 veces el número -67108864 (= 0xfc000000 en complemento a 2).

Para obtener -2^32 (0xffffffff00000000 en complemento a 2) como resultado de
la suma, debe sumarse 32 veces el número -134217728 (= 0xf8000000 en 
complemento a 2).

**6)**

La suma tomaría el valor 0xfffffffc00000000.

La suma de los valores del ciclo es:
```
   0xf0000000 =   -268435456
   0xe0000000 =   -536870912
   0xe0000000 =   -536870912
   0xc0000000 =   -805306368
                -------------
                 -2147483648 
```				 
El ciclo se repite 8 veces:

-2147483648 * 8 = -17179869184 = 0xfffffffc00000000.

### Media de N enteros con signo de 32bits, en plataforma de 32bits

Para calcular la media de los 32 enteros, realizo la suma con signo, pero
esta vez guardo el resultado en `%edx:%eax` en lugar de en `%edi:%eax` ya que
la orden `idiv` trabaja con esos registros, he dividido el resultado de la 
suma almacenado en `%edx:%eax` entre la longitud de la lista, que he movido
previamente a `%ecx` (con `pop` recupera su valor), el resultado de la división 
se guarda en `%eax`, y posteriormente lo muevo a *media*. La orden `%idiv` 
almacena el resto de la división en `%edx`, lo he movido a *resto*.

```assembly
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
	idiv %ecx		    # Divido EDX:EAX entre la longitud de la lista (conservando el signo)
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
```

### Cuestiones sobre media.s

**1)**

Llenando la lista de -1, el resto es 0 y la media -1.
Cambiando uno de ellos (estoy trabajando con 32 elementos en total) por 0,
el resto pasa a ser -31 y la media 0.

La instrucción de división, está programada para que el cociente esté lo más
cerca de 0 posible. De esta forma, el resto puede ser tanto positivo como 
negativo.
