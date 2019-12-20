a = 1
b = 2
c = 3

result1 = a + b > c and a + c > b and b + c > a

a = False
b = False
c = True

result2 = not a and b or c
result3 = not a and (b or c)
result4 = not(not a and (b or c))

a = 'this'
b = 'is'
c = 'test'

result5 = a > b and a > c and c > b
result6 = a < b or a < c or c < b

a = ''
b = ""
c = 0

result7 = a > b
result8 = a or b or c

a = 0
b = 100
c = ''

result9 = a or b or c

a = None
b = None

result10 = a and b
result11 = not a or b

a = 1
b = 1
c = 2

result12 = a == b and a != c

a = '1'
b = '1'
c = "2"

result13 = a == b and a != c

print (result1, result2, result3, result4, result5, result6, result7, result8, result9, result10, result11, result12, result13)