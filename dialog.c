#include "dialog.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "table.h"

int getInt(int *a){
    int n;
    do{
        n = scanf("%d", a);
        if(n<0){
            return 0;
        }
        if(n==0){
            printf("%s\n", "Error! Repeat, please");
            scanf("%*[^\n]");
            scanf("%*c");

        }
    }while(n==0);
    return 1;
}


char *getStr(){
    int flag = 1, len = 0;
    char *input = NULL;
    while (flag > 0){
        char *buff = malloc(200 * sizeof(char));
        flag = scanf("%199[^\n]", buff);
        if (flag == EOF) {
            input = NULL;
        } else if (flag == 0) {
            scanf("%*c");
        } else {
            int buffLen = strlen(buff);
            input = realloc(input, (len + buffLen) * sizeof(char));
            for (int i = 0; i < buffLen; i++) {
                input[len + i] = buff[i];
            }
            len += buffLen;
        }
        free(buff);
    }
    if (flag != - 1) {
        input = realloc(input, (len + 1) * sizeof(char));
        input[len] = '\0';
    }
    return input;
}

int dialog(const char *msgs[], int N) {
    char *errmsg = "";
    int rc;
    int i, n;
    do {
        puts(errmsg);
        errmsg = "You're wrong. Repeat, please!\n";
        for (i = 0; i < N; i++) {
            puts(msgs[i]);
        }
        puts("Make your choice: ");
        n = getInt(&rc);
        if (n == 0) rc = 0;

    } while (rc < 0 || rc >= N);
    return rc;
}


int D_Add(Table *table) {
    int k, rc, n;
    char *info = NULL;
    printf("%s","Enter key --> ");
    n = getInt(&k);
    scanf("%*c");
    if (n == 0){
        printf("%s\n",Error_names[ERR_INPUT]);
        return 0;
    }
    printf("%s", "Enter info --> ");
    info = getStr();
    //scanf("%*c");
    if (info == NULL) {
        printf("%s\n",Error_names[ERR_INPUT]);
        return 0;
    }
    rc = add_el(table, k, info);
    if(rc == 0){
        D_show(table);
    }

    if(rc == 3){
        printf("%s\n", "TABLE IS FULL");
    
    }
    free(info);
    return 1;
    
}

int D_searchbykeyandrelease(Table *table) {
    int k, r, rc, n;
    printf("Enter key --> ");
    n = getInt(&k);
    scanf("%*c");
    printf("%s\n", "Enter release(enter 0 if you want to search all releases: ");
    n = getInt(&r);
    scanf("%*c");

	Table table_copy = init_table();
	
    if (r == 0) {
        rc = search_by_key(*table, k, &table_copy);
        //if (rc == 0) D_show(&table_copy);
        while (rc == 1){
        	//freeTable(&table_copy);
            printf("Error %s! Repeat, please\n", "NOT FOUND");
            printf("Enter key --> ");
            n = getInt(&k);
            scanf("%*c");
            rc = search_by_key(*table, k, &table_copy);
        }
        if (rc == 0) D_show(&table_copy);
        freeTable(&table_copy);
        return 1;
    }

    else {
        rc = search_by_key_and_release(*table, k, r, &table_copy);
        //if (rc == 0) D_show(&table_copy);
        while (rc == 1){
            //freeTable(&table_copy);
            printf("Error %s! Repeat, please\n", "NOT FOUND");
            printf("Enter key --> ");
            n = getInt(&k);
            scanf("%*c");
            printf("Enter release --> ");
            n = getInt(&r);
            scanf("%*c");
            rc = search_by_key_and_release(*table, k, r, &table_copy);
        }
        while (rc == 2){
        	//freeTable(&table_copy);
            printf("Error %s! Repeat, please\n", "OUT OF TABLE");
            printf("Enter key --> ");
            n = getInt(&k);
            scanf("%*c");
            printf("Enter release --> ");
            n = getInt(&r);
            scanf("%*c");
            rc = search_by_key_and_release(*table, k, r, &table_copy);
        }
        if (rc == 0) D_show(&table_copy);
    }
        
	freeTable(&table_copy);
    return 1;
    
   
}


int D_deletebykeyandrelease(Table *table) {
    int k, r, rc, n;
    printf("%s", "Enter key --> ");
    n = getInt(&k);
    scanf("%*c");
    printf("%s\n", "Enter release (enter 0 if you want to delete all releases): ");
    n = getInt(&r);
    scanf("%*c");

    if (r == 0) {
        rc = delete_el_by_key(table, k);
        //if (rc == 0) D_show(table);
        while (rc == 1){
            printf("Error %s! Repeat, please\n", "NOT FOUND");
            printf("Enter key --> ");
            n = getInt(&k);
            scanf("%*c");
            rc = delete_el_by_key(table, k);
        }
        if (rc == 0) D_show(table);
    }

    else {
        rc = delete_el_by_key_and_release(table, k, r);
        //if (rc == 0) D_show(table);
        while (rc == 1){
            printf("Error %s! Repeat, please\n", "NOT FOUND");
            printf("Enter key --> ");
            n = getInt(&k);
            scanf("%*c");
            rc = delete_el_by_key_and_release(table, k, r);
        }
        while (rc == 2){
            printf("Error %s! Repeat, please\n", "OUT OF TABLE");
            printf("Enter key --> ");
            n = getInt(&k);
            scanf("%*c");
            rc = delete_el_by_key_and_release(table, k, r);
        }
        if (rc == 0) D_show(table);
    }

    return 1;
    
}



int D_show(Table *table) {
    Node *this_node;
    for (int i = 0; i < table->table_size; i++) {
        this_node = table->ks[i].node;
        while (this_node != NULL) {
            printf("%d| %d| %s\n", table->ks[i].key, this_node->release, this_node->info);
            this_node = this_node->next;
        }
    }
    return 1;
}

