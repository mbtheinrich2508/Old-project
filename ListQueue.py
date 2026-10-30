from DSA import *

class ListQueue:
    class Node:
        def __init__(self, item):
            self.item = item
            self.next = None

    def __init__(self):
        self.first = None
        self.last = None

    def enqueue(self, e):
        oldlast = self.last
        self.last = self.Node(e)
        if self.isEmpty():
            self.first = self.last
        else:
            oldlast.next = self.last

    def dequeue(self):
        item = self.first.item
        self.first = self.first.next
        if self.isEmpty():
            self.last = None
        return item

    def isEmpty(self):
        return self.first == None


if __name__ == "__main__":
    queue = ListQueue()
    while not stdIsEmpty():
        queue.enqueue(stdReadString())
    while not queue.isEmpty():
        print(queue.dequeue(), end=" ")
    print()
