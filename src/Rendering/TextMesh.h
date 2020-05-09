#pragma once

#include <iostream>
#include "Entity.h"

using namespace std;

const static string s_charMap = " abcdefghijklmnopqrstuvwxyz0123456789,._!/?[](){}<>\\+-*~='\":;";

class TextMesh : public Entity
{
public:
	TextMesh();

	void SetText(string text);

	unsigned int GetStringSize() { return m_stringSize; };

	void Create();

	void UpdateTextBuffer(string newText);

private:

	uint32 m_stringSize;
	uint32 m_stringCapacity;
	string m_textString;
};
