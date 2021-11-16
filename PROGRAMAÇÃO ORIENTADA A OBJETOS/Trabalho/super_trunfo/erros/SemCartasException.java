package super_trunfo.erros;

public class SemCartasException extends RuntimeException {
    public SemCartasException(String razao) {
        super(razao);
    }
    public SemCartasException(){
        super();
    }
}
