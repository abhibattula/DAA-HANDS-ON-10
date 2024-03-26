#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10
#define LOAD_FACTOR_THRESHOLD 0.75
#define SHRINK_FACTOR_THRESHOLD 0.25

// Structure for a node in the doubly linked list
struct ListNode 
{
    int key;
    int data;
    struct ListNode* next;
    struct ListNode* prev;
};


struct LinkedList 
{
    struct ListNode* head;
    struct ListNode* tail;
};


struct HashTable 
{
    int capacity;
    int size;
    struct LinkedList* table;
};


int hashFunction(int key, int capacity);
void insert(struct HashTable* hashTable, int key, int data);
void del (struct HashTable* hashTable, int key);
int search(struct HashTable* hashTable, int key);
void display(struct HashTable* hashTable);
void rehashify(struct HashTable* hashTable);
void resize(struct HashTable* hashTable, int newCapacity);


struct LinkedList* createLinkedList() 
{
    struct LinkedList* list = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}


struct HashTable* createHashTable(int capacity) 
{
    struct HashTable* hashTable = (struct HashTable*)malloc(sizeof(struct HashTable));
    hashTable->capacity = capacity;
    hashTable->size = 0;
    hashTable->table = (struct LinkedList*)malloc(capacity * sizeof(struct LinkedList));
    for (int i = 0; i < capacity; i++) 
    {
        hashTable->table[i] = *createLinkedList();
    }
    return hashTable;
}


int hashFunction(int key, int capacity) 
{
    return key % capacity;
}

// Insert key-value pair into the hash table
void insert(struct HashTable* hashTable, int key, int data) 
{
    int index = hashFunction(key, hashTable->capacity);
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->key = key;
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (hashTable->table[index].head == NULL) 
    {
        hashTable->table[index].head = newNode;
        hashTable->table[index].tail = newNode;
    } 
    
    else 
    {
        newNode->prev = hashTable->table[index].tail;
        hashTable->table[index].tail->next = newNode;
        hashTable->table[index].tail = newNode;
    }
    hashTable->size++;

    // Check load factor and resize if necessary
    if ((double)hashTable->size / hashTable->capacity >= LOAD_FACTOR_THRESHOLD) 
    {
        rehashify(hashTable);
    }
}


void del (struct HashTable* hashTable, int key) 
{
    int index = hashFunction(key, hashTable->capacity);
    struct ListNode* current = hashTable->table[index].head;

    while (current != NULL) 
    {
        if (current->key == key) 
        {
            if (current->prev != NULL) 
            {
                current->prev->next = current->next;
            } 
            else 
            {
                hashTable->table[index].head = current->next;
            }
            if (current->next != NULL) 
            {
                current->next->prev = current->prev;
            } 
            
            else 
            {
                hashTable->table[index].tail = current->prev;
            }
            free(current);
            hashTable->size--;

            
            if ((double)hashTable->size / hashTable->capacity <= SHRINK_FACTOR_THRESHOLD) 
            
            {
                rehashify(hashTable);
            }
            return;



        }
        current = current->next;
    }



    printf("\n\n Key not found in the hash table.\n");
}









int search(struct HashTable* hashTable, int key) 
{
    int index = hashFunction(key, hashTable->capacity);
    struct ListNode* current = hashTable->table[index].head;

    while (current != NULL) 
    {
        if (current->key == key) 
        {
            return current->data;
        }

        current = current->next;
    }

    return -1; // Key not found
}





void display(struct HashTable* hashTable) 

{
    printf("Hash Table:\n");
    for (int i = 0; i < hashTable->capacity; i++) 
    {
        printf("[%d]: ", i);
        struct ListNode* current = hashTable->table[i].head;
        while (current != NULL) 
        {
            printf("(%d, %d) -> ", current->key, current->data);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Resize the hash table to the new capacity
void resize(struct HashTable* hashTable, int newCapacity) 
{
    struct HashTable* newHashTable = createHashTable(newCapacity);
    for (int i = 0; i < hashTable->capacity; i++) 
    {
        struct ListNode* current = hashTable->table[i].head;
        while (current != NULL) 
        {
            insert(newHashTable, current->key, current->data);
            current = current->next;
        }
    }
    // Swap old hash table with the new one
    struct LinkedList* tempTable = hashTable->table;
    hashTable->table = newHashTable->table;
    newHashTable->table = tempTable;

    // Update capacity
    int tempCapacity = hashTable->capacity;
    hashTable->capacity = newHashTable->capacity;
    newHashTable->capacity = tempCapacity;

    // Free memory
    for (int i = 0; i < newHashTable->capacity; i++) 
    {
        free(newHashTable->table[i].head);
    }
    free(newHashTable->table);
    free(newHashTable);
}

// Rehash the hash table with a larger capacity
void rehashify(struct HashTable* hashTable) 
{
    resize(hashTable, hashTable->capacity * 2);
}

int main() 
{
    struct HashTable* hashTable = createHashTable(INITIAL_CAPACITY);

    int choice, key, data;
    while (1) 
    {
        printf("\nOperations on Hash Table:\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Search\n");
        printf("4. Rehashify\n");
        printf("5. Display\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);


        switch (choice) 
        {
            case 1:
                printf("Enter key and data to insert: ");
                scanf("%d %d", &key, &data);
                insert(hashTable, key, data);
                break;

            case 2:
                printf("Enter the key to delete: ");
                scanf("%d", &key);
                del(hashTable, key);
                break;
            case 3:

                printf("Enter the key to search: ");
                scanf("%d", &key);
                data = search(hashTable, key);
                if (data != -1) 
                {
                    printf("Key found. Data: %d\n", data);
                } 
                else 
                {
                    printf("Key not found.\n");
                }
                break;
            case 4:
                rehashify(hashTable);
                printf("Hash table rehashified.\n");
                break;
            case 5:
                display(hashTable);
                break;
            case 6:
                
                exit(0);
            default:
            
                printf("Invalid choice!\n");
       
       
        }
    }

    




}
