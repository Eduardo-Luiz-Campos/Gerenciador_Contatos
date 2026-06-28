#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ESTRUTURA DO CONTATO

typedef struct {
    char nome[100];
    char telefone[20];
    char email[100];
} Contato;



// Valida se o e-mail tem '@'
int validar_email(char* email) {
    if (strchr(email, '@') != NULL) {
        return 1; 
    }
    return 0; 
}

Contato* criar_contato() {
    Contato* novo = (Contato*) malloc(sizeof(Contato));
    if (novo == NULL) {
        printf("Erro de memoria!\n");
        return NULL;
    }

    printf("\nNome: ");
    fgets(novo->nome, 100, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    printf("Telefone: ");
    fgets(novo->telefone, 20, stdin);
    novo->telefone[strcspn(novo->telefone, "\n")] = '\0';

    do {
        printf("E-mail: ");
        fgets(novo->email, 100, stdin);
        novo->email[strcspn(novo->email, "\n")] = '\0';
        
        if (!validar_email(novo->email)) {
            printf("Erro: O e-mail deve conter '@'.\n");
        }
    } while (!validar_email(novo->email));

    return novo;
}


void exibir_contato(Contato* c) {
    printf("Nome: %-20s | Tel: %-15s | E-mail: %s\n", c->nome, c->telefone, c->email);
}

// Lista todos os contatos
void listar_todos(Contato** agenda, int total) {
    if (total == 0) {
        printf("\nA agenda esta vazia.\n");
        return;
    }
    printf("\n--- LISTA DE CONTATOS ---\n");
    for (int i = 0; i < total; i++) {
        printf("[%d] ", i + 1);
        exibir_contato(agenda[i]);
    }
}

// Busca contatos por parte do nome
void buscar_contato(Contato** agenda, int total) {
    if (total == 0) return;

    char nome_busca[100];
    printf("\nDigite o nome para buscar: ");
    fgets(nome_busca, 100, stdin);
    nome_busca[strcspn(nome_busca, "\n")] = '\0';

    int achou = 0;
    for (int i = 0; i < total; i++) {
        if (strstr(agenda[i]->nome, nome_busca) != NULL) {
            exibir_contato(agenda[i]);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum contato encontrado.\n");
}

// remover um contato
void remover_contato(Contato*** agenda, int* total) {
    if (*total == 0) return;

    char nome_busca[100];
    printf("\nDigite o nome EXATO do contato a remover: ");
    fgets(nome_busca, 100, stdin);
    nome_busca[strcspn(nome_busca, "\n")] = '\0';

    for (int i = 0; i < *total; i++) {
        if (strcmp((*agenda)[i]->nome, nome_busca) == 0) {
            free((*agenda)[i]); 
            for (int j = i; j < *total - 1; j++) {
                (*agenda)[j] = (*agenda)[j + 1];
            }
            (*total)--;
            if (*total > 0) {
                *agenda = (Contato**) realloc(*agenda, (*total) * sizeof(Contato*));
            } else {
                free(*agenda);
                *agenda = NULL;
            }
            printf("Contato removido!\n");
            return;
        }
    }
    printf("Contato nao encontrado.\n");
}

// Mostra a contagem total
void contar_contatos(int total) {
    printf("\nTotal de contatos: %d\n", total);
}

// Libera memória
void liberar_memoria(Contato** agenda, int total) {
    if (agenda != NULL) {
        for (int i = 0; i < total; i++) free(agenda[i]);
        free(agenda);
    }
    printf("Memoria liberada.\n");
}

void editar_contato(Contato** agenda, int total) {
    if (total == 0) {
        printf("\nAgenda vazia.\n");
        return;
    }

    char nome_busca[100];
    printf("\nDigite o nome EXATO do contato a editar: ");
    fgets(nome_busca, 100, stdin);
    nome_busca[strcspn(nome_busca, "\n")] = '\0';

    for (int i = 0; i < total; i++) {
        if (strcmp(agenda[i]->nome, nome_busca) == 0) {
            printf("Editando contato: %s\n", agenda[i]->nome);
            
            printf("Novo Telefone: ");
            fgets(agenda[i]->telefone, 20, stdin);
            agenda[i]->telefone[strcspn(agenda[i]->telefone, "\n")] = '\0';

            do {
                printf("Novo E-mail: ");
                fgets(agenda[i]->email, 100, stdin);
                agenda[i]->email[strcspn(agenda[i]->email, "\n")] = '\0';
            } while (!validar_email(agenda[i]->email));

            printf("Contato atualizado com sucesso!\n");
            return;
        }
    }
    printf("Contato nao encontrado.\n");
}

int main() {
    Contato** agenda = NULL; 
    int total_contatos = 0;
    int opcao = 0;
    char buffer[10];

    do {
        printf("\n1. Adicionar | 2. Listar | 3. Buscar\n");
        printf("4. Remover   | 5. Contar | 6. Editar (Bonus)\n");
        printf("7. Sair\nEscolha: ");
        
        fgets(buffer, 10, stdin);
        opcao = atoi(buffer);

        switch(opcao) {
            case 1:
                agenda = (Contato**) realloc(agenda, (total_contatos + 1) * sizeof(Contato*));
                agenda[total_contatos] = criar_contato();
                if (agenda[total_contatos]) total_contatos++;
                break;
            case 2:
                listar_todos(agenda, total_contatos);
                break;
            case 3:
                buscar_contato(agenda, total_contatos);
                break;
            case 4:
                remover_contato(&agenda, &total_contatos);
                break;
            case 5:
                contar_contatos(total_contatos);
                break;
            case 6:
                editar_contato(agenda, total_contatos);
                break;
            case 7:
                liberar_memoria(agenda, total_contatos);
                break;
            default:
                printf("Invalido.\n");
        }
    } while (opcao != 7);

    return 0;
}