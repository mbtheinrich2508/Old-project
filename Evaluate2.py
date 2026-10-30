from ArrayStack2 import *
from DSA import *

vals = Stack()
while not stdIsEmpty():
    s = stdReadString()
    if s == "+":
        vals.push(vals.pop() + vals.pop())
    elif s == "*":
        vals.push(vals.pop() * vals.pop())
    else:
        vals.push(float(s))

print(vals.pop())
