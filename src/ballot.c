
#include "ballot.h"
#include "helpers.h"

#include <lib211.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A `ballot_t` (defined in `ballot.h`) is be a pointer to a
// heap-allocated `struct ballot`, with the following invariant:
//
//  - `length <= MAX_CANDIDATES`
//
//  - the first `length` elements of `entries` are initialized
//
//  - the `name` field of each of the first `length` entries is non-NULL
//    and points to an OWNED, heap-allocated string containing only
//    letters, all of which are uppercase (a la isupper(3)).
//
// The remaining elements of `entries` (`MAX_CANDIDATES - length`)
// should be considered uninitialized.

struct entry
{
    char* name;
    bool active;
};

struct ballot
{
    size_t length;
    struct entry entries[MAX_CANDIDATES];
};


ballot_t ballot_create(void)
{
    ballot_t ballot = malloc(sizeof( struct ballot));
    if(ballot  == NULL)
        exit(2);
    ballot->length = 0;
    return ballot;
}

void ballot_destroy(ballot_t ballot)
{
    if(ballot != NULL){
        for (size_t i = 0; i < ballot->length; ++i){
            free(ballot->entries[i].name);}
        free(ballot);
    }
}

void ballot_insert(ballot_t ballot, char* name)
{
    if (ballot->length < MAX_CANDIDATES){
        clean_name(name);
        ballot->entries[ballot->length].name = name;
        ballot->entries[ballot->length].active = true;
        ++(ballot->length);
    }else{
        exit(3);
    }       
}

const char* ballot_leader(ballot_t ballot)
{
    for (size_t i = 0; i < ballot->length; ++i){
        if (ballot->entries[i].active){
            return ballot->entries[i].name;
        }
    }
    return NULL;
}

void ballot_eliminate(ballot_t ballot, const char* name)
{
    for (size_t i = 0; i < ballot->length; ++i){
        if (strcmp(ballot->entries[i].name, name) == 0){
            ballot->entries[i].active = false;
        }
    }
}

void count_ballot(vote_count_t vc, ballot_t ballot)
{
    const char* name = ballot_leader(ballot);
    if(name != NULL){
        size_t * count = vc_update(vc, name);
        if(count == NULL)
        {
            exit(4);
        } else{
            ++*count;
        }
    }
}

ballot_t read_ballot(FILE* inf)
{
    
    ballot_t ballot = ballot_create();
    char* line;
    while ((line = fread_line(inf))){
      
        if (strcmp(line, "%") == 0){
            free(line);
            return ballot;
        }
        if (ballot->length > MAX_CANDIDATES){
            free(line);
            exit(3);
        }
        ballot_insert(ballot,line);
    }
    if (ballot->length > 0){
        return ballot;
    } else {
        free(line);
        ballot_destroy(ballot);
        return NULL;
    }
    ballot_destroy(ballot);
}

void clean_name(char* name)
{
    char* dst = name;
    size_t i = 0;
    size_t j = 0;
 
    while(name[i] != '\0'){
        if (!(isalpha(name[i]))){
            i++;
            
        }
        else{
            dst[j] = toupper(name[i]);
            i++;
            j++;
        }
    }
    name[j] = '\0';
    return;
        
}

void print_ballot(FILE* outf, ballot_t ballot)
{
    for (size_t i = 0; i < ballot->length; ++i) {
        bool active = ballot->entries[i].active;
        fprintf(outf, "%c%s%s\n",
                active? ' ' : '[',
                ballot->entries[i].name,
                active? "" : "]");
    }
}
