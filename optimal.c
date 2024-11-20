#include <stdio.h>
#include <stdlib.h>

// Function to check if a page is in frames
int isPageInFrames(int frames[], int numFrames, int page) {
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == page) {
            return 1; // Page found
        }
    }
    return 0; // Page not found
}

// Function to find the page to be replaced based on future usage
int findPageToReplace(int frames[], int numFrames, int pages[], int currentIndex, int numPages) {
    int futureUse[numFrames];
    for (int i = 0; i < numFrames; i++) {
        futureUse[i] = -1; // Initialize as not used in the future
        for (int j = currentIndex + 1; j < numPages; j++) {
            if (frames[i] == pages[j]) {
                futureUse[i] = j; // Record the next occurrence
                break;
            }
        }
    }

    int farthestIndex = 0;
    for (int i = 1; i < numFrames; i++) {
        if (futureUse[farthestIndex] == -1 || (futureUse[i] > futureUse[farthestIndex] && futureUse[i] != -1)) {
            farthestIndex = i; // Choose the page with farthest or no future use
        }
    }

    return farthestIndex;
}

// Function to implement Optimal Page Replacement
int optimalPageReplacement(int pages[], int numPages, int numFrames) {
    int frames[numFrames]; // Holds the current pages in memory
    int pageFaults = 0;
    int pageHits = 0;

    // Initialize frames as empty
    for (int i = 0; i < numFrames; i++) {
        frames[i] = -1;
    }

    for (int i = 0; i < numPages; i++) {
        int page = pages[i];

        if (!isPageInFrames(frames, numFrames, page)) {
            // Page fault occurs
            pageFaults++;

            // If frames are full, replace a page using the optimal strategy
            if (i >= numFrames) {
                int replaceIndex = findPageToReplace(frames, numFrames, pages, i, numPages);
                frames[replaceIndex] = page;
            } else {
                // If frames are not yet full, add the page to the next available slot
                frames[i] = page;
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

    int faults = optimalPageReplacement(pages, numPages, numFrames);
    return 0;
}
