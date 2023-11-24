#include <stdlib.h>  // Alocação de memória e funções relacionadas.
#include <stdio.h>   // Entrada e saída padrão.
#include <string.h>  // Manipulação de strings.
#define queue_max 3       // Definindo o limite máximo da fila;
#define stack_capacity 3  // Definindo a capacidade máxima das pilhas;

// Declarando a estrutura das caixas;
typedef struct box{
    short id;            // Identificador das caixas;
    int size;            // Size são os tamanhos das caixas (1,2 ou 3);
    char status[8];      // Status é o estado da caixa (WAITING[7], STACKED[7], REMOVED[7]);
    struct box* next;    // Ponteiro para o proximo da fila;
} Box;

// Estrutura para a pilha;
typedef struct stack {
    Box* top;     // Ponteiro para o topo da pilha;
    short id;     // Identificador da pilha;
    short count;  // Contador de elementos na pilha;
} Stack;

// Variáveis globais;
Box* first_box = NULL;     // Ponteiro para a primeira caixa;
Box* new_box = NULL;       // Ponteiro para nova caixa;
Stack* stack1 = NULL;      // Ponteiro para stack1;
Stack* stack2 = NULL;      // Ponteiro para stack2;
short queue_count = 0;     // Variável para controlar o limite das caixas na fila;

//----------------------------------------- Funcoes -----------------------------------------+//
//Interno: Função para inicializar as pilhas;
void initialize_stack();

// Função para cadastrar caixas, desde que o limite (queue_max) ainda não tenha sido atingido;
void insert_box();

// Função para exibir as caixas que estão na fila;
void show_box_queue();

// Função para remover a primeira caixa da fila (se existir) e liberar a memória associada;
void pop_first_box_queue();

// Função para empilhar o primeiro elemento da fila na pilha;
void push_first_box_to_stack(Stack* stack);

// Função para exibir o conteúdo de uma pilha;
void show_stack(Stack* stack, int stackNumber);

// Função para desempilhar um elemento da pilha;
void pop_from_stack(Stack* stack);

// Interno:Função para liberar toda a memória alocada para as caixas quando o programa termina;
void clean_memory();

// Interno: Função para validar o tamanho da caixa;
int get_valid_box_size();

// Interno: Função para liberar a memória alocada para a pilha;
void clean_stack_memory(Stack* stack);

// Interno: Função para liberar toda a memória alocada;
void clean_all_memory();

// Funcao para escolher onde empilhar 1 caixa;
void push_stack_choice();

// Funcao para escolher de onde remover;
void pop_stack_choice();

// Funcao para escolher qual pilha exibir ;
void show_stack_choice();

// Funcao para empilhar todos os itens da fila;
void stack_full_queue();

//Funcao para desempilhar;
void unstack();
//---------------------------------------------------------------------------------------//

int main() {
    puts("MA[I]N IN THE BOX HAVE BEEN STARTED!");
    initialize_stack();
    int option = -1;  // Variável para armazenar a opção do usuário no menu;
    do {
        puts("\n------------------------\n>>>>      MENU      <<<<");
        printf("------------------------\n1 - INSERT BOX\n2 - SHOW BOXES QUEUE\n3 - POP FIRST BOX QUEUE\n"
               "4 - PUSH FIRST TO STACK\n5 - POP FROM STACK\n6 - SHOW STACK\n7 - PUSH ALL QUEUE\n8 - UNSTACK ALL\n0 - EXIT\n------------------------\n");
        scanf_s("%d", &option);  // Leitura da opção do usuário;

        switch (option) {
            case 1:
                insert_box();          // Chama a função para inserir uma caixa na fila;
                break;
            case 2:
                show_box_queue();      // Chama a função para mostrar as caixas na fila;
                break;
            case 3:
                pop_first_box_queue(); // Chama a função para remover a próxima caixa da fila;
                break;
            case 4:
                push_stack_choice();  // Chama a função para empilhar a primeira caixa da fila na pilha 1 ou 2 ;
                break;
            case 5:
                pop_stack_choice();   // Chama a função para desempilhar uma caixa da pilha 1 ou 2;
                break;
            case 6:
                show_stack_choice();  // Chama a função para mostrar o conteúdo da pilha 1 ou 2;
                break;
            case 7:
                stack_full_queue();  // Chama a função para empilhar todas as caixas da fila em uma pilha;
                break;
            case 8:
                unstack();           // Chama a função para desempilhar totalmente uma pilha;
                break;
            case 0:
                puts("MA[I]N IN THE BOX HAS FINISHED!");  // Mensagem de encerramento do programa;
                break;
            default:
                puts("INVALID OPTION! TRY AGAIN");    // Mensagem para opção inválida;
                break;
        }
    } while (option != 0);      // Loop até que o usuário escolha sair (opção 0);
    clean_all_memory();         // Chama a função para liberar a memória alocada para as caixas e pilhas;
    return 0;                   // Retorna 0 indicando que o programa foi executado com sucesso;
}

