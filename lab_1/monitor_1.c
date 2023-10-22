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
Константы, N - размер буфера
*/
#define N 100 
#define TRUE 1
#define FALSE 0

/*
buffer - массив размера N, по 4 байта
count, lo, и hi служат счетчиками элементов в буфере и индексами для вставки и удаления элементов
*/
int buffer[N];
int count = 0;
int lo = 0;
int hi = 0;

/*
Инициализирует мьютекс с использованием макроса PTHREAD_MUTEX_INITIALIZER
Условные переменные not_full и not_empty для синхронизации потоков производителя и потребителя
*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;


//
int produce_item(int data) {
    printf("Creation of a record %i\n", hi);
    // Реализация производства записей
    // ...
}


//
int consume_item(int data) {
    printf("Remove of a record %i\n", lo);
    // Реализация потребления записи
    // ...
}


//
void go_to_sleep() {
    pthread_cond_wait(&not_full, &mutex);
}


//
void insert(int val) {
    pthread_mutex_lock(&mutex);
    while (count == N) {
        pthread_cond_signal(&not_empty);
        pthread_cond_wait(&not_full, &mutex);
    }

    buffer[hi] = val;
    hi = (hi + 1) % N;
    count++;

    pthread_mutex_unlock(&mutex);
}


//
int remove_item() {
    int val;

    pthread_mutex_lock(&mutex);
    while (count == 0) {
        pthread_cond_signal(&not_full);
        pthread_cond_wait(&not_empty, &mutex);
    }

    val = buffer[lo];
    lo = (lo + 1) % N;
    count--;

    pthread_mutex_unlock(&mutex);

    return val; 
}


// Функция производителя
void* producer(void* arg) {
    int item;
    while (TRUE) {
        item = produce_item(item); // Производство записей
        insert(item); // Вставка записи в буфер
    }
    return NULL;
}


// Функция потребителя
void* consumer(void* arg) {
    int item;
    while (TRUE) {
        item = remove_item(); // Извлечение записи из буфера
        consume_item(item); // Потребление записи
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
