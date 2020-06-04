/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

// YOU MUST NOT MODIFY THIS FILE.
//
// If you do, we may not be able to compile our tests against
// your code.

#pragma once

#include "libvc.h"
#include "ballot.h"

// A `ballot_box_t` represents a ballot box full of ballots;
// essentially, it's a collection of owned ballots.
//
// Because a `ballot_box_t` owns its `ballot_t`s, the owner of a
// `ballot_box_t` must in general call `bb_destroy` in order to free
// both the ballots and the `ballot_box_t`'s own storage. However, only
// a non-empty ballot box actually owns any resources at all, so it is
// unnecessary (but harmless) to call `bb_destroy` on an empty ballot
// box.
//
// How does this work? A `ballot_box_t` is a pointer to a `struct
// bb_node` (which is abstract here and defined in ballot_box.c). Unlike
// the other abstract types we've worked with, there is no function to
// create new `ballot_box_t`; instead, the empty `ballot_box_t` is
// represented by the null pointer:
typedef struct bb_node* ballot_box_t;

// Declares `empty_ballot_box` as the empty ballot box. This is
// represented as NULL as discussed above, but defining the constant
// will make our code more readable and give us a bit of type checking.
//
// Note that `extern` is how we *declare* a constant in a header file;
// the *definition* is in ballot_box.c. This ensures that even though
// multiple files .c may include this header, the constant is defined in
// only one .o file.
extern const ballot_box_t empty_ballot_box;

// Frees the memory associated with a ballot box, including all of
// its ballots (which it owns). `bb` may be null.
//
// OWNERSHIP:
//  - Takes ownership of the argument in order to release its resources.
void bb_destroy(ballot_box_t bb);

// Reads ballots from the given file handle until there are none left to
// read.
//
// PRECONDITION:
//  - `inf` must be open for reading, as by fopen(3).
//
// OWNERSHIP:
//  - Borrows the argument transiently.
//  - The caller takes ownership of the result and must release it with
//    bb_destroy.
//
// ERRORS:
//  - Calls `read_ballot` and `bb_insert`, which exit with a non-zero
//    error code if they cannot allocate memory.
ballot_box_t read_ballot_box(FILE* inf);

// Adds a ballot to a ballot box. Takes a pointer to the `ballot_box_t`,
// or in other words, a `struct bb_node**`, and updates it in place.
//
// PRECONDITIONS:
//  - `bbp` is non-null.
//  - `*bbp` is initialized (but may be null).
//
// OWNERSHIP:
//  - Borrows `bbp` transiently, but takes ownership of `*bbp`, in the
//    sense that any other references to `*bbp` are invalidated after
//    the call.
//  - Takes ownership of `ballot`; thus, the caller must own `ballot`
//    before the call, and must not access it again after `bb_insert`
//    returns.
//
// ERRORS:
//  - Calls perror("bb_insert") and exit(1) on out-of-memory.
void bb_insert(ballot_box_t* bbp, ballot_t ballot);

// Eliminates all votes for the given candidate.
//
// OWNERSHIP:
//  - Borrows both arguments transiently
void bb_eliminate(ballot_box_t bb, const char* candidate);

// Counts each ballot's leading candidate (i.e., the candidate returned
// by `ballot_leader`, if any).
//
// OWNERSHIP:
//  - Borrows the argument transiently.
//  - The caller takes ownership of the result and must release it with
//    `vc_destroy`.
//
// ERRORS:
//  - Exits with a non-zero error code if `vc_update` cannot allocate
//    memory.
//
//  - Calls `count_ballot`, which exits with error code 4 if it cannot
//    allocate memory.
vote_count_t bb_count(ballot_box_t bb);

// Runs the IRV algorithm and returns the name of the winner.
//
// OWNERSHIP:
//  - Borrows the argument transiently.
//
//  - The caller takes ownership of the result and must free it.
//
// ERRORS:
//  - If there are no votes then there is no winner and the result is
//    NULL.
char* get_irv_winner(ballot_box_t bb);
