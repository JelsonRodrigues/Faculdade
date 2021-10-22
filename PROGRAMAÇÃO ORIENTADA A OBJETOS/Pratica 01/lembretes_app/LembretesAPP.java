// package lembretes_app;

import java.util.Scanner;

public class LembretesAPP {
    public static void main(String[] args){
        Scanner leitor = new Scanner(System.in);
        BlocoDeLembretes handlerLembretes = new BlocoDeLembretes();
        
        int opcao = 0;

        while (true){
            opcao = menu(leitor);
            switch (opcao) {
            case 1:
                Lembrete novo_lembrete = lerLembrete(leitor);
                handlerLembretes.adicionarLembrete(novo_lembrete);
                break;
            case 2:
                if (handlerLembretes.getNumeroLembretes() == 0) {
                    System.out.println("Nenhum Lembrete na lista");
                    continue;
                }
                Lembrete lembrete_excluir = lerLembrete(leitor);
                handlerLembretes.excluirLembrete(lembrete_excluir);
                break;
            case 3:
                System.out.println("LEMBRETES ARMAZENADOS");
                handlerLembretes.imprimirLembretes();
                break;
            case 4:
                Data data = lerData(leitor);
                handlerLembretes.buscarPorData(data).imprimirLembretes();
                break;
            case 5:
                System.out.println("Digite o nome: ");
                String nome = leitor.nextLine();
                handlerLembretes.buscarPorNome(nome).imprimirLembretes();
                break;
            case 6:
                return;
            }
        }
    }
    public static int menu(Scanner leitor){
        System.out.println("----MENU----");
        System.out.println("1 - Adicionar Lembrete");
        System.out.println("2 - Remover Lembrete");
        System.out.println("3 - Listar");
        System.out.println("4 - Buscar por Data");
        System.out.println("5 - Buscar por Nome");
        System.out.println("6 - Sair");
        
        int opcao = 0;
        do {
            try {
                opcao = leitor.nextInt();
            } 
            catch (java.util.InputMismatchException e) {
                System.out.println("Dado Inválido -- Digite novamente");
            }
            leitor.nextLine();  // Pega o enter digitado
        } while (opcao < 1 || opcao > 6);

        return opcao;
    }
    public static Lembrete lerLembrete(Scanner leitor){
        Lembrete lembrete = new Lembrete("", "", new Data( (byte) 00, (byte) 00, (short) 00), new Hora((byte) 00, (byte) 00, (byte) 00));
        System.out.println("Digite o Nome do Lembrete: ");
        String nome = leitor.nextLine();
        lembrete.alterarNome(nome);

        System.out.println("Digite a Descrição do Lembrete: ");
        String descricao = leitor.nextLine();
        lembrete.alterarDescricao(descricao);

        System.out.println("Adicionar Data? s/n");
        String opcao = leitor.nextLine();
        if ( opcao.compareToIgnoreCase("s") == 0 ) {
            lembrete.alterarData(lerData(leitor));
        }

        System.out.println("Adicionar Hora? s/n");
        opcao = leitor.nextLine();
        if (opcao.compareToIgnoreCase("s") == 0 ) {
            lembrete.alterarHora(lerHora(leitor));
        }
        
        return lembrete;
    }
    public static Hora lerHora(Scanner leitor){
        byte hora = 0;
        byte minuto = 0;
        byte segundo = 0;

        System.out.println("Digite a Hora do lembrete: ");
        do {
            try {
                hora = leitor.nextByte();
            }
            catch (java.util.InputMismatchException e){
                System.out.println("Dado Inválido -- Digite novamente");
            }
        } while (hora < 0 || hora > 24);

        System.out.println("Digite o Minuto do lembrete: ");
        do {
            try {
                minuto = leitor.nextByte();
            }
            catch (java.util.InputMismatchException e){
                System.out.println("Dado Inválido -- Digite novamente");
            }
        } while (minuto < 0 || minuto > 60);

        System.out.println("Digite o segundo do lembrete: ");
        do {
            try {
                segundo = leitor.nextByte();
            }
            catch (java.util.InputMismatchException e){
                System.out.println("Dado Inválido -- Digite novamente");
            }
        } while (segundo < 0 || segundo > 60);

        leitor.nextLine();  // Limpa o buffer
        return new Hora(hora, minuto, segundo);
    }
    public static Data lerData(Scanner leitor){
        byte dia = 0;
        byte mes = 0;
        short ano = 0;

        System.out.println("Digite o Dia do Lembrete ");
        do {
            try {
                dia = leitor.nextByte();
            }
            catch (java.util.InputMismatchException e){
                System.out.println("Dado Inválido -- Digite novamente");
            }
        } while (dia < (byte) 1 || dia > (byte) 31);

        System.out.println("Digite o Mês do Lembrete: ");
        do {
            try {
                mes = leitor.nextByte();
            }
            catch (java.util.InputMismatchException e){
                System.out.println("Dado Inválido -- Digite novamente");
            }
        } while (mes < (byte) 1 || mes > (byte) 12);

        System.out.println("Digite o Ano do Lembrete: ");
        do {
            try {
                ano = leitor.nextShort();
            }
            catch (java.util.InputMismatchException e){
                System.out.println("Dado Inválido -- Digite novamente");
            }
        } while (ano < (short) 0);
        
        leitor.nextLine();  // Limpa o buffer
        return new Data(dia, mes, ano);
    }
}
