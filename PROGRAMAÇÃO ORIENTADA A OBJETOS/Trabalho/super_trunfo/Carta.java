package super_trunfo;

public abstract class Carta {
    // Atributos
    private Codigo codigo;
    private String nome;
    private String descricao;
    private String tipo;
    private Cor cor;
    private double decomposicao;
    private int ataque;
    
    // Construtor
    public Carta(Codigo codigo, String nome, String descricao, String tipo, Cor cor, double decomposicao, int ataque){
        this.codigo = codigo;
        this.nome = nome;
        this.descricao = descricao;
        this.tipo = tipo;
        this.cor = cor;
        this.decomposicao = decomposicao;
        this.ataque = ataque;
    }

    // Métodos
    abstract boolean ehReciclavel();
    abstract int compararReciclagem(Carta c);
    abstract int compararReciclagem(boolean reciclavel);

    @Override
    public String toString(){
        return ("\nCodigo: " + this.codigo +
                "\nNome: " + this.nome + 
                "\nDescrição: " + this.descricao + 
                "\nTipo: " + this.tipo +
                "\nCor: " + this.cor + 
                "\nDecomposição: " + this.decomposicao +
                "\nAtaque: " + this.ataque);
    }
    public boolean equals(Carta c) { 
        return this.toString().compareTo(c.toString()) == 0;
    }

    // Getters
    public int getAtaque() {
        return this.ataque;
    }
    public Codigo getCodigo() {
        return this.codigo;
    }
    public String getNome() {
        return this.nome;
    }
    public String getDescricao() {
        return this.descricao;
    }
    public double getDecomposicao() {
        return this.decomposicao;
    }
    public String getTipo() {
        return this.tipo;
    }
    public Cor getCor() {
        return this.cor;
    }

    // Comparação dos atributos
    public int compararAtaque(int ataque){
        if (this.getAtaque() == ataque) return 0;
        if (this.getAtaque() > ataque) {
            return 1;
        }
        else {
            return -1;
        }
    }
    public int compararAtaque(Carta c){
        return compararAtaque(c.getAtaque());
    }
    public int compararDecomposicao(double decomposicao) {
        if (this.getDecomposicao() == decomposicao) return 0;
        if (this.getDecomposicao() < decomposicao) {
            return 1;
        }
        else {
            return -1;
        }
    }
    public int compararDecomposicao(Carta c) {
        return compararDecomposicao(c.getDecomposicao());
    }
    public int compararCor(Cor c) {
        Cor esta_carta = this.getCor();
        Cor outra_carta = c;

        if (esta_carta.equals(outra_carta)) return 0;
        
        /*
        Na concepção do jogo o tipo vermelho ganha do preto, mas o preto também ganha do vermelho, 
        isso ocorre com laranja e verde também e com marrom e roxo, nestes casos é considerado empate
        */
        
        if (esta_carta.equals(Cor.PRETO) && outra_carta.equals(Cor.VERMELHO)) return 0;
        if (esta_carta.equals(Cor.VERMELHO) && outra_carta.equals(Cor.PRETO)) return 0;
        if (esta_carta.equals(Cor.LARANJA) && outra_carta.equals(Cor.VERDE)) return 0;
        if (esta_carta.equals(Cor.VERDE) && outra_carta.equals(Cor.LARANJA)) return 0;
        if (esta_carta.equals(Cor.ROXO) && outra_carta.equals(Cor.MARROM)) return 0;
        if (esta_carta.equals(Cor.MARROM) && outra_carta.equals(Cor.ROXO)) return 0;

        switch (esta_carta) {
            case VERMELHO:
                switch (outra_carta) {
                    case AMARELO: return 1;
                    case VERDE: return 1;
                    case MARROM: return 1;
                    case CINZA: return 1;
                    case PRETO: return 1;
                    default: return -1;
                }
            case AZUL:
                switch (outra_carta) {
                    case AMARELO: return 1;
                    case VERDE: return 1;
                    case MARROM: return 1;
                    case CINZA: return 1;
                    case VERMELHO: return 1;
                    default: return -1;
                }
            case AMARELO:
                switch (outra_carta) {
                    case BRANCO: return 1;
                    case VERDE: return 1;
                    case MARROM: return 1;
                    case CINZA: return 1;
                    case PRETO: return 1;
                    default: return -1;
                }
            case VERDE:
                switch (outra_carta) {
                    case BRANCO: return 1;
                    case LARANJA: return 1;
                    case MARROM: return 1;
                    case CINZA: return 1;
                    case PRETO: return 1;
                    default: return -1;
                }
            case CINZA:
                switch (outra_carta) {
                    case BRANCO: return 1;
                    case LARANJA: return 1;
                    case ROXO: return 1;
                    case AZUL: return 1;
                    case PRETO: return 1;
                    default: return -1;
                }
            case LARANJA:
                switch (outra_carta) {
                    case VERMELHO: return 1;
                    case AMARELO: return 1;
                    case ROXO: return 1;
                    case AZUL: return 1;
                    case VERDE: return 1;
                    default: return -1;
                }
            case MARROM:
                switch (outra_carta) {
                    case BRANCO: return 1;
                    case LARANJA: return 1;
                    case ROXO: return 1;
                    case CINZA: return 1;
                    case PRETO: return 1;
                    default: return -1;
                }
            case ROXO:
                switch (outra_carta) {
                    case AZUL: return 1;
                    case VERMELHO: return 1;
                    case AMARELO: return 1;
                    case VERDE: return 1;
                    case MARROM: return 1;
                    default: return -1;
                }
            case BRANCO:
                switch (outra_carta) {
                    case AZUL: return 1;
                    case VERMELHO: return 1;
                    case AMARELO: return 1;
                    case ROXO: return 1;
                    case LARANJA: return 1;
                    default: return -1;
                }
            case PRETO:
                switch (outra_carta) {
                    case BRANCO: return 1;
                    case LARANJA: return 1;
                    case ROXO: return 1;
                    case AZUL: return 1;
                    case VERMELHO: return 1;
                    default: return -1;
                }
            case MAIOR:
                return 1;
            case MENOR:
                return -1;
            case INDEFINIDA: 
                return 0;
        }
        return 0;
    }
    public int compararCor(Carta c) {
        return compararCor(c.getCor());
    }
}
