#include "position.h"

Position::Position()
{
}

Position::Position(int posX, int posY, char letter)
{
	m_PosX = posX;
	m_PosY = posY;
	m_Letter = letter;
}

void Position::SetLetterAsPartOfWord(char letter)
{
	m_Letter = letter;
	m_IsPartOfWord = true;
}

void Position::SetFillerLetter(char letter)
{
	m_Letter = letter;
	m_IsPartOfWord = false;
}
