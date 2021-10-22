// package lembretes_app;

public class Data {
    // Atributos    
    private byte dia;
    private byte mes;
    private short ano;
    private boolean formato_americano;
    private byte[] diasNoMes;
    private byte inicio_ano;


    // Construtores
    public Data(byte dia, byte mes, short ano){
        this.formato_americano = false;
        if (dia < 1 || dia > 31){
            this.dia = (byte) 01;
        }
        else {
            this.dia = dia;
        }

        if (mes < 1 || mes > 12){
            this.mes = 01;
        }
        else {
            this.mes = mes;
        }

        if (ano < 1){
            this.ano = 1;
        }
        else {
            this.ano = ano;
        }

        this.diasNoMes = new byte[12];
        atualizarNumeroDiasEmCadaMes();

        
        if (ano > 1900 && ano < 2399){
            int a = ano - 1900;
            int b = (int) a / 4;
            int c = mes;
            int d = dia - 1;
            this.inicio_ano = (byte) ((a + (eBissexto() ? b-1: b) + c + d) % 7 + 1);
        }
        else {
            this.inicio_ano = 1;
        }
    }
    public Data(byte dia, byte mes) {
        this(dia, mes, (short) 01);
    }
    public Data(byte dia){
        this(dia, (byte) 01, (short) 01);
    }
    public Data(byte mes, short ano){
        this((byte) 01, mes, ano);
    }
    public Data(short ano){
        this((byte) 01, (byte) 01, ano);
    }
    public Data(Data data){
        this(data.dia, data.mes, data.ano);
    }


