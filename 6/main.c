#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct Node {
    char name[10];
    char second_name[10];
    char tel[10];
    struct Node* next;
    struct Node* prev;
};

void clearinput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

struct Node* create_node(const char* name, const char* second_name, const char* tel){
    struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
    if (!newnode){
        perror("Ошибка выделения памяти\n");
        exit(1);
    }

    strncpy(newnode->name, name, sizeof((newnode->name) - 1));
    newnode->name[sizeof(newnode->name) - 1] = '\0';

    strncpy(newnode->second_name, second_name, sizeof(newnode->second_name) - 1);
    newnode->second_name[sizeof(newnode->second_name) - 1] = '\0';

    strncpy(newnode->tel, tel, sizeof(newnode->tel) - 1);
    newnode->tel[sizeof(newnode->tel) - 1] = '\0';

    newnode->next = NULL;
    newnode->prev = NULL;

    return newnode;
}

void add_node(struct Node** head, const char* name, const char* second_name, const char* tel){
    struct Node* newnode = create_node(name, second_name, tel);

    if (*head == NULL){
        *head = newnode;
        return;
    }

    struct Node* cursor = *head;

    while (cursor->next != NULL){
        cursor = cursor->next;
    }

    cursor->next = newnode;
    newnode->prev = cursor;

}

void del_node(struct Node** head, const char* name){
    if (*head == NULL){
        printf("Список пуст!\n");
        return;
    }

    struct Node* cursor = *head;

    while (cursor != NULL){
        if (strcmp(cursor->name, name) == 0){
            break;
        }
        cursor = cursor->next;
    }

    if (cursor == NULL){
        printf("Абонент с таким именем не найден\n");
        return;
    }

    if (cursor-> prev != NULL){
        cursor->prev->next = cursor->next;
    } else {
        *head = cursor-> next;
    }
    if (cursor->next != NULL){
        cursor->next->prev = cursor->prev;
    }

    printf("Абонент %s %s %s удален\n", cursor->name, cursor->second_name, cursor->tel);
    free(cursor);
}

void show_all_nodes(struct Node** head){
    if (*head == NULL){
        printf("Список пуст!\n");
        return;
    }

    struct Node* cursor = *head;

    while (cursor != NULL){
        printf("| %s | %s | %s |\n", cursor->name, cursor->second_name, cursor->tel);
        cursor = cursor->next;
    }
}

void search_node(struct Node** head, const char* name){
    if (*head == NULL){
        printf("Список пуст!\n");
        return;
    }

    struct Node* cursor = *head;

    while (cursor != NULL){
        if (strcmp(cursor->name, name) == 0){
            break;
        }
        cursor = cursor->next;
    }

    if (cursor == NULL){
        printf("Абонент с таким именем не найден\n");
        return;
    }

    printf("Абонент найден %s %s %s\n", cursor->name, cursor->second_name, cursor->tel);
}

void clear_nodes(struct Node** head){
    if (*head == NULL){
        printf("Список пуст!\n");
        return;
    }

    struct Node* cursor = *head;

    while (*head != NULL) {
        cursor = *head;
        *head = (*head)->next;
        free(cursor);
    }

    printf("Список успешно очищен.\n");
}

int main(){
    struct Node* head = NULL;
    short choise;

    while (1)
    {
        printf("1) Добавить абонента\n");
        printf("2) Удалить абонента\n");
        printf("3) Поиск абонентлв по имени\n");
        printf("4) Вывод всех записей\n");
        printf("5) Выход\n");

        if (scanf("%hd", &choise) == 0){
            printf("Ошибка\n");
            clearinput();
            continue;
        }

        switch (choise)
        {
        case 1: {
            char searhed_name[10];
            char searhed_second_name[10];
            char searhed_tel[10];

            printf("Введите имя, фамилию и телефон через пробел: ");
            
            if (scanf("%9s %9s %9s", searhed_name, searhed_second_name, searhed_tel) != 3) {
                printf("Ошибка ввода!\n");
                clearinput();
                break;
            }

            add_node(&head, searhed_name, searhed_second_name, searhed_tel);

            continue;
        }
        case 2: {
            char searhed_name[10];
            printf("Введите имя абонента, которого вы хотите удалить: ");
            if (scanf("%9s", searhed_name) != 1) {
                printf("Ошибка!\n");
                clearinput();
                break;
            }
            del_node(&head, searhed_name);
            printf("абонент успешно удален");
            continue;
        }
        case 3: {
            char searhed_name[10];
            printf("Введите имя, которое хотели бы найти: ");
            if (scanf("%9s", searhed_name) != 1){
                printf("Ошибка\n");
                clearinput();
                continue;
            }
            search_node(&head, searhed_name);
            continue;
        }
        case 4:
            show_all_nodes(&head);
            continue;
        case 5:
            clear_nodes(&head);
            break;
        default:
            printf("Выберите от 1 до 5\n");
            continue;
        }
        break;
    }
}