//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define SIZE (1<<16)	// tamaño suficiente para tiempo apreciable
unsigned lista[SIZE];
int resultado=0;

int popcountv1(unsigned *array, int len){

  int k, j;

  int result = 0;

  for(j = 0; j < len; j++){     // recorre el array 
    for(k = 0; k < 32; k++){    // recorre los 32 bits de cada elemento del array

      unsigned mask = 1 <<k;               // genera la máscara con el bit k-ésimo
      result += (array[j] & mask) != 0;    // la aplica
    }
  }

  return result;
}

int popcountv2(unsigned *array, int len){

  int j;

  int result = 0;

  for(j = 0; j < len; j++){       // recorre el array

    unsigned mask = array[j];

    do{                              // sólo recorre cada elemento del array mientras sea distinto de 0
      result += (mask & 0x1) != 0;   // suma un bit del elemento
      mask >>= 1;                    // pasa al siguiente bit
    }while(mask);
  }

  return result;
}

int popcountv3(unsigned *array, int len){

  int j;

  int result = 0;

  for(j = 0; j < len; j++){       // recorre el array

    unsigned x = array[j];
    
    asm("\n"                    // Lo mismo pero en asm
"ini3:                \n\t"
	"shr %[x]     \n\t"     // Desplaza x un bit a la derecha
	"adc $0, %[r] \n\t"     // Si el número menos significativo era un 1, se va al acarreo y lo suma en result
	"cmp $0, %[x] \n\t"     // si x es distinto de 0, hago otra iteración
	"jne ini3     \n\t"

	: [r]"+r" (result)      // e/s, e = 0, s = result
	: [x] "r" (x)           // e = x
	);
  }

  return result;
}

int popcountv4(unsigned *array, int len){

  int i, j, val, result = 0;

  for(j = 0; j < len; j++){       // recorre el array

    unsigned x = array[j];

    val = 0;
    
    for (i = 0; i < 8; i++) {    // Suma en árbol
      val += x & 0x01010101;      
      x >>= 1;
    }
        
    val += (val >> 16);     // Suma la parte más significativa con la parte menos significativa
    val += (val >> 8);      // Suma las dos partes (más y menos significativa) del resultado anterior
      
    result += val & 0x1f;   // Elimina la basura
  }
  return result;
}

int popcountv5(unsigned * array, int len){

  int i;
  int val, result = 0;
  int SSE_mask[] = {0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f};  // para sumar por bloques como en la suma en árbol
  int SSE_LUTb[] = {0x02010100, 0x03020201, 0x03020201, 0x04030302};  // el byte en la posición i-ésima contiene el valor del número de 1s en el número i en binario

  for(i = 0; i < len; i += 4){       // recorre el array
    
    asm("movdqu %[x], %%xmm0     \n\t"
	"movdqa %%xmm0, %%xmm1     \n\t" // dos copias de x
	"movdqu %[m], %%xmm6       \n\t" // máscara
	"psrlw $4, %%xmm1          \n\t" 
	"pand %%xmm6, %%xmm0   \n\t" //; xmm0 - nibbles inferiores
	"pand %%xmm6, %%xmm1   \n\t" //; xmm1 -nibbles superiores

	"movdqu %[l], %%xmm2      \n\t" //; ...como pshufb sobrescribe LUT
	"movdqa %%xmm2, %%xmm3     \n\t" //; ...queremos 2 copias
	"pshufb %%xmm0, %%xmm2 \n\t" //; xmm2 = vector popcount inferiores
	"pshufb %%xmm1, %%xmm3 \n\t" //; xmm3 = vector popcount superiores

	"paddb   %%xmm2, %%xmm3 \n\t"  //; xmm3 - vector popcount bytes
	"pxor    %%xmm0, %%xmm0 \n\t"  //; xmm0= 0, 0, 0, 0
	"psadbw  %%xmm0, %%xmm3 \n\t"  //; xmm3= [pcnt bytes..7|pcnt bytes8..15]
	"movhlps %%xmm3, %%xmm0 \n\t"  //; xmm0= [ 0|pcnt bytes0..7]
	"paddd   %%xmm3, %%xmm0 \n\t"  //; xmm0= [no usado|pcnt bytes0..15]
	"movd    %%xmm0, %[val] \n\t"
	: [val] "=r" (val)             //; salida = val (número de 1s del elemento)
	: [x] "m" (array[i]),     //; operandos de memoria, todos de entrada
	  [m] "m" (SSE_mask[0]),
	  [l] "m" (SSE_LUTb[0])
	);
    result += val; 
  }
  return result;
}

int popcountv6(unsigned* array, int len){

  int i;
  unsigned x;
  int val, result = 0;

  for(i = 0; i < len; i++){       // recorre el array

    x = array[i];

    asm("popcnt %[x], %[val]"    // almacena en val el número de 1s de x
	: [val] "=r" (val)       // salida, val = nº de 1s en x
	: [x] "r" (x)            // entrada, el elemento del array x
	);
    result += val;               // se añade al resultado total
  }

  return result;
}

int popcountv7(unsigned* array, int len){

  int i;
  unsigned x1, x2;
  int val, result = 0;

  for(i = 0; i < len; i += 2){       // recorre el array con saltos de dos elementos
    
    x1 = array[i];                   // dos elementos del array
    x2 = array[i+1];

    asm("popcnt %[x1], %[val] \n\t"  // val = nº de 1s del elemento x1
	"popcnt %[x2], %%edi \n\t"   // EDI = nº de 1s del elemento x2
	"add %%edi, %[val] \n\t"     // val += nº de 1s del elemento x2 (en EDI)
	: [val] "=&r" (val)          // salida sobreescrita (early clobber). nº de 1s en x1 + nº de elementos den x2
	: [x1] "r" (x1),             // entrada, elemento x1
	  [x2] "r" (x2)              // entrada, elemento x2
	: "edi"
	);

    result += val;
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

    crono(popcountv1, "popcountv1 (con bucle for)");
    crono(popcountv2, "popcountv2 (con bucle while)");
    crono(popcountv3, "popcountv3 (con bloque asm)");
    crono(popcountv4, "popcountv4 (suma en árbol)");
    crono(popcountv5, "popcountv5 (SSSE3 pshufb)");
    crono(popcountv6, "popcountv6 (SSSE4 popcount");
    crono(popcountv7, "popcountv7 (popcount 64 bits");
    
    exit(0);
}
