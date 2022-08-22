#include <string>
#include <cstring>
#include "Instruction.h"

class Object {
	int level;

public:
	char name[20];
	std::string file;

	//Constructor
	Object(char mName[20], int mLevel);
	int getLevel();

};

// Constructor
Object::Object(char mName[30], int mLevel) {
	this->file = "";
	strcpy(this->name, mName);
	this->level = mLevel;

}

int Object::getLevel() {
	return this->level;
}