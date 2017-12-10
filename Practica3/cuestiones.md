# Estructura de Computadores: Práctica 2 (Cuestiones)

#### David Cabezas Berrido

### Cuestiones sobre popcount.c

**1)**

Los elementos del array son *unsigned int* de 32 bits. Luego en el
peor caso (en el que los 32 bits de cada uno de los elementos del
array son 1), el número de 1s en cada elemento sería 32, por lo que el
número de 1s en el array sería **32\*N**.

Un *int* puede almacenar hasta el valor 32767 = 2^15-1. 32767/32 =
1023'96875, por lo que tamaño máximo del array será 1023 para asegurar
que en el peor caso no se produzca overflow.

En cambio, si almacenamos el resultado en un *unsigned int*, que puede
almacenar hasta el valor 65535 = 2^16-1. 65535/32 = 2.047,96875, por
que el tamaño máximo del array será 2047 para asegurar que en el peor
caso no se produzca overflow.

**2)**

Para N potencia de 2 (N = 2^k), podemos calcular el peso de Hamming
del array de números consecutivos del 0 al 2^k-1 usando el siguiente
razonamiento:

El array tendrá un número par de elementos, luego el bit 0 estará
activado en la mitad de los casos.

El array tendrá un múltiplo de 4 de elementos, 

Si el array tuviera 2 elementos (0 y 1) el bit 0 estaría activado la
mitad de las veces.

Si el array tuviera 4 elementos (00, 01, 10, 11) los bits 0 y 1
estarían activados la mitad de las veces.

Si el array tuviera 8 elementos (000, 001, 010, 011, 100, 101, 110 y
111), los bits 0, 1 y 2 estarían activados la mitad de las veces.

Siguiento este razonamiento, se llega a la conclusión de que cada bit
estará activado la mitad de las veces, por lo que el peso de Hamming
será k*N/2, cada uno de los k bits estará activo en la mitad de los
elementos de array (N/2).

**3)**

Al desplazar hacia la derecha un *signed int*, se copia el bit de
signo para que el resultado sea el valor original dividido entre dos
elevado al número de bits desplazados. Esto se llama desplazamiento
aritmético (en *unsigned int* se realiza desplazamiento lógico, que
siempre rellena con 0s el hueco que queda al desplazar).

Un número negativo en complemento a 2 tiene un 1 en el bit 31, por lo
que al desplazar hacia la derecha se copia el bit de signo (1) para
rellenar el hueco que queda. Esto genera más 1s de los que realmente
tenía el elemento, lo que afecta al resultado.

No lo notamos porque todos los números con los que estamos trabajando
son positivos.

**4)** 

Con **+r/r** el tiempo medio de ejecución ha sido de 658'2 us.
Con **+m/r** el tiempo medio de ejecución ha sido de 2122'2 us.

El programa no compilaba eligiendo restricción a memoria para el dato
de entrada (un elemento del array). He obtenido el siguiente mensaje:

	popcount.c: Assembler messages:
	popcount.c:60: Error: no instruction mnemonic suffix given and no register operands; can't size instruction
	
Esto ocurre porque no se puede determinar el tamaño de *x*, y se soluciona sustituyendo `shr` por `shrl`.

Con **+r/m** el tiempo medio de ejecución ha sido de 1431'3 us.
Con **+m/m** el tiempo medio de ejecución ha sido de 2167 us.

Las prestaciones con restricciones a registros son notablemente
mejores que las prestaciones con restricciones a memoria.

**5)**

Porque los arrays no caben en registros.

**7)**

Con `gcc popcount.c -S -O0`, `gcc popcount.c -S -O1` y `gcc popcount.c
-S -O2` se puede comprobar que los códigos de ensamblador generados
son muy similares.

**8)**

*Gráficas aparte*

La mejor versión ha sido la 5 (SSSE3 con operación de baraje), código ASM.

\newpage

### Cuestiones sobre parity.c

**1)**

Array de tamaño N con N = 2^k, con elementos consecutivos desde el 0
al 2^k-1.

Los dos primeros primeros elementos son 0 y 1, que tienen paridad 0
y 1.  Los dos siguientes se representan de la misma forma pero
añadiendo un 1 en la siguiente posición, por lo que se invierte la
paridad (1 y 0), pasa lo mismo con los 4 siguientes (1, 0, 0 y 1). Y
así hasta 2^k.

Cada vez que añado un bit, se repite la secuencia pero invertida,
luego la proporción de elementos con un número impar de 1s es la misma
(la mitad). Por tanto, el resultado es N/2.

**2)** 

