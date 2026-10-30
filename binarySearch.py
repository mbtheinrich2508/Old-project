def binarySearch(a,item):
    lo=0
    hi=len(a)-1
    while lo<=hi:
        mid=(lo+hi)//2
        print(mid)
        if item>a[mid]:
            lo=mid+1
        elif item<a[mid]:
            hi=mid-1
        else:
            return mid
    return -1

a=[2,4,6,8,10,12,14,16,18,20]
print(binarySearch(a,16))