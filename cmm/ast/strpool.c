#include <stdlib.h>
#include <string.h>

#include "strpool.h" 

#define POOL_TABLE_SIZE 1024

typedef struct String {
    char *name;
    struct String *next;
} String;

static String *string_table[POOL_TABLE_SIZE];

unsigned int hash(const char *str) {
    unsigned int h = 0;
    while (*str) {
        h = (h << 5) + h + (unsigned char)*str++;
    }
    return h % POOL_TABLE_SIZE;
}

char *lookup(const char *name) {
    unsigned int h = hash(name);
    String *sym = string_table[h];
    while (sym) {
        if (strcmp(sym->name, name) == 0)
            return sym->name;
        sym = sym->next;
    }
    // Not found; allocate and store it.
    String *new_sym = malloc(sizeof(String));
    new_sym->name = strdup(name);
    new_sym->next = string_table[h];
    string_table[h] = new_sym;
    return new_sym->name;
}

/* Call this at the end of your program to free the interned strings. */
void free_strpool() {
    for (int i = 0; i < POOL_TABLE_SIZE; i++) {
        String *sym = string_table[i];
        while (sym) {
            String *next = sym->next;
            free(sym->name);
            free(sym);
            sym = next;
        }
    }
}