// Interno: Função para inicializar as pilhas;
void initialize_stack()
{
    puts("\nTHE STACKS HAVE BEEN INITIALIZED!");  // Imprime mensagem informando que as pilhas foram inicializadas;

    stack1 = (Stack*)calloc(stack_capacity, sizeof(Stack)); // Aloca dinamicamente memória para a pilha 1;
    stack2 = (Stack*)calloc(stack_capacity, sizeof(Stack)); // Aloca dinamicamente memória para a pilha 2;

    if (stack1 == NULL || stack2 == NULL) {   // Verifica se a alocação de memória foi bem-sucedida;
        printf("ERROR: MEMORY ALLOCATION FAILED!\n");
        exit(1);                       // Encerra o programa com código de erro 1 em caso de falha na alocação;
    }

    stack1->top = NULL;  // Inicializa o topo da pilha como nulo;
    stack1->id = 1;      // Define o identificador da pilha 1;
    stack1->count = 0;   // Inicializa o contador de elementos na pilha 1;

    stack2->top = NULL;  // Inicializa o topo da pilha como nulo;
    stack2->id = 2;      // Define o identificador da pilha 2;
    stack2->count = 0;   // Inicializa o contador de elementos na pilha 2;
}

// Função para cadastrar caixa, desde que o limite (queue_max) ainda não tenha sido atingido;
void insert_box()
{
    if (queue_max > queue_count) {                                                // Verificar se o limite de caixas na fila foi atingido;
        if (first_box == NULL) {                                                  // Verificar se a fila está vazia (primeira caixa);
            new_box = (Box*)calloc(1, sizeof(Box));    // Aloca memória para a nova caixa;
            if (new_box == NULL) {                                                // Verificar se houve erro na alocação de memória;
                printf("ERROR: MEMORY ALLOCATION FAILED!\n");                     // Mensagem indicando um erro na alocação de memória;
                exit(1);                                                    // Encerra o programa com código de erro 1;
            }
            first_box = new_box;                                                  // A primeira caixa na fila é a nova caixa criada;
        } else {
            new_box->next = (Box*)calloc(1, sizeof(Box)); // Aloca memória para a próxima caixa na fila;
            if (new_box->next == NULL) {                                             // Verificar se houve erro na alocação de memória;
                printf("ERROR: MEMORY ALLOCATION FAILED!\n");
                exit(1);
            }
            new_box = new_box->next;                      // Move o ponteiro para a próxima caixa na fila;
        }
        puts("==============");                       // Imprime uma linha de separação;
        printf("BOX ID:");                                // Solicita o ID da caixa ao usuário;
        scanf_s("%d", &new_box->id);               // Lê o ID da caixa inserido pelo usuário;
        strcpy(new_box->status, "WAITING");  // Define o status da caixa como "WAITING";
        new_box->size = get_valid_box_size();             // Chama a função interna para obter um tamanho de caixa válido;
        puts("==============");                       // Imprime uma linha de separação;
        new_box->next = NULL;                            // Configura o próximo da caixa como nulo, indicando o final da fila;
        queue_count++;                                   // Incrementa o contador de caixas na fila;
    }else{
        printf("THE QUEUE HAS REACHED THE MAXIMUM NUMBER OF BOXES!\nYOU NEED TO STACK THE BOXES IN THE QUEUE TO INSERT MORE BOXES\n");
        // Mensagem informando que a fila atingiu o número máximo de caixas, que é necessário empilhar as caixas na fila para inserir mais caixas;
        show_box_queue();                                 // Chama a função para mostrar o conteúdo da fila de caixas;
    }
}

