package super_trunfo;

import java.util.LinkedList;
import super_trunfo.erros.SemCartasException;

public class Jogador {
    // Atributos
    private String nome;
    private LinkedList<Carta> cartas_jogador;

    // Construtor
    public Jogador(String nome){
        this.cartas_jogador = new LinkedList<Carta>();
        this.nome = nome;
    }

    // Métodos
    @Override
    public String toString(){
        return ("Nome: " + this.getNome() +
                "\nTotal Cartas: " + this.getNumeroCartas());
    }

    public String getNome() {
        return this.nome;
    }
    public int getNumeroCartas() {
        return cartas_jogador.size();
    }
    public void incluirCarta(Carta carta) {
        cartas_jogador.addLast(carta);
    }
    public Carta excluir() throws SemCartasException {
        if (this.temCartas() == false) {
            throw new SemCartasException("JOGADOR NÃO POSSUI NENHUMA CARTA");
        }
        return cartas_jogador.remove(0);
    }
    public boolean temCartas() {
        return !cartas_jogador.isEmpty();
        // return getNumeroCartas() == 0;
    }
    public void imprimir(){
        System.out.println(this.toString());
    }
    public void verCarta() {
        try {
            Carta carta_temporaria = cartas_jogador.getFirst();
            System.out.println(carta_temporaria.toString());
        } catch (SemCartasException e) {
            System.out.println("JOGADOR NÃO POSSUI NENHUMA CARTA");
        }
    }
}
