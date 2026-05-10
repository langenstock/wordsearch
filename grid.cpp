#include "grid.h"

Grid::Grid()
{
}

Grid::Grid(int width, int height)
{
	{
		std::mt19937 mt(std::time(nullptr));
		m_Mt = mt;
	}
	std::uniform_real_distribution<> dis(0.0, 26.99);

	m_SizeWidth = width;
	m_SizeHeight = height;

	for (int i = 0; i < m_SizeWidth; i++) {
		listOfRows.push_back(i);
	}
	for (int i = 0; i < m_SizeHeight; i++) {
		listOfColumns.push_back(i);
	}

	for (int j = 0; j < height; j++) {
		std::vector<Position> row;
		for (int i = 0; i < width; i++) {
			float rand = dis(m_Mt);
			int rInt = static_cast<int>(rand);

			Position pos{ i, j, 'a'};
			row.push_back(pos);
		}

		m_Grid.push_back(row);
	}
}

EResult Grid::TryInsertWord(const char* word)
{
	int sizeOfWord = std::strlen(word);
	std::vector<PossibleWordPosition> possibilities;
	
	// Forwards / Backwards
	for (int rowToTry = 0; rowToTry < m_SizeHeight; rowToTry++) {

		for (int col = 0; col < m_SizeWidth; col++) {
			bool isPossibility = true;
			bool forwardsOnly = false;
			bool backwardsOnly = false;
			for (int j = 0; j < sizeOfWord; j++) {
				if (GetAreCoordinatesValid(rowToTry, col + j)) {
					char thisLetter = word[j];
					if (GetPositionIsPartOfExistingWord(rowToTry, col + j)) {
						char backwardsThisLetter = word[sizeOfWord - 1 - j];

						if (GetPositionIsUsedByIsMatchingLetter(rowToTry, col + j, thisLetter)) {
							forwardsOnly = true;
						}
						else if (GetPositionIsUsedByIsMatchingLetter(rowToTry, col + j, backwardsThisLetter)) {
							backwardsOnly = true;
						}
						else {
							isPossibility = false;
						}
					}
				}
				else {
					isPossibility = false;
				}
			}
			if (isPossibility) {
				if (forwardsOnly && backwardsOnly) {
					continue;
				}
				if (!backwardsOnly) {
					PossibleWordPosition p{ rowToTry, col, EDirection::Forwards };
					possibilities.push_back(p);
				}
				if (!forwardsOnly) {
					PossibleWordPosition pb{ rowToTry, col, EDirection::Backwards };
					possibilities.push_back(pb);
				}
			}
		}
	}

	// Up / Down
	for (int col = 0; col < m_SizeWidth; col++) {
		for (int row = 0; row < m_SizeHeight; row++) {
			bool isPossibility = true;
			bool upwardsOnly = false;
			bool downwardsOnly = false;
			for (int j = 0; j < sizeOfWord; j++) {
				if (GetAreCoordinatesValid(row+j, col)) {
					char thisLetter = word[j];
					if (GetPositionIsPartOfExistingWord(row+j, col)) {
						char backwardsThisLetter = word[sizeOfWord - 1 - j];

						if (GetPositionIsUsedByIsMatchingLetter(row+j, col, thisLetter)) {
							upwardsOnly = true;
						}
						else if (GetPositionIsUsedByIsMatchingLetter(row+j, col, backwardsThisLetter)) {
							downwardsOnly = true;
						}
						else {
							isPossibility = false;
						}
					}
				}
				else {
					isPossibility = false;
				}

			}
			if (isPossibility) {
				if (upwardsOnly && downwardsOnly) {
					continue;
				}
				if (!downwardsOnly) {
					PossibleWordPosition p1{ row, col, EDirection::Up };
					possibilities.push_back(p1);
				}
				if (!upwardsOnly) {
					PossibleWordPosition p2{ row, col, EDirection::Down };
					possibilities.push_back(p2);
				}
			}
		}
	}

	// Diagonal DownRight / UpLeft
	for (int row = 0; row < m_SizeWidth; row++) {
		for (int col = 0; col < m_SizeHeight; col++) {
			bool isPossibility = true;
			for (int j = 0; j < sizeOfWord; j++) {
				if (GetAreCoordinatesValid(row + j, col + j)) {
					if (GetPositionIsPartOfExistingWord(row + j, col + j)) {
						isPossibility = false;
					}
				}
				else {
					isPossibility = false;
				}
			}
			if (isPossibility) {
				PossibleWordPosition p1{ row, col, EDirection::DiagonalDownRight };
				possibilities.push_back(p1);
				PossibleWordPosition p2{ row, col, EDirection::DiagonalUpLeft };
				possibilities.push_back(p2);
			}
		}
	}
	
	// Diagonal DownLeft / UpRight
	// the "front" version of this is DownLeft
	for (int row = 0; row < m_SizeWidth; row++) {
		for (int col = 0; col < m_SizeHeight; col++) {
			bool isPossibility = true;
			for (int j = 0; j < sizeOfWord; j++) {
				if (GetAreCoordinatesValid(row - j, col + j)) {
					if (GetPositionIsPartOfExistingWord(row - j, col + j)) {
						isPossibility = false;
					}
				}
				else {
					isPossibility = false;
					break;
				}
			}
			if (isPossibility) {
				PossibleWordPosition p1{ row, col, EDirection::DiagonalDownLeft };
				possibilities.push_back(p1);
				PossibleWordPosition p2{ row, col, EDirection::DiagonalUpRight };
				possibilities.push_back(p2);
			}
		}
	}
	
	if (possibilities.size() > 0) {
		int idxOfChosen = RIntInRange(0, possibilities.size() - 1);
		auto& p = possibilities[idxOfChosen];
		InsertWord(word, p.row, p.column, p.dir);
		return EResult::Success;
	}
	return EResult::Fail;
}

