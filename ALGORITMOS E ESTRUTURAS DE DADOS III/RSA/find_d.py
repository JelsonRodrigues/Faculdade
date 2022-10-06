p = 11
q = 13
N = p*q
phi = (p-1)*(q-1)

e = 19
d = 1

while True:
    if e * d % phi == 1:
        break
    d+=1

print(f"D = {d}")
