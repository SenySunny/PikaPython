f = open('test/python/main.py', 'r')
s = f.read(10)
print(s)
f.close()
f = open('test/assets/test.jpg', 'rb')
b = f.read(-1)
print(b)
f.close()
f = open('test/assets/write.txt', 'w')
f.write('Hello World!\n')
f.close()
