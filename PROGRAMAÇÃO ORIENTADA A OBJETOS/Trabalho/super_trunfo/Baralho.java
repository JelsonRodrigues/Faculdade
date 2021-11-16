package super_trunfo;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import java.util.ArrayList;
import java.util.Random;

import super_trunfo.erros.SemCartasException;
import super_trunfo.erros.ArquivoNaoEncontradoException;

public class Baralho {
    // Atributos
    private ArrayList<Carta> cartas;

    // Contrutor
    public Baralho(String arquivo_para_leitura) {
        this.cartas = new ArrayList<Carta>();
        lerDoArquivo(arquivo_para_leitura);
    }

    // Métodos
    public Carta selecionarCarta() throws SemCartasException {
        if (cartas.isEmpty()) {
            throw new SemCartasException("NENHUMA CARTA NO BARALHO");
        }
        else {
            int indice = (new Random()).nextInt(cartas.size());
            return cartas.get(indice);
        }
    }
    private void lerDoArquivo(String arquivo) throws ArquivoNaoEncontradoException {
        // Eu poderia deixar a classe lançar o FileNotFoundException, mas para demonstrar um pouco de 
        // tratamento de exceções aprendido na disciplina fiz meu proprio ArquivoNaoEncontradoException
        BufferedReader entrada;
        String linha;
        
        try {
            entrada = new BufferedReader(new FileReader(arquivo));
            while (entrada != null) {
                linha = entrada.readLine();
                if (linha == null) break;   // Fim do arquivo
                String[] colunas = linha.split(";");
                if (colunas.length == 8) {
                    if (colunas[7].equals("sim")) {
                        cartas.add(new Reciclavel(
                                    Codigo.stringParaCodigo(colunas[0]),
                                    colunas[1],
                                    colunas[2],
                                    colunas[3],
                                    Cor.stringToCor(colunas[4]),
                                    Double.parseDouble(colunas[5]),
                                    Integer.parseInt(colunas[6])
                                ));
                    }
                    else if (colunas[7].equals("não")) {
                        cartas.add(new NaoReciclavel(
                                    Codigo.stringParaCodigo(colunas[0]),
                                    colunas[1],
                                    colunas[2],
                                    colunas[3],
                                    Cor.stringToCor(colunas[4]),
                                    Double.parseDouble(colunas[5]),
                                    Integer.parseInt(colunas[6])
                                ));
                    }
                }
            }
        }
        catch (FileNotFoundException e) {
            throw new ArquivoNaoEncontradoException("O arquivo "+ arquivo +" não foi encontrado!");
        }
        catch (IOException e){
            System.err.println("ERRO DE I/O");
        }
    }
    public int getNumeroCartas() {
        return cartas.size();
    }
    public void imprimir() {
        for (Carta carta : cartas) {
            System.out.println(carta.toString());
        }
    }
}
