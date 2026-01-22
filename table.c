#include <stddef.h>
#include <malloc.h>
#include "table.h"
#include <stdio.h>
#include <string.h>
#define MAX_TABLE_SIZE 4 

Table init_table() {
    Table table;
    table.table_size = 0;
    table.max_table_size = MAX_TABLE_SIZE;
    table.ks = malloc(MAX_TABLE_SIZE * sizeof(KeySpace));
    return table;
}

int bin_search(Table table, int key){
    int left = 0;
    int right = table.table_size - 1;
    int mid;

    while (left <= right) {
        mid = (left + right) / 2;
        if (table.ks[mid].key == key) return mid;
        if (table.ks[mid].key > key) {
            right = mid - 1;
            continue;
        }
        if (table.ks[mid].key < key) {
            left = mid + 1;
            continue;
        }
    }
    return -1;
}

int search_by_key(Table table, int key, Table *table_copy) {
    int n = bin_search(table, key);
    if(n == -1) return NOT_FOUND;
    else{
        Node* this_node = table.ks[n].node;
        while(this_node != NULL) {
            add_el(table_copy, key, this_node->info);
            this_node = this_node->next;
        }
    }
    return OK;
}

int search_by_key_and_release(Table table, int key, int release, Table* table_copy) {
    int n = bin_search(table, key);
    if(n == -1) return NOT_FOUND;
    if(table.ks[n].node->release < release) return OUT_OF_TABLE;
    else{
        Node* this_node = table.ks[n].node;
        while (this_node->release > release) {
            this_node = this_node->next;
        }
        add_el(table_copy, key, this_node->info);
        table_copy->ks[0].node->next = NULL;
    }

    return OK;
}

int add_el(Table *table, int key, char *info) {
    if (table->table_size == table->max_table_size){
    	//printf("Table is full");
    	return OVERFLOW;
    }
    int idx = bin_search(*table, key);
    if (idx != -1) {
        Node* this_node = calloc(1, sizeof(Node));
        this_node->release = (table->ks[idx].node->release) + 1;
        this_node->info = strdup(info);
        this_node->next = table->ks[idx].node;
        table->ks[idx].node = this_node;
        return OK;
    }
    
    idx = 0;
    while ((idx != table->table_size) && (table->ks[idx].key < key)) {
        idx++;
    }
    /*if (idx != table->table_size) {
        for (int j = table->max_table_size - 1; j >= idx; j--) {
            table->ks[j + 1] = table->ks[j];
        }
    }*/

    for (int j = table->table_size; j > idx; j--) {
                table->ks[j] = table->ks[j-1];
    }
    
    table->ks[idx].node = calloc(1, sizeof(Node));
    table->ks[idx].node->release = table->ks[idx].node->release + 1;
    table->ks[idx].node->info = strdup(info);
    table->ks[idx].key = key;
    table->ks[idx].node->next = NULL;
    table->table_size++;
    return OK;
}

int delete_el_by_key(Table* table, int key){
    int idx = bin_search(*table, key);
    if (idx == -1) return NOT_FOUND;
 
    	Node* node = table->ks[idx].node;
    	Node* next = table->ks[idx].node->next;
    	while (next != NULL){
    	    free(node->info);
    	    free(node);
    	    node = next;
    	    next = next->next;
    	}
    	free(node->info);
    	free(node);
        for (int i = idx; i<table->table_size - 1; i++){
            table->ks[i].key = table->ks[i+1].key;
            table->ks[i].node = table->ks[i+1].node;
        }
        table->table_size -= 1;
 
    
    return OK;
}

int delete_el_by_key_and_release(Table* table, int key, int release){
    int idx = bin_search(*table, key);

    if (idx == -1) return NOT_FOUND;

    if(table->ks[idx].node->release < release) return OUT_OF_TABLE;

    if (release == 1 && table->ks[idx].node->release == 1) {
        int rc = delete_el_by_key(table, key);
        return rc;
    }
    Node* head = table->ks[idx].node;
    Node* prev = table->ks[idx].node;
    Node* this_node = table->ks[idx].node;
    while (this_node->release != release){
        prev = this_node;
        this_node = this_node->next;
    }
    Node* sub_node = this_node->next;
    if (head == this_node){
        table->ks[idx].node = sub_node;
    }
    else {
        prev->next = sub_node;
    }
    free(this_node->info);
    free(this_node);
    return OK;
}

void freeTable(Table* table){
    for(int i = 0; i<table->table_size; i++){
    	if (table->ks[i].node == NULL) continue;
    	Node* node = table->ks[i].node;
    	Node* next = table->ks[i].node->next;
    	while (next != NULL){
        	free(node->info);
        	free(node);
        	node = next;
      		next = next->next;
        }
        free(node->info);
        free(node);
    }
    free(table->ks);
    //free(table);
}
	
