led = [0]*30

print(led)

for j in range(3):
    led = [0]*30
    for i in range(len(led)):
        if i % 3 == j:
            led[i] = 1
    print(led)
