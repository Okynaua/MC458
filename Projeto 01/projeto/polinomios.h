#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

uint32_t *add(uint32_t m, uint32_t k, const uint32_t *coef1, const uint32_t *coef2){

    uint32_t n = (m > k) ? m : k;
    uint32_t *resultado = (uint32_t *) calloc(n, sizeof(uint32_t));

    uint32_t i;
    for (i = 0; i < n; i++) {
        uint32_t a = (i < m) ? coef1[i] : 0;
        uint32_t b = (i < k) ? coef2[i] : 0;
        resultado[i] = a + b;
    }

    return resultado;
}

uint32_t *sub(uint32_t m, uint32_t k, const uint32_t *coef1, const uint32_t *coef2){

    uint32_t n = (m > k) ? m : k;
    uint32_t *resultado = (uint32_t *) calloc(n, sizeof(uint32_t));

    uint32_t i;
    for (i = 0; i < n; i++) {
        uint32_t a = (i < m) ? coef1[i] : 0;
        uint32_t b = (i < k) ? coef2[i] : 0;
        resultado[i] = a - b;
    }

    return resultado;
}

// Baseado na Aula 09 do dia 24 de março de 2026 slide 10 "Algoritmo de Karatsuba" do Prof. Dr. Hilder Vitor Lima Pereira de MC458
uint32_t *multiplica_polinomio(uint32_t grau, const uint32_t *coef1, const uint32_t *coef2)
{
    uint32_t n = grau + 1;
    uint32_t *resultado = (uint32_t *) calloc(2 * grau + 1, sizeof(uint32_t));

    // Caso base (n <= 16): Multiplicação ingênua (O(n^2))
    if (n <= 16){
        uint32_t i, j, k;
        for (i = 0; i <= grau; i++){
            for (j = 0; j <= grau; j++){
                k = i + j;
                resultado[k] += coef1[i] * coef2[j];
            }
        }
        return resultado;
    }

    uint32_t m = n/2;
    uint32_t k = n - m;

    uint32_t *uE = coef1;
    uint32_t *uD = coef1 + m;
    uint32_t *vE = coef2;
    uint32_t *vD = coef2 + m;

    uint32_t *y0 = add(m, k, uE, uD);
    uint32_t *y1 = add(m, k, vE, vD);

    uint32_t *y  = multiplica_polinomio(k - 1, y0, y1);
    uint32_t *x0 = multiplica_polinomio(m, uE, vE);
    uint32_t *x2 = multiplica_polinomio(k, uD, vD);

    uint32_t *temp = sub(2*k - 1, 2*m - 1, y, x0);
    uint32_t *x1 = sub(2*k - 1, 2*k - 1, temp, x2);

    uint32_t i;

    // x0
    for (i = 0; i < 2*m - 1; i++)
        resultado[i] += x0[i];

    // x1 (shift m)
    for (i = 0; i < 2*k - 1; i++)
        resultado[i + m] += x1[i];

    // x2 (shift 2m)
    for (i = 0; i < 2*k - 1; i++)
        resultado[i + 2*m] += x2[i];

    // liberar memória auxiliar
    free(y0); free(y1); free(y);
    free(x0); free(x1); free(x2);
    free(temp);

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


// Decodifica
uint32_t *divide_por_binomio(uint32_t grau, uint32_t *coef, int32_t raiz)
{
    // Divide o polinomio de grau 'grau' por um binomio do tipo (x - raiz), onde raiz é uma raiz do polinomio, ou seja, P(raiz) == 0
    // Dividir por um binomio vai diminuir um grau
    uint32_t *resultado = (uint32_t *)calloc(grau, sizeof(uint32_t));
    resultado[0] = coef[0]; // O coeficiente de maior grau do resultado é igual ao coeficiente de maior grau do polinômio original
    for(uint32_t i = 1; i < grau; i++)
    {
        resultado[i] = coef[i] + raiz * resultado[i - 1];
    }
    return resultado;
}

uint32_t *decodifica_mensagem(uint32_t grau, uint32_t *coef, uint32_t n_raizes, int32_t *raizes)
{
    // Decodifica a mensagem original M(X) a partir do polinômio codificado P(X) e das raízes fornecidas
    uint32_t *coeficientes = (uint32_t *)calloc(grau + 1, sizeof(uint32_t));
    // Copia P
    for (uint32_t i = 0; i <= grau; i++)
    {
        coeficientes[i] = coef[i];
    }
    for (uint32_t i = 0; i < n_raizes; i++)
    {
        uint32_t *temp = coeficientes;
        coeficientes = divide_por_binomio(grau - i, temp, raizes[i]);
        free(temp); // Libera a memória da iteração anterior para evitar memory leak
    }
    return coeficientes;
}


_Bool verifica_erros(uint32_t grau, const uint32_t *coef, uint32_t n_raizes, const int32_t *raizes)
{
    for (uint32_t i = 0; i < n_raizes; i++)
    {
        if (avalia_polinomio(raizes[i], grau, coef) != 0)
        {
            return 1; // erro detectado
        }
    }
    return 0;
}

// Adiciona ruido
void adiciona_ruido(uint32_t grau, uint32_t *coef, uint32_t m)
{
    // Adiciona ruído em m coeficientes aleatórios do polinômio
    for (uint32_t i = 0; i < m; i++)
    {
        uint32_t indice = rand() % (grau + 1);
        uint32_t ruido = (rand() % 10) + 1; // Ruído entre 1 e 10
        coef[indice] += ruido;
    }
}