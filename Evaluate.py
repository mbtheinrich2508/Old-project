from ArrayStack2 import *
from DSA import *

ops = Stack()
vals = Stack()
while not stdIsEmpty():
    s = stdReadString()
    if s == "(":
        pass
    elif s == "+":
        ops.push(s)
    elif s == "*":
        ops.push(s)
    elif s == ")":
        op = ops.pop()
        if op == "+":
            vals.push(vals.pop() + vals.pop())
        elif op == "*":
            vals.push(vals.pop() * vals.pop())
    else:
        vals.push(float(s))

print(vals.pop())
