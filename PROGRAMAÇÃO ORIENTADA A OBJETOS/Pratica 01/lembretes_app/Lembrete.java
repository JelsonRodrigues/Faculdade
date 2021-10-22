// package lembretes_app;

public class Lembrete {
    // Atributos
    private String descricao_lembrete;
    private Data data_lembrete;
    private Hora hora_lembrete;
    private String nome_lembrete;
    
    // Construtores
    public Lembrete(String nome_lembrete, String descricao_lembrete, Data data_lembrete, Hora hora_lembrete){
        this.nome_lembrete = nome_lembrete;
        this.descricao_lembrete = descricao_lembrete;
        this.data_lembrete = data_lembrete;
        this.hora_lembrete = hora_lembrete;
    }
    public Lembrete(String nome_lembrete, Data data_lembrete, Hora hora_lembrete){
        this(nome_lembrete, "Sem Descrição", data_lembrete, hora_lembrete);
    }
    public Lembrete(String nome_lembrete, Data data_lembrete){
        this(nome_lembrete, "Sem Descrição", data_lembrete, new Hora((byte)0, (byte)0, (byte)0));
    }
    public Lembrete(String nome_lembrete, byte dia, byte mes, short ano){
        this(nome_lembrete, "Sem Descrição", new Data(dia, mes, ano), new Hora((byte)0, (byte)0, (byte)0));
    }
    public Lembrete(String nome_lembrete, byte dia, byte mes, short ano, byte hora, byte minuto, byte segundo){
        this(nome_lembrete, "Sem Descrição", new Data(dia, mes, ano), new Hora(hora, minuto, segundo));
    }
    public Lembrete(Lembrete lembrete){
        this(lembrete.nome_lembrete, lembrete.descricao_lembrete, lembrete.data_lembrete, lembrete.hora_lembrete);
    }

    // Métodos
    public void imprimirLembrete(){
        System.out.println("Lembrete: "+ this.nome_lembrete);
        System.out.println("Descrição: "+ this.descricao_lembrete);
        System.out.print("Data: ");
        this.data_lembrete.mostrarData();
        System.out.print(" Hora: "); 
        this.hora_lembrete.mostraHora();
        System.out.println("");
    }
    public void alterarNome(String novo_nome){
        this.nome_lembrete = null;
        this.nome_lembrete = novo_nome;
    }
    public void alterarDescricao(String nova_descricao){
        this.descricao_lembrete = null;
        this.descricao_lembrete = nova_descricao;
    }
    public void alterarData(Data nova_data){
        this.data_lembrete = null;
        this.data_lembrete = nova_data;
    }
    public void alterarHora(Hora nova_hora){
        this.hora_lembrete = null;
        this.hora_lembrete = nova_hora;
    }
    public void adiarEmDias(int numero_dias){
        this.data_lembrete.incrementarDias(numero_dias);
    }
    public void adiarEmMinutos(int minutos){
        if (minutos <= 0) return;
        this.data_lembrete.incrementarDias(this.hora_lembrete.incrementaMinutos(minutos));
    }
    public void adiarEmSegundos(int numero_segundos){
        if (numero_segundos <= 0) return;
        this.data_lembrete.incrementarDias(this.hora_lembrete.incrementaSegundos(numero_segundos));
    }
    public void adiantarEmDias(int numero_dias){
        this.data_lembrete.decrementarDias(numero_dias);
    }
    public void adiantarEmMinutos(int minutos){
        if (minutos <= 0) return;
        this.data_lembrete.decrementarDias(this.hora_lembrete.decrementaMinutos(minutos));
    }
    public void adiantarEmSegundos(int numero_segundos){
        if (numero_segundos <= 0) return;
        this.data_lembrete.decrementarDias(this.hora_lembrete.decrementaSegundos(numero_segundos));
    }
    public String getNome(){
        return this.nome_lembrete;
    }
    public String getDescricao(){
        return this.descricao_lembrete;
    }
    public Data getData(){
        return this.data_lembrete;
    }
    public Hora getHora(){
        return this.hora_lembrete;
    }
    public boolean compararCom(Lembrete lembrete){
        if (lembrete.getNome() == this.nome_lembrete) {
            if (lembrete.getDescricao() == this.descricao_lembrete){
                if (lembrete.getData().compararCom(this.getData())){
                    if (lembrete.getHora().compararCom(this.getHora())){
                        return true;
                    }
                }                
            }
        }
        return false;
    }
}
