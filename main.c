#include <stdio.h>

// ===== Definição da classe Pessoa =====
typedef struct {
    // Parâmetros (dados)
    char* nome;
    int idade;
    
    // Métodos (ponteiros para funções)
    void (*apresentar)(void*);
    void (*envelhecer)(void*);
    void (*destruir)(void*);
} Pessoa;

// Implementações dos métodos da Pessoa
void pessoa_apresentar(void* self_ptr) {
    Pessoa* self = (Pessoa*)self_ptr;
    printf("Olá, meu nome é %s e tenho %d anos.\n", self->nome, self->idade);
}

void pessoa_envelhecer(void* self_ptr) {
    Pessoa* self = (Pessoa*)self_ptr;
    self->idade++;
    printf("%s agora tem %d anos.\n", self->nome, self->idade);
}

void pessoa_destruir(void* self_ptr) {
    Pessoa* self = (Pessoa*)self_ptr;
    printf("Destruindo pessoa: %s\n", self->nome);
    free(self->nome);
    free(self);
}

// Construtor para Pessoa
Pessoa* pessoa_criar(const char* nome, int idade) {
    Pessoa* nova_pessoa = (Pessoa*)malloc(sizeof(Pessoa));
    
    // Aloca e copia o nome
    nova_pessoa->nome = (char*)malloc(strlen(nome) + 1);
    strcpy(nova_pessoa->nome, nome);
    
    nova_pessoa->idade = idade;
    
    // Atribui os métodos
    nova_pessoa->apresentar = pessoa_apresentar;
    nova_pessoa->envelhecer = pessoa_envelhecer;
    nova_pessoa->destruir = pessoa_destruir;
    
    return nova_pessoa;
}

// ===== Definição da classe Estudante (herda de Pessoa) =====
typedef struct {
    // Herança: primeiro campo é a classe base
    Pessoa base;
    
    // Atributos específicos de Estudante
    char* curso;
    
    // Métodos específicos de Estudante
    void (*estudar)(void*);
} Estudante;

// Implementações dos métodos do Estudante
void estudante_apresentar(void* self_ptr) {
    Estudante* self = (Estudante*)self_ptr;
    printf("Olá, sou %s, tenho %d anos e estudo %s.\n", 
           self->base.nome, self->base.idade, self->curso);
}

void estudante_estudar(void* self_ptr) {
    Estudante* self = (Estudante*)self_ptr;
    printf("%s está estudando %s.\n", self->base.nome, self->curso);
}

void estudante_destruir(void* self_ptr) {
    Estudante* self = (Estudante*)self_ptr;
    printf("Destruindo estudante: %s do curso %s\n", self->base.nome, self->curso);
    free(self->curso);
    
    // Não libera self->base.nome porque isso será feito pelo destrutor da classe base
    pessoa_destruir(&self->base);
}

// Construtor para Estudante
Estudante* estudante_criar(const char* nome, int idade, const char* curso) {
    Estudante* novo_estudante = (Estudante*)malloc(sizeof(Estudante));
    
    // Inicializa a parte "Pessoa" do Estudante
    novo_estudante->base = *pessoa_criar(nome, idade);
    
    // Aloca e copia o curso
    novo_estudante->curso = (char*)malloc(strlen(curso) + 1);
    strcpy(novo_estudante->curso, curso);
    
    // Sobrescreve métodos da classe base
    novo_estudante->base.apresentar = estudante_apresentar;
    novo_estudante->base.destruir = estudante_destruir;
    
    // Atribui os métodos específicos
    novo_estudante->estudar = estudante_estudar;
    
    return novo_estudante;
}

// ===== Exemplo de uso =====
int main() {
    printf("=== Demonstração de POO em C ===\n\n");
    
    // Criando uma pessoa
    Pessoa* p1 = pessoa_criar("João", 30);
    p1->apresentar(p1);
    p1->envelhecer(p1);
    
    printf("\n");
    
    // Criando um estudante (demonstra herança)
    Estudante* e1 = estudante_criar("Maria", 20, "Administração");
    e1->base.apresentar(e1);  // Método sobrescrito
    e1->base.envelhecer(e1);  // Método herdado
    e1->estudar(e1);          // Método específico
    
    printf("\n");
    
    // Polimorfismo: vetor de "pessoas"
    printf("=== Demonstração de Polimorfismo ===\n");
    Pessoa* pessoas[2];
    pessoas[0] = p1;
    pessoas[1] = (Pessoa*)e1;  // Upcasting
    
    // Iterando e chamando o método "apresentar" de forma polimórfica
    for (int i = 0; i < 2; i++) {
        printf("Objeto %d: ", i + 1);
        pessoas[i]->apresentar(pessoas[i]);
    }
    
    printf("\n=== Limpando a memória ===\n");
    // Limpando a memória (destruindo os objetos)
    p1->destruir(p1);
    e1->base.destruir(e1);
    
    return 0;
}
