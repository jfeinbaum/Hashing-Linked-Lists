
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100000
#define MAX_WORD_LENGTH 50
#define M 701

typedef struct ll_node {
    char* key;
    int count;
    struct ll_node* next ;
} node;

typedef struct ll {
    struct ll_node* head;
} list;



node* create_node(char* key) {
    node* new_node;
    new_node = (node*)(malloc(sizeof(node)));
    if (new_node != NULL) {
        new_node->next = NULL;
        new_node->key = strdup(key);
        new_node->count = 1;
        //printf("Created node '%s' at address %p\n", new_node->key, new_node);
    }
    else {
        printf("Failure creating node '%s'\n", key);
    }
    return new_node;
}


list* create_list(char* key) {
    list* new_list;
    new_list = (list*)(malloc(sizeof(list)));
    if (new_list != NULL) {
        node* head = create_node(key);
        new_list->head = head;
    }
    else {
        printf("Failure creating list with first node '%s'\n", key);
    }
    return new_list;
}


void increase_key(list* l, char* key) {
    node* current = l->head;
    while (strcmp(current->key,key) != 0) {
        current = current->next;
    }
    current->count += 1;
    //printf("Increased count of Node '%s'\n", current->key);
}

int find(list* l, char* key) {
    node *tmp = l->head;
    while (tmp != NULL) {
        if (strcmp(tmp->key,key) == 0) {
            //printf("Key '%s' found!\n",key);
            return 1;
        }
        tmp = tmp->next;
    }
    //printf("Key '%s' not found...\n",key);
    return 0;
}

void insert(list* l, char* key) {
    if (l == NULL) {
        printf("ERROR: tried to insert into an empty list\n");
    }
    else if (find(l, key)) {
        increase_key(l, key);
    }
    else {
        node* new_head = create_node(key);
        new_head->next = l->head;
        l->head = new_head;
        //printf("Inserted Node '%s' to front of list, address: %p\n", l->head->key, l->head);
    }
}


void free_list(list* l) {
    if (l->head == NULL) {
        //printf("Freed empty list\n");
        free(l);
        return;
    }
    node* current = l->head;
    while (current) {
        node* tmp = current;
        //printf("Freeing Node '%s'\n",tmp->key);
        current = tmp->next;
    }
    //printf("Freed all nodes\n");
    free(l);
    //printf("Freed empty list\n");
}


void list_delete(list* l, char* key) {
    if (find(l, key)) {
        node* head = l->head;
        if (head->next == NULL) {
            printf("Only one node - Deleting node and list\n");
            free_list(l);
            return;
        }
        node* prev = head;
        while (strcmp(prev->next->key,key) != 0) {
            prev = prev->next;
        }
        if (prev->next->next) {
            printf("Found node to delete in middle of list\n");
            prev->next = prev->next->next;
        }
        else {
            printf("Found node to delete at end of list\n");
            prev->next = NULL;
        }
        printf("Deleting node\n");
        free(prev);

    }
    else {
        printf("ERROR: cannot delete - key not found\n");
    }
}



void print_list(list* l) {
    if (l == NULL) {
        printf("ERROR: tried to print an empty list\n");
    }
    printf("\nPrinting list:\n");
    node* tmp = l->head;
    while (tmp != NULL) {
        printf("Node '%s'\taddress: %p count:%d\n",
               tmp->key, tmp, tmp->count);
        tmp = tmp->next;
    }
    printf("\n");
}

void list_all_keys(list* l) {
    if (l == NULL) {
        printf("ERROR: empty list, cannot list all keys\n");
        return;
    }
    node* tmp = l->head;
    while (tmp != NULL) {
        printf("%s: %d\n",tmp->key, tmp->count);
        tmp = tmp->next;
    }
}




void UnitTest() {
    list* list = create_list("aaa");

    print_list(list);

    insert(list,"bbb");
    insert(list,"ccc");
    insert(list,"aaa");
    insert(list,"ccc");

    print_list(list);
    list_all_keys(list);

    list_delete(list,"111");
    list_delete(list,"bbb");

    print_list(list);
    list_all_keys(list);

    free_list(list);
}

int hash(char s[]) {
    int index;
    int sum = 0;
    int l = strlen(s);
    for (int i = 1; i < l; i++) {
        sum += s[i-1] * (i*i);
    }
    index = sum % M;
    return index;

}


int main() {


    FILE *fp;
    fp = fopen("text.txt", "r");

    if (fp == NULL) {
        printf("Problem opening file\n");
    }


    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int i = 0;


    while (!feof(fp)) {
        fscanf(fp, " %s", words[i]);
        //printf("Scanned %s into words[%d]\n",words[i],i);
        i++;
    }

    list* table[M];


    for (int j = 0; j < i; j++) {
        int index = hash(words[j]);
        //printf("%s --> %d\n", words[j], index);

        if (table[index] == NULL) {
            list* new_list = create_list(words[j]);
            //printf("Making a new list for key '%s' into table[%d]\n", words[j], index);
            table[index] = new_list;

        }
        else {
            insert(table[index], words[j]);
        }
    }

    for (int m = 0; m < M; m++) {
        list* l = table[m];
        if (l != NULL) {
            list_all_keys(l);
            free_list(l);

        }
    }

    fclose(fp);





    //UnitTest();
    return 0;

}
