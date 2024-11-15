#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Initialize the semaphores and mutex
sem_t wrt;                  // Semaphore for writer (exclusive access to writer)
pthread_mutex_t mutex;       // Mutex for protecting access to read_count
int read_count = 0;          // Number of readers currently reading
int data = 0;                // Shared resource (data)

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    for (int i = 0; i < 5; i++) {
        // Writer wants to write, so wait for the wrt semaphore
        sem_wait(&wrt);

        // Critical section: Write to the shared resource
        data++;
        printf("Writer %d is writing data: %d\n", writer_id, data);

        // Signal that writing is done
        sem_post(&wrt);

        // Simulate writing time
        sleep(1);
    }
    pthread_exit(NULL);
}

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    for (int i = 0; i < 5; i++) {
        // Lock the mutex before accessing the read_count
        pthread_mutex_lock(&mutex);
        read_count++;

        // If this is the first reader, block the writer
        if (read_count == 1) {
            sem_wait(&wrt);
        }
        pthread_mutex_unlock(&mutex);

        // Critical section: Read the shared resource
        printf("Reader %d is reading data: %d\n", reader_id, data);

        // Lock the mutex before modifying read_count
        pthread_mutex_lock(&mutex);
        read_count--;

        // If this was the last reader, unblock the writer
        if (read_count == 0) {
            sem_post(&wrt);
        }
        pthread_mutex_unlock(&mutex);

        // Simulate reading time
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t read[5], write[3];
    int reader_ids[5], writer_ids[3];

    // Initialize the semaphore and mutex
    sem_init(&wrt, 0, 1);                // wrt semaphore initialized to 1
    pthread_mutex_init(&mutex, NULL);     // mutex initialized

    // Create writer threads
    for (int i = 0; i < 3; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&write[i], NULL, writer, &writer_ids[i]);
    }

    // Create reader threads
    for (int i = 0; i < 5; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&read[i], NULL, reader, &reader_ids[i]);
    }

    // Join all threads
    for (int i = 0; i < 3; i++) {
        pthread_join(write[i], NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(read[i], NULL);
    }

    // Destroy the semaphore and mutex
    sem_destroy(&wrt);
    pthread_mutex_destroy(&mutex);

    return 0;
}
