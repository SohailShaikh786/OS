#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Buffer and its capacity
#define BUFFER_CAPACITY 5
#define MAX_ITEMS 20 // Total number of items to produce/consume
int buffer[BUFFER_CAPACITY];
int count = 0;  // Current number of items in the buffer
int produced_items = 0; // Total items produced
int consumed_items = 0; // Total items consumed

// Mutex and condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Protects access to the buffer
pthread_cond_t buffer_not_full = PTHREAD_COND_INITIALIZER; // Buffer not full condition
pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER; // Buffer not empty condition

// Producer function
void* producer(void* arg) {
    int producer_id = ((int)arg);

    while (1) {
        pthread_mutex_lock(&mutex);

        // Check if we've reached the maximum number of items
        if (produced_items >= MAX_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Wait if the buffer is full
        while (count == BUFFER_CAPACITY) {
            pthread_cond_wait(&buffer_not_full, &mutex);
        }

        // Add the item to the buffer
        int item = rand() % 100; // Generate a random item
        buffer[count] = item;
        count++;
        produced_items++;
        printf("Producer %d produced: %d. Buffer size: %d\n", producer_id, item, count);

        // Signal that the buffer is no longer empty
        pthread_cond_signal(&buffer_not_empty);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 2 + 1); // Simulate production time
    }

    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    int consumer_id = ((int)arg);

    while (1) {
        pthread_mutex_lock(&mutex);

        // Check if all items have been consumed
        if (consumed_items >= MAX_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Wait if the buffer is empty
        while (count == 0) {
            pthread_cond_wait(&buffer_not_empty, &mutex);
        }

        // Remove an item from the buffer
        int item = buffer[--count];
        consumed_items++;
        printf("Consumer %d consumed: %d. Buffer size: %d\n", consumer_id, item, count);

        // Signal that the buffer is no longer full
        pthread_cond_signal(&buffer_not_full);
        pthread_mutex_unlock(&mutex);

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

    printf("All items have been produced and consumed.\n");

    return 0;
}
