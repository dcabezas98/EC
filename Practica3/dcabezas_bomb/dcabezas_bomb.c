// Bomba de David Cabezas Berrido

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <string.h>	// para strncmp()/strlen()
#include <sys/time.h>	// para gettimeofday(), struct timeval

char p2[]="n";
char p4[]="o";
char p6[]="r";
char p8[]="c";
char p10[]="a";

int  c1  = 8004;
int  c2  = 1618;

void boom(){
  printf("***************\n");
  printf("*** BOOM!!! ***\n");
  printf("***************\n");
  exit(-1);
}

void defused(){
  printf("·························\n");
  printf("··· bomba desactivada ···\n");
  printf("·························\n");
  exit(0);
}

int main(){
  
  char p1[]="i";
  char p3[]="c";
  char p5[]="r";
  char p7[]="e";
  char p9[]="t";
  
#define SIZE 100
  char pass[SIZE];
  int  pasv;
#define TLIM 60
  struct timeval tv1, tv2;	// gettimeofday() secs-usecs

  char p[16];
  
  sprintf(p,"%s%s%s%s%s%s%s%s%s%s\n",p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);

  gettimeofday(&tv1, NULL);

  printf("Introduce la contraseña: ");
  fgets(pass, SIZE, stdin);
  if (strcmp(pass, p)){
    printf("La contraseña es incorrecta\n");
    boom();
  }

  gettimeofday(&tv2,NULL);
  if (tv2.tv_sec - tv1.tv_sec > TLIM){
    printf("Time's over!\n");
    boom();
  }

  printf("Introduce el código: ");
  scanf("%i", &pasv);
  if (pasv - c2 != c1){
    printf("Código incorrecto\n");
    boom();
  }

  gettimeofday(&tv1, NULL);
  if (tv1.tv_sec - tv2.tv_sec > TLIM){
    printf("Time's over!\n");
    boom();
  }

  defused();
}
