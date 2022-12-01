#include <iostream>
#include "../include/lib_func.h"

void print_node(const node *p)
{
    printf("%s", p->value);
}

int main()
{
    tree t;
    initialize(&t);

    const char *word = u8"Now I won!";
    for (int i = 0; i < strlen(word); i++) {
        char c[2] = { word[i] };
        add(&t, (int)strlen(word) - 1 - i, c);
    }

    printf("%sを逆さに読むと", word);

    search(&t, print_node);

    printf("\n");

    finalize(&t);
}
