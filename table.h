
#ifndef LAB3_TABLE_H
#define LAB3_TABLE_H

#include "table.h"

enum Errors{
    OK = 0,
    NOT_FOUND = 1,
    OUT_OF_TABLE = 2,
    OVERFLOW = 3,
};

typedef struct Node {
// номер версии
    int release;
// указатель на информацию
    char *info;
// указатель на следующий элемент
    struct Node *next;
} Node;

typedef struct KeySpace {
// ключ элемента
    int key;
// указатель на информацию
    Node *node;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    int max_table_size;
    int table_size;
} Table;

Table init_table();

int bin_search(Table table, int key);

int search_by_key(Table table, int key, Table *table_copy);

int search_by_key_and_release(Table table, int key, int release, Table* table_copy);

int add_el(Table *table, int key, char *info);

int delete_el_by_key(Table* table, int key);

int delete_el_by_key_and_release(Table* table, int key, int release);

void freeTable(Table* table);


#endif //LAB3_TABLE_H
