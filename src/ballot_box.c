#include "ballot_box.h"
#include "helpers.h"

#include <stdlib.h>
#include <string.h>

// A `ballot_box_t` (defined in `ballot_box.h`) is a linked list made
// of `struct bb_node`s. Each node owns the next node, so the whole list
// is owned by the pointer to the first node.
struct bb_node
{
    ballot_t ballot;
    struct bb_node* next;
};


// The empty ballot box is the null pointer.
const ballot_box_t empty_ballot_box = NULL;


void bb_destroy(ballot_box_t bb)
{
    ballot_box_t current = bb;
    ballot_box_t  temp;
    
    while (current != NULL){
        temp = current;
        current = current->next;
        ballot_destroy(temp->ballot);
        free(temp);
    }
}

void bb_insert(ballot_box_t* bbp, ballot_t ballot)
{
    ballot_box_t head = mallocb(sizeof *head, "bb_insert");
    head->ballot = ballot;
    head->next   = *bbp;
    *bbp = head;
}

ballot_box_t read_ballot_box(FILE* inf)
{
    ballot_box_t bb = empty_ballot_box;
    ballot_t temp;
    
    while((temp = read_ballot(inf)) != NULL){
        bb_insert(&bb, temp);
        //bb = bb->next;
    }
   
    return bb;
}

vote_count_t bb_count(ballot_box_t bb)
{
    vote_count_t result = vc_create();
    ballot_box_t current = bb;
    while(current != NULL){
        count_ballot(result, current->ballot);
        current = current->next;
    }

    return result;
   
}

void bb_eliminate(ballot_box_t bb, const char* candidate)
{
    ballot_box_t current = bb;
    while (current != NULL){
        ballot_eliminate(current->ballot, candidate);
        current = current->next;
    }
}

char* get_irv_winner(ballot_box_t bb)
{
    int success = 0;
    size_t votes_candidate;
    char* winner = NULL;

    while(!success){
        vote_count_t current = bb_count(bb);
        votes_candidate = vc_lookup(current, vc_max(current));
        size_t total = vc_total(current);
        if (total == 0) {
            vc_destroy(current);
            return NULL;
        }
        if (votes_candidate > .5 * total){
            success = 1;
            winner = strdupb(vc_max(current), "get_irv_winner");
        } else {
            bb_eliminate(bb, vc_min(current));
        }
        vc_destroy(current);
    }
    
    return winner;
}
