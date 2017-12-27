# Desactivación dcabezas\_bomb
##### David Cabezas Berrido

Para hallar la contraseña, buscamos la primera llamada a *boom*, y nos vamos
a la instrucción *test* inmediatamente superior. Podemos consultar esto con
*objdump -d dcabezas_bomb*.

```
 a8b:	48 89 d6             	mov    %rdx,%rsi
 a8e:	48 89 c7             	mov    %rax,%rdi
 a91:	e8 42 fd ff ff       	callq  7d8 <.plt.got+0x28>
 a96:	85 c0                	test   %eax,%eax
 a98:	74 16                	je     ab0 <main+0x11c>
 a9a:	48 8d 3d 50 02 00 00 	lea    0x250(%rip),%rdi        # cf1 <_IO_stdin_used+0xb1>
 aa1:	e8 0a fd ff ff       	callq  7b0 <.plt.got>
 aa6:	b8 00 00 00 00       	mov    $0x0,%eax
 aab:	e8 80 fe ff ff       	callq  930 <boom>
```

Podemos comprobar que la explosión de la bomba depende del valor de
*%eax* tras la llamada a la función en *758*. El programa previamente
carga algo en los registros *%rsi* y *%rdi*, así que introducimos una
contraseña que seamos capaces de reconocer y hacemos un volcado de
memoria de ambos registros:

```
Introduce la contraseña: hola

Breakpoint 1, 0x0000555555554a96 in main ()
(gdb) x/s $rdi
0x7fffffffdcd0:	"hola\n"
(gdb) x/s $rsi
0x7fffffffdcc0:	"incorrecta\n"
```

Ya sabemos que la contraseña es **incorrecta**.

Para el código, buscamos la tercera llamada a *boom*, unas líneas
antes compara los valores de dos registros. 

```
 b34:	39 c2                	cmp    %eax,%edx
 b36:	74 16                	je     b4e <main+0x1ba>
 b38:	48 8d 3d f6 01 00 00 	lea    0x1f6(%rip),%rdi        # d35 <_IO_stdin_used+0xf5>
 b3f:	e8 6c fc ff ff       	callq  7b0 <.plt.got>
 b44:	b8 00 00 00 00       	mov    $0x0,%eax
 b49:	e8 e2 fd ff ff       	callq  930 <boom>
```

Ponemos un código que sepamos reconocer y consultamos ambos registros.

```
Introduce la contraseña: incorrecta
Introduce el código: 7777

Breakpoint 2, 0x0000555555554b34 in main ()
(gdb) info reg
rax            0x1f44	8004
rbx            0x0	0
rcx            0x10	16
rdx            0x180f	6159
```

El código que hemos introducido no se encuentra en ninguno de los dos
registros, primeramente podemos plantear dos posibilidades:

Existe un número *a* que se resta al introducido (`7777-a=6159, a=1618`), en
este caso el código correcto sería `8004+a=9622`.

Existe un número *b* que se suma al introducido (`7777+b=8004,
b=227`), en este caso el código corrector sería `6159+b=6386`.

Podemos probar con ambas opciones, y descubrimos que el código es **9622**.