// Interno: Função para validar o tamanho da caixa;
int get_valid_box_size()
{
    int size;
    do {
        printf("SIZE(1,2,3):");         // Solicita ao usuário que insira o tamanho da caixa;
        scanf_s("%d", &size);    // Lê o tamanho da caixa fornecido pelo usuário;
    } while (size < 1 || size > 3);     // Continua solicitando até que o tamanho seja válido (1,2 ou 3);
    return size;                        // Retorna o tamanho validado da caixa;
}

// Função para exibir as caixas que estão na fila;
void show_box_queue()
{
    Box* current = first_box;                                   // Ponteiro para a primeira caixa da fila;
    if(queue_count >= 1){                                       // Verifica se há caixas na fila;
        while (current != NULL) {                               // Enquanto verdadeira irá exibir as caixas da fila;
            puts("================");
            printf_s("BOX ID: %d\n", current->id);      // Imprime o ID da caixa;
            printf_s("SIZE: %d\n", current->size);      // Imprime o tamanho da caixa;
            printf_s("STATUS: %s\n", current->status);  // Imprime o status da caixa;
            puts("================");
            current = current->next;                            // Move para a próxima caixa na fila;
        }
    }else{
        printf("QUEUE IS EMPTY! INSERT NEW BOXES\n");           // Imprime mensagem se a fila estiver vazia;
    }
}

// Função para remover a primeira caixa da fila (se existir) e liberar a memória associada;
void pop_first_box_queue()
{
    Box* delete_first = first_box;  // Cria um ponteiro para a primeira caixa na fila;
    if(queue_count >= 1) {          // Verifica se há pelo menos uma caixa na fila;
        if (first_box != NULL) {    // Verifica se a primeira caixa na fila existe;
            strcpy(first_box->status, "REMOVED");
            puts("================");
            printf_s("BOX ID: %d\n", first_box->id);
            printf_s("SIZE: %d\n", first_box->size);
            printf_s("STATUS: %s\n", first_box->status);
            puts("================");
            printf("THAT BOX WAS REMOVED FROM QUEUE\n");
            first_box = first_box->next;  // Move o ponteiro da primeira caixa para a próxima na fila;
            free(delete_first);   // Libera a memória associada à primeira caixa;
            delete_first = NULL;          // Garante que o ponteiro deletado não aponta para lixo de memória;
            queue_count--;                // Atualiza a contagem de caixas na fila;
        }
    } else {
        printf("QUEUE IS EMPTY. INSERT NEW BOXES!\n");  // Mensagem indicando que não há caixas para remover;
    }
}

// Função para exibir o conteúdo de uma pilha;
void show_stack(Stack* stack, int stackNumber)
{
    printf("STACK %d:\n", stackNumber);   // Imprime o número da pilha;
    Box* current = stack->top;            // Cria um ponteiro para percorrer a pilha a partir do topo;

    if (stack->count > 0) {               // Verifica se a pilha não está vazia;
        while (current != NULL) {         // Loop enquanto houver caixas na pilha;
            puts("================");
            printf_s("BOX ID: %d\n", current->id);
            printf_s("SIZE: %d\n", current->size);
            printf_s("STATUS: %s\n", current->status);
            puts("================");
            current = current->next;       // Aponta para a próxima caixa na pilha;
        }
    } else {
        printf("STACK IS EMPTY");         // Mensagem indicando que a pilha está vazia;
    }
}

