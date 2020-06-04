/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

// YOU MUST NOT MODIFY THIS FILE.
//
// If you do, we may not be able to compile our tests against
// your code.

#pragma once

// A ballot is a sequence of candidate names, where each name is marked
// as active or inactive. Names are entered as active and may be
// deactivated, and the current vote of the ballot is the first active
// name, if any.
//
// The maximum number of candidates that a ballot can hold is
// MAX_CANDIDATES as defined in libvc.h.

#include "libvc.h"
#include <stdio.h>

// Represents a single voter's ballot.
typedef struct ballot* ballot_t;

// Allocates and returns a new, empty ballot.
//
// OWNERSHIP:
//  - The result is owned by the caller and must be freed using
//  `ballot_destroy`.
//
// ERRORS:
//  - Exits with code 2 if memory cannot be allocated.
ballot_t ballot_create(void);

// Frees the memory associated with a ballot, including any
// owned strings. If `ballot == NULL`, does nothing.
//
// OWNERSHIP:
//  - Takes ownership of `ballot` in order to free it, so the caller
//    must not use it again after that.
void ballot_destroy(ballot_t ballot);

// Reads a ballot from the given file handle and returns it. The input
// format consists of one candidate per line, terminated by either EOF
// or a '%' character on a line by itself. Each candidate name is be
// standardized using `clean_name` before storing it. Returns NULL if
// there is no ballot to read.
//
// OWNERSHIP:
//  - Borrows the argument transiently.
//  - The result is owned by the caller and must be destroyed using
//    `ballot_destroy`.
//
// ERRORS:
//  - Exits with an error code if memory cannot be allocated.
ballot_t read_ballot(FILE* inf);

// Standardizes `name` with `clean_name` and adds it to the end of the
// ballot.
//
// OWNERSHIP
//  - Borrows `ballot` transiently.
//  - Takes ownership of `name`, which means that 1) `name` must
//    have been allocated with `malloc` and owned by the caller prior
//    to the call, and 2) the caller cannot access `name` again after
//    the call.
//
// ERRORS:
//  - Exits with code 3 if the ballot is full.
void ballot_insert(ballot_t ballot, char* name);

// Marks the given candidate as inactive, if present.
//
// OWNERSHIP:
//  - Both arguments are borrowed transiently.
void ballot_eliminate(ballot_t ballot, const char* name);

// Returns the first still-active candidate, or NULL if none remains.
//
// OWNERSHIP:
//  - The result is borrowed from the `ballot` argument, and is valid
//    only as long as the argument is.
const char* ballot_leader(ballot_t ballot);

// Counts a ballot into an existing `vote_count_t` and incrementing the
// count of the current (first active) candidate.
//
// OWNERSHIP:
//  - Both arguments are borrowed transiently.
//
// ERRORS:
//  - If there is no more room in the `vote_count_t` (meaning
//    `vc_update` returns NULL), exits with code 4.
void count_ballot(vote_count_t vc, ballot_t ballot);

// Prints a ballot to the given file handle in a human-readable format.
//
// OWNERSHIP:
//  - Borrows both arguments transiently.
void print_ballot(FILE* outf, ballot_t);

// Standardizes `name` in place by
//  - removing all non-alphabetic characters, and
//  - converting all letters to uppercase.
void clean_name(char* name);

