import threading
import time
import random

# Shared resource (initialized to None to indicate no data available)
shared_resource = None

# Mutex locks
mutex = threading.Lock()          # Protects the reader count
resource_access = threading.Lock()  # Protects the shared resource
writer_done = threading.Event()  # Event to signal when the writer has written

# Reader count
read_count = 0

# Reader function
def reader(reader_id):
    global read_count
    while True:
        # Wait until a writer has written data
        writer_done.wait()

        # Entry section
        with mutex:
            read_count += 1
            if read_count == 1:  # First reader locks the resource
                resource_access.acquire()

        # Critical section (Reading)
        print(f"Reader {reader_id} is reading the value: {shared_resource}")
        time.sleep(random.uniform(0.1, 1))  # Simulate reading time

        # Exit section
        with mutex:
            read_count -= 1
            if read_count == 0:  # Last reader unlocks the resource
                resource_access.release()

        # Simulate delay before reading again
        time.sleep(random.uniform(0.5, 2))

# Writer function
def writer(writer_id):
    global shared_resource
    while True:
        # Entry section
        resource_access.acquire()

        # Critical section (Writing)
        shared_resource = random.randint(1, 100)  # Generate a new value
        print(f"Writer {writer_id} updated the value to: {shared_resource}")
        writer_done.set()  # Signal that the writer has written
        time.sleep(random.uniform(0.5, 1))  # Simulate writing time

        # Exit section
        resource_access.release()

        # Simulate delay before writing again
        time.sleep(random.uniform(1, 3))

# Main function
if _name_ == "_main_":
    # Create reader and writer threads
    readers = [threading.Thread(target=reader, args=(i,)) for i in range(3)]  # 3 readers
    writers = [threading.Thread(target=writer, args=(i,)) for i in range(1)]  # 1 writer

    # Start threads
    for r in readers:
        r.start()
    for w in writers:
        w.start()

    # Stop the program after 10 seconds
    time.sleep(10)
    print("Program terminated.")
    for r in readers:
        r.join(timeout=1)
    for w in writers:
        w.join(timeout=1)
