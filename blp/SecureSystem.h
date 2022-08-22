#include "ReferenceMonitor.h"
#include <stdlib.h>

ReferenceMonitor *refMon;

// Checks if a string consists only of digits
int hasNumbersOnly(const char *s) {
	while(*s) {
		if(isdigit(*s++) == 0)
			return 0;

	}
	return 1;
}

// Checks if a string is a valid textfile
bool isValidFile(const std::string s) {
	std::string x;
	for(auto i=s.length()-4; i<s.length(); i++)
		x.push_back(s[i]);
	if(x==".txt")
		return true;
	else
		return false;
}

/* 
 * The following function takes a line of instruction
 * as input and creates an Instruction object for the line 
 * if the instruction is valid.
 * The Instruction Object is then passed to 
 */

void parseSub(char *line) {
	int tokenCount = 0;
	char *pch, orgLine[80];
	char tokens[4][30];

	strcpy(orgLine, line); // save a copy of the current line

	//std::cout << "here";
	pch = strtok(line, "\n"); // Remove the trailing new line character added by fgets

	/* Now we use strok to tokenize a line of instruction */
	pch = strtok(line, " "); 
	while(pch != NULL) {
		if(tokenCount > 2) { // number of tokens should never be more than 2
			std::cout << "\nBad Instruction: " << orgLine;
			return;
		}
		//std::cout << "here";
		strcpy(tokens[tokenCount++], pch);
		pch = std::strtok(NULL, " ");
	}

	// Create subjects
	if(tokenCount != 2) {
		std::cout << "\nBad Instruction: " << orgLine;
		return;
	}
	if(hasNumbersOnly(tokens[1])) 
		refMon->createSubject(tokens[0], atoi(tokens[1]));
	else 
		std::cout << "\nBad Instruction: " << orgLine;
}

void parseObj(char *line) {
	int tokenCount = 0;
	char *pch, orgLine[80];
	char tokens[4][30];

	strcpy(orgLine, line); // save a copy of the current line

	//std::cout << "here";
	pch = strtok(line, "\n"); // Remove the trailing new line character added by fgets

	/* Now we use strok to tokenize a line of instruction */
	pch = strtok(line, " "); 
	while(pch != NULL) {
		if(tokenCount > 2) { // number of tokens should never be more than 2
			std::cout << "\nBad Instruction: " << orgLine;
			return;
		}
		//std::cout << "here";
		strcpy(tokens[tokenCount++], pch);
		pch = std::strtok(NULL, " ");
	}

	// Create objects
	if(tokenCount != 2) {
		std::cout << "\nBad Instruction: " << orgLine;
		return;
	}
	if(hasNumbersOnly(tokens[1])) 
		refMon->createObject(tokens[0], atoi(tokens[1]));
	else 
		std::cout << "\nBad Instruction: " << orgLine;
}

int parseCal(char *line) {
	int count = 0;

	int tokenCount = 0;
	char *pch, orgLine[80];
	char tokens[4][30];

	strcpy(orgLine, line); // save a copy of the current line

	//std::cout << "here";
	pch = strtok(line, "\n"); // Remove the trailing new line character added by fgets

	/* Now we use strok to tokenize a line of instruction */
	pch = strtok(line, " "); 
	while(pch != NULL) {
		if(tokenCount > 2) { // number of tokens should never be more than 2
			std::cout << "\nBad Instruction: " << orgLine;
			return 0;
		}
		//std::cout << "here";
		strcpy(tokens[tokenCount++], pch);
		pch = std::strtok(NULL, " ");
	}

	// Create subjects
	if(tokenCount != 2) {
		std::cout << "\nBad Instruction: " << orgLine;
		return 0;
	}
	if(hasNumbersOnly(tokens[1])) 
		count = std::max(count, atoi(tokens[1]));
	else 
		std::cout << "\nBad Instruction: " << orgLine;
	
	return count;
}

void parseIns(char *line, CryptoContext<Poly> cc, vector<LPKeyPair<Poly>> v_kp, vector<vector<LPEvalKey<Poly>>> vv_relatK) {
	int tokenCount = 0;
	char *pch, orgLine[80];
	char tokens[4][30];

	strcpy(orgLine, line); // save a copy of the current line

	//std::cout << "here";
	pch = strtok(line, "\n"); // Remove the trailing new line character added by fgets

	/* Now we use strok to tokenize a line of instruction */
	pch = strtok(line, " "); 
	while(pch != NULL) {
		if(tokenCount > 4) { // number of tokens should never be more than 4
			std::cout << "\nBad Instruction: " << orgLine;
			return;
		}
		//std::cout << "here";
		strcpy(tokens[tokenCount++], pch);
		pch = std::strtok(NULL, " ");
	}

	/* Check the first token to decide whether it is a READ or WRITE instruction */
	if((strcasecmp(tokens[0], "read")) == 0) {
		//read
		if(tokenCount != 3) {
			std::cout << "\nBad Instruction: " << orgLine;
			return;
		}
		// Create a new Instruction object that is passed to the ReferenceMonitor
		Instruction *ins = new Instruction(0, tokens[1], tokens[2], cc, v_kp, vv_relatK);
		refMon->executeInstruction(ins);
	} else if(strcasecmp(tokens[0], "write") == 0) {
		//write
		if(tokenCount != 4) {
			std::cout << "\nBad Instruction: " << orgLine;
			return;
		}
		// We check if the last token i.e the value, is a valid text file
		if(isValidFile(tokens[3])) {
			Instruction *ins = new Instruction(1, tokens[1], tokens[2], tokens[3], cc, v_kp, vv_relatK);
			refMon->executeInstruction(ins);
		}
		else 
			std::cout << "\nBad Instruction: " << orgLine;
	} else {
		std::cout << "\nBad Instruction: " << orgLine;
	}
}