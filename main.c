#include <stdio.h>
#include <stdlib.h>

// Define structure for doubly linked list node
struct ListNode {
    int data;
    struct ListNode* prev;
    struct ListNode* next;
};

// Define structure for hash table entry
struct HashEntry {
    struct ListNode* head;
};

// Define structure for hash table
struct HashTable {
    struct HashEntry* table;
    int size;
    int capacity;
};

// Function to initialize a doubly linked list node
struct ListNode* createListNode(int data) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Function to initialize a hash table
struct HashTable* createHashTable(int capacity) {
    struct HashTable* hashTable = (struct HashTable*)malloc(sizeof(struct HashTable));
    if (hashTable == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    hashTable->table = (struct HashEntry*)malloc(capacity * sizeof(struct HashEntry));
    if (hashTable->table == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    hashTable->size = 0;
    hashTable->capacity = capacity;
    // Initialize hash table entries
    for (int i = 0; i < capacity; i++) {
        hashTable->table[i].head = NULL;
    }
    return hashTable;
}

// Function to calculate hash value using multiplication method
int hash(int key, int capacity) {
    const float A = 0.6180339887; // Golden ratio
    float temp = key * A;
    temp -= (int)temp;
    return (int)(capacity * temp);
}

// Function to insert a key into the hash table
void insert(struct HashTable* hashTable, int key) {
    int index = hash(key, hashTable->capacity);
    struct ListNode* newNode = createListNode(key);
    // Insert at the beginning of the linked list (chaining)
    if (hashTable->table[index].head == NULL) {
        hashTable->table[index].head = newNode;
    } else {
        newNode->next = hashTable->table[index].head;
        hashTable->table[index].head->prev = newNode;
        hashTable->table[index].head = newNode;
    }
    hashTable->size++;
    // Check if hash table needs to be resized
    if (hashTable->size >= hashTable->capacity * 3 / 4) {
        // Double the size of the array and re-hash everything
        int newCapacity = hashTable->capacity * 2;
        struct HashTable* newHashTable = createHashTable(newCapacity);
        for (int i = 0; i < hashTable->capacity; i++) {
            struct ListNode* current = hashTable->table[i].head;
            while (current != NULL) {
                insert(newHashTable, current->data);
                current = current->next;
            }
        }
        // Copy the new hash table to the original one
        free(hashTable->table);
        hashTable->table = newHashTable->table;
        hashTable->capacity = newHashTable->capacity;
        free(newHashTable);
    }
}

// Function to delete a key from the hash table
void delete(struct HashTable* hashTable, int key) {
    int index = hash(key, hashTable->capacity);
    struct ListNode* current = hashTable->table[index].head;
    while (current != NULL) {
        if (current->data == key) {
            if (current->prev == NULL) {
                hashTable->table[index].head = current->next;
                if (current->next != NULL) {
                    current->next->prev = NULL;
                }
            } else {
                current->prev->next = current->next;
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
            }
            free(current);
            hashTable->size--;
            // Check if hash table needs to be resized
            if (hashTable->size <= hashTable->capacity / 4) {
                // Halve the size of the array and re-hash everything
                int newCapacity = hashTable->capacity / 2;
                struct HashTable* newHashTable = createHashTable(newCapacity);
                for (int i = 0; i < hashTable->capacity; i++) {
                    struct ListNode* current = hashTable->table[i].head;
                    while (current != NULL) {
                        insert(newHashTable, current->data);
                        current = current->next;
                    }
                }
                // Copy the new hash table to the original one
                free(hashTable->table);
                hashTable->table = newHashTable->table;
                hashTable->capacity = newHashTable->capacity;
                free(newHashTable);
            }
            return;
        }
        current = current->next;
    }
    printf("Key not found in the hash table.\n");
}

// Function to display the hash table
void display(struct HashTable* hashTable) {
    printf("Hash table:\n");
    for (int i = 0; i < hashTable->capacity; i++) {
        printf("Bucket %d: ", i);
        struct ListNode* current = hashTable->table[i].head;
        while (current != NULL) {
            printf("%d -> ", current->data);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main() {
    // Initialize hash table with initial capacity
    struct HashTable* hashTable = createHashTable(10);

    int choice, key;
    while (1) {
        printf("\nOperations on Hash Table:\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Display\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Enter the key to insert: ");
                scanf("%d", &key);
                insert(hashTable, key);
                break;
            case 2:
                printf("Enter the key to delete: ");
                scanf("%d", &key);
                delete(hashTable, key);
                break;
            case 3:
                display(hashTable);
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

