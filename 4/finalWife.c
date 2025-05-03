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
#define MOVE_SIZE 7
const char CHESS_TOKEN[] = "CHESS";

char* createBlackToken(char* name)
{
	char* tokenHolder = (char*)(malloc(sizeof(char) * (TOKEN_SIZE + 1)));
	if (tokenHolder == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	if (name != NULL)
	{
		srand((unsigned int)time(0));
		for (int i = 0; i < TOKEN_SIZE; ++i)
		{
			tokenHolder[i] = rand() % 255;
		}
		tokenHolder[TOKEN_SIZE] = '\0';
	}
	else
	{
		free(tokenHolder);
		return NULL;
	}

	return tokenHolder;
}

char* createWhiteToken(char* previousToken)
{
	char* currentToken = (char*)(malloc(sizeof(char) * (TOKEN_SIZE + 1)));
	if (currentToken == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	if (previousToken != NULL)
	{
		for (int i = 0; i < TOKEN_SIZE; ++i)
		{
			if (i < strlen(CHESS_TOKEN))
				currentToken[i] = CHESS_TOKEN[i];
			else
				currentToken[i] = previousToken[i] + 1;
		}
		currentToken[TOKEN_SIZE] = '\0';
	}
	else
	{
		free(currentToken);
		return NULL;
	}

	return currentToken;
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

	if (scanf("%d", &choice) != 1)
	{
		fprintf(stderr, "Invalid input\n");
		exit(EXIT_FAILURE);
	}

	char* moveToken = (char*)malloc(sizeof(char) * MOVE_SIZE);
	if (moveToken == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	switch (choice)
	{
	case(0):
		strncpy(moveToken, KING, MOVE_SIZE - 1);
		break;
	case(1):
		strncpy(moveToken, QUEEN, MOVE_SIZE - 1);
		break;
	case(2):
		strncpy(moveToken, ROOK, MOVE_SIZE - 1);
		break;
	case(3):
		strncpy(moveToken, KNIGHT, MOVE_SIZE - 1);
		break;
	case(4):
		strncpy(moveToken, PAWN, MOVE_SIZE - 1);
		break;
	default:
		fprintf(stderr, "Invalid move\n");
		free(moveToken);
		return NULL;
	}

	moveToken[MOVE_SIZE - 1] = '\0'; // Ensure null termination
	return moveToken;
}

int main(int argc, char* argv[])
{
	char* token = createBlackToken(argv[1]);
	if (token != NULL)
	{
		char* tempToken = (char*)malloc(sizeof(char) * (TOKEN_SIZE + 1));
		if (tempToken == NULL)
		{
			fprintf(stderr, "Failed to dup token\n");
			free(token);
			return EXIT_FAILURE;
		}
		memcpy(tempToken, token, TOKEN_SIZE + 1);
		printf("Token: %s\n", token);
		free(token);
		token = createWhiteToken(tempToken);
		free(tempToken);
	}
	else
	{
		printf("Token was empty!\n");
	}

	if (token != NULL)
	{
		char* move = initFirstMove(token);
		if (move != NULL)
		{
			printf("White's move: %s\n", move);
			free(move);
		}
		free(token);
	}

	return EXIT_SUCCESS;
}
