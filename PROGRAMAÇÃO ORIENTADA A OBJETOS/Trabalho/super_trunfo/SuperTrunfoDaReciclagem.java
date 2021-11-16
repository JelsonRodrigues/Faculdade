package super_trunfo;

import java.util.ArrayList;
import java.util.Random;

import super_trunfo.erros.ArquivoNaoEncontradoException;
import super_trunfo.erros.NaoHaJogadoresSucifientesException;
import super_trunfo.erros.NumeroMaximoDeJogadoreException;
import super_trunfo.erros.SemCartasException;

public class SuperTrunfoDaReciclagem {
    private ArrayList<Jogador> jogadores_iniciais;  // Guarda os jogadores que foram adicinados inicialmente
    private ArrayList<Jogador> jogadores_ativos;    // Uma cópia dos jogadores iniciais, porém conforme os jogadores vão sendo eliminados eles saem daqui
    private ArrayList<Jogador> jogadores_rodada;    // Jogadores da rodada atual, usado também em caso de empate
    private Jogador vencedor_rodada;
    private ArrayList<Carta> cartas_mesa;
    private ArrayList<Carta> cartas_comparando;
    private Jogador jogador_atual;
    private Baralho baralho;
    private final int cartas_por_jogador = 2;
    public final int numero_maximo_jogadores = 4;
    public final int numero_minimo_jogadores = 2;

    SuperTrunfoDaReciclagem() {
        jogadores_iniciais = new ArrayList<Jogador>();
        jogadores_ativos = new ArrayList<Jogador>();
        jogadores_rodada = new ArrayList<Jogador>();
        jogador_atual = null;
        vencedor_rodada = null;
        cartas_comparando = new ArrayList<Carta>();
        cartas_mesa = new ArrayList<Carta>();
        try {
            baralho = new Baralho("Super Trunfo da Reciclagem.csv");
        }
        catch (ArquivoNaoEncontradoException e) {
            System.out.println("NÃO FOI POSSÍVEL CARREGAR O ARQUIVO!!!");   
            System.exit(-1);
        }
    }

    public void reiniciarJogo() {
        jogadores_rodada.clear();
        jogador_atual = null;
        vencedor_rodada = null;
        cartas_comparando.clear();
        cartas_mesa.clear();
        iniciar();
    }
    public int getNumeroCartasBaralho() {
        return baralho.getNumeroCartas();
    }
    public int getNumJogadoresAtivos() {
        return this.jogadores_ativos.size();
    }
    public int getNumJogadoresRodada() {
        return this.jogadores_rodada.size();
    }
    public boolean ocorreuEmpate(){
        return jogadores_rodada.size() > 1;
    }
    public ArrayList<Carta> getCartasComparando(){
        return this.cartas_comparando;
    }
    public Jogador getVencedor() {
        if (jogadores_ativos.size() == 1) {
            return jogadores_ativos.get(0);
        }
        else if (jogadores_rodada.size() == 1) {
            return jogadores_rodada.get(0);
        }
        return null;
    }
    public Jogador getJogadorAtual() throws NaoHaJogadoresSucifientesException {
        if (jogadores_ativos.size() == 0) {
            throw new NaoHaJogadoresSucifientesException("NÃO EXISTEM JOGADORES ATIVOS");
        }
        if (jogador_atual == null) {
            // Escolher aleatóriamente
            jogador_atual = jogadores_ativos.get((new Random()).nextInt(jogadores_ativos.size()));
        }
        else if (jogadores_rodada.contains(jogador_atual) == false) {
            // Isso vai acontecer quando após uma rodada ocorrer um empate e o jogador 
            // que era o atual não estiver entre os empatados
            jogador_atual = jogadores_rodada.get((new Random()).nextInt(jogadores_rodada.size()));
        }
        else if (vencedor_rodada != null) {
            jogador_atual = vencedor_rodada;
        }
        return jogador_atual;
    }
    public void adicionarJogador(String nome) throws NumeroMaximoDeJogadoreException {
        this.adicionarJogador(new Jogador(nome));
    }
    public void adicionarJogador(Jogador jogador) throws NumeroMaximoDeJogadoreException {
        if (jogadores_iniciais.size() >= this.numero_maximo_jogadores) {
            throw new NumeroMaximoDeJogadoreException("O NÚMERO MÁXIMO DE JOGADORES É " + this.numero_maximo_jogadores);
        }
        jogadores_iniciais.add(jogador);
    }
    private void atualizarVencedor() {
        if (jogadores_rodada.size() == 1) {
            vencedor_rodada = jogadores_rodada.get(0);
        }
        else {
            vencedor_rodada = null;
        }
    }
    public void imprimirJogadores() {
        imprimirJogadores(this.jogadores_iniciais);
    }
    public void imprimirJogadoresRodada(){
        imprimirJogadores(jogadores_rodada);
    }
    public static void imprimirJogadores(ArrayList<Jogador> jogadores) {
        if (jogadores.size() == 0) {
            System.out.println("NENHUM JOGADOR");
        }
        else {
            for (int c = 0; c < jogadores.size(); c++) {
                System.out.printf("--Jogador %02d--\n", c+1);
                jogadores.get(c).imprimir();
            }
        }
    }
    public void iniciar() throws NaoHaJogadoresSucifientesException {
        if (jogadores_iniciais.size() < numero_minimo_jogadores) {
            throw new NaoHaJogadoresSucifientesException("NÃO É POSSÍVEL INICIAR O JOGO, ADICIONE MAIS JOGADORES");
        }
        jogadores_ativos = copiarJogadores(jogadores_iniciais);
        distribuirCartas();
    }
    public void atualizarJogadoresRodada() {
        jogadores_rodada.clear();
        jogadores_rodada = copiarJogadores(jogadores_ativos);
    }
    private ArrayList<Jogador> copiarJogadores(ArrayList<Jogador> jogadores_copiar) {
        ArrayList<Jogador> jogadores = new ArrayList<Jogador>();
        jogadores.ensureCapacity(jogadores_copiar.size());
        for (int c = 0; c < jogadores_copiar.size(); c++){
            jogadores.add(jogadores_copiar.get(c));
        }
        return jogadores;
    }
    
