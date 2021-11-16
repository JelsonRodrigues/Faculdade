package super_trunfo.erros;

public class ArquivoNaoEncontradoException extends RuntimeException {
    public ArquivoNaoEncontradoException(String razao) {
        super(razao);
    }
    public ArquivoNaoEncontradoException(){
        super();
    }
}
