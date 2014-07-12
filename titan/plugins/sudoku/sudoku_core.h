#ifndef _SUDOKU_CORE_H_
#define _SUDOKU_CORE_H_

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long ULONG;
typedef unsigned long BITV;
typedef unsigned long COOR;
#define RANDOM_COOR (rand() % 81)
typedef unsigned long BOOL;

#if 0
#define ASSERT(x)                  if (x)    \
                                   { ; }     \
                                   else      \
                                   { _assert(__FILE__, __LINE__); }
#define VERIFY(x)                  ASSERT(x)
#else
#define ASSERT(x)                      ;
#define VERIFY(x)                  x;
#endif // DEBUG

#ifdef PERF_COUNTERS
#define INC(x)                     ((x) += 1)
#else
#define INC(x)
#endif

typedef struct _SQUARE
{
    BITV bvPossibilities;
    ULONG uValue;
    ULONG cCol;
    ULONG cRow;
    ULONG cGroup;
} SQUARE;

//
//  0  1  2    3  4  5    6  7  8
//  9 10 11   12 13 14   15 16 17
// 18 19 20   21 22 23   24 25 26
// 
// 27 28 29   30 31 32   33 34 35
// 36 37 38   39 40 41   42 43 44
// 45 46 47   48 49 50   51 52 53
// 
// 54 55 56   57 58 59   60 61 62
// 63 64 65   66 67 68   69 70 71
// 72 73 74   75 76 77   78 79 80
// 
COOR g_cGroup[81] = { 
    0, 0, 0, 1, 1, 1, 2, 2, 2, 
    0, 0, 0, 1, 1, 1, 2, 2, 2,
    0, 0, 0, 1, 1, 1, 2, 2, 2,
    3, 3, 3, 4, 4, 4, 5, 5, 5, 
    3, 3, 3, 4, 4, 4, 5, 5, 5, 
    3, 3, 3, 4, 4, 4, 5, 5, 5, 
    6, 6, 6, 7, 7, 7, 8, 8, 8,
    6, 6, 6, 7, 7, 7, 8, 8, 8,
    6, 6, 6, 7, 7, 7, 8, 8, 8 
};

COOR g_cSquareByGroup[9][9] =
{
    {  0,  1,  2,  9, 10, 11, 18, 19, 20 },
    {  3,  4,  5, 12, 13, 14, 21, 22, 23 },
    {  6,  7,  8, 15, 16, 17, 24, 25, 26 },
    { 27, 28, 29, 36, 37, 38, 45, 46, 47 },
    { 30, 31, 32, 39, 40, 41, 48, 49, 50 },
    { 33, 34, 35, 42, 43, 44, 51, 52, 53 },
    { 54, 55, 56, 63, 64, 65, 72, 73, 74 },
    { 57, 58, 59, 66, 67, 68, 75, 76, 77 },
    { 60, 61, 62, 69, 70, 71, 78, 79, 80 }
};

#define IS_EMPTY(x) ((x) == 0)
#define COL(c) ((c) % 9)
#define FIRST_OF_COL(c) (c)
#define ROW(c) ((c) / 9)
#define FIRST_OF_ROW(c) ((c) * 9)
#define FOREACH_ROW(c, r) \
    for((c) = FIRST_OF_ROW(r); (c) < FIRST_OF_ROW((r) + 1); (c)++)
#define FOREACH_COL(c, f) \
    for((c) = FIRST_OF_COL(f); (c) < 81; (c) += 9)
#define FOREACH_GROUP(c, g, x) \
    for((x) = 0, (c) = g_cSquareByGroup[(g)][(x)]; \
        (x) < 9; \
        (x)++, (c) = g_cSquareByGroup[(g)][(x)])
#define FOREACH_SQUARE(x) \
    for((x) = 0; (x) < 81; (x)++)
COOR g_cGroupsByRow[9][3] =
{
    { 0, 1, 2 },
    { 0, 1, 2 },
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 3, 4, 5 },
    { 3, 4, 5 },
    { 6, 7, 8 },
    { 6, 7, 8 },
    { 6, 7, 8 } 
};    

COOR g_cRowsByGroup[9][3] =
{
    { 0, 1, 2 },
    { 0, 1, 2 },
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 3, 4, 5 },
    { 3, 4, 5 },
    { 6, 7, 8 },
    { 6, 7, 8 },
    { 6, 7, 8 } 
};    

COOR g_cGroupsByCol[9][3] =
{
    { 0, 3, 6 },
    { 0, 3, 6 }, 
    { 0, 3, 6 },
    { 1, 4, 7 },
    { 1, 4, 7 },
    { 1, 4, 7 },
    { 2, 5, 8 },
    { 2, 5, 8 },
    { 2, 5, 8 }
};

COOR g_cColsByGroup[9][3] =
{
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 },
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 },
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 }
};

typedef struct _POSITION
{
    SQUARE rgSquare[81];
    BITV bvRemainingByGroup[9];
    BITV bvRemainingByCol[9];
    BITV bvRemainingByRow[9];
    ULONG uEmpty;
} POSITION;

#define ONE_BIT(x) (((x) & (x - 1)) == 0)

#define TRUE (1)
#define FALSE (0)

#endif /* _SUDOKU_CORE_H_ */
