// main.c
// Main Thread of BeatBox program
#include <stdio.h>

#include "shutdownManager.h"
#include "periodTimer.h"

// Initialize/cleanup the module's data structures.
static void main_init(void);
static void main_cleanup(void);

int main(void)
{
    main_init();
    Shutdown_wait();
    main_cleanup();
    return 0;
}

static void main_init(void)
{
    Shutdown_init();
}

static void main_cleanup(void)
{
    Shutdown_cleanup();
}