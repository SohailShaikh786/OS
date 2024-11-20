#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Buffer and its capacity
#define BUFFER_CAPACITY 5
#define MAX_ITEMS 20 // Total number of items to produce/consume
int buffer[BUFFER_CAPACITY];
int in = 0;  // Index for the next produced item
int out = 0; // Index for the next consumed item

// Semaphores and Mutex
sem_t empty;  // Tracks empty slots in the buffer
sem_t full;   // Tracks filled slots in the buffer
pthread_mutex_t mutex; // Ensures mutual exclusion for buffer access

int produced_count = 0; // Total items produced
int consumed_count = 0; // Total items consumed

// Producer function
void* producer(void* arg) {
    int producer_id = ((int)arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        if (produced_count >= MAX_ITEMS) { // Stop if maximum items are produced
            pthread_mutex_unlock(&mutex);
            break;
        }
        produced_count++;
        pthread_mutex_unlock(&mutex);

        int item = rand() % 100; // Generate a random item

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        // Add the item to the buffer
        buffer[in] = item;
        printf("Producer %d produced: %d. Buffer index: %d\n", producer_id, item, in);
        in = (in + 1) % BUFFER_CAPACITY;

        pthread_mutex_unlock(&mutex);
        sem_post(&full); // Signal that a new item is in the buffer

        sleep(rand() % 2 + 1); // Simulate production time
    }

    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    int consumer_id = ((int)arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        if (consumed_count >= MAX_ITEMS) { // Stop if maximum items are consumed
            pthread_mutex_unlock(&mutex);
            break;
        }
        consumed_count++;
        pthread_mutex_unlock(&mutex);

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // Remove an item from the buffer
        int item = buffer[out];
        printf("Consumer %d consumed: %d. Buffer index: %d\n", consumer_id, item, out);
        out = (out + 1) % BUFFER_CAPACITY;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Signal that a slot is now free

        sleep(rand() % 3 + 1); // Simulate consumption time
    }

    return NULL;
}

// Main function
int main() {
    pthread_t producers[2]; // Array of producer threads
    pthread_t consumers[3]; // Array of consumer threads

    int producer_ids[2] = {1, 2}; // Producer IDs
    int consumer_ids[3] = {1, 2, 3}; // Consumer IDs

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_CAPACITY); // Initially, all slots are empty
    sem_init(&full, 0, 0);                // Initially, no slots are filled
    pthread_mutex_init(&mutex, NULL);

    // Create producer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // Create consumer threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // Join threads
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Destroy semaphores and mutex (cleanup)
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("All items have been produced and consumed.\n");

    return 0;
}
