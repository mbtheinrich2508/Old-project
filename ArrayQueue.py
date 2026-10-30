from DSA import *


class ArrayQueue:
    def __init__(self):
        self.data = objArray(2)
        self.N = 0
        self.tail = len(self.data) - 1
        self.head = 0

    def __resize(self, capacity):
        copy = objArray(capacity)
        for i in range(self.N):
            copy[i] = self.data[(self.head + i) % len(self.data)]
            # The modulo operator ensures that we wrap around the array
        self.data = copy
        self.head = 0
        self.tail = self.N - 1

    def enqueue(self, e):
        if self.N == len(self.data):
            self.__resize(len(self.data) * 2)
        self.tail = (self.tail + 1) % len(self.data)
        self.data[self.tail] = e
        self.N += 1

    def dequeue(self):
        item = self.data[self.head]
        self.data[self.head] = None
        self.head = (self.head + 1) % len(self.data)
        self.N -= 1
        if self.N > 0 and self.N == len(self.data) // 4:
            self.__resize(len(self.data) // 2)
        return item

    def isEmpty(self):
        return self.N == 0


if __name__ == "__main__":
    queue = ArrayQueue()
    while not stdIsEmpty():
        queue.enqueue(stdReadString())
    while not queue.isEmpty():
        print(queue.dequeue(), end=" ")
    print()
