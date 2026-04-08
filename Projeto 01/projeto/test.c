#include <stdio.h>
#include <stdlib.h>

typedef __uint32_t uint32_t;
typedef __int32_t int32_t;

uint32_t *multiplica_polinomio(uint32_t grau, const uint32_t *coef1, const uint32_t *coef2)
{
    // Você deve implementar essa função
    // Os dois polinômios de entrada têm grau "grau", ou seja, cada um tem "grau + 1" coeficientes.
    // O resultado da multiplicação terá grau no máximo 2*grau, ou seja, terá no máximo 2*grau + 1 coeficientes.
    // Você deve implementar a multiplicação usando o método de Karatsuba, que é mais eficiente do que o método ingênuo para polinômios de grau grande.
    // Você pode criar sub-funções auxiliares

    uint32_t *resultado = (uint32_t *) malloc(sizeof(uint32_t) * 2 * grau + 1);
    uint32_t i, j, g;

    //for (i = 0; i <= 2 * grau; i++)
        //resultado[i] = 0;

    for (i = 0; i <= grau; i++){
        for (j = 0; j <= grau; j++){
            g = i + j;
            resultado[g] += coef1[i] * coef2[j];
        }
    }
    
    return resultado;
}

// Verifica erros
int32_t avalia_polinomio(int32_t x, uint32_t grau, const uint32_t *coef)
{
    // Você deve implementar essa função
    // Avalia o polinômio de grau "grau" com coeficientes "coef" no ponto "x"

    int32_t i, resultado = 0;
    int32_t x_grau_n = 1;

    for (i = grau; i >= 0; i--){
        resultado += coef[i] * x_grau_n;
        x_grau_n *= x;
    }

    return resultado;
}

int main(){
    uint32_t grau = 5;
    uint32_t coef1_arr[] = {1, 25, 238, 1054, 2065, 1225};
    uint32_t coef2_arr[] = {92, 49, 52, 86, 75, 10};

    uint32_t *coef1 = coef1_arr;
    uint32_t *coef2 = coef2_arr;

    uint32_t *resultado = multiplica_polinomio(grau, coef1, coef2);

    uint32_t i;
    for (i = 0; i <= 2*grau; i++)
        printf("%d ", resultado[i]);

    printf("\n");

    int32_t y = avalia_polinomio(-40, 2*grau, resultado);

    printf("%d\n", y);

    //free(resultado);

    return 0;
}