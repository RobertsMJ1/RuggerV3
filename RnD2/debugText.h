
#ifndef __DEBUGTEXT_H
#define __DEBUGTEXT_H

#include <vector>
#include <string>
using std::vector;
using std::string;

// very important if you are doing file input 
using std::ifstream;

struct DebugTextLine
{
	DebugTextLine(string ss = "", int xx = -1, int yy = -1){s = ss; x = xx; y = yy;}
	string s;
	int x;
	int y;
};

class DebugText
{
public:
	DebugText();
	void addLine(string s, int x = -1, int y = -1);
	int getSize(){return lines.size();}
	vector<DebugTextLine> lines;
	void funClear(){for (int i = 0; i < lines.size(); i++)lines.erase(lines.begin());}

private:

};

#endif