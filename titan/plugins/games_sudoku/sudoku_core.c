#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//#include <unistd.h>

#include "sudoku_core.h"

ULONG g_uNodes = 0;
ULONG g_uGuesses = 0;
ULONG g_uSolutions = 0;
POSITION g_Solution;

BOOL
PositionIsLegal(POSITION *, BOOL);


char * ShowPossibilities(BITV bv) {
	static char bits[] = "*123456789";
	static char sz[10];
	ULONG u;
	ULONG v;
	
	memset(&sz, 0, sizeof(sz));
	v = 0;
	for (u = 1; u < 10; u++) {
		if (bv & (1 << (u - 1)))
		{
			sz[v++] = bits[u];
		}
	}
	return(sz);
}

void DumpBoard(POSITION *pos) {
	COOR c;
	
	FOREACH_SQUARE(c)
	{
		if ((c % 9) == 0) printf("\n");
		if ((c == 27) || (c == 54)) printf("-----------------------------------------------------------------------------\n");
		if (((c % 3) == 0) && ((c % 9) != 0)) printf(" | ");
		if (pos->rgSquare[c].uValue)
		{
			printf("[%u]	 ", (unsigned)pos->rgSquare[c].uValue);
		}
		else
		{
			printf("%-7s ", 
				   ShowPossibilities(pos->rgSquare[c].bvPossibilities));
		}
	}
	printf("\n");
}

void DumpBoardHtml(POSITION *pos) {
	COOR c;

	FOREACH_SQUARE(c)
	{
		if (pos->rgSquare[c].uValue)
		{
			printf("%u:%u\n", (unsigned)c, 
				   (unsigned)pos->rgSquare[c].uValue);
		}
	}
	printf("\n");
	printf("%u empty squares...\n", (unsigned)pos->uEmpty);
}

void DumpBoardSimple(POSITION *pos) {
	COOR c;

	FOREACH_SQUARE(c) 
	{
		if (pos->rgSquare[c].uValue) {
			printf("%u", (unsigned)pos->rgSquare[c].uValue);
		} else {
			printf("-");
		}
	}
	printf("\n");
}

BOOL LiftValue(POSITION *pos, COOR sq) {
	ULONG uValue = pos->rgSquare[sq].uValue;
	BITV bvMask = (1UL << (uValue - 1));
	COOR c;
	COOR cCol, cRow;
	ULONG x;
	
	// Make sure it's full
	if (IS_EMPTY(uValue)) {
		return(FALSE);
	}
	
	// uValue is a possibility on the file/rank/group again.
	cCol = COL(sq);
	ASSERT(cCol == pos->rgSquare[sq].cCol);
	FOREACH_COL(c, cCol) {
		pos->rgSquare[c].bvPossibilities |= bvMask;
	}
	
	cRow = ROW(sq);
	ASSERT(cRow == pos->rgSquare[sq].cRow);
	FOREACH_ROW(c, cRow) {
		pos->rgSquare[c].bvPossibilities |= bvMask;
	}
	
	FOREACH_GROUP(c, pos->rgSquare[sq].cGroup, x) {
		pos->rgSquare[c].bvPossibilities |= bvMask;
	}

	pos->uEmpty += 1;
	pos->rgSquare[sq].uValue = 0;
	pos->rgSquare[sq].bvPossibilities |= bvMask;
	pos->bvRemainingByGroup[pos->rgSquare[sq].cGroup] |= bvMask;
	pos->bvRemainingByCol[pos->rgSquare[sq].cCol] |= bvMask;
	pos->bvRemainingByRow[pos->rgSquare[sq].cRow] |= bvMask;

	return(TRUE);
}

