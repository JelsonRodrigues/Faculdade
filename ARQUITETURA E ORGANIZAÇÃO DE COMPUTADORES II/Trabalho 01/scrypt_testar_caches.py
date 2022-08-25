import os
import sys
import glob

"""-------------------"""

local_sim_cache = "./sim-cache"

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


def simulaCacheInstrucoes(tamanho_total_cache : int, tamanho_do_bloco : int, associatividade : int, politica_substituicao : str, benchmark : str):
        if associatividade < 1: # Totalmente associativa
            associatividade = tamanho_total_cache / tamanho_do_bloco
        assoc = associatividade
        nsets = tamanho_total_cache / tamanho_do_bloco / assoc
        bsyze = tamanho_do_bloco
        repl = politica_substituicao

        os.system(f"{local_sim_cache} -cache:il1 il1:{nsets}:{bsyze}:{assoc}:{repl} -cache:il2 none -cache:dl1 none -cache:dl2 none -tlb:itlb none -tlb:dtlb none {benchmark}")

def simulaCacheDados(tamanho_total_cache : int, tamanho_do_bloco : int, associatividade : int, politica_substituicao : str, benchmark : str):
        if associatividade < 1: # Totalmente associativa
            associatividade = tamanho_total_cache / tamanho_do_bloco
        assoc = associatividade
        nsets = tamanho_total_cache / tamanho_do_bloco / assoc
        bsyze = tamanho_do_bloco
        repl = politica_substituicao

        os.system(f"{local_sim_cache} -cache:il1 none -cache:il2 none -cache:dl1 dl1:{nsets}:{bsyze}:{assoc}:{repl} -cache:dl2 none -tlb:itlb none -tlb:dtlb none {benchmark}")


def exercicio_01(tamanho_total_cache : int, tamanho_do_bloco : int):
    for nome, benchmark in benchmarks.items():
        """ INSTRUCOES """
        # Mapeamento direto        
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 1, 'r', benchmark)
        # Mapeamento 2-vias
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 2, 'r', benchmark)
        # Mapeamento 4-vias
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 4, 'r', benchmark)
        # Mapeamento Totalmente associativo
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 0, 'r', benchmark)

        """ DADOS """
        # Mapeamento direto        
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 1, 'r', benchmark)
        # Mapeamento 2-vias
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 2, 'r', benchmark)
        # Mapeamento 4-vias
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 4, 'r', benchmark)
        # Mapeamento Totalmente associativo
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 0, 'r', benchmark)

def exercicio_02(tamanho_total_cache : int):
    for nome, benchmark in benchmarks.items():
        """ INSTRUCOES """
        # LRU      
        simulaCacheInstrucoes(tamanho_total_cache, 16, 0, 'l', benchmark)
        # FIFO
        simulaCacheInstrucoes(tamanho_total_cache, 16, 0, 'f', benchmark)
        # RANDOM
        simulaCacheInstrucoes(tamanho_total_cache, 16, 0, 'r', benchmark)

        """ DADOS """
        # LRU      
        simulaCacheDados(tamanho_total_cache, 16, 0, 'l', benchmark)
        # FIFO
        simulaCacheDados(tamanho_total_cache, 16, 0, 'f', benchmark)
        # RANDOM
        simulaCacheDados(tamanho_total_cache, 16, 0, 'r', benchmark)

def exercicio_03(tamanho_total_cache : int, tamanho_do_bloco : int):
    for nome, benchmark in benchmarks.items():
        """ INSTRUCOES """
        # Mapeamento direto        
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 1, 'r', benchmark)
        # Mapeamento 2-vias
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 2, 'r', benchmark)
        # Mapeamento 4-vias
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 4, 'r', benchmark)
        # Mapeamento Totalmente associativo
        simulaCacheInstrucoes(tamanho_total_cache, tamanho_do_bloco, 0, 'r', benchmark)

        """ DADOS """
        # Mapeamento direto        
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 1, 'r', benchmark)
        # Mapeamento 2-vias
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 2, 'r', benchmark)
        # Mapeamento 4-vias
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 4, 'r', benchmark)
        # Mapeamento Totalmente associativo
        simulaCacheDados(tamanho_total_cache, tamanho_do_bloco, 0, 'r', benchmark)

exercicio_01(tamanho_total_cache=256, tamanho_do_bloco=4)
exercicio_02(tamanho_total_cache=256)
exercicio_03(tamanho_total_cache=256)