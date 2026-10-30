from DSA import *


class Stack:
    def __init__(self):
        self.data = objArray(1)
        self.N = 0

    def __resize(self, capacity):
        copy = objArray(capacity)
        for i in range(self.N):
            copy[i] = self.data[i]
        self.data = copy

    def push(self, item):
        if self.N == len(self.data):
            self.__resize(len(self.data) + 1)
        self.data[self.N] = item
        self.N += 1

    def pop(self):
        self.N -= 1
        item = self.data[self.N]
        if self.N == 0:
            self.__resize(len(self.data) - 1)
        return item

    def isEmpty(self):
        return self.N == 0


if __name__ == "__main__":
    stack = Stack()
    while not stdIsEmpty():
        stack.push(stdReadString())
    while not stack.isEmpty():
        print(stack.pop(), end=" ")
    print()
