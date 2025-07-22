#include <stdio.h>     
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 3
int buffer[SIZE];
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    for (int i = 0; i < 5; i++) {
        int item = rand() % 100;

        sem_wait(&empty);                  
        pthread_mutex_lock(&mutex);       
        buffer[in] = item;
        printf("Producer produced: %d\n", item);
        in = (in + 1) % SIZE;             

        pthread_mutex_unlock(&mutex);    
        sem_post(&full);                 

        sleep(1); // Simulasi produksi
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 5; i++) {
        sem_wait(&full);                  
        pthread_mutex_lock(&mutex);   

        int item = buffer[out];
        printf("Consumer consumed: %d\n", item);
        out = (out + 1) % SIZE;           

        pthread_mutex_unlock(&mutex);  
        sem_post(&empty);               

        sleep(2); // Simulasi konsumsi
    }
    return NULL;
}

int main() {
    pthread_t prodThread, consThread;

    // Inisialisasi semaphore dan mutex
    sem_init(&empty, 0, SIZE); // Awalnya semua slot kosong
    sem_init(&full, 0, 0);     // Belum ada item
    pthread_mutex_init(&mutex, NULL);

    // Buat thread producer dan consumer
    pthread_create(&prodThread, NULL, producer, NULL);
    pthread_create(&consThread, NULL, consumer, NULL);

    // Tunggu thread selesai
    pthread_join(prodThread, NULL);
    pthread_join(consThread, NULL);

    // Bersihkan semaphore dan mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
