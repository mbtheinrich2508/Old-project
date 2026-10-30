from DSA import *


class Stack:
    class Node:
        def __init__(self, item):
            self.item = item
            self.next = None

    def __init__(self):
        self.first = None

    def push(self, e):
        oldfirst = self.first
        self.first = self.Node(e)
        self.first.next = oldfirst

    def pop(self):
        item = self.first.item
        self.first = self.first.next
        return item

    def isEmpty(self):
        return self.first == None


if __name__ == "__main__":
    stack = Stack()
    while not stdIsEmpty():
        stack.push(stdReadString())
    while not stack.isEmpty():
        print(stack.pop(), end=" ")
    print()
