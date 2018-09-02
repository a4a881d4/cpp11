
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
		if len(self.A)==1 and len(self.B)==2:
			if self.A[0] == sum(self.B):
				self.R += [(self.A[0],self.B[0],self.B[1])]
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
		TwoPart.__init__(self,[x/2+1 for x in A],[(x+1)/2 for x in B])
	def result(self):
		return [((a+1)*2,2*b+1,2*c+1) for (a,b,c) in self.R]

class AEBE(TwoPart):
	"""
	" A/2
	" B/2
	"""
	def __init__(self,A,B):
		TwoPart.__init__(self,[x/2 for x in A],[x/2 for x in B])
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
		if len(self.A)<1:
			return self
		if len(self.B)<1:
			return self
		if len(self.C)<1:
			return self
		if len(self.A)==1 and len(self.B)==1 and len(self.C)==1:
			if self.A[0] == self.B[0] + self.C[0]:
				self.R += [(self.A[0],self.B[0],self.C[1])]
				return self

		LA = analysis(self.A)
		LB = analysis(self.B)
		LC = analysis(self.C)

		self.R += AEBECE(LA['E'],LB['E'],LC['E']).do().result()
		self.R += AEBOCO(LA['E'],LB['O'],LC['O']).do().result()
		self.R += AOBECO(LA['O'],LB['E'],LC['O']).do().result()
		self.R += AOBECO(LA['O'],LC['E'],LB['O']).do().result()
		return self



class AEBECE(ThreePart):
	"""
	" A/2
	" B/2
	" C/2
	"""
	def __init__(self,A,B,C):
		ThreePart.__init__(self,[x/2 for x in A],[x/2 for x in B],[x/2 for x in C])
	def result(self):
		return [(a*2,2*b,2*c) for (a,b,c) in self.R]

class AEBOCO(ThreePart):
	"""
	" A/2+1
	" (B+1)/2
	" (C+1)/2
	"""
	def __init__(self,A,B,C):
		ThreePart.__init__(self,[x/2+1 for x in A],[(x+1)/2 for x in B],[(x+1)/2 for x in C])
	def result(self):
		return [((a+1)*2,2*b+1,2*c+1) for (a,b,c) in self.R]

class AOBECO(ThreePart):
	"""
	" (A+1)/2
	" B/2
	" (C+1)/2
	"""
	def __init__(self,A,B,C):
		ThreePart.__init__(self,[(x+1)/2 for x in A],[x/2 for x in B],[(x+1)/2 for x in C])
	def result(self):
		return [(a*2+1,2*b,2*c+1) for (a,b,c) in self.R]

class Solution:
	def threeSum(self,nums):
		r = []
		s = set(nums)
		for x in s:
			if nums.count(x)>1 and (-2*x in nums):
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
		return r 

"""
class Ideal2:
	def __init__(self,nums,k):
		self.k = k
		self.nums = [x for x in nums if (x%3)==(k%3)]
	@staticmethod
	def analysis(nums):
		return [Ideal(nums,0),Ideal(nums,1),Ideal(nums,+1)]
	def Solution(self,N):
		if(len(nums)) < 3:
			return []
		if len(nums)==3:
			if sum(nums)==N:
				return [nums]
			else:
				return []
		L = Ideal.analysis(self.nums)
		if i==0:
			r = L[0].Solution(0) + L[1].Solution(1) + L[2].Solution(-1) + Ideal.one3(L,0)
		elif i==1:
			r = Ideal.twoone(L[0],L[1],1) + Ideal.twoone(L[1],L[2],1) + Ideal.twoone(L[2],L[0],1)
		elif i==-1:
			r = Ideal.twoone(L[0],L[2],-1) + Ideal.twoone(L[1],L[0],-1) + Ideal.twoone(L[2],L[1],-1)
		else:
			print("some error")
			r = []
		return r
"""

a = Solution()
l = [-1, 0, 1, 2, -1, -4]
r = a.threeSum(l)
print(r)