    /*
    // interface textual
    public void jogar(Scanner leitor) {
        
        distribuirCartas();

        // Escolhe aleatóriamente o primeiro jogador
        Jogador jogador_atual = jogadores_ativos.get((new Random()).nextInt(jogadores_ativos.size()));
        System.out.println("\nO JOGO COMEÇA PELO JOGADOR " + jogador_atual.getNome());
        
        // Variáveis que vão ser utilizadas dentro do loop
        int opcao = 1;
        Categoria categoria = Categoria.COR;
        int rodada = 1;
        boolean ha_jogadores_empatados = false;
        ArrayList<Jogador> jogadores_removidos = new ArrayList<Jogador>();

        while (jogadores_ativos.size() > 1) {
            System.out.println("---RODADA " + rodada + "---");
            rodada++;

            // Remove jogadores sem cartas
            jogadores_removidos = removerJogadoresSemCartas();
            if (jogadores_removidos.size() > 0) {
                System.out.println("Jogadores Removidos");
                imprimirJogadores(jogadores_removidos);
            }
            if (jogadores_ativos.size() == 1) {
                // Ficou apenas um jogar, ele é o ganhador
                break; 
            }
            if (jogadores_ativos.size() == 0) {
                // As cartas de todos jogadores acabaram ao mesmo tempo
                // Ganhador é escolhido aleatóriamente
                jogadores_ativos.add(jogadores_removidos.get((new Random()).nextInt(jogadores_removidos.size())));
                break;
            }

            

            System.out.println("O JOGADOR " + jogador_atual.getNome() + " ESCOLHEU COMPARAR PELA CATEGORIA " + categoria.toString());

            ha_jogadores_empatados = false;
            cartas_mesa.clear();
            jogadores_rodada.clear();
            jogadores_rodada = copiarJogadores(jogadores_ativos);   // Inicialmente todos os jogadores ativos participam da rodada

            rodada(categoria);

            ha_jogadores_empatados = jogadores_rodada.size() > 1;

            // Só executa se jogadores tiverem empatado
            while (ha_jogadores_empatados) {
                System.out.println("\nEMPATE ENTRE OS JOGADORES");
                imprimirJogadores(jogadores_rodada);

                jogadores_removidos = removerJogadoresSemCartas(jogadores_rodada);
                if (jogadores_removidos.size() > 0) {
                    System.out.println("Jogadores Removidos");
                    imprimirJogadores(jogadores_removidos);
                }

                if (jogadores_rodada.size() == 1) {
                    break;
                }
                if (jogadores_rodada.size() == 0) {
                    // Escolhe o jogador vencedor da rodada aleatóriamente
                    jogadores_rodada.add(jogadores_removidos.get((new Random()).nextInt(jogadores_removidos.size())));
                    break;
                }
                
				// Ocorreu empate e o jogador que era o atual da rodada não está entre os empatados
                if (jogadores_rodada.contains(jogador_atual) == false) {
                    // O jogador da rodada será escolhido aleatóriamente
                    jogador_atual = jogadores_rodada.get((new Random()).nextInt(jogadores_rodada.size()));
                }
                
                System.out.println("\nO JOGADOR ATUAL É");
                jogador_atual.imprimir();
            
                opcao = 1;
                while (opcao == 1) {
                    opcao = menu(leitor);
                    if (opcao == 1) {
                        jogador_atual.verCarta();
                    }
                    else if (opcao == 2) {
                        categoria = escolhaCategoria(leitor);
                    }
                }

                System.out.println("O JOGADOR " + jogador_atual.getNome() + " ESCOLHEU COMPARAR PELA CATEGORIA " + categoria.toString());

                rodada(categoria);

                ha_jogadores_empatados = jogadores_rodada.size() > 1;   // Condição de saída do loop
            }

            adicionarCartasJogador(cartas_mesa, jogadores_rodada.get(0));
            
            // O jogador vencedor da rodada
            jogador_atual = jogadores_rodada.get(0);
            System.out.println("\nO JOGADOR " + jogador_atual.getNome() + " VENCEU A RODADA\n");
        }

        System.out.println("\nO VENCEDOR DA PARTIDA É ");
        jogadores_ativos.get(0).imprimir();

    }
    */
    // Vai salvar em jogadores_rodada o jogador que venceu, ou uma lista com os jogadores empatados
    // Ainda atualiza as cartas da mesa e retorna os jogadores que estavam no início da rodada
    public ArrayList<Jogador> rodada(Categoria categoria) {
        // Este método assume que os jogadores estão na rodada estão salvos na variável jogadores_rodada
        ArrayList<Jogador> jogadores_inicio_rodada = copiarJogadores(jogadores_rodada);
        jogadores_rodada = copiarJogadores(jogadores_ativos);
        cartasRodada();
        int[] resultado_comparacao = compararCartas(categoria);
        jogadores_rodada = vencedorRodada(resultado_comparacao);
        atualizarVencedor();
        return jogadores_inicio_rodada;
    }
    
    
    /*
    public void simular() throws NaoHaJogadoresSucifientesException {
        Random aleatorio = new Random();
        System.out.println("Seja bem vindo ao SUPER TRUNFO DA RECICLAGEM!!!");

        adicionarJogador("TESTE 01");
        adicionarJogador("TESTE 02");
        adicionarJogador("TESTE 03");
        adicionarJogador("TESTE 04");
        
        jogadores_ativos = copiarJogadores(jogadores_iniciais);
        System.out.println("\nDistribuindo as cartas...");
        distribuirCartas();

        // Escolhe aleatóriamente o primeiro jogador
        Jogador jogador_atual = jogadores_ativos.get(aleatorio.nextInt(jogadores_ativos.size()));
        System.out.println("\nO JOGO COMEÇA PELO JOGADOR " + jogador_atual.getNome());
        
        // Variáveis que vão ser utilizadas dentro do loop
        Categoria categoria = Categoria.COR;
        int rodada = 1;
        boolean ha_jogadores_empatados = false;
        ArrayList<Jogador> jogadores_rodada = new ArrayList<Jogador>();
        ArrayList<Jogador> jogadores_empatados = new ArrayList<Jogador>();
        ArrayList<Carta> cartas_comparando = new ArrayList<Carta>();
        ArrayList<Carta> cartas_mesa = new ArrayList<Carta>();
        ArrayList<Jogador> jogadores_removidos = new ArrayList<Jogador>();
        int[] resultado_comparacao = null;

        while (jogadores_ativos.size() > 1) {
            System.out.println("\n---RODADA " + rodada + "---");
            rodada++;
            
            jogadores_removidos = removerJogadoresSemCartas();
            if (jogadores_removidos.size() > 0) {
                System.out.println("Jogadores Removidos");
                imprimirJogadores(jogadores_removidos);
            }

            if (jogadores_ativos.size() == 1) break;
            if (jogadores_ativos.size() == 0) {
                jogadores_ativos.add(jogadores_removidos.get(aleatorio.nextInt(jogadores_removidos.size())));
                break;
            }
            
            System.out.println("O JOGADOR ATUAL É");
            jogador_atual.imprimir();
            
            categoria = Categoria.inteiroParaCategoria(aleatorio.nextInt(4) + 1);

            System.out.println("O JOGADOR " + jogador_atual.getNome() + " ESCOLHEU COMPARAR PELA CATEGORIA " + categoria.toString());

            ha_jogadores_empatados = false;
            cartas_mesa.clear();
            cartas_comparando.clear();
            resultado_comparacao = null;
            jogadores_empatados.clear();
            jogadores_rodada.clear();
            jogadores_rodada = copiarJogadores(jogadores_ativos);

            // Inicialmente todos os jogadores ativos participam da rodada
            cartas_comparando = cartasRodada(jogadores_rodada);
            imprimirCartasRodada(jogadores_rodada, cartas_comparando);
            for (int c = 0; c < cartas_comparando.size(); c++) {
                cartas_mesa.add(cartas_comparando.get(c));
            }
            resultado_comparacao = compararCartas(cartas_comparando, categoria);
            jogadores_empatados = vencedorRodada(resultado_comparacao, jogadores_rodada);
            ha_jogadores_empatados = jogadores_empatados.size() > 1;

            // Só executa se jogadores tiverem empatado
            while (ha_jogadores_empatados) {
                jogadores_rodada.clear();
                jogadores_rodada = copiarJogadores(jogadores_empatados);
                jogadores_empatados.clear();
                System.out.println("\nEMPATE ENTRE OS JOGADORES");
                imprimirJogadores(jogadores_rodada);

                jogadores_removidos = removerJogadoresSemCartas(jogadores_rodada);
                if (jogadores_removidos.size() > 0) {
                    System.out.println("Jogadores Removidos por estarem sem cartas");
                    imprimirJogadores(jogadores_removidos);
                }
                // Após a remoção ficou apenas um jogador
                if (jogadores_rodada.size() == 1) {
                    jogadores_empatados.add(jogadores_rodada.get(0));
                    break;
                } 
                if (jogadores_rodada.size() == 0) {
                    // Todos os jogadores da rodada foram removidos (não tinham mais cartas),
                    // então é escolhido aleatoriamente um vencedor da rodada
                    jogadores_empatados.add(jogadores_removidos.get(aleatorio.nextInt(jogadores_removidos.size())));
                    break;
                }

                System.out.println("\nO JOGADOR ATUAL É");
                jogador_atual.imprimir();
            
                categoria = Categoria.inteiroParaCategoria(aleatorio.nextInt(4) + 1);

                System.out.println("O JOGADOR " + jogador_atual.getNome() + " ESCOLHEU COMPARAR PELA CATEGORIA " + categoria.toString());

                cartas_comparando = cartasRodada(jogadores_rodada);
                imprimirCartasRodada(jogadores_rodada, cartas_comparando);
                for (int c = 0; c < cartas_comparando.size(); c++) {
                    cartas_mesa.add(cartas_comparando.get(c));
                }
                resultado_comparacao = compararCartas(cartas_comparando, categoria);
                jogadores_empatados = vencedorRodada(resultado_comparacao, jogadores_rodada);
                ha_jogadores_empatados = jogadores_empatados.size() > 1;   // Condição de saída do loop
            }

            adicionarCartasJogador(cartas_mesa, jogadores_empatados.get(0));
            
            // O jogador vencedor da rodada
            jogador_atual = jogadores_empatados.get(0);
            System.out.println("\nO JOGADOR " + jogador_atual.getNome() + " VENCEU A RODADA\n");
        }

        System.out.println("\nO VENCEDOR DA PARTIDA É ");
        jogadores_ativos.get(0).imprimir();
    }
    */
    // Distribui as cartas para os jogadores ativos
    private void distribuirCartas() {
        for (int c = 0; c < jogadores_ativos.size() * this.cartas_por_jogador; c++) {
            Carta carta;
            try {
                carta = baralho.selecionarCarta();
                jogadores_ativos.get(c % jogadores_ativos.size()).incluirCarta(carta);
            } catch (SemCartasException e) {
                break;
            }
        }
    }
    public static void imprimirCartasRodada(ArrayList<Jogador> jogadores, ArrayList<Carta> cartas) {
        if (jogadores.size() == cartas.size()) {
            System.out.println("---Cartas desta Rodada---");
            for (int c = 0; c < jogadores.size(); c++) {
                System.out.println("\nJogador: " + jogadores.get(c).getNome());
                System.out.println(cartas.get(c).toString());
            }
        }
    }
    public void imprimirCartasRodada() {
        imprimirCartasRodada(this.jogadores_rodada, this.cartas_comparando);
    }
    private void adicionarCartasJogador(ArrayList<Carta> cartas, Jogador jogador){
        while (!cartas.isEmpty()) {
            jogador.incluirCarta(cartas.remove(0));
        }
    }
    public void adicionarCartasVencedor() {
        adicionarCartasJogador(cartas_mesa, getVencedor());
    }
    // Salva as cartas dos baralhos dos jogadores da rodada na variável cartas comparando
    private void cartasRodada() {
        cartas_comparando.clear();
        for (int c = 0; c < jogadores_rodada.size(); c++) {
            if (jogadores_rodada.get(c).temCartas()) {
                cartas_comparando.add(jogadores_rodada.get(c).excluir());
            }
        }
        for (int c = 0; c < cartas_comparando.size(); c++) {
            cartas_mesa.add(cartas_comparando.get(c));
        }
    }
    // Retorna o um vetor com a comparação das cartas
    private int[] compararCartas(Categoria categoria) {
        int[] ganhou_quantas_cartas = new int[cartas_comparando.size()];
        for (int c = 0; c < cartas_comparando.size(); c++) {
            ganhou_quantas_cartas[c] = 0;
        }

        switch (categoria) {
            case COR: // Cor
                for (int c = 0; c < cartas_comparando.size(); c++) {
                    for (int i = 0; i < cartas_comparando.size(); i++) {
                        if (i != c) {
                            int comparacao = cartas_comparando.get(c).compararCor(cartas_comparando.get(i).getCor());
                            if ( comparacao == 1 ) {
                                ganhou_quantas_cartas[c]++;
                            }
                        }
                    }
                }
                break;
            case DECOMPOSICAO: // Decomposição
                for (int c = 0; c < cartas_comparando.size(); c++) {
                    for (int i = 0; i < cartas_comparando.size(); i++) {
                        if (i != c) {
                            int comparacao = cartas_comparando.get(c).compararDecomposicao(cartas_comparando.get(i).getDecomposicao());
                            if ( comparacao == 1 ) {
                                ganhou_quantas_cartas[c]++;
                            }
                        }
                    }
                }
                break;
            case RECICLAVEL: // Reciclável
                for (int c = 0; c < cartas_comparando.size(); c++) {
                    for (int i = 0; i < cartas_comparando.size(); i++) {
                        if (i != c) {
                            int comparacao = cartas_comparando.get(c).compararReciclagem(cartas_comparando.get(i).ehReciclavel());
                            if ( comparacao == 1 ) {
                                ganhou_quantas_cartas[c]++;
                            }
                        }
                    }   
                }
                break;
            case ATAQUE: // Ataque
                for (int c = 0; c < cartas_comparando.size(); c++) {
                    for (int i = 0; i < cartas_comparando.size(); i++) {
                        if (i != c) {
                            int comparacao = cartas_comparando.get(c).compararAtaque(cartas_comparando.get(i).getAtaque());
                            if ( comparacao == 1 ) {
                                ganhou_quantas_cartas[c]++;
                            }
                        }
                    }
                }
                break;
        }
        return ganhou_quantas_cartas;
    }
    // Retorna uma lista de jogadores que estão empatados, se a lista contiver um único jogador, ele é o vencedor
    // Caso contrário houve empate entre os jogadores listados
    private ArrayList<Jogador> vencedorRodada(int[] comparacao_cartas) {
        int indice_carta_ganhadora = 0;
        ArrayList<Jogador> jogadores_empatados = new ArrayList<Jogador>();

        for (int c = 0; c < jogadores_rodada.size(); c++) {
            if (comparacao_cartas[c] > comparacao_cartas[indice_carta_ganhadora]) {
                indice_carta_ganhadora = c;
                jogadores_empatados.clear();
                jogadores_empatados.add(jogadores_rodada.get(c));
            }
            else if (comparacao_cartas[c] == comparacao_cartas[indice_carta_ganhadora]) {
                jogadores_empatados.add(jogadores_rodada.get(c));
            }
        }
        
        return jogadores_empatados;
    }
    // Retorna uma lista de jogadores removidos
    public ArrayList<Jogador> removerJogadoresAtivosSemCartas() {
        ArrayList<Jogador> removidos = removerJogadoresSemCartas(this.jogadores_ativos);
        if (jogadores_ativos.size() == 0) {
            jogadores_ativos.add(removidos.get((new Random()).nextInt(removidos.size())));
        }
        return removidos;
    }
    public ArrayList<Jogador> removerJogadoresRodadaSemCartas() {
        ArrayList<Jogador> removidos = removerJogadoresSemCartas(this.jogadores_rodada);
        if (jogadores_rodada.size() == 0) {
            jogadores_rodada.add(removidos.get((new Random()).nextInt(removidos.size())));
        }
        return removidos;
    }
    private static ArrayList<Jogador> removerJogadoresSemCartas(ArrayList<Jogador> jogadores) {
        ArrayList<Jogador> jogadores_removidos = new ArrayList<Jogador>();
        for (int c = 0; c < jogadores.size(); c++){
            if (jogadores.get(c).temCartas() == false){
                jogadores_removidos.add(jogadores.remove(c));
                c--;
            }
        }
        return jogadores_removidos;
    }
}