#include <stdio.h>
#include <stdlib.h>

// Function to check if a page is in frames
int isPageInFrames(int frames[], int numFrames, int page) {
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == page) {
            return i; // Return the index if the page is found
        }
    }
    return -1; // Page not found
}

// Function to implement LRU Page Replacement
int lruPageReplacement(int pages[], int numPages, int numFrames) {
    int frames[numFrames];    // Holds the current pages in memory
    int usageOrder[numFrames]; // Tracks usage order (recently used indices)
    int pageFaults = 0;
    int pageHits = 0;

    // Initialize frames and usageOrder arrays
    for (int i = 0; i < numFrames; i++) {
        frames[i] = -1;       // Empty frame
        usageOrder[i] = -1;   // No usage order
    }

    for (int i = 0; i < numPages; i++) {
        int page = pages[i];
        int pageIndex = isPageInFrames(frames, numFrames, page);

        if (pageIndex == -1) {
            // Page fault occurs
            pageFaults++;

            // Find the least recently used (LRU) frame to replace
            int lruIndex = 0;
            for (int j = 1; j < numFrames; j++) {
                if (usageOrder[j] < usageOrder[lruIndex]) {
                    lruIndex = j;
                }
            }

            // Replace the LRU page with the new page
            frames[lruIndex] = page;
            usageOrder[lruIndex] = i;

            printf("Page: %d | Page Fault | Frames: ", page);
        } else {
            // Page hit occurs
            pageHits++;

            // Update the usage order for the page
            usageOrder[pageIndex] = i;

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

    int faults = lruPageReplacement(pages, numPages, numFrames);
    return 0;
}
