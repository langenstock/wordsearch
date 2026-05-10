#pragma once

#include "position.h"
#include <vector>
#include <set>
#include <string>
#include <cstring>
#include <random>

enum class EResult
{
	Success, Fail
};

enum class EDirection
{
	Forwards, Backwards, Up, Down, DiagonalUpLeft, DiagonalUpRight, 
	DiagonalDownLeft, DiagonalDownRight
};

struct PossibleWordPosition
{
	int row;
	int column;
	EDirection dir;
};

class Grid
{
public:
	Grid();
	Grid(int width, int height);
	EResult TryInsertWord(const char* word);
	void FillInBlankSpots();

	void DebugConsolePrint(bool includeSpaces);
private:
	int m_SizeWidth;
	int m_SizeHeight;
	std::vector<int> listOfRows;
	std::vector<int> listOfColumns;
	std::vector<std::vector<Position>> m_Grid;  // rows<columns> i.e. m_Grid[row][column]
	char m_LetterPool[26]{
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
		'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z'
	};
	std::mt19937 m_Mt;

	void InsertWord(const char* word, int row, int column, EDirection direction);
	char GetLetterFromCoordinates(int row, int column);
	bool GetPositionIsPartOfExistingWord(int row, int column);
	bool GetPositionIsUsedByIsMatchingLetter(int row, int column, char proposedLetter);
	
	bool GetAreCoordinatesValid(int row, int column);
	bool CoordinatesHaveSpaceForWord(int row, int column, int wordSize, EDirection direction);

	// Randomness Functions
	int RIntInRange(int min, int max);
	float RFloatInRange(float min, float max);
	float RFloat();
	bool RBool();
};