#include <stdio.h>
#include <stdlib.h>

// Function to check if a page is already in frames
int isPageInFrames(int frames[], int numFrames, int page) {
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == page) {
            return 1; // Page found
        }
    }
    return 0; // Page not found
}

// Function to implement FIFO Page Replacement
int fifoPageReplacement(int pages[], int numPages, int numFrames) {
    int frames[numFrames];  // Array to hold the current pages in memory
    int pageOrder[numFrames]; // Array to track the order of pages for FIFO
    int pageFaults = 0;
    int pageHits = 0;
    int front = 0;  // Front of the queue for FIFO
    int size = 0;   // Current size of the queue

    // Initialize frames and pageOrder to -1 (empty state)
    for (int i = 0; i < numFrames; i++) {
        frames[i] = -1;
        pageOrder[i] = -1;
    }

    for (int i = 0; i < numPages; i++) {
        int page = pages[i];

        if (!isPageInFrames(frames, numFrames, page)) {
            // Page fault occurs
            pageFaults++;

            // Replace the oldest page if frames are full
            if (size == numFrames) {
                int oldestPageIndex = front;
                frames[oldestPageIndex] = page;
                pageOrder[oldestPageIndex] = page;
                front = (front + 1) % numFrames; // Move front pointer
            } else {
                // Add the page to the next available slot
                frames[size] = page;
                pageOrder[size] = page;
                size++;
            }

            printf("Page: %d | Page Fault | Frames: ", page);
        } else {
            // Page hit occurs
            pageHits++;
            printf("Page: %d | Page Hit   | Frames: ", page);
        }

        // Output the current state of frames
        for (int j = 0; j < numFrames; j++) {
            if (frames[j] != -1) {
                printf("%d ", frames[j]);
            } else {
                printf("- ");
            }
        }
        printf("\n");
    }

    printf("Total Page Faults: %d\n", pageFaults);
    printf("Total Page Hits: %d\n", pageHits);

    return pageFaults;
}

int main() {
    int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5}; // Example page reference sequence
    int numPages = sizeof(pages) / sizeof(pages[0]);
    int numFrames = 3; // Number of frames in memory

    int faults = fifoPageReplacement(pages, numPages, numFrames);
    return 0;
}
