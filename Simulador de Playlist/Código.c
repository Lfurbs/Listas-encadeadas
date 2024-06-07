#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma música
typedef struct Song {
    char artist[100];
    char name[100];
    struct Song *next;
    struct Song *prev;
} Song;

// Função para criar um novo nó de música
Song* create_node(char *artist, char *name) {
    Song *new_node = (Song*)malloc(sizeof(Song));
    if (new_node == NULL) {
        printf("Erro ao alocar memoria para o no da musica\n");
        exit(1);
    }
    strcpy(new_node->artist, artist);
    strcpy(new_node->name, name);
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

// Função para inserir uma música no final da playlist
void insert_end(Song **head, char *artist, char *name) {
    Song *new_node = create_node(artist, name);
    if (*head == NULL) {
        *head = new_node;
        (*head)->next = *head;
        (*head)->prev = *head;
    } else {
        Song *last = (*head)->prev;
        last->next = new_node;
        new_node->prev = last;
        new_node->next = *head;
        (*head)->prev = new_node;
    }
}

// Função para exibir a playlist na ordem de cadastro
void display_playlist(Song *head) {
    if (head == NULL) {
        printf("Playlist vazia\n");
        return;
    }
    printf("Playlist:\n");
    Song *current = head;
    do {
        printf("%s - %s\n", current->artist, current->name);
        current = current->next;
    } while (current != head);
}

// Função para exibir a playlist ordenada pelo nome das músicas
void display_sorted_playlist(Song *head) {
    if (head == NULL) {
        printf("Playlist vazia\n");
        return;
    }
    // Ordenação da playlist pelo nome das músicas
    Song *current = head;
    Song *temp = NULL;
    do {
        temp = current->next;
        while (temp != head) {
            if (strcmp(current->name, temp->name) > 0) {
                char temp_artist[100];
                char temp_name[100];
                strcpy(temp_artist, current->artist);
                strcpy(temp_name, current->name);
                strcpy(current->artist, temp->artist);
                strcpy(current->name, temp->name);
                strcpy(temp->artist, temp_artist);
                strcpy(temp->name, temp_name);
            }
            temp = temp->next;
        }
        current = current->next;
    } while (current != head);

    // Exibição da playlist ordenada
    printf("Playlist ordenada pelo nome das musicas:\n");
    current = head;
    do {
        printf("%s - %s\n", current->artist, current->name);
        current = current->next;
    } while (current != head);
}

// Função para remover uma música da playlist
void remove_song(Song **head, char *name) {
    if (*head == NULL) {
        printf("Playlist vazia, nao e possivel remover musica\n");
        return;
    }
    Song *current = *head;
    do {
        if (strcmp(current->name, name) == 0) {
            if (current == *head) {
                if ((*head)->next == *head) {
                    free(*head);
                    *head = NULL;
                } else {
                    (*head)->prev->next = (*head)->next;
                    (*head)->next->prev = (*head)->prev;
                    *head = (*head)->next;
                    free(current);
                }
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                free(current);
            }
            printf("'%s' foi removido\n", name);
            return;
        }
        current = current->next;
    } while (current != *head);
    printf("Musica nao encontrada\n");
}

// Função para buscar uma música na playlist
void search_song(Song *head, char *name) {
    if (head == NULL) {
        printf("Playlist vazia\n");
        return;
    }
    Song *current = head;
    int found = 0;
    do {
        if (strcmp(current->name, name) == 0) {
            printf("'%s' - %s\n", current->name, current->artist);
            found = 1;
        }
        current = current->next;
    } while (current != head);
    if (!found) {
        printf("Musica nao encontrada\n");
    }
}

// Função para avançar para a próxima música na playlist
Song* next_song(Song *current) {
    if (current == NULL) {
        printf("Playlist vazia, nao é possível avançar\n");
        return NULL;
    }
    return current->next;
}

// Função para retornar para a música anterior na playlist
Song* previous_song(Song *current) {
    if (current == NULL) {
        printf("Playlist vazia, nao e possivel retornar\n");
        return NULL;
    }
    return current->prev;
}

// Função para liberar a memória alocada para a playlist
void free_playlist(Song **head) {
    if (*head == NULL) {
        return;
    }
    Song *temp = *head;
    Song *current = *head;
    do {
        temp = current;
        current = current->next;
        free(temp);
    } while (current != *head);
    *head = NULL;
}

// Função para salvar as músicas no txt
void save_to_file(Song *head) {
    FILE *file = fopen("musicas.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo musicas.txt\n");
        return;
    }
    if (head == NULL) {
        fclose(file);
        return;
    }
    Song *current = head;
    do {
        fprintf(file, "%s;%s\n", current->artist, current->name);
        current = current->next;
    } while (current != head);
    fclose(file);
}

