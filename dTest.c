#include "dTest.h"

/* Common strings */
static const char PASS_STRING[]         = "PASS";
static const char FAIL_STRING[]         = "FAIL";
static const char LINE_BREAK_STRING[]   = "--------------------";

/* These EXIT macros save us when our tests fail.*/
#define TEST_IS_SAFE() (setjmp(Results.exit_point) == 0)

#define FAIL_AND_GOTO_EXIT                      \
    {Results.active_test_failed = 1;            \
     longjmp(Results.exit_point, 1);}

/* Forward Output Functions/declarations. */
#define PRINT_EOL()     putchar('\n')
void PrintTestStatus(void);


/* We store all of our pass/fail results and exit info inside this singleton */
struct _testResults
{
    const char *active_file;
    const char *active_test;
    int cur_line;
    int num_tests;
    int num_success_tests;
    int num_failed_tests;
    int active_test_failed;
    jmp_buf exit_point;
};
struct _testResults Results;

/*
 *  dTest Suite Major Functions
 */

void InitializeTesting (const char* filename)
{
    Results.active_file = filename;
    Results.active_test = NULL;
    Results.cur_line = 0;
    Results.num_tests = 0;
    Results.num_success_tests = 0;
    Results.num_failed_tests = 0;
    Results.active_test_failed = 0;
}

int EndTesting(void)
{
    PRINT_EOL();
    printf("%s\n", LINE_BREAK_STRING);
    printf("Tests: %d | Fails: %d\n", Results.num_tests, Results.num_failed_tests);
    if( Results.num_failed_tests == 0 ){
        printf("Passed all tests.");
    } else {
        printf("FAIL");
    }
    PRINT_EOL();
    fflush(stdout);
    return Results.num_failed_tests;
}

void EndActiveTest(void)
{
    if (Results.active_test_failed){
        Results.num_failed_tests++;
    } else {
        PrintTestStatus();
        printf("%s", PASS_STRING);
    }
    Results.active_test_failed = 0;
    PRINT_EOL();
    fflush(stdout);
}

/* The ASSERTs inside your Test function take care of reporting failure */
void RunTest (testFunc_p Func, const char *func_name, const int func_line_num)
{
    Results.active_test = func_name;
    Results.cur_line = func_line_num;
    Results.num_tests++;
    /* If we haven't ran our test function yet, Do it! */
    if (TEST_IS_SAFE()){
        Func();
    }
    EndActiveTest();
}

void FailTest(const char *str, int line_num)
{
    if (Results.active_test_failed)
        return;

    //PrintTestStatus(Results.active_file, line_num);
    PrintTestStatus();
    printf("%s", FAIL_STRING);
    if (str != NULL){
        printf(": %s", str);
    }
    FAIL_AND_GOTO_EXIT;
}

/*
 * Assert Functions
 * See macros in dTest.h, they will be easier to use.
 */


/*
 * Output Functions
 */

void PrintTestStatus(void)
{
    if (Results.active_file != NULL){
        printf("%s: %d : %s : ", Results.active_file, Results.cur_line, Results.active_test);
    }
}
