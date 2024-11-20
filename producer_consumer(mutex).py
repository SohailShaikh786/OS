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

# Termination variables
total_items = 20  # Total number of items to produce/consume
produced_count = 0
consumed_count = 0

# Producer function
def producer(producer_id):
    global produced_count
    while True:
        item = random.randint(1, 100)  # Generate a random item

        with buffer_not_full:  # Acquire buffer lock and check the condition
            # Terminate if all items have been produced
            if produced_count >= total_items:
                break

            while len(buffer) == buffer_capacity:  # Wait if buffer is full
                buffer_not_full.wait()

            # Add item to the buffer
            buffer.append(item)
            produced_count += 1
            print(f"Producer {producer_id} produced: {item}. Buffer: {buffer}")

            # Notify consumers that the buffer is no longer empty
            buffer_not_empty.notify()

        time.sleep(random.uniform(0.5, 1.5))  # Simulate production time

# Consumer function
def consumer(consumer_id):
    global consumed_count
    while True:
        with buffer_not_empty:  # Acquire buffer lock and check the condition
            # Terminate if all items have been consumed
            if consumed_count >= total_items:
                break

            while len(buffer) == 0:  # Wait if buffer is empty
                buffer_not_empty.wait()

            # Remove item from the buffer
            item = buffer.pop(0)
            consumed_count += 1
            print(f"Consumer {consumer_id} consumed: {item}. Buffer: {buffer}")

            # Notify producers that the buffer is no longer full
            buffer_not_full.notify()

        time.sleep(random.uniform(1, 2))  # Simulate consumption time

# Main function
if __name__ == "__main__":
    # Create producer and consumer threads
    producers = [threading.Thread(target=producer, args=(i,)) for i in range(2)]  # 2 producers
    consumers = [threading.Thread(target=consumer, args=(i,)) for i in range(3)]  # 3 consumers

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
