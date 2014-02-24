
#include "debugText.h"

DebugText::DebugText()
{
	
}

void DebugText::addLine(string s, int x, int y)
{
	lines.push_back(DebugTextLine(s, x, y));
}
