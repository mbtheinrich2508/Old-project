import DSA, sys

def threeSum(a):
    count=0
    N=len(a)
    for i in range(0,N):
        for j in range(i+1,N):
            for k in range(j+1,N):
                if a[i]+a[j]+a[k]==0:
                    count+=1
    return count


f = DSA.In(sys.argv[1])
a = f.readAllInts()

stopwatch=DSA.Stopwatch()
print(threeSum(a))
print("Elapsed Time:",stopwatch.elapsedTime())