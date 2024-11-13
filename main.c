#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX 53

typedef struct lista {
    char name[20];
    int key;
    struct lista *next;
    struct lista *prev;
} no;

no* list[MAX];
int hashSize = 0;
bool check(no *head);
void pushNames(FILE *f, no **head);
void print(no *list[], int quant, int i);
no* push(no **head, char name[20]);
no* split(no *source, no **front, no **back, int keyFind);
no* merge(no *front, no *back);
void mergeSort(no **head);
void hashList(no *head, no **list, int *quant);
void menu(no *head, FILE *f);

void startList(no **head, no **list) {
    *head = NULL;
    *list = NULL;
}

bool check(no *head) {
    return head != NULL;
}

int hash(int chave) {
    return chave % MAX;
}

no* push(no **head, char name[20]) {
    no *new = malloc(sizeof(no));
    if(!new) {
        exit(1);
    }
    int nameKey = hash((int)name[0]-12);
    new->key = nameKey;
    strcpy(new->name, name);

    if(!check(*head)){
        new->next = NULL;
    } else {
        new->next = *head;
        (*head)->prev = new;
    }
    new->prev = NULL;
    *head = new;
    hashSize++;
    return new;
}

void pushNames(FILE *f, no **head) {
    char namesFile[20];
    while (fgets(namesFile, sizeof(namesFile), f) != NULL) {
        namesFile[strcspn(namesFile, "\n")] = '\0';
        push(head, namesFile);
    }
}

no* split(no *source, no **front, no **back, int keyFind) {
    if (!source || !source->next) {
        *front = source;
        *back = NULL;
        return;
    }

    no *slow = source;
    no *fast = source->next;

    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

no* merge(no *front, no *back) {
    if (!front) return back;
    if (!back) return front;

    if (front->key <= back->key) {
        front->next = merge(front->next, back);
        front->next->prev = front;
        front->prev = NULL;
        return front;
    } else {
        back->next = merge(front, back->next);
        back->next->prev = back;
        back->prev = NULL;
        return back;
    }
}

void mergeSort(no **head) {
    if (!check(*head) || !check((*head)->next)) return;

    no *front, *back;
    split(*head, &front, &back, 0);

    mergeSort(&front);
    mergeSort(&back);

    *head = merge(front, back);
}

void hashList(no *head, no **list, int *quant) {
    no *aux = head;
    no *aux2 = aux->next;

    for (int j = 0; j < MAX; j++) {
        list[j] = NULL;
    }
    while (aux != NULL) {
        int i = hash(aux->key);
            list[i] = aux;
        while (aux2 != NULL && aux->key == aux2->key) {
                aux2 = aux2->next;
            }
            aux = aux2;
        if(aux2!=NULL) {
            aux2->prev->next = NULL;
            aux2->prev = NULL;
            aux2 = aux2->next;
        }
        *quant = i;
    }
}
int binarySearch(no *list[], int quant, int key) {
    int low = 0;
    int high = quant - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        no *midNode = list[mid];

        if (midNode!=NULL && midNode->key == key) {
            print(&list[mid], quant, 0);
            return 1;
        } if (midNode!=NULL && midNode->key < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    printf("Não encontrado!\n");
    return 0;
}

no* findNode(char data[30], no *list[]) {
    int index = hash((int)data[0]-12);
    no* aux = list[index];
    no *temp = aux;
        while (temp != NULL) {
            if(strcmp(temp->name,data)==0) {
                printf("Encontrado na lista no índice [%d]\n Nome: %s chave: {%d}\n", index, temp->name, temp->key);
                return temp;
            }
            printf("%s [%d] ", temp->name, temp->key);
            temp = temp->next;
        }
    return NULL;
}

void removeNode(char data[30], no *list[]){
    no* remove = findNode(data, list);
    if(!remove) {
        printf("Não encontrado!\n");
    }else {
       // printf(remove->prev->prev);
        if(remove->prev==NULL)
        {
            list[remove->key] = remove->next;
        }else
        {
            remove->prev->next = remove->next;
        }
        remove->next->prev = remove->prev;
        free(remove);
        printf("Removido com sucesso!\n");
    }
}

void print(no *list[], int quant, int i) {
    int hist = 0;
    no* *aux = &list[0];
    no *temp = malloc(sizeof(no));
    for ( i = i; i < quant; i++) {
        printf("NOMES NA POSIÇÃO:[%d]\n", i);
        temp = aux[i];
        while (temp != NULL) {
            printf("%s [%d]| ", temp->name, temp->key);
            temp = temp->next;
            hist++;
        }printf("TOTAL:%d\n", hist);
        printf("\n__________________________\n");
        hist = 0;
    }
}

int main(void) {
    FILE *f = fopen("/home/yhas/Documents/TabelaHashNomes/nomes.txt", "r");
    if (f == NULL) {
        printf("Arquivo vazio!");
        return 1;
    }
    no *head;
    int quant = 0;
    startList(&head, list);
    pushNames(f, &head);
    mergeSort(&head);
    hashList(head, list, &quant);
   // binarySearch(list, quant, 0);
    printf("Buscando um nome:\n");
    findNode("ANDRESSA", list);
    printf("\nImprimindo lista de chaves do nome a ser removido:\n");
    getchar();
    print(list, 1, 0);
    printf("\n Removendo o mesmo nome:\n");
    getchar();
    removeNode("ANDRESSA", list);
    printf("\n______________________________________________________________________________________________________________________\n");getchar();
    printf("\n________________________________________________________________________________________________________________________-_\n");getchar();
    printf("\nImprimindo lista de chaves do nome removido:\n");
    printf("\n_____________________________\n");getchar();
    print(list, 1, 0);
    getchar();
    printf("\nImprimindo lista com todos os nomes:\n");
  /*  print(list, quant, 0);*/
    fclose(f);

    return 0;
}
