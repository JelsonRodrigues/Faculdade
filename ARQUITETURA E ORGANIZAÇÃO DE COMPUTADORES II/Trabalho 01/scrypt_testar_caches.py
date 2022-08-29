import os
import sys
import glob

"""-------------------"""

local_sim_cache = "./sim-cache"
pasta_base = "/home/jelson/Downloads/Resultados"

benchmarks = {
    "GCC_1" : "cc1.ss gcc.i",
    "GCC_2" : "cc1.ss cccp.i",
    "GCC_3" : "cc1.ss stmt.i",
    "GCC_4" : "cc1.ss jump.i",
    "GO_1" : "go.ss 50 9 2stone9.in",
    "LI_1" : "li.ss li.lsp",
    "LI_2" : "li.ss queen6.lsp",
    "LI_2" : "li.ss train.lsp",
    "PERL_1" : "perl.ss scrabbl.pl < scrabbl.in",
    "VORTEX_1" : "vortex.ss vortex.in",
    "VORTEX_2" : "vortex.ss tiny.in",
    "IJPEG_1" : "ijpeg.ss -image_file tinyrose.ppm -compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp",
    "IJPEG_2" : "ijpeg.ss -image_file specmun.ppm -compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp",
    "IJPEG_3" : "ijpeg.ss -image_file ijpeg.ppm -compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp"
}

"""-------------------"""

def simulaCacheInstrucoes(tamanho_total_cache : int, tamanho_do_bloco : int, associatividade : int, politica_substituicao : str, benchmark : str, arquivo_saida : str):
        if associatividade < 1: # Totalmente associativa
            associatividade = int(tamanho_total_cache / tamanho_do_bloco)
        assoc = associatividade
        nsets = int(tamanho_total_cache / tamanho_do_bloco / assoc)
        bsyze = tamanho_do_bloco
        repl = politica_substituicao

        arquivo_saida += f"_{nsets}:{bsyze}:{assoc}:{repl}.out"

        comando = print(f"{local_sim_cache} -cache:il1 il1:{nsets}:{bsyze}:{assoc}:{repl} -cache:il2 none -cache:dl1 none -cache:dl2 none -tlb:itlb none -tlb:dtlb none -redir:sim {arquivo_saida} {benchmark}")
        print(f"COMANDO = {comando}")
        os.system(comando)

def simulaCacheDados(tamanho_total_cache : int, tamanho_do_bloco : int, associatividade : int, politica_substituicao : str, benchmark : str, arquivo_saida : str):
        if associatividade < 1: # Totalmente associativa
            associatividade = int(tamanho_total_cache / tamanho_do_bloco)
        assoc = associatividade
        nsets = int(tamanho_total_cache / tamanho_do_bloco / assoc)
        bsyze = tamanho_do_bloco
        repl = politica_substituicao

        arquivo_saida += f"_{nsets}:{bsyze}:{assoc}:{repl}.out"

        comando = f"{local_sim_cache} -cache:il1 none -cache:il2 none -cache:dl1 dl1:{nsets}:{bsyze}:{assoc}:{repl} -cache:dl2 none -tlb:itlb none -tlb:dtlb none -redir:sim {arquivo_saida} {benchmark}"
        print(f"COMANDO = {comando}")
        os.system(comando)

def exercicio_01(tamanho_total_cache : int, tamanho_do_bloco : int):
    for nome, benchmark in benchmarks.items():
        """ INSTRUCOES """
        # Mapeamento direto        
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 1, 'r', benchmark, f"{pasta_base}/EX01/Instrucoes/{nome}")
        # Mapeamento 2-vias
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 2, 'r', benchmark, f"{pasta_base}/EX01/Instrucoes/{nome}")
        # Mapeamento 4-vias
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 4, 'r', benchmark, f"{pasta_base}/EX01/Instrucoes/{nome}")
        # Mapeamento Totalmente associativo
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 0, 'r', benchmark, f"{pasta_base}/EX01/Instrucoes/{nome}")

        """ DADOS """
        # Mapeamento direto        
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 1, 'r', benchmark, f"{pasta_base}/EX01/Dados/{nome}")
        # Mapeamento 2-vias
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 2, 'r', benchmark, f"{pasta_base}/EX01/Dados/{nome}")
        # Mapeamento 4-vias
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 4, 'r', benchmark, f"{pasta_base}/EX01/Dados/{nome}")
        # Mapeamento Totalmente associativo
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 0, 'r', benchmark, f"{pasta_base}/EX01/Dados/{nome}")

def exercicio_02(tamanho_total_cache : int):
    for nome, benchmark in benchmarks.items():
        """ INSTRUCOES """
        # LRU      
        simulaCacheInstrucoes(tamanho_total_cache, 16, 0, 'l', benchmark, f"{pasta_base}/EX02/Instrucoes/{nome}")
        # FIFO
        simulaCacheInstrucoes(tamanho_total_cache, 16, 0, 'f', benchmark, f"{pasta_base}/EX02/Instrucoes/{nome}")
        # RANDOM
        simulaCacheInstrucoes(tamanho_total_cache, 16, 0, 'r', benchmark, f"{pasta_base}/EX02/Instrucoes/{nome}")

        """ DADOS """
        # LRU      
        simulaCacheDados(tamanho_total_cache, 16, 0, 'l', benchmark, f"{pasta_base}/EX02/Dados/{nome}")
        # FIFO
        simulaCacheDados(tamanho_total_cache, 16, 0, 'f', benchmark, f"{pasta_base}/EX02/Dados/{nome}")
        # RANDOM
        simulaCacheDados(tamanho_total_cache, 16, 0, 'r', benchmark, f"{pasta_base}/EX02/Dados/{nome}")

def exercicio_03(tamanho_total_cache : int, tamanho_do_bloco : int):
    for nome, benchmark in benchmarks.items():
        """ INSTRUCOES """
        # Mapeamento direto        
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 1, 'r', benchmark, f"{pasta_base}/EX03/Instrucoes/{nome}")
        # Mapeamento 2-vias
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 2, 'r', benchmark, f"{pasta_base}/EX03/Instrucoes/{nome}")
        # Mapeamento 4-vias
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 4, 'r', benchmark, f"{pasta_base}/EX03/Instrucoes/{nome}")
        # Mapeamento Totalmente associativo
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 0, 'r', benchmark, f"{pasta_base}/EX03/Instrucoes/{nome}")

        """ DADOS """
        # Mapeamento direto        
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 1, 'r', benchmark, f"{pasta_base}/EX03/Dados/{nome}")
        # Mapeamento 2-vias
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 2, 'r', benchmark, f"{pasta_base}/EX03/Dados/{nome}")
        # Mapeamento 4-vias
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 4, 'r', benchmark, f"{pasta_base}/EX03/Dados/{nome}")
        # Mapeamento Totalmente associativo
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 0, 'r', benchmark, f"{pasta_base}/EX03/Dados/{nome}")

exercicio_01(tamanho_total_cache=256 * 1024, tamanho_do_bloco=8)
exercicio_02(tamanho_total_cache=256 * 1024)
## exercicio_03(tamanho_total_cache=256)