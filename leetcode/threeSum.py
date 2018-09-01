class Ideal3:
	def __init__(self,nums,k):
		self.k = k
		self.nums = [x for x in nums if (x%3)==(k%3)]
	@staticmethod
	def analysis(nums):
		return [Ideal(nums,0),Ideal(nums,1),Ideal(nums,-1)]
	def Solution(self,i):
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
	@staticmethod
	def one3(nums,i):
		pass

class Ideal2:
	def __init__(self,nums,k):
		self.k = k
		self.nums = [x for x in nums if (x%2)==(k%2)]
	def down(self,nums=self.nums):
		r = [(x-self.k)/2 for x in nums]
		return r
	def up(self,nums=self.nums):
		r = [x*2+self.k for x in nums]
	
	@staticmethod
	def analysis(nums):
		return {0:Ideal2(nums,0), 1:Ideal2(nums,1), -1:Ideal2(nums,-1)}
	@staticmethod
	def Solution(nums,N):
		L = Ideal2.analysis(nums)
		if N==0:
			r = L[0]._Solution(0) + Ideal2.twoone(L[1],L[0],0)
		elif N==1:
			r = L[1]._Solution(-1) + Ideal2.twoone(L[0],L[1],0)
		elif N==-1:
			r = L[-1]._Solution(1) + Ideal2.twoone(L[0],L[-1],0)
		else:
			print("something error")
			r = []
		return r
	@staticmethod 
	def twoone(I2,I1,N):
		n2 = I2.down()
		n1 = I1.down()
		r = []
		if len(n1)<0:
			return []
		elif len(n1)==1:
			if len(n2)<2:
				return []
			elif len(n2)==2:
				r += [(n2,n1)]
		else:
			L2 = Ideal2.analysis(n2)
			L1 = Ideal2.analysis(n1)
			if N==0:
				r += Ideal2.twoone(L2[-1],L1[0],1) + Ideal2.twoone(L2[0],L[0],0) + Ideal2.three((L2[0],L2[1]),L1[1])
			elif N==1:
				r += Ideal2.twoone(L2[1],L1[-1],0) + Ideal2.three((L2[0],L2[1]),L1[0])
		return [I2.up(x)+I1.up(y) for (x,y) in r]


	def _Solution(self,N):
		nums = self.down()
		if(len(nums)) < 3:
			return []
		if len(nums)==3:
			if sum(nums)==N:
				return [self.up(nums)]
			else:
				return []
		r = Ideal2.Solution(nums,N)
		return [self.up(x) for x in r]
		
		