// Função para desempilhar um elemento da pilha;
void pop_from_stack(Stack* stack)
{
    if (stack->top != NULL && stack->count != 0) {      // Verifica se a pilha não está vazia;
        Box* temp = stack->top;                         // Cria um ponteiro temporário para a caixa no topo da pilha;
        strcpy(temp->status, "REMOVED");  // Atualiza o status da caixa para "REMOVED";
        stack->top = stack->top->next;                  // Move o ponteiro de topo para a próxima caixa na pilha;
        stack->count--;                                 // Decrementa o contador de elementos na pilha;

        // Exibir os dados do elemento desempilhado
        puts("================");
        printf_s("BOX ID: %d\n", temp->id);
        printf_s("SIZE: %d\n", temp->size);
        printf_s("STATUS: %s\n", temp->status);
        puts("================");
        printf("THAT BOX WAS REMOVED FROM STACK!\n");   // Mensagem informando que a caixa foi removida da pilha;
        free(temp);                             // Libera a memória alocada para a caixa desempilhada;
        temp = NULL;                                    // Define o ponteiro temporário como nulo;
    } else {
        printf("STACK IS EMPTY. CANNOT POP!\n");
    }
}

// Funcao para escolher onde empilhar;
void push_stack_choice()
{
    int opt;        // Variável para ler a opção da pilha;
    do {
        printf("CHOOSE THE STACK TO 'PUSH' THE BOX (1 OR 2):"); // Solicita ao usuário que escolha a pilha para empilhar uma caixa;
        scanf_s("%d", &opt);
    } while (opt != 1 && opt != 2);     // Se repete até uma opção válida ser informada(1 ou 2);


    if (opt == 1) {                             // Verifica a opção;
        push_first_box_to_stack(stack1); // Chama a função para empilhar a próxima caixa na pilha escolhida;
    } else {
        push_first_box_to_stack(stack2);
    }
}

// Funcao para escolher de onde remover;
void pop_stack_choice()
{
    int opt;
    do {
        printf("CHOOSE THE STACK TO 'POP' THE BOX (1 OR 2):"); // Solicita ao usuário que escolha a pilha para desempilhar uma caixa;
        scanf_s("%d", &opt);
    } while (opt != 1 && opt != 2);

    if (opt == 1) {
        pop_from_stack(stack1);  // Chama a função para desempilhar a caixa do topo da pilha escolhida;
    } else {
        pop_from_stack(stack2);
    }
}

// Funcao para escolher qual pilha exibir ;
void show_stack_choice()
{
    int opt;
    do {
        printf("CHOOSE THE STACK TO 'SHOW' THE BOXES STACKED (1 OR 2):");
        scanf_s("%d", &opt);
    } while (opt != 1 && opt != 2);

    if (opt == 1) {
        show_stack(stack1,1);
    } else {
        show_stack(stack2,2);
    }
}

// Função para desempilhar todas as caixas de uma pilha;
void unstack()
{
    int opt;
    do {
        printf("CHOOSE THE STACK TO 'UNSTACK' THE BOXES STACKED (1 OR 2):");
        scanf_s("%d", &opt);
    } while (opt != 1 && opt != 2);

    if (opt == 1) {
        do {
            pop_from_stack(stack1);  // Chama a função para desempilhar todas as caixa da pilha escolhida;
        } while (stack1->count > 0);       // Condição de repetição, até a pilha estar vazia;

    } else {
        do {
            pop_from_stack(stack2);
        } while (stack2->count > 0);
    }
}

// Função para empilhar o primeiro elemento da fila na pilha;
void push_first_box_to_stack(Stack* stack)
{
    if (first_box != NULL && stack->count < stack_capacity) {  // Verifica se há caixas na fila e se a pilha não está cheia;
        Box* temp = first_box;                                 // Cria um ponteiro temporário para a primeira caixa na fila;

        if (stack->count == 0 || stack->top->size >= temp->size) { // Verifica se a pilha está vazia ou se a caixa na pilha tem tamanho igual ou menor;
            strcpy(temp->status, "STACKED");           // Atualiza o status da caixa para "STACKED";
            first_box = first_box->next;                           // Move o ponteiro da primeira caixa para a próxima na fila;
            temp->next = stack->top;                               // Configura o próximo da caixa como o topo atual da pilha;
            stack->top = temp;                                     // Atualiza o topo da pilha para a nova caixa;
            stack->count++;                                       // Incrementa o contador de elementos na pilha;
            queue_count--;                                        // Decrementa o contador de caixas na fila;
            puts("\nBOX ADDED TO STACK!\n");
            show_stack(stack,stack->id);
        } else {
            // Mensagem indicando que não é possível empilhar uma caixa maior sobre uma caixa menor;
            printf("IT IS NOT POSSIBLE TO STACK A SIZE %d BOX ON TOP OF A SIZE %d\n", temp->size, stack->top->size);
        }
    } else if (stack->count >= stack_capacity) {
        printf("STACK IS FULL. CANNOT PUSH MORE BOXES.\n");  // Mensagem indicando que a pilha está cheia;
        show_stack(stack,stack->id);
    } else {
        printf("QUEUE IS EMPTY. CANNOT PUSH TO STACK.\n");   // Mensagem indicando que a fila está vazia;
    }
}

