package super_trunfo.erros;

public class NumeroMaximoDeJogadoreException extends RuntimeException {
    public NumeroMaximoDeJogadoreException(String razao) {
        super(razao);
    }
    public NumeroMaximoDeJogadoreException() {
        super();
    }
}