BOOL PlaceValue(POSITION *pos, COOR sq, ULONG uValue, BOOL fReduce) {
	BITV bvMask = ~(1UL << (uValue - 1));
	COOR c;
	COOR cCol, cRow;
	ULONG x;

	// Make sure it's empty
	if (!IS_EMPTY(pos->rgSquare[sq].uValue)) {
		if (pos->rgSquare[sq].uValue == uValue) {
			return(TRUE);
		}
		else {
			return(FALSE);
		}
	}
	
	// There can only be one uValue per file/rank/group
	cCol = COL(sq);
	ASSERT(cCol == pos->rgSquare[sq].cCol);
	FOREACH_COL(c, cCol) {
		pos->rgSquare[c].bvPossibilities &= bvMask;
	}
	
	cRow = ROW(sq);
	ASSERT(cRow == pos->rgSquare[sq].cRow);
	FOREACH_ROW(c, cRow) {
		pos->rgSquare[c].bvPossibilities &= bvMask;
	}
	
	FOREACH_GROUP(c, pos->rgSquare[sq].cGroup, x) {
		pos->rgSquare[c].bvPossibilities &= bvMask;
	}

	ASSERT(pos->uEmpty > 0);
	pos->uEmpty -= 1;
	ASSERT(pos->uEmpty < 81);
	pos->rgSquare[sq].uValue = uValue;
	pos->rgSquare[sq].bvPossibilities = (1 << (uValue - 1));
	pos->bvRemainingByGroup[pos->rgSquare[sq].cGroup] &= bvMask;
	pos->bvRemainingByCol[pos->rgSquare[sq].cCol] &= bvMask;
	pos->bvRemainingByRow[pos->rgSquare[sq].cRow] &= bvMask;
	
	return(PositionIsLegal(pos, fReduce));
}

ULONG BitNumber(BITV x) {
	ULONG u = 0;
	do {
		x >>= 1;
		u++;
	}
	while(x > 0);
	return(u);
}

ULONG BitCount(BITV x) {
	ULONG uCount = 0;

	ASSERT(!(x & 0xFFFFFE00));
	while(x) {
		uCount += (x & 1);
		x >>= 1;
	}
	return(uCount);
}

ULONG LegalValue(POSITION *pos, COOR c) {
	ULONG x, y;
	if (!IS_EMPTY(pos->rgSquare[c].uValue)) {
		return(pos->rgSquare[c].uValue);
	}

	x = pos->rgSquare[c].bvPossibilities;
	if ((x & 0x1FF) == 0) return(0);
	do {
		y = 1 << (rand() % 9); 
		if (y & x)
		{
			return(BitNumber(y));
		}
	}
	while(1);
}

