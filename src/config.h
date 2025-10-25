#ifndef CONFIG_H
#define CONFIG_H

#define PRINTDEBUG 1

// __ARRAYSIZE__ in preprocessor.c
// By default its 30000.
#define ARRAYSIZE 30000

// error printing limit
// does nothing right now.
#define ERRORLIMIT 5

// allow the compiler to use overflow and underflows in the compiled program
// 1 = true, 0 = false
#define OVERFLOW 1

#endif
