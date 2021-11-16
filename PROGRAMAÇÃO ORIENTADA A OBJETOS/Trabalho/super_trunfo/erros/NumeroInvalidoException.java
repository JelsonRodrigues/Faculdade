package super_trunfo.erros;

public class NumeroInvalidoException extends RuntimeException {
    public NumeroInvalidoException(String razao){
        super(razao);
    }
    public NumeroInvalidoException(){
        super();
    }
}