BOOL PositionIsLegal(POSITION *pos, BOOL fReduce) {
	COOR c, sq, cOnly;
	ULONG x, y, uCount;
	BITV bvMaskFull, bvMaskPoss;

	g_uNodes += 1;
	
	FOREACH_SQUARE(c) {
		if (IS_EMPTY(pos->rgSquare[c].uValue)) {
			x = pos->rgSquare[c].bvPossibilities;
			if (x == 0) {
				return(FALSE);
			}
		}
	}
	
	// Note: I don't combine the two loops here because this way we
	// detect invalid positions (empty squares w/ no legal moves in
	// them) earlier instead of recursing on obviously bad positions.
	FOREACH_SQUARE(c) {
		if (IS_EMPTY(pos->rgSquare[c].uValue)) {
			x = pos->rgSquare[c].bvPossibilities;
			if (ONE_BIT(x)) {
				if ((TRUE == fReduce) &&
					(FALSE == PlaceValue(pos, c, BitNumber(x), fReduce)))
				{
					return(FALSE);
				}
			}
		}
	}

	// If there's only one square in a row|col|group that can be
	// a given number, make the move.
	for (x = 1; x <= 256; x <<= 1) {
		for (c = 0; c < 9; c++) {
			uCount = 0;
			FOREACH_COL(sq, c) {
				if (IS_EMPTY(pos->rgSquare[sq].uValue) &&
					(pos->rgSquare[sq].bvPossibilities & x))
				{
					uCount++;
					cOnly = sq;
					if (uCount > 1) break;
				}
			}
			if (uCount == 1) {
				if ((TRUE == fReduce) && 
					(FALSE == PlaceValue(pos, cOnly, BitNumber(x), fReduce)))
				{
					return(FALSE);
				}
			}
				
			uCount = 0;
			FOREACH_ROW(sq, c) {
				if (IS_EMPTY(pos->rgSquare[sq].uValue) &&
					(pos->rgSquare[sq].bvPossibilities & x))
				{
					uCount++;
					cOnly = sq;
					if (uCount > 1) break;
				}
			}
			if (uCount == 1) {
				if ((TRUE == fReduce) && 
					(FALSE == PlaceValue(pos, cOnly, BitNumber(x), fReduce)))
				{
					return(FALSE);
				}
			}

			uCount = 0;
			FOREACH_GROUP(sq, c, y) {
				if (IS_EMPTY(pos->rgSquare[sq].uValue) &&
					(pos->rgSquare[sq].bvPossibilities & x))
				{
					uCount++;
					cOnly = sq;
					if (uCount > 1) break;
				}
			}
			if (uCount == 1)
			{
				if ((TRUE == fReduce) && 
					(FALSE == PlaceValue(pos, cOnly, BitNumber(x), fReduce)))
				{
					return(FALSE);
				}
			}
		}
	}
	
	for (c = 0; c < 9; c++) {
		bvMaskPoss = bvMaskFull = 0;
		FOREACH_ROW(sq, c) {
			bvMaskPoss |= pos->rgSquare[sq].bvPossibilities;
			if (!IS_EMPTY(pos->rgSquare[sq].uValue)) {
				ASSERT(pos->rgSquare[sq].bvPossibilities == 
					   (1 << pos->rgSquare[sq].uValue));
				if (bvMaskFull & pos->rgSquare[sq].bvPossibilities) {
					return(FALSE);
				}
				bvMaskFull |= pos->rgSquare[sq].bvPossibilities;
			}
		}
		if (bvMaskPoss != 0x1FF) {
			return(FALSE);
		}
		
		bvMaskPoss = bvMaskFull = 0;
		FOREACH_COL(sq, c) {
			bvMaskPoss |= pos->rgSquare[sq].bvPossibilities;
			if (!IS_EMPTY(pos->rgSquare[sq].uValue))
			{
				ASSERT(pos->rgSquare[sq].bvPossibilities == 
					   (1 << pos->rgSquare[sq].uValue));
				if (bvMaskFull & pos->rgSquare[sq].bvPossibilities) {
					return(FALSE);
				}
				bvMaskFull |= pos->rgSquare[sq].bvPossibilities;
			}
		}
		if (bvMaskPoss != 0x1FF) {
			return(FALSE);
		}

		bvMaskPoss = bvMaskFull = 0;
		FOREACH_GROUP(sq, c, y) {
			bvMaskPoss |= pos->rgSquare[sq].bvPossibilities;
			if (!IS_EMPTY(pos->rgSquare[sq].uValue)) {
				ASSERT(pos->rgSquare[sq].bvPossibilities == 
					   (1 << pos->rgSquare[sq].uValue));
				if (bvMaskFull & pos->rgSquare[sq].bvPossibilities) {
					return(FALSE);
				}
				bvMaskFull |= pos->rgSquare[sq].bvPossibilities;
			}
		}
		if (bvMaskPoss != 0x1FF) {
			return(FALSE);
		}
	}
	return(TRUE);
}


void InitializePosition(POSITION *pos) {
	COOR c;
	memset(pos, 0, sizeof(POSITION));
	pos->uEmpty = 81;
	FOREACH_SQUARE(c) {
		pos->rgSquare[c].bvPossibilities = 0x1FF;
		pos->rgSquare[c].cGroup = g_cGroup[c];
		pos->rgSquare[c].cRow = ROW(c);
		pos->rgSquare[c].cCol = COL(c);
	}
	
	for (c = 0; c < 9; c++) {
		pos->bvRemainingByRow[c] = 0x1FF;
		pos->bvRemainingByCol[c] = 0x1FF;
		pos->bvRemainingByGroup[c] = 0x1FF;
	}
}


