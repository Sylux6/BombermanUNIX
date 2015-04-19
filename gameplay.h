#ifndef _GAMEPLAY_INCLUDED
#define _GAMEPLAY_INCLUDED

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>

#include "term.h"
#include "player.h"
#include "mylib.h"

void initGameplay();

char *random_named_pipe();

#endif
