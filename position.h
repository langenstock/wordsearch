#pragma once

#include <iostream>

class Position
{
public:
	Position();
	Position(int posX, int posY, char letter);
	char GetLetter() const {
		return m_Letter;
	}

	bool GetIsPartOfWord() const {
		return m_IsPartOfWord;
	}

	void SetLetterAsPartOfWord(char letter);
	void SetFillerLetter(char letter);

private:
	int m_PosX;
	int m_PosY;
	char m_Letter;
	bool m_IsPartOfWord{ false };
};

