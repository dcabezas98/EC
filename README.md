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

#### Práctica 3: Bomba digital - Desensambladores

La práctica consiste en crear un programa en *C* que solicite una contraseña y un código y simule
una explosión en el caso de que no sean correctos. Del mismo modo, tan sólo con la información
del ejecutable compilado sin el flag de depuración *-g*, se debe intentar hallar la contraseña
y el código de las bombas del resto de compañeros.
Se incluye:

+ **dcabezas_bomb**: Mi bomba, incluye el ejecutable, el código fuente, un archivo en markdown donde se explican los pasos sequidos para desactivarla y el desensamblado con *objdump -d*.
+ **MaxiBomb**: Bomba de [@maxisuarez](https://github.com/maxisuarez). Incluye el ejecutable, el archivo en markdown con los pasos seguidos para desactivarla y el desensamblado con *objdump -d*.
+ **Bomba_NBA_2015**: Bomba de otro compañero, recomendada por el profesor. Incluye el ejecutable, el markdown con los pasos seguidos para desactivarla y el desensamblado con *objdump -d*.
