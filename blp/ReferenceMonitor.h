#include <iostream>
#include <iomanip>
#include <map>
#include "Subject.h"
using namespace std;

class ReferenceMonitor {
	int objCount;
	int subCount;

	Object *objects[10];
	Subject *subjects[10];


	void executeRead(char subName[20], char objName[30], CryptoContext<Poly> cc, vector<vector<LPEvalKey<Poly>>> vv_relatK, vector<LPKeyPair<Poly>> v_kp);
	void executeWrite(char subName[20], char objName[30], std::string fileName, CryptoContext<Poly> cc, vector<LPKeyPair<Poly>> v_kp);


	Object* getObjByName(char name[20]);
	Subject* getSubByName(char name[20]);

public:
	ReferenceMonitor();
	void createObject(char name[20], int level);
	void createSubject(char name[20], int level);

	void executeInstruction(Instruction *ins);

	void printState();

	int classCount;
	int getObjCount();
	int getSubCount();

    map<string, Ciphertext<Poly>> m_ctext;
};

ReferenceMonitor::ReferenceMonitor() { 
	this->objCount = 0;
	this->subCount = 0;
	this->classCount = 0;
}

void ReferenceMonitor::createObject(char name[20], int level) {
	// Create a new Object and add it to our array of objects
	this->objects[this->objCount++] = new Object(name, level);
}

void ReferenceMonitor::createSubject(char name[20], int level) {
	// Create a new Subject and add it to our array of subjects
	this->subjects[this->subCount++] = new Subject(name, level);
}

/*
 * Searches the array of Objects maintained by the ReferenceMonitor
 * that has its name field matching with the argument 'name'
 */
Object* ReferenceMonitor::getObjByName(char name[20]) {
	int i = 0;
	while(i < this->objCount) {
		if(strcasecmp(name, (this->objects[i])->name) == 0)
			return this->objects[i++];
		i++;
	}
	return NULL;
}

/*
 * Searches the array of Subjects maintained by the ReferenceMonitor
 * that has its name field matching with the argument 'name'
 */
Subject* ReferenceMonitor::getSubByName(char name[20]) {
	int i = 0;
	while(i < this->subCount) {
		if(strcasecmp(name, (this->subjects[i])->name) == 0)
			return this->subjects[i++];
		i++;
	}
	return NULL;
}

/*
 * Recieves an Instruction object from SecureSystem
 * and decides whether to READ or WRITE
 */
void ReferenceMonitor::executeInstruction(Instruction *ins) {
	if(ins->type == 0) {
		this->executeRead(ins->subjectName, ins->objectName, ins->cc, ins->vv_relatK, ins->v_kp);
	} else {
		this->executeWrite(ins->subjectName, ins->objectName, ins->fileName, ins->cc, ins->v_kp);
	}
}

/*
 * Allows a subject to read a value from an object if the
 * access labels of the both allow reading by the policies of BLP model.
 */
void ReferenceMonitor::executeRead(char subName[20], char objName[30], CryptoContext<Poly> cc, vector<vector<LPEvalKey<Poly>>> vv_relatK, vector<LPKeyPair<Poly>> v_kp) {

	Subject *subject = getSubByName(subName);
	Object *object = getObjByName(objName);

	if(subject == NULL || object == NULL ) {
		cout << "\nBad Instruction : " << "read " << subName << " " << objName;
		return;
	}

	/* Allow READ only if the subject has a higher access level than the object */
	if(subject->getLevel() >= object->getLevel()) {
		// Grant access
		LPEvalKey<Poly> rekey = vv_relatK[object->getLevel()-1][subject->getLevel()-1];
		Ciphertext<Poly> ctext = m_ctext[objName];
		LPPrivateKey<Poly> priv_key = v_kp[subject->getLevel()-1].secretKey;
		subject->readObject(object, cc, ctext, rekey, priv_key);
		cout << "\nAcess Granted : " << subName << " reads " << objName;
		this->printState();
	} else {
		//Access denied
		cout << "\nAccess Denied : " << "read " << subName << " " << objName;
	}
}

void ReferenceMonitor::executeWrite(char subName[20], char objName[30], std::string fileName, CryptoContext<Poly> cc, vector<LPKeyPair<Poly>> v_kp) {

	Subject *subject = getSubByName(subName);
	Object *object = getObjByName(objName);

	if(subject == NULL || object == NULL ) {
		cout << "\nBad Instruction : " << "write " << subName << " " << objName << " " << fileName;
		return;
	}

	/* Allow WRITE only if the subject has a lower access level than the object */
	if(subject->getLevel() <= object->getLevel()) {
		// Grant access
		LPPublicKey<Poly> pub_key = v_kp[object->getLevel()-1].publicKey;
		Ciphertext<Poly> ctext = subject->writeObject(object, fileName, cc, pub_key);
		m_ctext[objName] = ctext;
		cout << "\nAcess Granted : " << subName << " writes value in " << fileName << " to " << objName; 
		cout << "\n" << fileName << " is AONT transformed and encrypted";
		this->printState();
	} else {
		//Access denied
		cout << "\nAccess Denied : " << "write " << subName << " " << objName << " " << fileName;

	}
}

void ReferenceMonitor::printState() {
	int i=0;

	cout << "\n+------------------------The current state is-------------------------+\n";
	cout << "| subject |";
	while(i < subCount) {
		cout << " " << setw(7)<< (this->subjects[i])->name << " |";
		i++;
	}
	i=0;
	cout <<"\n| value   |";
	while(i < subCount) {
		cout << " " << setw(7) << (this->subjects[i])->temp << " |";
		i++;
	}
	i=0;
	cout << "\n+---------------------------------------------------------------------+\n";
	cout << "| object  |";
	while(i < objCount) {
		cout << " " << setw(7) << (this->objects[i])->name << " |";
		i++;
	}
	i=0;
	cout << "\n| value   |";
	while(i < subCount) {
		cout << " " << setw(7) << (this->objects[i])->file << " |";
		i++;
	}

	cout << "\n+---------------------------------------------------------------------+\n";
}

int ReferenceMonitor::getObjCount() {
	return this->objCount;
}

int ReferenceMonitor::getSubCount() {
	return this->subCount;
}