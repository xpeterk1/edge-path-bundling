coordinate = ['x', 'y', 'z']
value = [3, 4, 5]

result = zip(coordinate, value)
result_list = list(result)
print(result_list)

c, v =  zip(*result_list)
print('c =', c)
print('v =', v)


boundaries = [[0,0],[0,1],[1,1],[1,0]]
x,y = zip(*boundaries)
print(x)
print(y)