/*---------------------------------------
UNIVERSIDAD DEL VALLE DE GUATEMALA
* Programación de Microprocesadores
* Autor: Leonardo Dufrey Mejía Mejía & Maria José Girón Isidro
* Ver.: 1.00
* Fecha: 2024/09/24
*
*
* References:
* ---------------------------------------
* Fábrica de producción de café tostado molido que funciona con dos máquinas.
*----------------------------------------
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int silo = 0; 
int bodega = 0; 
int continuarProduccion = 1;

pthread_mutex_t candado = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *tostador1(void *arg) { //Hilo para el primer tostador
    while (1) {
        sleep(1); //condición sleep es en segundos y unsleep es en microsegundos
        pthread_mutex_lock(&candado);
        if (!continuarProduccion) {  //Para que se deje de producir si bodega ya esta en el tope
            pthread_mutex_unlock(&candado);
            break;
        }
        silo += 1;
        printf("Tostador 1 produjo 1 libra. Silo: %d\n", silo);
        pthread_cond_signal(&cond); // Despertar a la empacadora si es necesario
        pthread_mutex_unlock(&candado);
    }
    return NULL; 
}

void *tostador2(void *arg) //Hilo del tostador 2
{ 
    while (1) {
        sleep(1); // 1 libra de café por segundo
        pthread_mutex_lock(&candado);
        if (!continuarProduccion) {  //Para que se deje de producir si bodega ya esta en el tope
            pthread_mutex_unlock(&candado);
            break;
        }
        silo += 1;
        printf("Tostador 2 produjo 1 libra. Silo: %d\n", silo);
        pthread_cond_signal(&cond); // Despertar a la empacadora si es necesario
        pthread_mutex_unlock(&candado);
        
    }
    return NULL;
}

void *empacadora(void *arg) {
    while (bodega < 400) { // Empacar 400 bolsas
        pthread_mutex_lock(&candado);
        while (silo < 5) {
            pthread_cond_wait(&cond, &candado); //5 libras de condición
        }
        for (int i = 0; i < 5; i++) {
            silo -= 1;
            bodega += 1;
            printf("Empacadora produjo: 1 bolsa. Bodega: %d\n", bodega);
            sleep(1); 
        }
        if (bodega >= 400) {  //Condición para salir de producción
            continuarProduccion = 0;
            pthread_cond_broadcast(&cond); // Despertar a los tostadores para que revisen la condición
        }
        pthread_mutex_unlock(&candado);
    }
    return NULL; 
}

int main() {
    //Condición de un hilo por máquina
    pthread_t hilo_tostador1, hilo_tostador2, hilo_empacadora;
    
    pthread_create(&hilo_tostador1, NULL, tostador1, NULL);
    pthread_create(&hilo_tostador2, NULL, tostador2, NULL);
    pthread_create(&hilo_empacadora, NULL, empacadora, NULL);
    
    pthread_join(hilo_empacadora, NULL); // Espera a que la empacadora termine

    printf("Bolsas de café en bodega: %d\n", bodega);
    printf("Libras de café en el silo: %d\n", silo);
    return 0;
}