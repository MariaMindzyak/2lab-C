#include <stdio.h>
#include "dialog.h"
#include "menu.h"
#include "table.h"

int main() {

    Table table = init_table();
    int rc;
    while (rc = dialog(msgs, N)) {
        if (!fptr[rc](&table)) break;
    }
    freeTable(&table);
    printf("That's all, bye!\n");

    return 1;
};
