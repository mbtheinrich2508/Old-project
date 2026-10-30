from DSA import *


class Stack:
    def __init__(self, capacity):
        self.data = objArray(capacity)
        self.N = 0

    def push(self, item):
        self.data[self.N] = item
        self.N += 1

    def pop(self):
        self.N -= 1
        return self.data[self.N]

    def isEmpty(self):
        return self.N == 0


if __name__ == "__main__":
    stack = Stack(10)
    while not stdIsEmpty():
        stack.push(stdReadString())
    while not stack.isEmpty():
        print(stack.pop(), end=" ")
    print()