// Função para exibir o menu de opções
void print_menu() {
    printf("==============[Simulador de Playlist]==============\n");
    printf("1. Play\n");
    printf("2. Avancar\n");
    printf("3. Retornar\n");
    printf("4. Ordenar por ordem de cadastro\n");
    printf("5. Ordenar por ordem alfabetica das musicas\n");
    printf("6. Busca\n");
    printf("7. Inserir musica\n");
    printf("8. Remover musica\n");
    printf("9. Salvar\n");
    printf("0. Sair\n");
    printf("===================================================\n");
}

// Função para tocar a música atual
void play_song(Song *current) {
    if (current == NULL) {
        printf("Nenhuma música para tocar\n");
        return;
    }
    printf("Tocando agora: %s - %s\n", current->artist, current->name);
}

int main() {
    Song *playlist = NULL;

    // Leitura do arquivo e criação da playlist
    FILE *file = fopen("musicas.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    char artist[100], name[100];
    while (fscanf(file, "%[^;];%[^\n]\n", artist, name) != EOF) {
        insert_end(&playlist, artist, name);
    }
    fclose(file);

    int choice;
    Song *current_song = playlist;

    do {
        print_menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);
        getchar(); // Limpar o buffer do teclado

        switch(choice) {
            case 1:
                play_song(current_song);
                break;
            case 2:
                current_song = next_song(current_song);
                if (current_song != NULL) {
                    printf("Avançou para a próxima música: %s - %s\n", current_song->artist, current_song->name);
                }
                break;
            case 3:
                current_song = previous_song(current_song);
                if (current_song != NULL) {
                    printf("Retornou para a música anterior: %s - %s\n", current_song->artist, current_song->name);
                }
                break;
            case 4:
                display_playlist(playlist);
                break;
            case 5:
                display_sorted_playlist(playlist);
                break;
            case 6:
                printf("Informe a musica: ");
                fgets(name, 100, stdin);
                name[strcspn(name, "\n")] = '\0'; // Remover o caractere de nova linha
                search_song(playlist, name);
                break;
            case 7:
                printf("Digite o nome do artista: ");
                fgets(artist, 100, stdin);
                artist[strcspn(artist, "\n")] = '\0'; // Remover o caractere de nova linha
                printf("Digite o nome da musica: ");
                fgets(name, 100, stdin);
                name[strcspn(name, "\n")] = '\0'; // Remover o caractere de nova linha
                insert_end(&playlist, artist, name);
                printf("Musica inserida\n");
                save_to_file (playlist);
                break;
            case 8:
                printf("Musica a ser removida: ");
                fgets(name, 100, stdin);
                name[strcspn(name, "\n")] = '\0'; // Remover o caractere de nova linha
                remove_song(&playlist, name);
                save_to_file (playlist);
                break;
            case 0:
                printf("Saindo\n");
                break;
            default:
                printf("Opcao nao existe\n");
        }
    } while (choice != 0);

    // Liberar a memória alocada para a playlist
    free_playlist(&playlist);

    return 0;
}
