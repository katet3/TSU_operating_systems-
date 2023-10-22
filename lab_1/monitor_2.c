/*
Монитор на основе создания записей в буфер

Эта задача моделирует ситуацию, в которой есть производитель, 
который помещает данные (записи) в буфер, и потребитель, который 
извлекает данные из буфера. Задача состоит в том, чтобы правильно 
синхронизировать их работу, чтобы избежать конфликтов и обеспечить 
правильное обслуживание буфера.
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


/*
Константы
*/
#define TRUE 1
#define FALSE 0

/*
Семафор для синхронизации и координации доступа к общим ресурсам
*/
int ready = 0;

/*
Инициализирует мьютекс с использованием макроса PTHREAD_MUTEX_INITIALIZER
Условные переменные cond1 для синхронизации потоков производителя и потребителя
*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;


//
void producer_up() {
    pthread_mutex_lock(&mutex);
    if (ready == 1) {
        pthread_mutex_unlock(&mutex);
        return;
    }

    ready = 1;
    printf("Provided\n");
    pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&mutex);
}


//
void consumer_up() {
    pthread_mutex_lock(&mutex);
    while (ready == 0)
    {
        pthread_cond_wait(&cond1, &mutex);
        printf("Awoke\n");
    }

    ready = 0;
    printf("Consumed\n");
    pthread_mutex_unlock(&mutex);
}


// Функция производителя
void* producer(void* arg) {
    while (TRUE) {
        producer_up();
    }
    return NULL;
}


// Функция потребителя
void* consumer(void* arg) {
    while (TRUE) {
        consumer_up();
    }
    return NULL;
}


int main() {
    pthread_t producer_thread, consumer_thread;

    // Создание потока производителя
    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0) {
        perror("Failed to create producer thread");
        exit(EXIT_FAILURE);
    }

    // Создание потока потребителя
    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(EXIT_FAILURE);
    }

    // Ожидание завершения потоков
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}
