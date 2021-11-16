package super_trunfo.erros;

public class NaoHaJogadoresSucifientesException extends RuntimeException {
    public NaoHaJogadoresSucifientesException(String razao){ 
        super(razao);
    }
    public NaoHaJogadoresSucifientesException(){ 
        super();
    }
}