    // Métodos
    public byte getDia(){
        return this.dia;
    }
    public byte getMes(){
        return this.mes;
    }
    public short getAno(){
        return this.ano;
    }
    public void setFormatoAmericano(){
        this.formato_americano = true;
    }
    public void setFormatoBrasileiro(){
        this.formato_americano = false;
    }
    public void mostrarData(){
        if (this.formato_americano) {
            System.out.printf("%02d/%02d/%04d", this.getMes(), this.getDia(), this.getAno());
        }
        else {
            System.out.printf("%02d/%02d/%04d", this.getDia(), this.getMes(), this.getAno());
        }
    }
    public void incrementarDia(){
        if (getNumeroDiasMes(this.mes) < this.dia + 1){
            if (this.mes >= 12){
                boolean ano_anterior_era_bissexto = eBissexto();
                //this.inicio_ano = (byte) ((getNumeroDiasAno() + (int) this.inicio_ano -1) % 7);
                if (ano_anterior_era_bissexto){
                    this.inicio_ano = (byte)((this.inicio_ano + 2) % 7);
                }
                else {
                    this.inicio_ano = (byte)((this.inicio_ano + 1) % 7);
                }
                this.ano++;
                this.mes = 1;
                this.dia = 1;
                atualizarNumeroDiasEmCadaMes();
            }
            else {
                this.dia = 1;
                this.mes++;
            }
        }
        else {
            this.dia++;
        }
    }
    public void incrementarDias(int num_dias){
        if (num_dias < 1) return;
        while (num_dias > 0){
            incrementarDia();
            num_dias--;
        }
    }
    public void decrementarDia(){
        if (this.dia == 1){
            if (this.mes == 1){
                if (this.ano == 0001){
                    return;
                }
                else {
                    this.ano--;
                    this.mes = 12;
                    this.dia = 31;
                    // Se o ano é bissexto então diminui 2
                    // Senão diminui 1
                    if (eBissexto()){
                        if (this.inicio_ano - 2 < 1){
                            this.inicio_ano = (byte) (7 + this.inicio_ano - 2);
                        }
                        else {
                            this.inicio_ano -= 2;
                        }
                    }
                    else {
                        if (this.inicio_ano - 1 < 1){
                            this.inicio_ano = (byte) (7 + this.inicio_ano - 1);
                        }
                        else {
                            this.inicio_ano -= 1;
                        }

                    }
                    atualizarNumeroDiasEmCadaMes();
                    
                }
            }
            else {
                this.mes--;
                this.dia = (byte) this.getNumeroDiasMes(this.mes);
            } 
        }
        else {
            this.dia--;
        }
    }
    public void decrementarDias(int num_dias){
        while (num_dias > 0){
            decrementarDia();
            num_dias--;
        }
    }
    public boolean eBissexto(){
        if (this.ano % 400 == 0 || (this.ano % 4 == 0 && this.ano % 100 != 0)){
            return true;
        }
        return false;
    }
    public void setInicioAno(byte inicio){
        if (inicio < 1 || inicio > 7){
            this.inicio_ano = 1;
        }
        else {
            this.inicio_ano = inicio;
        }
    }
    public int getNumeroDiasAno(){
        int dias = 0;
        for (int c = 0; c < this.diasNoMes.length; c++){
            dias += diasNoMes[c];
        }
        return dias;
    }
    public void mostrarCalendarioAno(){
        for (int c = 0; c < diasNoMes.length; c++) {
            mostrarCalendarioMes((byte) (c+1));
        }
    }
    public void mostrarCalendarioMes(byte mes){
        // Se a pessoa colocar um mês inválido ele imprime o mês atual
        if (mes < 1 || mes > 12){
            mostrarCalendarioMes(this.getMes()); 
            return;
        }
        int inicio_mes = getInicioMes(mes);
        System.out.printf("\t\t-----MES: %02d-----\n", mes);
        System.out.println("D\tS\tT\tQ\tQ\tS\tS");
        int imprimidos = 1;
        int total_dias_no_mes = getNumeroDiasMes(mes);
        for (int i = 0; i < (total_dias_no_mes + (inicio_mes - 1)); i++){
            if ((i +1) < inicio_mes){
                System.out.print("\t");
            }
            else{
                System.out.print((imprimidos++)+"\t");
            }
            if ((i + 1) % 7 == 0){
                System.out.print("\n");
            }
        }
        //inicio_mes = (inicio_mes + total_dias_no_mes) % 7;
        System.out.println("");
    }
    public void mostrarCalendarioMes(){
        this.mostrarCalendarioMes(this.getMes());
    }
    public void mostrarInformacoesAno() {
        System.out.println("Ano: " + this.getAno());
        System.out.println("O ano " + this.getAno() + (this.eBissexto() ? "" : " não") + " é bissexto");
        System.out.println("Total de dias no ano: " + this.getNumeroDiasAno());
        switch (this.inicio_ano) {
        case 1:
            System.out.println("Ano começa no Domingo");
            break;
        case 2:
            System.out.println("Ano começa na Segunda");
            break;
        case 3:
            System.out.println("Ano começa na Terça");
            break;
        case 4:
            System.out.println("Ano começa na Quarta");
            break;
        case 5:
            System.out.println("Ano começa na Quinta");
            break;
        case 6:
            System.out.println("Ano começa na Sexta");
            break;
        case 7:
            System.out.println("Ano começa no Sábado");
            break;
        }
    }
    public void mostrarInformacoesMes(){
        System.out.println("Mes: " + this.getMes());
        System.out.println("Total de dias no mes: " + this.getNumeroDiasMes(this.mes));
        switch (getInicioMes(this.mes)) {
        case 1:
            System.out.println("Mes começa no Domingo");
            break;
        case 2:
            System.out.println("Mes começa na Segunda");
            break;
        case 3:
            System.out.println("Mes começa na Terça");
            break;
        case 4:
            System.out.println("Mes começa na Quarta");
            break;
        case 5:
            System.out.println("Mes começa na Quinta");
            break;
        case 6:
            System.out.println("Mes começa na Sexta");
            break;
        case 7:
            System.out.println("Mes começa no Sábado");
            break;
        }
    }
    public void mostrarInformacoesDia(){
        System.out.println("Dia: "+ this.getDia());
        switch (getDiaSemana(this.getDia())) {
        case 1:
            System.out.println("Domingo");
            break;
        case 2:
            System.out.println("Segunda-Feira");
            break;
        case 3:
            System.out.println("Terça-Feira");
            break;
        case 4:
            System.out.println("Quarta-Feira");
            break;
        case 5:
            System.out.println("Quinta-Feira");
            break;
        case 6:
            System.out.println("Sexta-Feira");
            break;
        case 7:
            System.out.println("Sábado");
            break;
        }
    }
    public int getNumeroDiasMes(byte mes){
        if (mes > 12 || mes < 1){
            return -1;
        }
        else {
            return diasNoMes[mes -1];
        }
    }
    public int getInicioMes(byte mes){
        int inicio_mes = (int) this.inicio_ano;
        for (byte c = 1; c < mes; c++){
            inicio_mes += getNumeroDiasMes(c);
        }
        return inicio_mes % 7;
    }
    public byte getDiaSemana(byte dia){
        if (dia > 31 || dia < 1){
            return (byte) -1;
        }
        return (byte) ((byte) (this.getInicioMes(this.mes) + (dia - 1)) % 7);
    }
    public int diasProFimDoAno(){
        int dias_ate_fim_ano = 0;
        for (byte c = getMes(); c <= 12; c++){
            dias_ate_fim_ano += getNumeroDiasMes(c);
        }
        dias_ate_fim_ano -= getDia();
        return dias_ate_fim_ano;
    }
    private void atualizarNumeroDiasEmCadaMes(){
        this.diasNoMes[0] = 31;
        this.diasNoMes[1] = (this.eBissexto() ? (byte) 29 : (byte) 28);
        this.diasNoMes[2] = 31;
        this.diasNoMes[3] = 30;
        this.diasNoMes[4] = 31;
        this.diasNoMes[5] = 30;
        this.diasNoMes[6] = 31;
        this.diasNoMes[7] = 31;
        this.diasNoMes[8] = 30;
        this.diasNoMes[9] = 31;
        this.diasNoMes[10] = 30;
        this.diasNoMes[11] = 31;
    }
    public boolean compararCom(Data data){
        if (data.getDia() == this.getDia()) {
            if (data.getMes() == this.getMes()) {
                if (data.getAno() == this.getAno()) {
                    return true;
                }
            }
        }
        return false;
    }
}
