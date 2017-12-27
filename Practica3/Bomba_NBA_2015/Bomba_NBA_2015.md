# Desactivación Bomba\_NBA\_2015
#### David Cabezas Berrido

Con el comando *strings* o con *ghex* podemos encontrar la clave
fácilmente:

```
Oh, castitas lilium
Esta es la clave!!
Miauuu
```

Probamos y descubrimos que la clave es **Oh, castitas lilium**.

Cuando ponemos alguna de las otras dos, nos dirá *miau*, pero la bomba
explotará igualmente.

```
Introduce la contraseña: 
Esta es la clave!!
miau
```

```
Introduce la contraseña: 
Miauuu
miau
```

Para el código, miramos con *objdump -d* que realiza una serie de
comparaciones, introducimos un valor que seamos capaces de reconocer y
vamos probando hasta que lo descubrimos en la siguiente comparación.

```
8048877:	e8 64 fc ff ff       	call   80484e0 <__isoc99_scanf@plt>
804887c:	8b 54 24 2c          	mov    0x2c(%esp),%edx
8048880:	a1 64 b0 04 08       	mov    0x804b064,%eax
8048885:	39 c2                	cmp    %eax,%edx
8048887:	75 05                	jne    804888e <main+0x24d>
```

Ahí está nuestro código en *EDX* y el código correcto en *EAX*.

```
Introduce la contraseña: 
Oh, castitas lilium
Introduce el código: 7777

Breakpoint 1, 0x08048885 in main ()
(gdb) info reg
eax            0x406	1030
ecx            0x1	1
edx            0x1e61	7777
```

El código es **1030**.
