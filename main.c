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

void startList(no **head, no **list);
bool check(no *head);
void pushNames(FILE *f, no **head, int *quant);
no* push(no **head, char name[20], int *quant);
no* split(no *source, no **front, no **back, int keyFind);
no* merge(no *front, no *back);
void mergeSort(no **head);
void hashList(no *head, no **list, int *quant);
int binarySearch(no *list[], int quant, int key);
no* findNode(char data[30], no *list[]);
void removeNode(char data[30], no *list[]);
void checkElements(no *list[], int quant, int i);
void freeList(no **list[], int quant);
void print(no *list[], int quant, int i);

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

no* push(no **head, char name[20], int *quant) {
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
    *quant++;
    return new;
}

void pushNames(FILE *f, no **head, int *quant) {
    char namesFile[20];
    while (fgets(namesFile, sizeof(namesFile), f) != NULL) {
        namesFile[strcspn(namesFile, "\n")] = '\0';
        push(head, namesFile, quant);
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
             if (list[i] == NULL) {
                     list[i] = aux;
               }else
               {
                   while (aux2 != NULL && aux->key == aux2->key) {
                       aux2 = aux2->next;
                   }
                   aux = aux2;
                   if(aux2!=NULL) {
                       aux2->prev->next = NULL;
                       aux2->prev = NULL;
                       aux2 = aux2->next;
                   }
               }
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

no* findNode(char data[30], no *list[]){
    int index = hash((int)data[0]-12);
    no* aux = list[index];
    no *temp = aux;
        while (temp != NULL) {
            if(strcmp(temp->name,data)==0) {
                printf("Encontrado na lista no índice [%d]\n Nome: %s chave: {%d}\n", index, temp->name, temp->key);
                return temp;
            }
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

void checkElements(no *list[], int quant, int i){
    int elements = 0;
    no* *aux = &list[0];
    no *temp = malloc(sizeof(no));

    for ( i = i; i < quant; i++) {
        temp = aux[i];
        while (temp != NULL) {
            temp = temp->next;
            elements++;
        }printf("[%d] :%d\n",i, elements);
        printf("\n__________________________\n");
        elements = 0;
    }
    free(temp);
}
void freeList(no **list[], int quant){
    if(list==NULL)
    {
        printf("Lista Vazia!\n");
        return;
    }
    no* *aux = list;
    for (int i = 0; i < quant; i++) {
        no *temp = aux[i];

        while (temp != NULL) {
            if(temp->next==NULL)
            {
                free(temp);
                break;
            }
            temp = temp->next;
            free(temp->prev);
        }
        list[i] = NULL;
    }
    if(list==NULL)
    {
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    }
}

void print(no *list[], int quant, int i) {
    int elements = 0;
    no* *aux = &list[0];
    if(list[i]==NULL)
    {
        printf("Lista Vazia!\n");
        return;
    }
    for ( i = i; i < quant; i++) {
        printf("NOMES NA POSIÇÃO:[%d]\n", i);
        no *temp = aux[i];
        while (temp != NULL) {
            printf("%s [%d]| ", temp->name, temp->key);
            temp = temp->next;
            elements++;
        }printf("TOTAL:%d\n", elements);
        printf("\n__________________________\n");
        elements = 0;
    }
}

int main() {
    FILE *f = fopen("/usr/wsProjects/wsCLion/tabelaHash/nomes.txt", "r");
    if (f == NULL) {
        printf("Arquivo vazio!");
        return 1;
    }
    no *head;
    int quant = 0;
    startList(&head, list);
    pushNames(f, &head, &quant);
    mergeSort(&head);
    hashList(head, list, &quant);

    printf("Buscando um nome:\n");
    findNode("ALIKSON", list);

    printf("\nImprimindo lista de chaves do nome a ser removido:\n");
    getchar();
    print(list, 1, 0);

    printf("\n Removendo o mesmo nome:\n");
    getchar();
    removeNode("ALIKSON", list);

    printf("\n______________________________________________________________________________________________________________________\n");getchar();

    printf("\nImprimindo lista de chaves do nome removido:\n");
    print(list, 1, 0);

    printf("\nDistribuição de chaves:\n");
    getchar();
    checkElements(list,MAX, 0);

    printf("\nImprimindo lista com todos os nomes:\n");
    print(list, MAX, 0);

    printf("\nLiberando lista:\n");
    freeList(list, MAX);
    print(list, MAX, 0);
    fclose(f);

    return 0;
}
