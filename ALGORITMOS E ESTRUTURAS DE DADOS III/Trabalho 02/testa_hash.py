import base64
import hashlib
from itertools import permutations

senha = "yo"
caracteres = [chr(x) for x in range(32, 127)]
hash_fornecida = "bQCGPk/UvoRtLzgXY16bCooIxPWxJDpqjSFd0vl7u9M="
hash_decodificada_base64 = base64.b64decode(hash_fornecida)

for byte in hash_decodificada_base64:
    print(hex(byte)[2:], end=" ")

print()

contador = 1
while contador < 9:
    a = permutations(caracteres, contador)
    print(f"senhas com {contador + 2} caracteres")
    for permutacao in a:
        sufixo = ""
        for i in permutacao:
            sufixo += i
        #print(senha+sufixo)
        if (hashlib.sha256((senha+sufixo).encode("utf-8")).digest() == hash_decodificada_base64):
            senha = senha + sufixo
            break
    contador += 1

print(f"A senha e {senha}")
