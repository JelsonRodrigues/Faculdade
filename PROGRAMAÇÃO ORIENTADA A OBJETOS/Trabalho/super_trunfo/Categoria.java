package super_trunfo;

import super_trunfo.erros.NumeroInvalidoException;

public enum Categoria {
    COR, DECOMPOSICAO, RECICLAVEL, ATAQUE;

    public static Categoria inteiroParaCategoria(int numero) throws NumeroInvalidoException {
        if (numero > 4 || numero < 1){
            throw new NumeroInvalidoException("O NÚMERO DEVE SER ENTRE 1 E 4");
        }
        switch (numero){
            case 1:
                return Categoria.COR;
            case 2:
                return Categoria.DECOMPOSICAO;
            case 3:
                return Categoria.RECICLAVEL;
            case 4:
                return Categoria.ATAQUE;
            default:
                return Categoria.COR;
        }
    }
}
