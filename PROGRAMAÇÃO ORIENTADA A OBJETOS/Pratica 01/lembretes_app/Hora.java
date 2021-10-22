// package lembretes_app;

public class Hora {
    // Atributos
    private byte hora;
    private byte minuto;
    private byte segundo;
    private boolean formato_24h;

    // Construtores
    public Hora(byte hora, byte minuto, byte segundo){
        this.formato_24h = true;
        if (hora < 0 || hora > 23){
            this.hora = 0;
        }
        else {
            this.hora = hora;
        }

        if (minuto < 0 || minuto > 59){
            this.minuto = 0;
        }
        else {
            this.minuto = minuto;
        }
        
        if (segundo < 0 || segundo > 59){
            this.segundo = 0;
        }
        else {
            this.segundo = segundo;
        }
    }
    public Hora(Hora hora){
        this(hora.hora, hora.minuto, hora.segundo);
    }
    public Hora(byte hora, byte minuto){
        this(hora, minuto, (byte) 0);
    }
    public Hora(byte hora){
        this(hora, (byte) 0, (byte) 0);
    }
    public Hora(){
        this((byte) 0, (byte) 0, (byte) 0);
    }

    // Métodos
    public byte getHora(){
        return this.hora;
    }
    public byte getMinuto(){
        return this.minuto;
    }
    public byte getSegundo(){
        return this.segundo;
    }
    public void mostraHora(){
        if (this.formato_24h){
            System.out.printf("%02d:%02d:%02d", this.getHora(), this.getMinuto(), this.getSegundo());
        }
        else {
            if (this.getHora() > 12) {
                System.out.printf("%02d:%02d:%02d PM", this.getHora() % 12, this.getMinuto(), this.getSegundo());
            }
            else {
                System.out.printf("%02d:%02d:%02d AM", this.getHora(), this.getMinuto(), this.getSegundo());
            }
        }
    }
    public void setFormato24h(){
        this.formato_24h = true;
    }
    public void setFormato12h(){
        this.formato_24h = false;
    }
    // Retorna se foi avançado um dia ou não
    public boolean incrementaSegundo(){
        if (this.segundo >= 59){
            if (this.minuto >= 59){
                if (this.hora >= 23){
                    this.hora = (byte) 00;
                    this.minuto = (byte) 00;
                    this.segundo = (byte) 00;
                    return true;
                }
                else {
                    this.hora++;
                    this.minuto = (byte) 00;
                    this.segundo = (byte) 00;
                }
            }
            else {
                this.minuto++;
                this.segundo = (byte) 00;
            }
        }
        else {
            this.segundo++;
        }
        return false;
    }
    // Esta função retorna o número de dias que foi avançado
    public int incrementaSegundos(int num_segundos) {
        if (num_segundos <= 0) return 0;
        int numero_dias = 0;
        if ((int)(num_segundos / (int)(60*60*24)) > 1){
            numero_dias = num_segundos / (int)(60*60*24);
            num_segundos = num_segundos % (int)(60*60*24);
        }

        byte segundos_incrementar = (byte) (num_segundos % 60);
        byte minutos_incrementar = (byte) ((num_segundos / 60) % 60);
        byte horas_incrementar = (byte) (((num_segundos / 60) / 60) % 24);
        
        if (this.segundo + segundos_incrementar >= 60){
            minutos_incrementar++;
        }

        if (this.minuto + minutos_incrementar >= 60){
            horas_incrementar++;
        }

        if (this.hora + horas_incrementar >= 24) {
            numero_dias++;
        }


        this.segundo = (byte) ((this.segundo + segundos_incrementar) % 60);
        this.minuto = (byte) ((this.minuto + minutos_incrementar) % 60);
        this.hora = (byte) ((this.hora + horas_incrementar) % 24);

        return (int) numero_dias;
    }
    public int incrementaMinutos(int minutos){
        if (minutos <= 0) return 0;
        return this.incrementaSegundos(minutos * 60);
    }
    public int decrementaSegundos(int num_segundos) {
        if (num_segundos <= 0) return 0;
        int num_dias = 0;
        if (num_segundos / (24*60*60) > 1){
            num_dias = num_segundos / (24*60*60);
            num_segundos = num_segundos % (24*60*60);
        }
        
        byte segundos_decrementar = (byte)(num_segundos % 60);
        byte minutos_decrementar = (byte)((num_segundos / 60) % 60);
        byte horas_decrementar = (byte)(((num_segundos / 60) / 60) % 24);

        if (this.segundo - segundos_decrementar < 0){
            this.segundo = (byte) (60 - segundos_decrementar);
            minutos_decrementar++;
        }
        else {
            this.segundo -= segundos_decrementar;
        }

        if (this.minuto - minutos_decrementar < 0){
            this.minuto = (byte) (60 - minutos_decrementar);
            horas_decrementar++;
        }
        else {
            this.minuto -= minutos_decrementar;
        }

        if (this.hora - horas_decrementar < 0){
            this.hora = (byte) (24 - horas_decrementar);
            num_dias++;
        }
        else {
            this.hora -= horas_decrementar;
        }

        return num_dias;
    }
    public int decrementaMinutos(int minutos){
        if (minutos <= 0) return 0;
        return this.decrementaSegundos(60*minutos);
    }
    public boolean compararCom(Hora hora){
        if (hora.getHora() == this.getHora()){
            if (hora.getMinuto() == this.getMinuto()){
                if (hora.getSegundo() == this.getSegundo()){
                    return true;
                }
            }
        }
        return false;
    }
}
