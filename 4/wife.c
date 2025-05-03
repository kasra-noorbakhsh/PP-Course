#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TOKEN_SIZE 10
#define PAWN   "pawn"
#define KNIGHT "knight"
#define ROOK   "rook"
#define QUEEN  "queen"
#define KING   "king"
#define MOVE_SIZE 6
const char CHESS_TOKEN[] = "CHESS";

char* createBlackToken(char* name)
{
	char* tokenHolder = (char*)(malloc(sizeof(char) * TOKEN_SIZE));
	if(name != NULL)
	{
		for(int i = 0; i < TOKEN_SIZE; ++i)
		{
			srand(time(0));
			tokenHolder[i] = rand() % 255;
		}
		return tokenHolder;
	}
	return NULL;
}

char* createWhiteToken(char* previousToken)
{
	char* currentToken = (char*)(malloc(sizeof(char) * TOKEN_SIZE));
	int i = 0;
	if(previousToken != NULL)
	{
		while(1)
		{
			if (i >= TOKEN_SIZE)
				return currentToken;
			else if (i < strlen(CHESS_TOKEN))
				currentToken[i] = CHESS_TOKEN[i];
			else
				currentToken[i] = previousToken[i] + 1;
			i++;
		}
	}
	return NULL;
}

char* initFirstMove(char* whiteToken) 
{
	if (strncmp(whiteToken, CHESS_TOKEN, strlen(CHESS_TOKEN)) != 0)
		return whiteToken;

	int choice;
	printf(
		"0: A King move\n"
		"1: A Queen move\n"
		"2: A Rook move\n"
		"3: A Knight move\n"
		"4: A Pawn move\n"
		"White's turn, enter the first move: ");
	scanf("%d", &choice);
	whiteToken = (char*)(malloc(sizeof(char) * MOVE_SIZE));
	switch (choice)
	{
	case(0):
		strncpy(whiteToken, KING, strlen(KING));
		break;
	case(1):
		strncpy(whiteToken, QUEEN, strlen(QUEEN));
		break;
	case(2):
		strncpy(whiteToken, ROOK, strlen(ROOK));
		break;
	case(3):
		strncpy(whiteToken, KNIGHT, strlen(KNIGHT));
		break;
	case(4):
		strncpy(whiteToken, PAWN, strlen(PAWN));
		break;
	}
	return whiteToken;
}

int main(int argc, char* argv[])
{
	char* token = createBlackToken(argv[1]);
	printf("Token: %s\n", token);
	free(token);
	token = createWhiteToken(token);
	printf("White's move: %s\n", initFirstMove(token));
	free(token);
	return EXIT_SUCCESS;
}