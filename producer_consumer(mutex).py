import threading
import time
import random

# Shared buffer and its capacity
buffer = []
buffer_capacity = 5

# Mutex lock and condition variables
buffer_lock = threading.Lock()      # Mutex lock for accessing the buffer
buffer_not_full = threading.Condition(buffer_lock)  # Condition for not full buffer
buffer_not_empty = threading.Condition(buffer_lock)  # Condition for not empty buffer

# Producer function
def producer(producer_id):
    while True:
        item = random.randint(1, 100)  # Generate a random item
        with buffer_not_full:  # Acquire buffer lock and check the condition
            while len(buffer) == buffer_capacity:  # Wait if buffer is full
                buffer_not_full.wait()

            # Add item to the buffer
            buffer.append(item)
            print(f"Producer {producer_id} produced: {item}. Buffer: {buffer}")

            # Notify consumers that the buffer is no longer empty
            buffer_not_empty.notify()

        time.sleep(random.uniform(0.5, 1.5))  # Simulate production time

# Consumer function
def consumer(consumer_id):
    while True:
        with buffer_not_empty:  # Acquire buffer lock and check the condition
            while len(buffer) == 0:  # Wait if buffer is empty
                buffer_not_empty.wait()

            # Remove item from the buffer
            item = buffer.pop(0)
            print(f"Consumer {consumer_id} consumed: {item}. Buffer: {buffer}")

            # Notify producers that the buffer is no longer full
            buffer_not_full.notify()

        time.sleep(random.uniform(1, 2))  # Simulate consumption time

# Main function
if _name_ == "_main_":
    # Create producer and consumer threads
    producers = [threading.Thread(target=producer, args=(i,)) for i in range(2)]  # 2 producers
    consumers = [threading.Thread(target=consumer, args=(i,)) for i in range(3)]  # 3 consumers

    # Start threads
    for p in producers:
        p.start()
    for c in consumers:
        c.start()

    # Join threads (optional, for indefinite execution)
    for p in producers:
        p.join()
    for c in consumers:
        c.join()