// Função para empilhar todas as caixas da fila;
void stack_full_queue()
{
    if (queue_count > 0) { // Verifica se há pelo menos uma caixa na fila.
        int opt;

        do {
            printf("CHOOSE THE STACK TO 'PUSH THE BOXES QUEUE' (1 OR 2):");
            scanf_s("%d", &opt);
        } while (opt != 1 && opt != 2);

        if (opt == 1) {
            do {
                if (first_box != NULL && stack1->count < stack_capacity) {                    // Verifica se a pilha 1 está cheia;
                    push_first_box_to_stack(stack1);               // Empilha a próxima caixa na pilha 1.
                } else if (stack1->count >= stack_capacity) {            // Verifica a capacidade da pilha;
                    printf("STACK IS FULL. CANNOT PUSH MORE BOXES.\n");
                    show_stack(stack1, stack1->id);   // Exibe o conteúdo da pilha 1.
                }
            } while (first_box != NULL && stack1->top->size >= first_box->size && stack1->count < stack_capacity); // Condição de repetição;
        } else {
            do {
                if (stack2->count < stack_capacity) {                    // Verifica se a pilha 2 está cheia;
                    push_first_box_to_stack(stack2);
                } else if (stack2->count >= stack_capacity){
                    printf("STACK IS FULL. CANNOT PUSH MORE BOXES.\n");
                    show_stack(stack2, stack2->id);
                }
            } while (first_box != NULL && stack2->top->size >= first_box->size && stack2->count < stack_capacity); // Condição de repetição;
        }
    } else {
        printf("ERROR: THE QUEUE MUST HAVE AT LEAST ONE BOX\n");          // Mensagem de erro se a fila estiver vazia.
    }
}

// Interno: Função para liberar toda a memória alocada para as caixas quando o programa termina;
void clean_memory()
{
    Box* current;                      // Ponteiro temporário para percorrer a lista de caixas;
    while (first_box != NULL) {        // Loop enquanto houver caixas na lista;
        current = first_box->next;     // Salva o próximo elemento na lista;
        free(first_box);       // Libera a memória alocada para a primeira caixa;
        first_box = NULL;              // Define o ponteiro da primeira caixa como nulo;
        first_box = current;           // Atualiza o ponteiro da primeira caixa para o próximo elemento na lista;
    }
}

// Interno: Função para liberar a memória alocada para a pilha;
void clean_stack_memory(Stack* stack)
{
    Box* current = stack->top;  // Ponteiro para a caixa no topo da pilha;
    Box* next;                  // Ponteiro para o proximo;

    while (current != NULL) {       // Loop para percorrer todas as caixas na pilha;
        next = current->next;       // Salva o próximo elemento na pilha;
        free(current);      // Libera a memória alocada para a caixa atual;
        current = next;             // Move para a próxima caixa na pilha;
    }

    stack->top = NULL;   // Define o topo da pilha como nulo, indicando que a pilha está vazia;
    stack->id = 0;       // Define ID como zero;
    stack->count = 0;    // Zera o contador de elementos na pilha;
}

// Interno: Função para liberar toda a memória alocada;
void clean_all_memory()
{
    clean_memory();                     // Libera a memória alocada para as caixas na fila;
    clean_stack_memory(stack1);  // Libera a memória alocada para a pilha 1;
    clean_stack_memory(stack2);  // Libera a memória alocada para a pilha 2;
}