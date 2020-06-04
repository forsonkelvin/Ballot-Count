#pragma once

// It's perfectly fine to modify this header file. In particular,
// it's a good place to declare any helper functions that you wish to
// share between ballot.c and ballot_box.c. (Define the functions in
// helper.c, of course.)

// The two functions in this file, strdupb(), and mallocb(), are like
// strdup() and malloc(), except that:
//
//  1) They perform the necessary NULL check for you and exit with an
//  error if it fails (just like HW3’s optional strdup_or_else()).
//
//  2) They take a second argument: a string to include in the error
//  message, to provide some context for where or why the failure
//  happened.


// For size_t (used in the first declaration below):
#include <stddef.h>


// strdupb - Obtains an owned clone of a borrowed string or exits
// with an error.
//
// ARGUMENTS
//
// `s` - the string to copy; borrowed ephemerally
// `blame` - blamed in the error message; borrowed ephemerally
//
// RESULT
//
// A pointer to a new, heap-allocated, 0-terminated string containing
// the same characters as `s`. The caller owns the new string and is
// responsible for deallocating it using free(3) after its last use.
//
// ERRORS
//
// If memory cannot be allocated then the function prints an error
// message and exits with error code 1.
char* strdupb(const char* s, const char* blame);


// mallocb - Allocates heap memory or exits with an error.
//
// ARGUMENTS
//
// `size`: the number of bytes to allocate
// `blame` - blamed in the error message; borrowed ephemerally
//
// RESULT
//
// A pointer to a new, heap-allocated object suitably aligned to contain
// values of any type. The caller owns the new object and is responsible
// for deallocating it using free(3) after its last use.
//
// ERRORS
//
// If memory cannot be allocated then the function prints an error
// message and exits with error code 1.
void* mallocb(size_t size, const char* blame);

