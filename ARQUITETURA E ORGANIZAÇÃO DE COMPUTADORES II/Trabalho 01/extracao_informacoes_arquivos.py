import pandas as pd
import glob
import os

pastas = [
    "/home/jelson/Downloads/Resultados/EX01/Instrucoes/",
    "/home/jelson/Downloads/Resultados/EX01/Dados/",
    "/home/jelson/Downloads/Resultados/EX02/Instrucoes/",
    "/home/jelson/Downloads/Resultados/EX02/Dados/",
    "/home/jelson/Downloads/Resultados/EX03/Instrucoes/",
    "/home/jelson/Downloads/Resultados/EX03/Dados/",
    "/home/jelson/Downloads/Resultados/EX04/Instrucoes/",
    "/home/jelson/Downloads/Resultados/EX04/Dados/",
]

for pasta in pastas:
    if os.path.isdir(pasta) == False:
        continue

    arquivos = glob.glob(pathname=pasta + "*out", recursive=True)

    informacoes = pd.DataFrame(columns=['Total Instrucoes Executadas', 'Leitura/Escritas', 'Acessos', 'Hits', 'Misses', 'Benchmark'])


    for arquivo in arquivos:
        with open(arquivo, 'rt') as saida_bruta:
            lido = saida_bruta.readline()
            while lido != "sim: ** simulation statistics **\n" and saida_bruta.readable() and lido != '':
                lido = saida_bruta.readline()

            if lido == '':
                continue

            estatisticas = saida_bruta.readlines()

            total_instrucoes = int(estatisticas[0][estatisticas[0].find(' '):estatisticas[0].find('#')])
            leitura_escritas = int(estatisticas[1][estatisticas[1].find(' '):estatisticas[1].find('#')])
            acessos = int(estatisticas[4][estatisticas[4].find(' '):estatisticas[4].find('#')])
            hits = int(estatisticas[5][estatisticas[5].find(' '):estatisticas[5].find('#')])
            misses = int(estatisticas[6][estatisticas[6].find(' '):estatisticas[6].find('#')])

            informacoes = pd.concat([informacoes, pd.Series(
                {
                'Total Instrucoes Executadas': total_instrucoes,
                'Leitura/Escritas': leitura_escritas,
                'Acessos' : acessos,
                'Hits' : hits,
                'Misses' : misses,
                'Benchmark' : arquivo[arquivo.rfind('/')+1:]
                }
            ).to_frame().T], ignore_index=True)
            
        saida_bruta.close()
    informacoes.to_csv(pasta+"resultado.csv", index=False)