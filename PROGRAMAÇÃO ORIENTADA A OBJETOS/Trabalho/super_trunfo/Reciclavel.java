package super_trunfo;

public class Reciclavel extends Carta {
    Reciclavel(Codigo codigo, String nome, String descricao, String tipo, Cor cor, double decomposicao, int ataque) {
        super(codigo, nome, descricao, tipo, cor, decomposicao, ataque);
    }
    Reciclavel (Carta c){
        super(c.getCodigo(), c.getNome(), c.getDescricao(), c.getTipo(), c.getCor(), c.getDecomposicao(), c.getAtaque());
    }

    boolean ehReciclavel() {
        return true;
    }
    public int compararReciclagem(boolean reciclavel) {
        if (this.ehReciclavel() == reciclavel) return 0;
        if (this.ehReciclavel()) return 1;
        else return -1;
    }
    public int compararReciclagem(Carta c) {
        return compararReciclagem(c.ehReciclavel());
    }
    @Override
    public String toString(){
        return (super.toString() +
                "\nReciclavel: SIM");
    }
}
