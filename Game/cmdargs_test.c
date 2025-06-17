/**
 * @file cmdargs_test.c
 *
 * Test for functions in cmdargs.c
 */

#include <assert.h>
#include <stdio.h>

/* Include the interface of the module under testing */
#include"cmdargs.h"

/******************************************************/


/**
 * @brief Main entry for the test.
 */



static void test_failed_cmdargs(void)
{
    struct cmdargs cmdargs = {0};
    assert(cmdargs_parse(&cmdargs, 4,(char *[4]){"dummy","--window-size","17", "12a" }) == 0 );
    assert(cmdargs.x == 17);
    assert(cmdargs.windowSizeCounter == 1);

    cmdargs = (struct cmdargs){0};
    assert(cmdargs_parse(&cmdargs, 4,(char *[4]){"dummy", "--window-size", "32a", "dummy"}) == 0 );
    assert(cmdargs.windowSizeCounter == 1);

    cmdargs = (struct cmdargs){0};
    assert(cmdargs_parse(&cmdargs, 5,(char *[5]){"dummy","-17","12","13*x", "20"}) == 0);
    
    cmdargs = (struct cmdargs){0};
    assert(cmdargs_parse(&cmdargs, 4,(char *[4]){"dummy", "--cube-size", "32a", "dummy"}) == 0 );
    assert(cmdargs.cubeSizeCounter == 1);

    cmdargs = (struct cmdargs){0};
    assert(cmdargs_parse(&cmdargs, 4,(char *[4]){"dummy", "--delay", "32a", "dummy"}) == 0 );
    assert(cmdargs.delayCounter == 1);

    cmdargs = (struct cmdargs){0};
    assert(cmdargs_parse(&cmdargs, 4,(char *[4]){"dummy", "--deloay", "32a", "dummy"}) == 0 );
    
    
   
}

static void valid_tests()
{
    struct cmdargs cmdargs = {0};
    assert(cmdargs_parse(&cmdargs, 6,(char *[6]){"dummy","--cube-size","12","--no-music", "--delay", "10"}) == 1);
    
    assert(cmdargs.cubeSize == 12);
    assert(cmdargs.delay == 10);
    assert(cmdargs.cubeSizeCounter == 1);
    assert(cmdargs.noMusic == 1);
    assert(cmdargs.delayCounter == 1);


    cmdargs = (struct cmdargs){0};
    assert(cmdargs_parse(&cmdargs, 4,(char *[4]){"dummy","--window-size","17", "12" }) == 1 );
    assert(cmdargs.x == 17);
    assert(cmdargs.y == 12);
    assert(cmdargs.windowSizeCounter == 1);


}

static void test_testnum(void)
{
    assert(0 == testnum("23"));
    assert(1 == testnum("qdw21"));
}

int main(int argc, char **argv)
{    
    test_testnum();
    test_failed_cmdargs();
    valid_tests();
    return 0;
}
