package super_trunfo;

import java.util.Scanner;

import super_trunfo.erros.NaoHaJogadoresSucifientesException;
import super_trunfo.erros.NumeroMaximoDeJogadoreException;

import java.util.InputMismatchException;
import java.util.Random;
import java.util.ArrayList;

public class Interface {
    public static void main(String[] args) {
        simular();
        //jogar();
    }
    public static void simular(){
        SuperTrunfoDaReciclagem game = new SuperTrunfoDaReciclagem();
        Random aleatorio = new Random();
        
        System.out.println("BEM VINDO AO SUPER TRUNFO DA RECICLAGEM!!! (pressione enter)");
        //fakeProcessando();
        imprimirNovasLinhas(2);
        
        /* leitura dos jogadores */
        System.out.println("VOCÊ DEVE ADICIONAR JOGADORES PARA INCIAR");
        System.out.println("Quantos jogadores quer adicionar? min=" + game.numero_minimo_jogadores + ", max=" + game.numero_maximo_jogadores);
        
        game.adicionarJogador(new Jogador("TESTE 01"));
        game.adicionarJogador(new Jogador("TESTE 02"));
        game.adicionarJogador(new Jogador("TESTE 03"));
        game.adicionarJogador(new Jogador("TESTE 04"));

        //fakeProcessando();
        imprimirNovasLinhas(4);
        /* inicio do jogo */
        try {
            game.iniciar();
        } catch (NaoHaJogadoresSucifientesException e) {
            System.err.println("NÃO É POSSÍVEL INICIAR O JOGO, NÚMERO DE JOGADORES INSUFICIENTE");
            System.exit(-1);
        }
        
        Jogador jogador_atual = new Jogador("");
        Categoria categoria = Categoria.COR;
        ArrayList<Jogador> jogadores_removidos;
        ArrayList<Jogador> jogadores_inicio_rodada;

        while (game.getNumJogadoresAtivos() > 1) {
            // Adiciona uma pequena pausa
            jogadores_removidos = game.removerJogadoresAtivosSemCartas();
            if (jogadores_removidos.size() > 0) {
                System.out.println("JOGADORES REMOVIDOS POR ESTAREM SEM CARTAS!!!");
                //fakeProcessando();
                SuperTrunfoDaReciclagem.imprimirJogadores(jogadores_removidos);
                if (game.getNumJogadoresAtivos() == 1) {
                    System.out.println("APÓS A REMOÇÃO FICOU APENAS UM JOGADOR");
                    imprimirNovasLinhas(1);
                    break;
                }
            }
            imprimirNovasLinhas(1);
            
            jogador_atual = game.getJogadorAtual();
            game.atualizarJogadoresRodada();
            System.out.println("OS JOGADORES ATIVOS SÃO");
            //fakeProcessando();
            game.imprimirJogadoresRodada();
            
            //inserirPausa(500l);
            System.out.println("\nO JOGADOR ATUAL É");
            jogador_atual.imprimir();
            
            jogador_atual.verCarta();
            categoria = Categoria.inteiroParaCategoria(aleatorio.nextInt(4) + 1);
            System.out.println("O JOGADOR " + jogador_atual.getNome() + " ESCOLHEU COMPARAR PELA CATEGORIA " + categoria.toString());
            //inserirPausa(250l);
            //fakeProcessando();

            jogadores_inicio_rodada = game.rodada(categoria);
            SuperTrunfoDaReciclagem.imprimirCartasRodada(jogadores_inicio_rodada, game.getCartasComparando());

            while (game.ocorreuEmpate()) {
                //inserirPausa(250l);
                imprimirNovasLinhas(1);
                System.out.println("OCORREU EMPATE ENTRE OS JOGADORES: ");
                game.imprimirJogadoresRodada();
                
                jogadores_removidos = game.removerJogadoresRodadaSemCartas();
                if (jogadores_removidos.size() > 0) {
                    System.out.println("OS JOGADORES FORAM REMOVIDOS POR ESTAREM SEM CARTAS!!!");
                    //fakeProcessando();
                    SuperTrunfoDaReciclagem.imprimirJogadores(jogadores_removidos);
                    if (game.getNumJogadoresRodada() == 1) {
                        break;
                    }
                }
                
                jogador_atual = game.getJogadorAtual();

                imprimirNovasLinhas(1);
                System.out.println("O JOGADOR ATUAL É");
                jogador_atual.imprimir();
                
                jogador_atual.verCarta();
                categoria = Categoria.inteiroParaCategoria(aleatorio.nextInt(4) + 1);
                System.out.println("O JOGADOR " + jogador_atual.getNome() + " ESCOLHEU COMPARAR PELA CATEGORIA " + categoria.toString());
                //inserirPausa(250l);
                //fakeProcessando();
                
                jogadores_inicio_rodada = game.rodada(categoria);
                SuperTrunfoDaReciclagem.imprimirCartasRodada(jogadores_inicio_rodada, game.getCartasComparando());
            }

            //inserirPausa(250l);
            //fakeProcessando();
            System.out.println("O VENCEDOR DA RODADA É ");
            jogador_atual = game.getVencedor();
            jogador_atual.imprimir();
            game.adicionarCartasVencedor();
            System.out.println("\n(pressione enter)");
        }

        inserirPausa(333l);
        System.out.println("O JOGADOR VENCEDOR DA PARTIDA É " + game.getVencedor().getNome());
        game.getVencedor().imprimir();
    }
    public static void jogar(){
        SuperTrunfoDaReciclagem game = new SuperTrunfoDaReciclagem();
        Scanner leitor = new Scanner(System.in);
        
        System.out.println("BEM VINDO AO SUPER TRUNFO DA RECICLAGEM!!! (pressione enter)");
        leitor.nextLine();
        fakeProcessando();
        imprimirNovasLinhas(2);
        
        /* leitura dos jogadores */
        System.out.println("VOCÊ DEVE ADICIONAR JOGADORES PARA INCIAR");
        System.out.println("Quantos jogadores quer adicionar? min=" + game.numero_minimo_jogadores + ", max=" + game.numero_maximo_jogadores);
        int num_jogadores = 0;
        do {
            num_jogadores = lerNumero(leitor);
        } while (num_jogadores < game.numero_minimo_jogadores || num_jogadores > game.numero_maximo_jogadores);
        imprimirNovasLinhas(1);

        for (int c = 1; c <= num_jogadores; c++) {
            System.out.println("DIGITE O NOME DO JOGADOR " + c + ":");
            try {
                game.adicionarJogador(lerJogador(leitor));
            } catch (NumeroMaximoDeJogadoreException e){
                System.err.println("NÚMERO MÁXIMO DE JOGADORES ATINGIDO!!!");
            }
        }
        fakeProcessando();
        imprimirNovasLinhas(4);

        /* inicio do jogo */
        try {
            game.iniciar();
        } catch (NaoHaJogadoresSucifientesException e) {
            System.err.println("NÃO É POSSÍVEL INICIAR O JOGO, NÚMERO DE JOGADORES INSUFICIENTE");
            System.exit(-1);
        }
        
        Jogador jogador_atual = new Jogador("");
        Categoria categoria = Categoria.COR;
        int opcao;
        ArrayList<Jogador> jogadores_removidos;
        ArrayList<Jogador> jogadores_inicio_rodada;

        while (game.getNumJogadoresAtivos() > 1) {
            // Adiciona uma pequena pausa
            jogadores_removidos = game.removerJogadoresAtivosSemCartas();
            if (jogadores_removidos.size() > 0) {
                System.out.println("JOGADORES REMOVIDOS POR ESTAREM SEM CARTAS!!!");
                fakeProcessando();
                SuperTrunfoDaReciclagem.imprimirJogadores(jogadores_removidos);
                if (game.getNumJogadoresAtivos() == 1) {
                    System.out.println("APÓS A REMOÇÃO FICOU APENAS UM JOGADOR");
                    imprimirNovasLinhas(1);
                    break;
                }
            }
            imprimirNovasLinhas(1);
            
            jogador_atual = game.getJogadorAtual();
            game.atualizarJogadoresRodada();
            System.out.println("OS JOGADORES ATIVOS SÃO");
            fakeProcessando();
            game.imprimirJogadoresRodada();

            inserirPausa(500l);
            System.out.println("\nO JOGADOR ATUAL É");
            jogador_atual.imprimir();
            
            opcao = 1;
            while (opcao == 1) {
                opcao = opcoesUsuario(leitor);
                if (opcao == 1) {
                    jogador_atual.verCarta();
                }
                else if (opcao == 2) {
                    categoria = escolhaCategoria(leitor);
                }
            }
            System.out.println("O JOGADOR " + jogador_atual.getNome() + " ESCOLHEU COMPARAR PELA CATEGORIA " + categoria.toString());
            inserirPausa(250l);
            fakeProcessando();

            jogadores_inicio_rodada = game.rodada(categoria);
            SuperTrunfoDaReciclagem.imprimirCartasRodada(jogadores_inicio_rodada, game.getCartasComparando());

            while (game.ocorreuEmpate()) {
                inserirPausa(250l);
                imprimirNovasLinhas(1);
                System.out.println("OCORREU EMPATE ENTRE OS JOGADORES: ");
                game.imprimirJogadoresRodada();
                
                jogadores_removidos = game.removerJogadoresRodadaSemCartas();
                if (jogadores_removidos.size() > 0) {
                    System.out.println("OS JOGADORES FORAM REMOVIDOS POR ESTAREM SEM CARTAS!!!");
                    fakeProcessando();
                    SuperTrunfoDaReciclagem.imprimirJogadores(jogadores_removidos);
                    if (game.getNumJogadoresRodada() == 1) {
                        break;
                    }
                }
                
                jogador_atual = game.getJogadorAtual();

                imprimirNovasLinhas(1);
                System.out.println("O JOGADOR ATUAL É");
                jogador_atual.imprimir();
                
                opcao = 1;
                while (opcao == 1) {
                    opcao = opcoesUsuario(leitor);
                    if (opcao == 1) {
                        jogador_atual.verCarta();
                    }
                    else if (opcao == 2) {
                        categoria = escolhaCategoria(leitor);
                    }
                }
                System.out.println("O JOGADOR " + jogador_atual.getNome() + " ESCOLHEU COMPARAR PELA CATEGORIA " + categoria.toString());
                inserirPausa(250l);
                fakeProcessando();

                game.imprimirCartasRodada();

                game.rodada(categoria);
            }

            inserirPausa(250l);
            fakeProcessando();
            System.out.println("O VENCEDOR DA RODADA É ");
            jogador_atual = game.getVencedor();
            jogador_atual.imprimir();
            game.adicionarCartasVencedor();
            System.out.println("\n(pressione enter)");
            leitor.nextLine();
        }

        inserirPausa(333l);
        System.out.println("O JOGADOR VENCEDOR DA PARTIDA É " + game.getVencedor().getNome());
        leitor.close();
    } 

