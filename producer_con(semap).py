import threading
import time
import random

# Buffer and its capacity
buffer = []
buffer_capacity = 5

# Semaphores
empty = threading.Semaphore(buffer_capacity)  # Tracks empty slots in the buffer
full = threading.Semaphore(0)                # Tracks filled slots in the buffer
mutex = threading.Semaphore(1)               # Ensures mutual exclusion while accessing the buffer

# Shared flag to signal termination
terminate = False
iterations = 10  # Total number of items to produce/consume per producer/consumer

# Producer function
def producer(producer_id):
    global terminate
    for _ in range(iterations):  # Produce a fixed number of items
        item = random.randint(1, 100)  # Generate a random item
        empty.acquire()  # Wait if the buffer is full
        mutex.acquire()  # Lock access to the buffer

        # Add the item to the buffer
        buffer.append(item)
        print(f"Producer {producer_id} produced: {item}. Buffer: {buffer}")

        mutex.release()  # Unlock access to the buffer
        full.release()   # Signal that there is a new item in the buffer

        time.sleep(random.uniform(0.5, 1.5))  # Simulate production time
    
    print(f"Producer {producer_id} finished producing.")

# Consumer function
def consumer(consumer_id):
    global terminate
    for _ in range(iterations):  # Consume a fixed number of items
        full.acquire()  # Wait if the buffer is empty
        mutex.acquire()  # Lock access to the buffer

        # Remove the item from the buffer
        item = buffer.pop(0)
        print(f"Consumer {consumer_id} consumed: {item}. Buffer: {buffer}")

        mutex.release()  # Unlock access to the buffer
        empty.release()  # Signal that there is an empty slot in the buffer

        time.sleep(random.uniform(1, 2))  # Simulate consumption time
    
    print(f"Consumer {consumer_id} finished consuming.")

# Main function
if __name__ == "__main__":
    # Create producer and consumer threads
    num_producers = 2
    num_consumers = 3
    producers = [threading.Thread(target=producer, args=(i,)) for i in range(num_producers)]
    consumers = [threading.Thread(target=consumer, args=(i,)) for i in range(num_consumers)]

    # Start threads
    for p in producers:
        p.start()
    for c in consumers:
        c.start()

    # Wait for all threads to complete
    for p in producers:
        p.join()
    for c in consumers:
        c.join()

    print("All producers and consumers have finished. Program terminated.")
