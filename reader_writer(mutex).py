import threading
import time
import random

# Shared resource (initialized to None to indicate no data available)
shared_resource = None

# Mutex locks
mutex = threading.Lock()          # Protects the reader count (read_count)
resource_access = threading.Lock()  # Protects the shared resource (shared_resource)

# Reader count
read_count = 0
total_operations = 0  # Track total number of operations (reads + writes)
max_operations = 20  # Set a limit for operations

# Reader function
def reader(reader_id):
    global read_count, total_operations
    while True:
        # Check if we've reached the max number of operations
        if total_operations >= max_operations:
            break

        # Entry section (Locking to access read_count)
        with mutex:
            read_count += 1
            if read_count == 1:  # First reader locks the resource
                resource_access.acquire()

        # Critical section (Reading the shared resource)
        print(f"Reader {reader_id} is reading the value: {shared_resource}")
        total_operations += 1  # Increment total operations
        time.sleep(random.uniform(0.1, 1))  # Simulate reading time

        # Exit section (Releasing the lock on read_count)
        with mutex:
            read_count -= 1
            if read_count == 0:  # Last reader unlocks the resource
                resource_access.release()

        # Simulate delay before reading again
        time.sleep(random.uniform(0.5, 2))

# Writer function
def writer(writer_id):
    global shared_resource, total_operations
    while True:
        # Check if we've reached the max number of operations
        if total_operations >= max_operations:
            break

        # Entry section (Locking the resource for exclusive access)
        resource_access.acquire()

        # Critical section (Writing to the shared resource)
        shared_resource = random.randint(1, 100)  # Generate a new value
        print(f"Writer {writer_id} updated the value to: {shared_resource}")
        total_operations += 1  # Increment total operations
        time.sleep(random.uniform(0.5, 1))  # Simulate writing time

        # Exit section (Releasing the resource lock)
        resource_access.release()

        # Simulate delay before writing again
        time.sleep(random.uniform(1, 3))

# Main function
if __name__ == "__main__":  # Corrected the name guard
    # Create reader and writer threads
    readers = [threading.Thread(target=reader, args=(i,)) for i in range(3)]  # 3 readers
    writers = [threading.Thread(target=writer, args=(i,)) for i in range(1)]  # 1 writer

    # Start threads
    for r in readers:
        r.start()
    for w in writers:
        w.start()

    # Wait for all threads to finish
    for r in readers:
        r.join()
    for w in writers:
        w.join()

    # End statement after reaching the max operations
    print("End of operations. Program terminated.")
