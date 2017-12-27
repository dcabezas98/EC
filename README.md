# EC
### Estructura de Computadores - 2º Curso

Incluye las prácticas de la asignatura:

#### Práctica 1: Programación en ensamblador

Incluye una serie de cuestiones sobre el funcionamiento de ensamblador
y la depuración de programas con las herramientas ***gdb*** o ***ddd***.
También incluyes tres programas para practicar:

- **suma64unsigned.s** suma los elementos de una lista de enteros sin signo
de 32bits, guardando el resultado en una variable de 64bits para evitar 
desbordamiento.

- **suma64signed.s** suma los elementos de una lista de enteros con signo de
32bits representados en complemento a 2. Tiene en cuenta el signo a la hora
de sumar y almacena el resultado en una variable de 64bits para evitar 
desbordamiento.

- **media.s** realiza la media de los elementos de una lista de enteros con
signo de 32bits representados en complemento a 2.

#### Práctica 2: Programación mixta C-ASM

Incluye dos programas:

+ **popcount**: Cuenta el número de bits activados en un array de *unsigned int* de 32bits.
+ **parity**: Cuenta el número de elementos en un array de *unsigned int* de 32bits que tienen un número impar de bits activados.

Cada programa contiene varios algoritmos para realizar la misma tarea, en **C** o en **ensamblador**.
Y un informe del tiempo que tarda cada uno en ejecutarse.

También se incluyen tablas y gráficos (en formato *.ods*) con estadísticas que relacionan los
tiempos de ejecución de cada algoritmo con distintos niveles de optimización. Y un archivo con 
cuestiones sobre los programas realizados.
