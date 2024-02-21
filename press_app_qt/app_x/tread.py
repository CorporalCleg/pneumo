import threading
import random
import time

def generate_random():
    idx = random.randint(0, 100)
    for i in range(10):
        print(f"from thread with {idx} -> {random.random()}")
        time.sleep(1)

thread1 = threading.Thread(target=generate_random)
thread2 = threading.Thread(target=generate_random)

thread1.start()
thread2.start()