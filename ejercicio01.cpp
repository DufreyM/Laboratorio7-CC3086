#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Cantidad de hilos que usará el programa, en este caso se usarán 8 debido a que son 8 productos de los que se necesitan los cálculos. Cada hilo se encargará de realizarlos.
#define THREAD_NUM 8

// Mutex para proteger la sección crítica.
pthread_mutex_t mutex;

// Estructura que almacena los datos de cada producto.
typedef struct {
    char *nombre;
    int threadId;
    int unidadesVendidas;
    float precioUnitario;
    float costoFijo;
    float ventasPorProducto;
    float utilidadPorProducto;
} Producto;

// Cálculos realizados en paralelo.
void *calculosARealizar(void *arg) {
    Producto *producto = (Producto *)arg;

    // Bloqueo del mutex para la impresión.
    pthread_mutex_lock(&mutex);

    // Inicio del cálculo del hilo.
    printf("calculo hilo %d iniciado\n", producto->threadId);

    // Desbloqueo del mutex para permitir que otros hilos trabajen.
    pthread_mutex_unlock(&mutex);

    // Calculo del monto en ventas por producto.
    producto->ventasPorProducto = producto->unidadesVendidas * producto->precioUnitario;

    // Calculo de la utilidad por producto.
    producto->utilidadPorProducto = producto->ventasPorProducto - (producto->unidadesVendidas * producto->costoFijo);

    // Simulamos el tiempo de cálculo.
    sleep(1);

    // Bloqueo del mutex para la impresión final.
    pthread_mutex_lock(&mutex);

    // Fin del cálculo del hilo.
    printf("calculo hilo %d terminado\n", producto->threadId);

    // Desbloqueo del mutex para permitir que otros hilos trabajen.
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t tidJulio[THREAD_NUM];
    pthread_t tidAgosto[THREAD_NUM];

    // Inicialización del mutex
    pthread_mutex_init(&mutex, NULL);

    Producto productosJulio[THREAD_NUM] = {
        {"Porcion pastel de chocolate", 0, 300, 60.0, 20.0},
        {"White mocha", 1, 400, 32.0, 19.2},
        {"Café americano 8oz", 2, 1590, 22.0, 13.2},
        {"Latte 8oz", 3, 200, 24.0, 17.2},
        {"Toffee coffee", 4, 390, 28.0, 20.1},
        {"Cappuccino 8oz", 5, 1455, 24.0, 17.2},
        {"S'mores Latte", 6, 800, 32.0, 23.0},
        {"Café tostado molido", 7, 60, 60.0, 20.0}
    };

    Producto productosAgosto[THREAD_NUM] = {
        {"Porcion pastel de chocolate", 0, 250, 60.0, 20.0},
        {"White mocha", 1, 380, 32.0, 19.2},
        {"Café americano 8oz", 2, 800, 22.0, 13.2},
        {"Latte 8oz", 3, 250, 24.0, 17.2},
        {"Toffee coffee", 4, 600, 28.0, 20.1},
        {"Cappuccino 8oz", 5, 1200, 24.0, 17.2},
        {"S'mores Latte", 6, 1540, 32.0, 23.0},
        {"Café tostado molido", 7, 15, 60.0, 20.0}
    };

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&tidJulio[i], NULL, calculosARealizar, &productosJulio[i]);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(tidJulio[i], NULL);
    }

    float montoTotalVentasJulio = 0.0;
    float utilidadTotalJulio = 0.0;

    printf("------------------------------------------\n");
    printf("REPORTE DEL MES DE JULIO\n");
    printf("--- Monto Ventas por Producto ---\n");

    for (int i = 0; i < THREAD_NUM; i++) {
        printf("- %s: Q%.2f\n", productosJulio[i].nombre, productosJulio[i].ventasPorProducto);
        montoTotalVentasJulio += productosJulio[i].ventasPorProducto;
    }

    printf("--- Utilidad por Producto ---\n");

    for (int i = 0; i < THREAD_NUM; i++) {
        printf("- %s: Q%.2f\n", productosJulio[i].nombre, productosJulio[i].utilidadPorProducto);
        utilidadTotalJulio += productosJulio[i].utilidadPorProducto;
    }

    printf("--- Monto Total Ventas del Mes: Q%.2f\n", montoTotalVentasJulio);
    printf("--- Utilidad del mes: Q%.2f\n", utilidadTotalJulio);
    printf("------------------------------------------\n");

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&tidAgosto[i], NULL, calculosARealizar, &productosAgosto[i]);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(tidAgosto[i], NULL);
    }

    float montoTotalVentasAgosto = 0.0;
    float utilidadTotalAgosto = 0.0;

    // Impresión del reporte de ventas y utilidades de Agosto.
    printf("------------------------------------------\n");
    printf("REPORTE DEL MES DE AGOSTO\n");
    printf("--- Monto Ventas por Producto ---\n");

    for (int i = 0; i < THREAD_NUM; i++) {
        printf("- %s: Q%.2f\n", productosAgosto[i].nombre, productosAgosto[i].ventasPorProducto);
        montoTotalVentasAgosto += productosAgosto[i].ventasPorProducto;
    }

    printf("--- Utilidad por Producto ---\n");

    for (int i = 0; i < THREAD_NUM; i++) {
        printf("- %s: Q%.2f\n", productosAgosto[i].nombre, productosAgosto[i].utilidadPorProducto);
        utilidadTotalAgosto += productosAgosto[i].utilidadPorProducto;
    }

    printf("--- Monto Total Ventas del Mes: Q%.2f\n", montoTotalVentasAgosto);
    printf("--- Utilidad del mes: Q%.2f\n", utilidadTotalAgosto);
    printf("------------------------------------------\n");

    pthread_mutex_destroy(&mutex);

    return 0;
}
