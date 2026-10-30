from DSA import *


class FixedArrayQueue:
    def __init__(self, size):
        self.data = objArray(size)
        self.N = 0
        self.tail = len(self.data) - 1
        self.head = 0

    def enqueue(self, e):
        self.tail = (self.tail + 1) % len(self.data)
        self.data[self.tail] = e
        self.N += 1

    def dequeue(self):
        item = self.data[self.head]
        self.data[self.head] = None
        self.head = (self.head + 1) % len(self.data)
        self.N -= 1
        return item

    def isEmpty(self):
        return self.N == 0


if __name__ == "__main__":
    queue = FixedArrayQueue(100)
    while not stdIsEmpty():
        queue.enqueue(stdReadString())
    while not queue.isEmpty():
        print(queue.dequeue(), end=" ")
    print()