Al desplazar hacia la derecha un *signed int*, se copia el bit de
signo para que el resultado sea el valor original dividido entre dos
elevado al número de bits desplazados. Esto se llama desplazamiento
aritmético (en *unsigned int* se realiza desplazamiento lógico, que
siempre rellena con 0s el hueco que queda al desplazar).

Un número negativo en complemento a 2 tiene un 1 en el bit 31, por lo
que al desplazar hacia la derecha se copia el bit de signo (1) para
rellenar el hueco que queda. Esto genera más 1s de los que realmente
tenía el elemento, lo que afecta al resultado.

No lo notamos porque todos los números con los que estamos trabajando
son positivos.

**3)**

Supone mejoría, que es más notable con `-O1` y poco apreciable con `-O0`.
Puede consultarse en la tabla de la gráfica.

En la versión 4:

Con **+r/r** el tiempo medio de ejecución ha sido de 446 us.
Con **+m/r** el tiempo medio de ejecución ha sido de 1044.3 us.

El programa no compilaba eligiendo restricción a memoria para el dato
de entrada (un elemento del array). He obtenido el siguiente mensaje:

	parity.c: Assembler messages:
	parity.c:96: Error: no instruction mnemonic suffix given and no register operands; can't size instruction
	
Esto ocurre porque no se puede determinar el tamaño de *x*, y se soluciona sustituyendo `shr` por `shrl`.

Con **+r/m** el tiempo medio de ejecución ha sido de 2126.3 us.

Con **+m/m** he obtenido el siguiente error de compilación:
	
	parity.c: Assembler messages:
	parity.c:95: Error: too many memory references for `xor'
	
Este error no puede corregirse.

Las prestaciones con restricciones a registros son notablemente
mejores que las prestaciones con restricciones a memoria.

**4)**

Con el nivel de optimización *O0*, la versión 4 supone un notable
mejoría respecto a la 3.

Con el nivel de optimización *O1*, la versión 4 es bastante más lenta
que la 3.

Con el nivel de optimización *O2*, la versión 4 es ligeramente mejor
que la 3.

Los tiempos entre los niveles de optimización *O1* y *O2* son muy
similares, y el código en ensamblador es similar. Pero si difiere
mucho del código ensamblador generado con el nivel de optimización
*O0*, cuyo tiempo es mucho más lento.

**5)**

En la versión 4:

Con **+r/r** el tiempo medio de ejecución ha sido de 446 us.
Con **+m/r** el tiempo medio de ejecución ha sido de 1044.3 us.

El programa no compilaba eligiendo restricción a memoria para el dato
de entrada (un elemento del array). He obtenido el siguiente mensaje:

	parity.c: Assembler messages:
	parity.c:96: Error: no instruction mnemonic suffix given and no register operands; can't size instruction
	
Esto ocurre porque no se puede determinar el tamaño de *x*, y se soluciona sustituyendo `shr` por `shrl`.

Con **+r/m** el tiempo medio de ejecución ha sido de 2126.3 us.

Con **+m/m** he obtenido el siguiente error de compilación:
	
	parity.c: Assembler messages:
	parity.c:95: Error: too many memory references for `xor'
	
Este error no puede corregirse.

Las prestaciones con restricciones a registros son notablemente
mejores que las prestaciones con restricciones a memoria.

**6)**

La operación `xor` entre las dos mitades de un conjunto de 0s y 1s
preserva la paridad. Es fácil de comprobar para dos bits, y el
razonamiento se puede generalizar. Si hay un número impar de 1s, el
`xor` debe dejar un número impar de 1s sin emparajar con 1s, por lo
que se quedará paridad impar. Y si hay un número par de 1s, el `xor`
debe dejar un número par de 1s sin emparejar con 1s, por lo que se
quedará paridad par. Esto es independiente de cómo estén dispuestos
los 1s y los 0s, sólo se ve afectado por el número de 1s.

Por tanto en cada iteración reducimos el número
de bits significativos del elemento a la mitad, lo que hace este
algoritmo bastante eficiente.

**7)**

El algoritmo realiza un `xor` de *x* con *x>>16*, y lo guarda en
*EDX*, luego realiza un `xor` entre *DH* y *DL*, los 8 bits más y
menos significativos de los 16 bits en *EDX*, y almacena el resultado
en *DL*. A continuación calcula la paridad de los 8 bits en *DL*
directamente con la orden `setpo`.

**8)**

Obtengo resultados muy similares usando y sin usar *EDX* en clobber,
en los distintos niveles de optimización. 

**9)**

*Gráficas aparte*

Las mejores versiones son las de suma en árbol. Entre las cuales la
ASM tiene mejores prestaciones.

