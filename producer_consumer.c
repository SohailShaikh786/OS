#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>   // For sleep()

#define BUFFER_SIZE 5  // Size of the buffer
#define MAX_ITEMS 10   // Number of items to produce/consume

int buffer[BUFFER_SIZE];  // Shared buffer
int count = 0;            // Count of items in the buffer
int produced_items = 0;   // Total produced items counter
int consumed_items = 0;   // Total consumed items counter

// Semaphore and mutex declarations
sem_t empty;              // Tracks empty buffer slots
sem_t full;               // Tracks full buffer slots
pthread_mutex_t mutex;    // Protects access to the buffer

// Producer function
void *producer(void *arg) {
    int item;
    while (produced_items < MAX_ITEMS) {
        item = rand() % 100;  // Produce an item

        sem_wait(&empty);     // Decrement empty count
        pthread_mutex_lock(&mutex); // Lock the buffer

        // Add the item to the buffer
        buffer[count] = item;
        count++;
        produced_items++;
        printf("Producer produced: %d\n", item);

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&full);   // Increment full count

        sleep(1);  // Simulate time taken to produce
    }

    printf("Producer finished producing.\n");
    return NULL;
}

// Consumer function
void *consumer(void *arg) {
    int item;
    while (consumed_items < MAX_ITEMS) {
        sem_wait(&full);  // Decrement full count
        pthread_mutex_lock(&mutex); // Lock the buffer

        // Remove the item from the buffer
        count--;
        item = buffer[count];
        consumed_items++;
        printf("Consumer consumed: %d\n", item);

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&empty);  // Increment empty count

        sleep(1);  // Simulate time taken to consume
    }

    printf("Consumer finished consuming.\n");
    return NULL;
}

int main() {
    pthread_t prod, cons;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);  // Initialize empty to buffer size
    sem_init(&full, 0, 0);             // Initialize full to 0
    pthread_mutex_init(&mutex, NULL);  // Initialize mutex

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for the producer and consumer threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
