i = 1
data = i.to_bytes(1,'big')
print(data)
data += i.to_bytes(1,'big')
print(data)

