#include "ballot_box.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    ballot_box_t bb = read_ballot_box(stdin);
    char* winner    = get_irv_winner(bb);

    if (! winner) {
        fprintf(stderr, "%s: no votes, no winner\n", argv[0]);
        bb_destroy(bb);
        exit(1);
    }

    printf("%s\n", winner);
    free(winner);
    bb_destroy(bb);
}
