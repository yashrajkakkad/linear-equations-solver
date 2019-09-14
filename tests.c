#include <stdio.h>
#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))

int main(int argc, char const *argv[])
{
    int stack[1000];
    int *sp;
    sp = stack; /* initialize */
    push(sp, 10);
    int x = pop(sp);
    printf("%d", x);
    x = pop(sp);
    printf("%d", x);
    return 0;
}
