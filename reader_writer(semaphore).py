import threading
import time
import random

# Shared resource (initialized to None to indicate no data available)
shared_resource = None

# Semaphores
mutex = threading.Semaphore(1)         # To protect read_count
resource_access = threading.Semaphore(1)  # To protect the shared resource
writer_done = threading.Event()  # Event to signal when the writer has written

# Reader count
read_count = 0
stop_event = threading.Event()  # Event to stop the program

# Reader function
def reader(reader_id):
    global read_count
    while not stop_event.is_set():
        writer_done.wait()

        mutex.acquire()
        read_count += 1
        if read_count == 1:
            resource_access.acquire()
        mutex.release()

        print(f"Reader {reader_id} is reading the value: {shared_resource}")
        time.sleep(random.uniform(0.1, 1))

        mutex.acquire()
        read_count -= 1
        if read_count == 0:
            resource_access.release()
        mutex.release()

        time.sleep(random.uniform(0.5, 2))

# Writer function
def writer(writer_id):
    global shared_resource
    while not stop_event.is_set():
        resource_access.acquire()

        shared_resource = random.randint(1, 100)
        print(f"Writer {writer_id} updated the value to: {shared_resource}")
        writer_done.set()
        time.sleep(random.uniform(0.5, 1))

        resource_access.release()

        time.sleep(random.uniform(1, 3))

# Main function
if _name_ == "_main_":
    readers = [threading.Thread(target=reader, args=(i,)) for i in range(3)]
    writers = [threading.Thread(target=writer, args=(i,)) for i in range(1)]

    for r in readers:
        r.start()
    for w in writers:
        w.start()

    # Stop threads after 10 seconds
    time.sleep(10)
    stop_event.set()

    for r in readers:
        r.join()
    for w in writers:
        w.join()
    print("Program terminated.")
