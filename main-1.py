# Test code for IEEE course final project
# Fan Cheng, 2024

import minimatrix as mm

#Test your code here!
# The following code is only for your reference
# Please write the test code yourself

#test1 测试Matrix类的各个操作
print("**********test1 测试Matrix类的各个操作**********")

mat = mm.Matrix(data=[[2,1,3], [0,1,2], [1,0,3]])
print(mat.shape())

print(mat.reshape((1, 9)))
print(mat.reshape((9, 1)))

test_mat = mm.Matrix(data = [[2, 4, 1], [3, 2, 3], [1, 5, 4]])
print(mat.dot(test_mat))

print(mat.T())

print(mat.sum())
print(mat.sum(0))
print(mat.sum(1))

print(mat.copy())

print(mat.Kronecker_product(test_mat))

print(mat[1, 2])
print(mat[0:2, 1:2])
print(mat[:1, 1:])
print(mat[:, :])

mat[1, 2] = 0
print(mat)
mat[:, :] = mm.Matrix(data=[[3,4,-6], [1,-1,4], [-1,2,-7]])
print(mat)


print(mat ** 3)
print(mat + mat)
test_mat_2 = mm.Matrix(data=[[2,4,3], [1,7,7], [3,2,5]])
print(test_mat_2 - mat)
print(test_mat_2 * mat)

print(len(mat))
print(mat)

print(mat.det())
print(mat.inverse())
print(mat.rank())


print("")


#test2
print("**********test2 测试reshape**********")

m24 = mm.arange(0, 24)
print(m24.reshape((3, 8)))
print(m24.reshape((24, 1)))
print(m24.reshape((4, 6)))

print("")


#test3
print("**********test3 测试zeros和zeros_like**********")

print(mm.zeros((3,3)))
print(mm.zeros_like(m24))

print("")


#test4
print("**********test4 测试ones和ones_like**********")

print(mm.ones((3,3)))
print(mm.ones_like(m24))

print("")


#test5
print("**********test5 测试nrandom和nrandom_like**********")

print(mm.nrandom((3,3)))
print(mm.nrandom_like(m24))

print("")


#test6
print("**********test6 测试最小二乘法**********")

m = 1000
n = 100
X = mm.nrandom((m, n))
w = mm.nrandom((n, 1))
e = mm.nrandom((m,1))
a = mm.zero_mean(e)
Y = X.dot(w) + a #mx1的矩阵
X_T = X.T() #nxm的矩阵
A = X_T.dot(X) #nxn的矩阵

w_ = ((A.inverse()).dot(X_T)).dot(Y) #nx1的Matrix


print(w_)
print("")
print(w)
print("")

check_list = []
for i in range(n):
    difference = round(w_.data[i][0] - w.data[i][0], 8)
    check_list.append([difference])
print(mm.Matrix(data=check_list)) #测试两者误差为多少







 