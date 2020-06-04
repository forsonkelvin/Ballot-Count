///
/// Tests for functions in ../src/ballot_box.c.
///

#include "ballot_box.h"
#include "helpers.h"

#include <lib211.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


///
/// FORWARD DECLARATIONS
///

// Takes the expected winner, followed by all the votes. Each ballot is
// given as successive string arguments, terminated by "%". The final
// argument, after the final ballot's "%", must be NULL.
static void assert_election(const char* expected_winner, ...);
static ballot_box_t ballotreadfile(const char* filename);
static char* results(const char* filename);

static void test_file(void);

// Test case functions (you need more!):
static void three_candidates_tied(void),
    win_on_second_round(void),
    example_from_wikipedia(void),
    test_no_candidates(void),
    test_one_candidate(void),
    test_one_diff_ballots(void);



///
/// MAIN FUNCTION
///

int main(void)
{
    three_candidates_tied();
    win_on_second_round();
    example_from_wikipedia();
    test_one_candidate();
    test_one_diff_ballots();
    // test_no_candidates();
}


///
/// TEST CASE FUNCTIONS
///

static void test_file(void)
{
    char* winner = results("Resources/given_test.in");
    CHECK_STRING(winner, "CAMPBELL");

    free(winner);
}


static void test_no_candidates(void)
{
    char* winner = results("Resources/no_candidates.in");
    CHECK_POINTER(winner, NULL);
    free(winner);
}

static void test_one_candidate(void)
{
    char* winner = results("Resources/one_candidate.in");
    CHECK_STRING(winner, "KELVIN");
    free(winner);

    char* winner1 = results("Resources/two_cans_diff_ballots.in");
    CHECK_STRING(winner1, "FORSON");
    free(winner1);

    char* winner2 = results("Resources/two_cans_one_ballot.in");
    CHECK_STRING(winner2, "KELVIN");
    free(winner2);

    char* winner3 = results("Resources/three_can_one_ballot.in");
    CHECK_STRING(winner3, "KELVIN");
    free(winner3);

    char* winner4 = results("Resources/same_can_one_ballot.in");
    CHECK_STRING(winner4, "KELVIN");
    free(winner4);

    char* winner5 = results("Resources/tied_cans.in");
    CHECK_STRING(winner5, "KAGIMBI");
    free(winner5);

    char* winner6 = results("Resources/diff_length.in");
    CHECK_STRING(winner6, "CURRY");
    free(winner6);
    
}
    
static void test_one_diff_ballots(void)
{
    char* winner = results("Resources/one_can_diff_ballots.in");
    CHECK_STRING(winner, "KELVIN");
    free(winner);
}


static void three_candidates_tied(void)
{
    assert_election("A",
            "a", "b", "c", "%",
            "a", "b", "c", "%",
            NULL);
}

static void win_on_second_round(void)
{
    assert_election("BIDEN",
            "Bloomberg", "Biden", "%",
            "Yang", "Biden", "%",
            "Warren", "Biden", "%",
            "Sanders", "Biden", "%",
            "Biden", "Sanders", "%",
            "Biden", "Gabbard", "%",
            NULL);
}

static void example_from_wikipedia(void)
{
    assert_election("SUE",
            "bob", "bill", "s u e", "%",
            "Sue", "Bob", "Bill", "%",
            "Bill!", "Sue!", "BoB!", "%",
            "bob", "bill", "sue", "%",
            "sue", "bob", "bill", "%",
            NULL);
}


///
/// HELPER FUNCTIONS
///

static void assert_election(const char* expected, ...)
{
    ballot_box_t bb = empty_ballot_box;

    {
        ballot_t ballot = ballot_create();

        va_list ap;
        va_start(ap, expected);

        char* name;
        while ((name = va_arg(ap, char*))) {
            if (strcmp(name, "%") == 0) {
                bb_insert(&bb, ballot);
                ballot = ballot_create();
            } else {
                ballot_insert(ballot, strdupb(name, "assert_election"));
            }
        }

        va_end(ap);

        ballot_destroy(ballot);
    }

    char* actual = get_irv_winner(bb);
    if (expected)
        CHECK_STRING(actual, expected);
    else
        CHECK_POINTER(actual, expected);
    free(actual);

    bb_destroy(bb);
}

static ballot_box_t ballotreadfile(const char* filename)
{
    FILE* inf = fopen(filename, "r");

    if(inf == NULL){
        printf("unsuccessful");
        exit(3);
    }

    ballot_box_t bb;
    bb = read_ballot_box(inf);
    fclose(inf);

    return bb;

}

static char* results(const char* filename)
{
    ballot_box_t bb = ballotreadfile(filename);

    char* winner = get_irv_winner(bb);

    if (!winner){
        fprintf(stderr, "%s: no votes, no winner\n", filename);

        bb_destroy(bb);

        exit(1);

    }

    char* name;

    name = strdupb(winner, "results");

    free(winner);

    bb_destroy(bb);

    return name;


}