typedef struct _FEWEST {
	COOR c;
	ULONG uBitCount;
	BITV bvPossibilities;
} FEWEST;


void Sort(FEWEST *p, ULONG uLen) {
	ULONG u, v;
	ULONG uMinPos;
	FEWEST temp;
	
	for (u = 0; u < uLen; u++) {
		uMinPos = u;
		for (v = u + 1; v < uLen; v++) {
			if (p[v].uBitCount < p[uMinPos].uBitCount) {
				uMinPos = v;
			}
		}
		
		temp = p[u];
		p[u] = p[uMinPos];
		p[uMinPos] = temp;
	}
}

BOOL Solve(POSITION *pos, ULONG uDepth) {
	COOR c;
	ULONG u, v;
	POSITION p;
	FEWEST sFewest[81];
	BOOL fRet = FALSE;

	g_uNodes++;

	// Find the square with the fewest possibilities
	FOREACH_SQUARE(c) {
		sFewest[c].c = c;
		sFewest[c].bvPossibilities = pos->rgSquare[c].bvPossibilities;
		sFewest[c].uBitCount = BitCount(pos->rgSquare[c].bvPossibilities);
		if ((pos->rgSquare[c].uValue == 0) && (sFewest[c].uBitCount == 0)) {
			ASSERT(!sFewest[c].bvPossibilities);
			goto end;
		}
	}
	Sort(sFewest, 81);

	// Make the guess
	FOREACH_SQUARE(v) {
		if (IS_EMPTY(pos->rgSquare[sFewest[v].c].uValue)) {
			ASSERT(sFewest[v].uBitCount > 0);
			for (u = 1; u <= 9; u++) {
				// Only make guesses that are legal.  Don't "guess"
				// the same thing as a prior solution. 
				if ((sFewest[v].bvPossibilities & (1 << (u - 1))) &&
					((g_uSolutions == 0) ||
					 (g_Solution.rgSquare[sFewest[v].c].uValue != u))) {
					memcpy(&p, pos, sizeof(p));
					g_uGuesses += 1;
					if ((FALSE == PlaceValue(pos, sFewest[v].c, u, TRUE)) ||
						(FALSE == Solve(pos, uDepth + 1)))
					{
						// Unmake the guess move.
						memcpy(pos, &p, sizeof(p));

						// We've learned that the guess is wrong, mask it
						// off the possibilities bitv.
						pos->rgSquare[sFewest[v].c].bvPossibilities &=
							~(1 << (u - 1));

						// Are we in a screwed up state now after masking
						// off that bit?  If so something above us in the
						// callstack is screwed up.
						if (pos->rgSquare[sFewest[v].c].bvPossibilities == 0)
						{
							goto end;
						}
					}
					else
					{
						// This move leads to a win.  Count it as a
						// solution and carry on.
						fRet = TRUE;
						if (g_uSolutions == 1) 
						{
							memcpy(pos, &p, sizeof(p));
						} else {
							goto end;
						}
					}
				}
			}
		}
	}

	if (0 == pos->uEmpty) 
	{
		fRet = TRUE;
		g_uSolutions += 1;
		if (g_uSolutions == 1) {
			memcpy(&g_Solution, pos, sizeof(POSITION));
		}
		goto end;
	}
	
 end:
	return fRet;
}

