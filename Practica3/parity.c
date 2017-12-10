//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define SIZE (1<<16)	// tamaño suficiente para tiempo apreciable
unsigned lista[SIZE]; 
   
int resultado=0;

int parityv1(unsigned * array, int len){

  int i, j, val, result = 0;

  unsigned dato;

  for(i = 0; i < len; i++){        // recorre el array

    dato = array[i];
    val = 0;

    for(j = 0; j < 32; j++){       // recorre los 32 bits de cada uno de los elementos

      val ^= (dato & 0x1);         // consulta el último bit, cambiando la paridad en caso de que sea 1
      dato >>= 1;                  // desplaza un bit a la derecha
    }

    result += val;                 // añade la paridad (0 ó 1) al resultado
  }

  return result;
}

int parityv2(unsigned * array, int len){

  int i, val, result = 0;

  unsigned dato;

  for(i = 0; i < len; i++){       // recorre el array

    dato = array[i];
    val = 0;

    while(dato){                 // sólo itera mientras el dato sea distinto de 0

      val ^= (dato & 0x1);       // consulta el último bit, cambiando la paridad en caso de que sea 1
      dato >>= 1;                // desplaza un bit a la derecha
    }

    result += val;               // añade la paridad (0 ó 1) al resultado
  }

  return result;
}

int parityv3(unsigned * array, int len){

  int i, val, result = 0;

  unsigned dato;

  for(i = 0; i < len; i++){       // recorre el array

    dato = array[i];
    val = 0;

    while(dato){                  // sólo itera mientras el dato sea distinto de 0

      val ^= dato;                // opera con cada bit, cambiando la paridad en caso de que sea 1 (sólo importa el bit menos significativo)
      dato >>= 1;                 // desplaza un bit a la derecha
    }

    result += val & 0x1;          // aplica la máscara para aislar el bit menos significativo
  }

  return result;
}

int parityv4(unsigned * array, int len){

  int i, val, result = 0;

  unsigned x;

  for(i = 0; i < len; i++){       // recorre el array

    x = array[i];
    val = 0;

    asm("\n"
	"ini3:            \n\t" // seguir mientras que x != 0
	"xor %[x], %[v]   \n\t" // realmente sólo nos interesa LSB
	"shr $1,   %[x]   \n\t" // desplaza x un bit a la derecha
	"cmp $0,   %[x]   \n\t" // si x != 0, vuelve a iterar
	"jne ini3         \n\t" 
	"and $0x1, %[v]   \n\t" // aisla el bit menos significativo
 	: [v]"+r" (val)  // e/s: entrada 0, salida paridad elemento
	: [x] "r" (x)    // entrada: valor elemento
	);
    
    result += val;
  }

  return result;
}

int parityv5(unsigned * array, int len){

  int i, j, result = 0;

  unsigned x;

  for(i = 0; i < len; i++){       // recorre el array

    x = array[i];

    for(j = 16; j > 0; j/=2){     // voy desplazando la mitad de bits en cada iteración
      x ^= x >>j;                 // xor de x y x desplazado la mitad de sus últimos bits significativos
    }
        
    result += x & 0x1;            // suma el último bit al resultado
  }

  return result ;
}

int parityv6(unsigned * array, int len){

  int i, j, result = 0;

  unsigned x;

  for(i = 0; i < len; i++){       // recorre el array

    x = array[i];

    asm(
	"mov %[x], %%edx  \n\t"  // sacar copia para XOR. Controlar el registro
	"shr $16, %[x]  \n\t"    // (EDX) nos permite usar nombres registros 8bits
	"xor %[x], %%edx  \n\t"  // xor entre x y x desplazado 16 bits
	"xor %%dh, %%dl  \n\t"   // XOR entre partes más y menos significativa de (EDX) (xor entre x y x desplazado 8 bits, dh y dl son registros de 8 bits)
	"setpo %%dl  \n\t"       // paridad de los 8 bits
	"movzx %%dl, %[x]  \n\t" // devolver en 32bits
	: [x]"+r" (x)            // e/s: entrada valor elemento salida paridad
	: 
        : "edx"  // clobber
	);
    
    result += x;
  }

  return result;

}

void crono(int (*func)(), char* msg){
    struct timeval tv1,tv2;	// gettimeofday() secs-usecs
    long           tv_usecs;	// y sus cuentas

    gettimeofday(&tv1,NULL);
    resultado = func(lista, SIZE);
    gettimeofday(&tv2,NULL);

    tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
             (tv2.tv_usec-tv1.tv_usec);
    printf("resultado = %d\t", resultado);
    printf("%s:%9ld us\n", msg, tv_usecs);
}

int main()
{
    int i;			// inicializar array
    for (i=0; i<SIZE; i++)	// se queda en cache
	 lista[i]=i;
 
    crono(parityv1, "parityv1 (bucle for)");
    crono(parityv2, "parityv2 (bucle while)");
    crono(parityv3, "parityv3 (ahorra máscaras en el while)");
    crono(parityv4, "parityv4 (bloque asm)");
    crono(parityv5, "parityv5 (suma en árbol)");
    crono(parityv6, "parityv6 (suma en árbol asm)");
    
    exit(0);
}
