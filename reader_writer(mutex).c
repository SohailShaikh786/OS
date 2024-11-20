#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Mutexes
pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER; // Protects the shared resource
pthread_mutex_t read_count_mutex = PTHREAD_MUTEX_INITIALIZER; // Protects read_count
pthread_mutex_t op_count_mutex = PTHREAD_MUTEX_INITIALIZER;  // Protects operation_count

int read_count = 0;  // Tracks the number of readers accessing the resource
int shared_data = 0; // Shared resource
int operation_count = 20; // Total operations to perform (read + write)

// Writer function
void* writer(void* arg) {
    int writer_id = *(int*)arg; // Correctly dereference the argument

    while (1) {
        pthread_mutex_lock(&op_count_mutex);
        if (operation_count <= 0) {
            pthread_mutex_unlock(&op_count_mutex);
            break;
        }
        operation_count--;
        pthread_mutex_unlock(&op_count_mutex);

        pthread_mutex_lock(&resource_mutex); // Exclusive access for writers

        // Writing to the shared resource
        shared_data += 1;
        printf("Writer %d wrote: %d\n", writer_id, shared_data);

        pthread_mutex_unlock(&resource_mutex); // Release the resource
        sleep(rand() % 2 + 1); // Simulate writing time
    }

    return NULL;
}

// Reader function
void* reader(void* arg) {
    int reader_id = *(int*)arg; // Correctly dereference the argument

    while (1) {
        pthread_mutex_lock(&op_count_mutex);
        if (operation_count <= 0) {
            pthread_mutex_unlock(&op_count_mutex);
            break;
        }
        operation_count--;
        pthread_mutex_unlock(&op_count_mutex);

        // Entry Section
        pthread_mutex_lock(&read_count_mutex);
        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&resource_mutex); // First reader locks the resource
        }
        pthread_mutex_unlock(&read_count_mutex);

        // Reading the shared resource
        printf("Reader %d read: %d\n", reader_id, shared_data);

        // Exit Section
        pthread_mutex_lock(&read_count_mutex);
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&resource_mutex); // Last reader releases the resource
        }
        pthread_mutex_unlock(&read_count_mutex);

        sleep(rand() % 2 + 1); // Simulate reading time
    }

    return NULL;
}

// Main function
int main() {
    pthread_t readers[3], writers[2]; // Arrays for reader and writer threads

    // Dynamically allocate IDs to avoid race conditions
    int* reader_ids[3];
    int* writer_ids[2];

    for (int i = 0; i < 3; i++) {
        reader_ids[i] = malloc(sizeof(int));
        *reader_ids[i] = i + 1;
    }
    for (int i = 0; i < 2; i++) {
        writer_ids[i] = malloc(sizeof(int));
        *writer_ids[i] = i + 1;
    }

    // Create writer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, writer_ids[i]);
    }

    // Create reader threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&readers[i], NULL, reader, reader_ids[i]);
    }

    // Join threads
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
        free(writer_ids[i]); // Free allocated memory
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(readers[i], NULL);
        free(reader_ids[i]); // Free allocated memory
    }

    return 0;
}
