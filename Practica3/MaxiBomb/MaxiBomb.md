# Desactivación MaxiBomb
#### David Cabezas Berrido

Con *ghex*, he descubierto que el ejecutable contenía la cadena
**holakuetal**, pero no he tenido suerte con ella.

Buscamos la primera llamada a *boom* con `objdump -d`.

```
804867e:	b8 30 a0 04 08       	mov    $0x804a030,%eax
8048683:	c7 r44 24 1c ff ff ff 	movl   $0xffffffff,0x1c(%esp)
804868a:	ff 
804868b:	89 c2                	mov    %eax,%edx
804868d:	b8 00 00 00 00       	mov    $0x0,%eax
8048692:	8b 4c 24 1c          	mov    0x1c(%esp),%ecx
8048696:	89 d7                	mov    %edx,%edi
8048698:	f2 ae                	repnz scas %es:(%edi),%al
804869a:	89 c8                	mov    %ecx,%eax
804869c:	f7 d0                	not    %eax
804869e:	83 e8 01             	sub    $0x1,%eax
80486a1:	89 44 24 08          	mov    %eax,0x8(%esp)
80486a5:	c7 44 24 04 30 a0 04 	movl   $0x804a030,0x4(%esp)
80486ac:	08 
80486ad:	8d 44 24 38          	lea    0x38(%esp),%eax
80486b1:	89 04 24             	mov    %eax,(%esp)
80486b4:	e8 27 fe ff ff       	call   80484e0 <strncmp@plt>
80486b9:	85 c0                	test   %eax,%eax
80486bb:	74 05                	je     80486c2 <main+0xb2>
80486bd:	e8 e2 fe ff ff       	call   80485a4 <boom>
```

Como podemos comprobar, antes de llamar a la función *strncmp*, carga
varios registros con *mov* y *movl*, consultamos los registros en
*gdb* hasta que damos con la tecla. En la instrucción *mov*, de la
dirección 0x804867e mueve algo al registro *EAX*, ponemos un brekpoint
en la siguiente dirección para consultar lo que se acaba de mover.

```
(gdb) r
Starting program: /home/dcabezas/dgiim/ec/practica3/MaxiBomb/MaxiBomb 
Introduce la contraseña: holakuetal

Breakpoint 1, 0x08048683 in main ()
(gdb) x/s $eax
0x804a030 <password>:	"holaquetal\n"
```

Hacemos un volcado de memoria y ahí la tenemos, la contraseña es
**holaquetal**.

\newpage

Para el código, notamos que se realiza una comparación en la dirección
0x8048719 antes de la llamada a *boom*.

```
804870b:	e8 c0 fd ff ff       	call   80484d0 <__isoc99_scanf@plt>
8048710:	8b 54 24 34          	mov    0x34(%esp),%edx
8048714:	a1 3c a0 04 08       	mov    0x804a03c,%eax
8048719:	39 c2                	cmp    %eax,%edx
804871b:	74 05                	je     8048722 <main+0x112>
804871d:	e8 82 fe ff ff       	call   80485a4 <boom>
```

Introducimos un código que sepamos reconocer y ponemos un breakpoint en esa
dirección para consultar el valor de los registros *EAX* y *EDX*.

```
Introduce la contraseña: holaquetal
Introduce el código: 7777

Breakpoint 1, 0x08048719 in main ()
(gdb) info reg
eax            0x7ce	1998
ecx            0x1	    1
edx            0x1e61	7777
ebx            0x0	    0
```

Ahí lo tenemos, el código es **1998**.
