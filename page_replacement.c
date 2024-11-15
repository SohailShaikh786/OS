#include <stdio.h>

// FIFO page replacement function
void fifo(int pages[], int n, int capacity) {
    int frames[capacity], faults = 0, hits = 0, front = 0;
    for (int i = 0; i < capacity; i++)
        frames[i] = -1; // Initialize frames as empty
    
    for (int i = 0; i < n; i++) {
        int found = 0;
        // Check if the page is already in the frames
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                hits++; // Increment page hit counter
                break;
            }
        }
        // If the page is not found, replace using FIFO
        if (!found) {
            frames[front] = pages[i];
            front = (front + 1) % capacity;
            faults++; // Increment page fault (miss) counter
        }

        // Display the current state of frames
        printf("Page %d: ", pages[i]);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf("\n");
    }
    
    float hit_ratio = (float)hits / n;
    float miss_ratio = (float)faults / n;

    printf("Total Page Faults (FIFO): %d\n", faults);
    printf("Total Page Hits (FIFO): %d\n", hits);
    printf("Page Hit Ratio (FIFO): %.2f\n", hit_ratio);
    printf("Page Miss Ratio (FIFO): %.2f\n\n", miss_ratio);
}

// Helper function for LRU page replacement to find the least recently used page
int findLRU(int time[], int n) {
    int i, min = time[0], pos = 0;
    for (i = 1; i < n; i++) {
        if (time[i] < min) {
            min = time[i];
            pos = i;
        }
    }
    return pos;
}

// LRU page replacement function
void lru(int pages[], int n, int capacity) {
    int frames[capacity], time[capacity], faults = 0, hits = 0, counter = 0;

    for (int i = 0; i < capacity; i++) 
        frames[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;
        // Check if the page is already in frames
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                time[j] = ++counter; // Update time for LRU
                hits++; // Increment page hit counter
                break;
            }
        }
        // Page not found, so replace LRU page
        if (!found) {
            int pos = findLRU(time, capacity);
            frames[pos] = pages[i];
            time[pos] = ++counter;
            faults++; // Increment page fault (miss) counter
        }

        // Display the current state of frames
        printf("Page %d: ", pages[i]);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf("\n");
    }

    float hit_ratio = (float)hits / n;
    float miss_ratio = (float)faults / n;

    printf("Total Page Faults (LRU): %d\n", faults);
    printf("Total Page Hits (LRU): %d\n", hits);
    printf("Page Hit Ratio (LRU): %.2f\n", hit_ratio);
    printf("Page Miss Ratio (LRU): %.2f\n\n", miss_ratio);
}

// Helper function for Optimal page replacement to predict the farthest page use
int predict(int pages[], int frames[], int n, int index, int capacity) {
    int farthest = index, pos = -1;
    for (int i = 0; i < capacity; i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        // If a page is never referenced again, return it
        if (j == n)
            return i;
    }
    // If all pages are referenced again, return the farthest
    return (pos == -1) ? 0 : pos;
}

// Optimal page replacement function
void optimal(int pages[], int n, int capacity) {
    int frames[capacity], faults = 0, hits = 0;
    for (int i = 0; i < capacity; i++)
        frames[i] = -1; // Initialize frames as empty

    for (int i = 0; i < n; i++) {
        int found = 0;
        // Check if the page is already in the frames
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                hits++; // Increment page hit counter
                break;
            }
        }
        // Page not found, so replace using optimal strategy
        if (!found) {
            if (i < capacity) {
                frames[i] = pages[i];
            } else {
                int pos = predict(pages, frames, n, i + 1, capacity);
                frames[pos] = pages[i];
            }
            faults++; // Increment page fault (miss) counter
        }

        // Display the current state of frames
        printf("Page %d: ", pages[i]);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf("\n");
    }

    float hit_ratio = (float)hits / n;
    float miss_ratio = (float)faults / n;

    printf("Total Page Faults (Optimal): %d\n", faults);
    printf("Total Page Hits (Optimal): %d\n", hits);
    printf("Page Hit Ratio (Optimal): %.2f\n", hit_ratio);
    printf("Page Miss Ratio (Optimal): %.2f\n\n", miss_ratio);
}

// Main function using switch-case for selection
int main() {
    int n, capacity, choice;
    printf("Enter number of pages: ");
    scanf("%d", &n);
    
    int pages[n];
    printf("Enter the page reference string: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter the number of frames: ");
    scanf("%d", &capacity);

    // Present user with options to select the page replacement algorithm
    printf("\nChoose Page Replacement Algorithm:\n");
    printf("1. FIFO\n");
    printf("2. LRU\n");
    printf("3. Optimal\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\nFIFO Page Replacement:\n");
            fifo(pages, n, capacity);
            break;
        case 2:
            printf("\nLRU Page Replacement:\n");
            lru(pages, n, capacity);
            break;
        case 3:
            printf("\nOptimal Page Replacement:\n");
            optimal(pages, n, capacity);
            break;
        default:
            printf("Invalid choice!\n");
    }

    return 0;
}
