package super_trunfo;

import super_trunfo.erros.CodigoNaoExistenteException;

public enum Codigo {
    A1("A1"), A2("A2"), A3("A3"), A4("A4"),
    B1("B1"), B2("B2"), B3("B3"), B4("B4"),
    C1("C1"), C2("C2"), C3("C3"), C4("C4"),
    D1("D1"), D2("D2"), D3("D3"), D4("D4"),
    E1("E1"), E2("E2"), E3("E3"), E4("E4"),
    F1("F1"), F2("F2"), F3("F3"), F4("F4"),
    G1("G1"), G2("G2"), G3("G3"), G4("G4"),
    H1("H1"), H2("H2"), H3("H3"), H4("H4"),
    MEGAWINNER("MEGAWINNER");

    private final String codigo;

    Codigo(String codigo) {
        this.codigo = codigo;
    }

    @Override
    public String toString(){
        return codigo;
    }
    public boolean equals(Codigo c) {
        return this.codigo.equals(c.toString());
    }

    public static Codigo stringParaCodigo(String s) throws CodigoNaoExistenteException {
        if (s.compareToIgnoreCase(A1.toString()) == 0) return A1;
        if (s.compareToIgnoreCase(A2.toString()) == 0) return A2;
        if (s.compareToIgnoreCase(A3.toString()) == 0) return A3;
        if (s.compareToIgnoreCase(A4.toString()) == 0) return A4;
        if (s.compareToIgnoreCase(B1.toString()) == 0) return B1;
        if (s.compareToIgnoreCase(B2.toString()) == 0) return B2;
        if (s.compareToIgnoreCase(B3.toString()) == 0) return B3;
        if (s.compareToIgnoreCase(B4.toString()) == 0) return B4;
        if (s.compareToIgnoreCase(C1.toString()) == 0) return C1;
        if (s.compareToIgnoreCase(C2.toString()) == 0) return C2;
        if (s.compareToIgnoreCase(C3.toString()) == 0) return C3;
        if (s.compareToIgnoreCase(C4.toString()) == 0) return C4;
        if (s.compareToIgnoreCase(D1.toString()) == 0) return D1;
        if (s.compareToIgnoreCase(D2.toString()) == 0) return D2;
        if (s.compareToIgnoreCase(D3.toString()) == 0) return D3;
        if (s.compareToIgnoreCase(D4.toString()) == 0) return D4;
        if (s.compareToIgnoreCase(E1.toString()) == 0) return E1;
        if (s.compareToIgnoreCase(E2.toString()) == 0) return E2;
        if (s.compareToIgnoreCase(E3.toString()) == 0) return E3;
        if (s.compareToIgnoreCase(E4.toString()) == 0) return E4;
        if (s.compareToIgnoreCase(F1.toString()) == 0) return F1;
        if (s.compareToIgnoreCase(F2.toString()) == 0) return F2;
        if (s.compareToIgnoreCase(F3.toString()) == 0) return F3;
        if (s.compareToIgnoreCase(F4.toString()) == 0) return F4;
        if (s.compareToIgnoreCase(G1.toString()) == 0) return G1;
        if (s.compareToIgnoreCase(G2.toString()) == 0) return G2;
        if (s.compareToIgnoreCase(G3.toString()) == 0) return G3;
        if (s.compareToIgnoreCase(G4.toString()) == 0) return G4;
        if (s.compareToIgnoreCase(H1.toString()) == 0) return H1;
        if (s.compareToIgnoreCase(H2.toString()) == 0) return H2;
        if (s.compareToIgnoreCase(H3.toString()) == 0) return H3;
        if (s.compareToIgnoreCase(H4.toString()) == 0) return H4;
        if (s.compareToIgnoreCase(MEGAWINNER.toString()) == 0) return MEGAWINNER;

        throw new CodigoNaoExistenteException("STRING PASSADA: " + s + " NÃO TEM NENHUM CÓDIGO VÁLIDO");
    }
}