void Grid::FillInBlankSpots()
{
	for (int row = 0; row < m_SizeWidth; row++) {
		for (int col = 0; col < m_SizeHeight; col++) {
			if (!m_Grid[row][col].GetIsPartOfWord()) {
				int randIdx = RIntInRange(0, 25);
				char randomLetter = m_LetterPool[randIdx];
				m_Grid[row][col].SetFillerLetter(randomLetter);
			}
		}
	}
}

void Grid::InsertWord(const char* word, int row, int column, EDirection direction)
{
	int wordLength = std::strlen(word);

	switch (direction) 
	{
	case(EDirection::Forwards):
		for (int i = 0; i < wordLength; i++) {
			int columnToInsertTo = column + i;
			m_Grid[row][columnToInsertTo].SetLetterAsPartOfWord(word[i]);
		}
		break;
	case(EDirection::Backwards):
		for (int i = 0; i < wordLength; i++) {
			int columnToInsertTo = column + i;
			int letterIdx = wordLength - 1 - i;
			m_Grid[row][columnToInsertTo].SetLetterAsPartOfWord(word[letterIdx]);
		}
		break;
	case(EDirection::Down):
		for (int i = 0; i < wordLength; i++) {
			int rowToInsertTo = row + i;
			m_Grid[rowToInsertTo][column].SetLetterAsPartOfWord(word[i]);
		}
		break;
	case(EDirection::Up):
		for (int i = 0; i < wordLength; i++) {
			int rowToInsertTo = row + i;
			int letterIdx = wordLength - 1 - i;
			m_Grid[rowToInsertTo][column].SetLetterAsPartOfWord(word[letterIdx]);
		}
		break;
	case(EDirection::DiagonalDownRight):
		for (int i = 0; i < wordLength; i++) {
			m_Grid[row + i][column + i].SetLetterAsPartOfWord(word[i]);
		}
		break;
	case(EDirection::DiagonalUpLeft):
		for (int i = 0; i < wordLength; i++) {
			int letterIdx = wordLength - 1 - i;
			m_Grid[row + i][column + i].SetLetterAsPartOfWord(word[letterIdx]);
		}
		break;
	case(EDirection::DiagonalUpRight):
		for (int i = 0; i < wordLength; i++) {
			int letterIdx = wordLength - 1 - i;
			m_Grid[row - i][column + i].SetLetterAsPartOfWord(word[letterIdx]);
		}
		break;
	case(EDirection::DiagonalDownLeft):
		for (int i = 0; i < wordLength; i++) {
			m_Grid[row - i][column + i].SetLetterAsPartOfWord(word[i]);
		}
		break;
	default:
		break;
	}
}

void Grid::DebugConsolePrint(bool includeSpaces)
{
	std::cout << std::endl;

	for (const std::vector<Position> p : m_Grid) {
		for (const Position pos : p) {
			std::cout << pos.GetLetter();
			if (includeSpaces) {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

char Grid::GetLetterFromCoordinates(int row, int column)
{
	Position& p = m_Grid[row][column];
	return p.GetLetter();
}

bool Grid::GetPositionIsPartOfExistingWord(int row, int column)
{
	Position& p = m_Grid[row][column];
	return p.GetIsPartOfWord();
}

bool Grid::GetPositionIsUsedByIsMatchingLetter(int row, int column, char proposedLetter)
{
	// this function assumes we already know that it is used
	char letterAlreadyThere = m_Grid[row][column].GetLetter();
	return (letterAlreadyThere == proposedLetter);
}

bool Grid::GetAreCoordinatesValid(int row, int column)
{
	if (row >= m_SizeWidth)
		return false;
	if (column >= m_SizeHeight)
		return false;
	if (row < 0 || column < 0)
		return false;

	return true;
}

bool Grid::CoordinatesHaveSpaceForWord(int row, int column, int wordSize, EDirection direction)
{
	switch (direction)
	{
	case(EDirection::Forwards):
		for (int i = column; i < column + wordSize; i++) {
			
		}
		break;
	default:
		return false; // TODO DL
	}
}

int Grid::RIntInRange(int min, int max)
{
	std::uniform_real_distribution<> dis(min, max);
	return static_cast<int>(dis(m_Mt));
}

float Grid::RFloatInRange(float min, float max)
{
	std::uniform_real_distribution<> dis(min, max);
	return dis(m_Mt);
}

float Grid::RFloat()
{
	std::uniform_real_distribution<> dis(0.f, 1.f);
	return dis(m_Mt);
}

bool Grid::RBool()
{
	std::uniform_real_distribution<> dis(0.f, 1.f);
	return (dis(m_Mt) < 0.5f);
}
