package super_trunfo.erros;

public class CodigoNaoExistenteException extends RuntimeException {
    public CodigoNaoExistenteException(String razao) {
        super(razao);
    }
    public CodigoNaoExistenteException() {
        super();
    }
}
