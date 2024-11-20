#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Maximum number of operations for readers and writers
#define MAX_OPERATIONS 20

sem_t rw_mutex;      // Semaphore to ensure mutual exclusion for writers
sem_t mutex;         // Semaphore to protect the read_count variable
int read_count = 0;  // Number of readers currently accessing the shared resource

// Shared resource
int shared_data = 0;
int total_operations = 0; // Tracks total read/write operations
pthread_mutex_t op_mutex = PTHREAD_MUTEX_INITIALIZER; // Protects total_operations

// Writer function
void* writer(void* arg) {
    int writer_id = ((int)arg);

    while (1) {
        pthread_mutex_lock(&op_mutex);
        if (total_operations >= MAX_OPERATIONS) {
            pthread_mutex_unlock(&op_mutex);
            break;
        }
        total_operations++;
        pthread_mutex_unlock(&op_mutex);

        sem_wait(&rw_mutex); // Lock the resource for writing

        // Writing to the shared resource
        shared_data += 1;
        printf("Writer %d wrote: %d\n", writer_id, shared_data);

        sem_post(&rw_mutex); // Release the resource
        sleep(rand() % 2 + 1); // Simulate writing time
    }

    return NULL;
}

// Reader function
void* reader(void* arg) {
    int reader_id = ((int)arg);

    while (1) {
        pthread_mutex_lock(&op_mutex);
        if (total_operations >= MAX_OPERATIONS) {
            pthread_mutex_unlock(&op_mutex);
            break;
        }
        total_operations++;
        pthread_mutex_unlock(&op_mutex);

        sem_wait(&mutex); // Lock to update read_count
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex); // First reader locks the resource
        }
        sem_post(&mutex); // Release the lock on read_count

        // Reading the shared resource
        printf("Reader %d read: %d\n", reader_id, shared_data);

        sem_wait(&mutex); // Lock to update read_count
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex); // Last reader releases the resource
        }
        sem_post(&mutex); // Release the lock on read_count

        sleep(rand() % 2 + 1); // Simulate reading time
    }

    return NULL;
}

// Main function
int main() {
    pthread_t readers[3], writers[2]; // Array of reader and writer threads
    int reader_ids[3] = {1, 2, 3};   // Reader IDs
    int writer_ids[2] = {1, 2};      // Writer IDs

    // Initialize semaphores
    sem_init(&rw_mutex, 0, 1); // Binary semaphore for rw_mutex
    sem_init(&mutex, 0, 1);    // Binary semaphore for mutex

    // Create writer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Create reader threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Join threads
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(readers[i], NULL);
    }

    // Destroy semaphores (cleanup)
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    printf("All operations completed. Exiting.\n");

    return 0;
}
