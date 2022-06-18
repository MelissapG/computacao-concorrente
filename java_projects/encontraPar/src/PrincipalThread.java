// classe da estrutura de dados (recurso) compartilhado entre as threads
class Compartilhado {
    // recurso compartilhado
    private int cont_global;
    private int total;

    // construtor
    public Compartilhado() {
        this.cont_global = 0;
        this.total = 0;
    }

    public synchronized int inc_contglobal() {
        this.cont_global++;
        return this.cont_global-1;
    }

    public synchronized void inc_total() {
        this.total++;
    }

    public synchronized int get_total() {
        return this.total;
    }
}

// funcao executada pelas threads
class Find extends Thread {
    Compartilhado cont_local;
    int[] vet;

    // construtor das threads
    public Find(Compartilhado compartilhado, int[] vet) {
        this.cont_local = compartilhado;
        this.vet = vet;
    }

    // metodo executado pelas threads
    public void run() {
        int cont = this.cont_local.inc_contglobal();
        while(cont < vet.length) {
            if (vet[cont] % 2 == 0) { // confere se eh par
                cont_local.inc_total();
            }
            cont = this.cont_local.inc_contglobal();
        }
    }
}

// classe da main
class PrincipalThread {
    static final int n_threads = 4;
    static final int tam_vet = 7235;

    public static void main (String[] args) {
        //--reserva espaço para um vetor de threads
        Thread[] threads = new Thread[n_threads];
        //--vetor inicial de tamanho tam_vet
        int[] vet = new int[tam_vet];
        //--total sequencial para comparar corretude
        int total_seq = 0;

        //cria uma instancia do recurso compartilhado entre as threads
        Compartilhado compartilhado = new Compartilhado();

        //preenche o vetor
        for(int i=0; i<tam_vet; i++) {
            vet[i] = i;
        }

        //--PASSO 2: cria as threads
        for (int i=0; i<threads.length; i++) {
//            System.out.println("--Cria a thread " + i);
            threads[i] = new Find(compartilhado, vet);
        }

        //--PASSO 3: inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //--PASSO 4: espera pelo termino das threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); }
            catch (InterruptedException e) { return; }
        }

        // cria um total de numeros pares executando sequencialmente
        for(int i=0; i<tam_vet; i++) {
            if(vet[i] % 2 == 0) {
                total_seq++;
            }
        }

        System.out.println("Total de pares calculado sequencial = " + total_seq);
        System.out.println("Total de pares calculado concorrente = " + compartilhado.get_total());

        if(total_seq == compartilhado.get_total()) {
            System.out.println("O programa rodou corretamente!");
        }
        else {
            System.out.println("O programa nao esta correto, valores totais diferem!");
        }
    }
}