// package lembretes_app;

public class BlocoDeLembretes {
    // Atributos
    private int numero_lembretes_maximo;
    private int numero_lembretes;
    private int indice_primeiro;
    private int indice_ultimo;
    private Lembrete[] lembretes;

    // Construtor
    BlocoDeLembretes(){
        this.numero_lembretes_maximo = 10;
        this.lembretes = new Lembrete[numero_lembretes_maximo];
        this.numero_lembretes = 0;
        this.indice_primeiro = 0;
        this.indice_ultimo = 0;
    }

    // Métodos
    // adicionarLembrete do exercício 08
    public boolean adicionarLembrete(Lembrete lembrete){
        if (numero_lembretes >= numero_lembretes_maximo) {
            numero_lembretes_maximo = numero_lembretes_maximo * 2;
            Lembrete temp[] = new Lembrete[numero_lembretes_maximo];
            for (int c = 0; c < numero_lembretes; c++) {
                temp[c] = lembretes[c];
            }
            lembretes = temp;
        }
        lembretes[indice_ultimo] = new Lembrete(lembrete);
        indice_ultimo++;
        numero_lembretes++;
        return true;
    }
    // adicionarLembrete antes do exercício 08
    /*  --- Adição de elementos no estilo de uma FILA com o numero_lembretes_maximo fixo 
    public boolean adicionarLembrete(Lembrete lembrete){
        // Funciona como uma fila
        lembretes[indice_ultimo] = new Lembrete(lembrete);
        indice_ultimo++;
        numero_lembretes++;
        if (numero_lembretes >= numero_lembretes_maximo) {
            indice_primeiro = (indice_primeiro + 1) % numero_lembretes_maximo;
            numero_lembretes = numero_lembretes_maximo; 
        }
        indice_ultimo = (indice_ultimo + 1) % numero_lembretes_maximo;
        return true;
    }   */
    public int getNumeroLembretes(){
        return this.numero_lembretes;
    }
    public Lembrete getLembrete(int posicao){
        if (posicao <= getNumeroLembretes() && posicao > 0){
            return lembretes[posicao - 1];
        }
        return null;
    }
    public boolean excluirLembrete(int posicao){
        Lembrete lembrete_excluir = this.getLembrete(posicao);
        if (lembrete_excluir != null) {            
            int elementos_reposicionar = (indice_ultimo - posicao) % numero_lembretes_maximo;
            // Se o número der negativo, então o valor vai ser o complemento do número de lembretes máximo
            // Ex.: -2, então -2 + 10 = 8
            if (elementos_reposicionar < 0) elementos_reposicionar += numero_lembretes_maximo;
            int c = posicao -1;
            while (elementos_reposicionar > 0){
                lembretes[c] = lembretes[(c + 1) % numero_lembretes_maximo];
                c = (c + 1) % numero_lembretes_maximo;
                elementos_reposicionar--;
            }
            indice_ultimo = (indice_ultimo -1) % numero_lembretes_maximo;
            if (indice_ultimo < 0) indice_ultimo += numero_lembretes_maximo;
            numero_lembretes--;
            return true;
        }
        return false;
    }
    public boolean excluirLembrete(Lembrete lembrete){
        return excluirLembrete(posicaoLembrete(lembrete));
    }
    public void imprimirLembretes(){
        if (numero_lembretes == 0){
            System.out.println("Nenhum lembrete na lista!");
            return;
        }
        int imprimidos = 0;
        int c = indice_primeiro;
        while (imprimidos < numero_lembretes){
            System.out.println("Lembrete " + (int) (imprimidos + 1));
            lembretes[c % numero_lembretes_maximo].imprimirLembrete();
            c++;
            imprimidos++;
        }
    }
    public boolean procurarLembrete(Lembrete lembrete) {
        int elementos_verificados = 0;
        int c = indice_primeiro;
        while (elementos_verificados < numero_lembretes){
            if (lembretes[c].compararCom(lembrete)) {
                return true;
            }
            c = (c + 1) % numero_lembretes_maximo;
            elementos_verificados++;
        }
        return false;
    }
    private int posicaoLembrete(Lembrete lembrete){
        int elementos_verificados = 0;
        int c = indice_primeiro;
        while (elementos_verificados < numero_lembretes){
            if (lembretes[c].compararCom(lembrete)) {
                return c+1;
            }
            c = (c + 1) % numero_lembretes_maximo;
            elementos_verificados++;
        }
        return -1;
    }
    public BlocoDeLembretes buscarPorNome(String nome) {
        BlocoDeLembretes resultado = new BlocoDeLembretes();
        
        int elementos_verificados = 0;
        int c = indice_primeiro;
        while (elementos_verificados < numero_lembretes){
            if (lembretes[c].getNome() == nome) {
                resultado.adicionarLembrete(lembretes[c]);
            }
            c = (c + 1) % numero_lembretes_maximo;
            elementos_verificados++;
        }

        return resultado;
    }
    public BlocoDeLembretes buscarPorData(Data data) {
        BlocoDeLembretes resultado = new BlocoDeLembretes();
        
        int elementos_verificados = 0;
        int c = indice_primeiro;
        while (elementos_verificados < numero_lembretes){
            if (lembretes[c].getData().compararCom(data)) {
                //return lembretes[c];
                resultado.adicionarLembrete(lembretes[c]);
            }
            c = (c + 1) % numero_lembretes_maximo;
            elementos_verificados++;
        }

        return resultado;
    }
}
