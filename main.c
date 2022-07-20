#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
 
// Lleva el registro de los números de fibonacci. Está realizada a modo de singleton.
struct {
    // El máximo número de la lista que se ha calculado
    unsigned max_num;
    // Marca las posiciones que se han calculado
    bool *calculado;
    // Tiene los max_num primeros números de fibonacci calculados
    uint64_t *data;
} cache;
 
// Inicializa la estructura cache
void inicializar() {
    cache.max_num = 1;
 
    // Sólo ofrecerá, desde un inicio, los casos bases del 0 y el 1
    cache.calculado = malloc(2 * sizeof(bool));
    cache.calculado[0] = cache.calculado[1] = true;
 
    cache.data = malloc((cache.max_num + 1) * sizeof(uint64_t));
    cache.data[0] = 0;
    cache.data[1] = 1;
}
 
// Hace crecer la capacidad de la cache en caso de que el número a calcular
// sea mayor que el mayor que hay en la estructura
void actualizar(unsigned x) {
    if(x > cache.max_num) {
        
        // Las nuevas posiciones que se crean deben estar a 0 (false)
        void *aux = calloc((x + 1), sizeof(bool));
        memcpy(aux, cache.calculado, (cache.max_num + 1) * sizeof(bool));
        free(cache.calculado);
        cache.calculado = aux;
        cache.max_num = x;
 
        // Las nuevas posiciones pueden tener cualquier dato por lo que
        // realloc es una opción más rápida que todo el código anterior
        aux = realloc(cache.data, (x + 1) * sizeof(uint64_t));
        cache.data = aux;
 
        cache.max_num = x;
    }
}
 
// Libera la memoria capturada para la caché
void finalizar() {
    free(cache.calculado);
    free(cache.data);
}
 
// Devuelve el número de fibonacci n-simo
uint64_t fib(unsigned n) {
    // Mirar si la cache debe crecer
    actualizar(n);
 
    // Comprobar si el número ya fué calculado
    if(!cache.calculado[n]) {
        // En caso de no estarlo calcularlo y meterlo en la caché
        cache.data[n] = fib(n-1) + fib(n-2);
        cache.calculado[n] = true;
    }
 
    // Devolver el número de fibonacci n-simo
    return cache.data[n];
}
 
int main() {
    inicializar();
 
    // 93 es en mayor número n-simo capaz de ser calculado por
    // por una máquina de 64 bits.
    for(unsigned i=93; i > 0; --i) {
        printf("fib(%u) = %I64u\n", i, fib(i));
    }
   
    finalizar();
}
