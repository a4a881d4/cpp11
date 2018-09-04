class Solution:
    def _twoSum(self,nums,target,r,add):
        if len(nums)<2:
            return r
        i=0
        j=len(nums)-1
        while i<j:
            if nums[i]+nums[j] == target:
                r.append(add+[nums[i],nums[j]])
                i += 1
                j -= 1
                while i<j and nums[i-1]==nums[i]:
                    i += 1
                while i<j and nums[j+1]==nums[j]:
                    j -= 1

            if nums[i]+nums[j]>target:
                j -= 1
            elif nums[i]+nums[j]<target:
                i += 1
        return r

    def _threeSum(self,nums,target,r,add):
        if len(nums)<3:
            return r
        i = 0
        for i in range(len(nums)-2):
            if nums[i]+nums[i+1]+nums[i+2]>target:
                break
            if i>0 and nums[i]==nums[i-1]:
                continue
            r = self._twoSum(nums[i+1:],target-nums[i],r,add+[nums[i]])
        return r

    def _fourSum(self,nums,target,r,add):
        if len(nums)<4:
            return r
        i = 0
        for i in range(len(nums)-3):
            if nums[i]+nums[i+1]+nums[i+2]+nums[i+3]>target:
                break
            if i>0 and nums[i]==nums[i-1]:
                continue
            r = self._threeSum(nums[i+1:],target-nums[i],r,add+[nums[i]])
        return r

    def threeSum(self,nums):
        nums.sort()
        return self._threeSum(nums,0,[],[])
    
    def fourSum(self, nums, target):
        nums.sort()
        return self._fourSum(nums,target,[],[])

#l = [3,0,-2,-1,1,2]
#l = [0,0]
import dataset
l = dataset.l
# l = [-1,-2,-3,4,1,3,0,3,-2,1,-2,2,-1,1,-5,4,-3]
# l = [-1,0,1,2,-1,-4]
a = Solution()
r = a.threeSum(l)
print(r)
# print(dataset.l2_res)
# print(list(zip(r[:len(dataset.l2_res)],dataset.l2_res)))

# print(len(dataset.l2_res),"  ",len(r))

