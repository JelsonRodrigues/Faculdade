from typing import Tuple, List

def rsa(entrada : List[int], key:Tuple[int, int]) -> list:
    resultado = []
    for item in entrada:
        cifrado = item ** key[0] % key[1]
        resultado.append(cifrado)
    return resultado

def main():
    mensagem = "AEDIII EH TOP"

    chave_publica = (17, 143)
    chave_privada = (113, 143)

    mensagem_como_lista_de_inteiros = [ord(x) for x in mensagem]

    criptografada = rsa(mensagem_como_lista_de_inteiros, chave_publica)
    mensagem_criptografada = [chr(x) for x in criptografada]
    descriptografada = rsa(criptografada, chave_privada)
    mensagem_descriptografada = [chr(x) for x in descriptografada]

    print(mensagem_criptografada)
    print("".join([str(x) for x in mensagem_criptografada]))
    print(mensagem_descriptografada)
    print("".join([str(x) for x in mensagem_descriptografada]))

if __name__ == "__main__":
    main()