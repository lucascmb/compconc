#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

int ehPrimo(long long int n) {
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;

    return 1;
}

int main(int argc, char const *argv[]) {

    char buf[50];
    double ini, fim;
    long long int inp, i;
    int primeCount = 0;

    // pega o valor com o usuário
    printf("digite N:\n");
    fgets(buf, 20, stdin);

    // converte para long long int
    inp = atoll(buf);

    //pega o tempo inicial
    GET_TIME(ini);

    for (i = 1; i <= inp; i++){
      if(ehPrimo(i)) primeCount++;
    }

    //pega o tempo final
    GET_TIME(fim);

    printf("Tempo = %lf\n", fim-ini);
    printf("Quantidade de números primos = %d\n", primeCount);

    return 0;
}