void GeneratePuzzle(BOOL fHard, POSITION *pos) {
	COOR c, x;
	ULONG v;
	POSITION p;
	ULONG u;
	ULONG uTries;
	COOR cSeed[] = { 
		1, 7, 12, 14, 17, 27, 31, 35, 37,40, 43, 
		45, 49, 53, 63, 66, 68, 71, 73, 79, 0 };

	srand(time(0));
	u = 0;
	do {
		// Clear the board
		InitializePosition(pos);

		if (!fHard) {
			// Place a number, try to solve it, repeat...
			while(pos->uEmpty > 25) {
				c = RANDOM_COOR;
				if (IS_EMPTY(pos->rgSquare[c].uValue)) {
					v = LegalValue(pos, c);
					if (0 == v) break;

					// See if the rest can be solved by deduction...
					memcpy(&p, pos, sizeof(p));
					if ((TRUE == PlaceValue(pos, c, v, TRUE)) &&
						(pos->uEmpty == 0))
					{
						memcpy(pos, &p, sizeof(p));
						pos->rgSquare[c].uValue = v;
						return;
					}
				}
			}
		}
		else
		{
			// Throw some random numbers out there on the board...
			u = 0;
			while(cSeed[u] != 0) {
				if (IS_EMPTY(pos->rgSquare[cSeed[u]].uValue))
				{
					v = LegalValue(pos, cSeed[u]);
					if (0 == v) break;

					(void)PlaceValue(pos, cSeed[u], v, TRUE);
				}
				u++;
			}
			
			// Solve the puzzle within these constraints...
			g_uSolutions = g_uGuesses = g_uNodes = 0;
			if (TRUE == Solve(pos, 0)) {
				// Now start taking away values until you get a puzzle
				// that is still uniquely solvable but requires
				// guessing
				uTries = 0;
				while(1) {
					c = RANDOM_COOR;				
					if (!IS_EMPTY(pos->rgSquare[c].uValue)) {
						InitializePosition(&p);
						FOREACH_SQUARE(x) {
							if ((x != c) && 
								(!IS_EMPTY(pos->rgSquare[x].uValue))) {
								PlaceValue(&p, x, 
										   pos->rgSquare[x].uValue, FALSE);
							}
						}
						ASSERT(p.uEmpty - 1 == pos->uEmpty);
						g_uSolutions = g_uGuesses = g_uNodes = 0;
						if ((TRUE == Solve(&p, 0)) && (g_uSolutions == 1))
						{
							uTries = 0;
							memcpy(pos, &p, sizeof(p));
							if ((g_uGuesses > 5) && 
								(g_uNodes > 200) &&
								(pos->uEmpty > 55)) {
								//printf("%u guesses, %u nodes.\n", 
								//		g_uGuesses, g_uNodes);
								return;
							}
						}
						else
						{
							uTries++;
							if (uTries > 50) {
								usleep(20);
								break;
							}
						}
					}
				}
			}
		}
	}
	while(1);
}

int main(int argc, char *argv[]) {
	POSITION pos;
	char buf[256];
	char *p;
	COOR c;
	
	InitializePosition(&pos);
	
	memset(buf, 0, sizeof(buf));

	if (argc == 1) {
		//printf("Enter board: ");
		scanf("%255s", buf);
	}
	else {
		if (!strcmp(argv[1], "--create-easy")) {
			GeneratePuzzle(FALSE, &pos);
			//DumpBoardHtml(&pos);
			DumpBoardSimple(&pos);
			exit(0);
		}
		else if (!strcmp(argv[1], "--create-hard")) {
			GeneratePuzzle(TRUE, &pos);
			//DumpBoardHtml(&pos);
			DumpBoardSimple(&pos);
			exit(0);
		}
		else {
			strncpy(buf, argv[1], 255);
		}
	}

	// Solve...
	p = buf;
	c = 0;
	while(*p) {
		if (!isspace(*p)) {
			if (isdigit(*p) && (*p != '0')) {
				if (FALSE == PlaceValue(&pos, c, *p - '0', TRUE)) {
					printf("%u!%u\n",
						   (unsigned)c, (unsigned)(*p - '0'));
					exit(0);
				}
			}
			c++;
		}
		p++;
	}

	g_uSolutions = g_uGuesses = g_uNodes = 0;
	if (TRUE == Solve(&pos, 0)) {
		memcpy(&pos, &g_Solution, sizeof(pos));
		//DumpBoardHtml(&pos);
		DumpBoardSimple(&pos);
	}
	else {
		//DumpBoardHtml(&pos);
		printf("No solution found...\n");
	}
	exit(0);
}
