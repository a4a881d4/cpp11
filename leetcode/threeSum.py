def three(nums):
    odd = [x for x in nums if (x%2)==1]
    even = [x for x in nums if (x%2)==0]
    r = dthree(even) + two(odd,even)
    return r

def dthree(even):
    r = three([x/2 for x in even])
    return [[x*2 for x in a] for a in r]
