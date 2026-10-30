def gcd(p, q):
    if q == 0:
        return p
    else:
        return gcd(q, p % q)


print(gcd(216, 192))