    public static Jogador lerJogador(Scanner leitor) {
        String nome = leitor.nextLine();
        return new Jogador(nome);
    }
    public static Categoria escolhaCategoria(Scanner leitor) {
        System.out.println("Escolha a categoria");
        System.out.println("\t1: COR");
        System.out.println("\t2: DECOMPOSIÇÃO");
        System.out.println("\t3: RECICLAGEM");
        System.out.println("\t4: ATAQUE");

        int opcao = 0;
        while (opcao > 4 || opcao < 1) {
            opcao = lerNumero(leitor);
        }

        return Categoria.inteiroParaCategoria(opcao);
    }
    public static int lerNumero(Scanner leitor) {
        int lido = 0;
        try {
            lido = leitor.nextInt();
        }
        catch (InputMismatchException e){
            System.out.println("VOCE DEVE DIGITAR UM NÚMERO");
        }
        finally {
            leitor.nextLine();
        }
        return lido;
    }
    public static int opcoesUsuario(Scanner leitor) {
        System.out.println("\n\t1 - VER CARTA");
        System.out.println("\t2 - ESCOLHER ATRIBUTO DE COMPARAÇÃO");
        int opcao = 0;
        while (opcao < 1 || opcao > 2) {
            opcao = lerNumero(leitor);
        }
        return opcao;
    }
    public static void imprimirNovasLinhas(int numero){
        for (int c = 0; c < numero; c++) {
            System.out.println("");
        }
    }
    public static void inserirPausa(long milissegundos) {
        try {
            Thread.sleep(milissegundos);
        } catch (InterruptedException e) {};
    }
    public static void fakeProcessando() {
        Random aleatorio = new Random();
        System.out.print("\t.");
        inserirPausa(aleatorio.nextInt(500));
        System.out.print(".");
        inserirPausa(aleatorio.nextInt(1000));
        System.out.print(".\n");
    }
}
