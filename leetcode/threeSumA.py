class Solution:
    def threeSum(self, nums):
        """
        :type nums: List[int]
        :rtype: List[List[int]]
        """
        r = []
        s = set(nums)

        for x in s:
            if x==0:
                if nums.count(x)>3:
                    r +=[(0,0,0)]
            elif nums.count(x)>1 and (-2*x in nums):
                r += [(x,x,-2*x)]
        l = list(s)
        l.sort()
        p = [x for x in l if x>0 ]
        n = [x for x in l if x<0 ]
        if 0 in l:
            sp = set(p).intersection([-x for x in n])
            for x in sp:
                r += [(-x,0,x)]
        r += AnBp(n,p).do().result()
        r += ApBn(p,n).do().result()
        rr = [sorted(x) for x in r]
        rr.sort()
        return rr

def analysis(nums):
    return  { 'E':[x for x in nums if (x%2)==0]
            , 'O':[x for x in nums if (x%2)==1]
            }

class TwoPart:
    def __init__(self,A,B):
        self.A = A
        self.B = B
        self.R = []

    def do(self):
        if len(self.A)<1:
            return self
        if len(self.B)<2:
            return self
        if len(self.B)==2:
            for x in self.A:
                if x == sum(self.B):
                    self.R += [(x,self.B[0],self.B[1])]
            return self

        LA = analysis(self.A)
        LB = analysis(self.B)

        self.R += AEBE(LA['E'],LB['E']).do().result()
        self.R += AEBO(LA['E'],LB['O']).do().result()
        self.R += AOBECO(LA['O'],LB['E'],LB['O']).do().result()
        return self


class AnBp(TwoPart):  
    """
    " A -
    " B +
    """
    def __init__(self,A,B):
        TwoPart.__init__(self,[-x for x in A],B)
    def result(self):
        return [(-a,b,c) for (a,b,c) in self.R]

class ApBn(TwoPart):
    """
    " A +
    " B -
    """
    def __init__(self,A,B):
        TwoPart.__init__(self,A,[-x for x in B])
    def result(self):
        return [(a,-b,-c) for (a,b,c) in self.R]

class AEBO(TwoPart):
    """
    " A/2+1
    " (B+1)/2
    """
    def __init__(self,A,B):
        TwoPart.__init__(self,[int(x/2)+1 for x in A],[int((x+1)/2) for x in B])
    def result(self):
        return [((a-1)*2,2*b-1,2*c-1) for (a,b,c) in self.R]

class AEBE(TwoPart):
    """
    " A/2
    " B/2
    """
    def __init__(self,A,B):
        TwoPart.__init__(self,[int(x/2) for x in A],[int(x/2) for x in B])
    def result(self):
        return [(a*2,2*b,2*c) for (a,b,c) in self.R]

class ThreePart:
    """
    " A
    " B
    " C
    """
    def __init__(self,A,B,C):
        self.A = A
        self.B = B
        self.C = C
        self.R = []

    def do(self):
        LLL = len(self.A)*len(self.B)*len(self.C)
        if LLL < 8*8*8:
            if LLL == 0:
                return self
            for x in set(self.B):
                for y in set(self.C):
                    if x+y in self.A:
                        self.R += [(x+y,x,y)]
            return self

        LA = analysis(self.A)
        LB = analysis(self.B)
        LC = analysis(self.C)

        self.R += AEBECE(LA['E'],LB['E'],LC['E']).do().result()
        self.R += AEBOCO(LA['E'],LB['O'],LC['O']).do().result()
        self.R += AOBECO(LA['O'],LB['E'],LC['O']).do().result()
        self.R += AOBOCE(LA['O'],LB['E'],LC['O']).do().result()
        return self



class AEBECE(ThreePart):
    """
    " A/2
    " B/2
    " C/2
    """
    def __init__(self,A,B,C):
        ThreePart.__init__(self,[int(x/2) for x in A],[int(x/2) for x in B],[int(x/2) for x in C])
    def result(self):
        return [(a*2,2*b,2*c) for (a,b,c) in self.R]

class AEBOCO(ThreePart):
    """
    " A/2+1
    " (B+1)/2
    " (C+1)/2
    """
    def __init__(self,A,B,C):
        ThreePart.__init__(self,[int(x/2)+1 for x in A],[int((x+1)/2) for x in B],[int((x+1)/2) for x in C])
    def result(self):
        return [((a-1)*2,2*b-1,2*c-1) for (a,b,c) in self.R]

class AOBECO(ThreePart):
    """
    " (A+1)/2
    " B/2
    " (C+1)/2
    """
    def __init__(self,A,B,C):
        ThreePart.__init__(self,[int((x+1)/2) for x in A],[int(x/2) for x in B],[int((x+1)/2) for x in C])
    def result(self):
        return [(a*2-1,2*b,2*c-1) for (a,b,c) in self.R]

class AOBOCE(ThreePart):
    """
    " (A+1)/2
    " (B+1)/2
    " C/2
    """
    def __init__(self,A,B,C):
        ThreePart.__init__(self,[int((x+1)/2) for x in A],[int((x+1)/2) for x in B],[int(x/2) for x in C])
    def result(self):
        return [(a*2-1,2*b-1,2*c) for (a,b,c) in self.R]

#l = [3,0,-2,-1,1,2]
#l = [0,0]
import dataset
l = dataset.l2
#l = [-1,-2,-3,4,1,3,0,3,-2,1,-2,2,-1,1,-5,4,-3]
a = Solution()
r = a.threeSum(l)
print(r)
print(dataset.l2_res)
print(list(zip(r[:len(dataset.l2_res)],dataset.l2_res)))

print(len(dataset.l2_res),"  ",len(r))
