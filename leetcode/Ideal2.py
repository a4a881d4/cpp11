class Ideal2:
	def __init__(self,nums,k):
			self.k = k
		self.nums = [x for x in nums if (x%3)==(k%3)]
	@staticmethod
	def analysis(nums):
		return [Ideal(nums,0),Ideal(nums,1),Ideal(nums,-1)]
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