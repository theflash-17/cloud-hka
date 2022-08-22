#include <string.h>
#include "Object.h"
#include "../aont_mod.h"

class Subject {
	int level;
	
public:
	char name[20];
	int temp;

	Subject(char mName[30], int level);

	void readObject(Object *obj, CryptoContext<Poly> cc, Ciphertext<Poly> ctext, LPEvalKey<Poly> rekey, LPPrivateKey<Poly> priv_key);

	Ciphertext<Poly> writeObject(Object *obj, std::string fileName, CryptoContext<Poly> cc, LPPublicKey<Poly> pub_key);

	int getLevel();
	map<Plaintext, string> m_ptext;
};

// Constructor
Subject::Subject(char mName[30], int mLevel) {
	this->temp = 0;
	strcpy(this->name, mName);
	this->level = mLevel;
}

// Reads value from an Object
void Subject::readObject(Object *obj, CryptoContext<Poly> cc, Ciphertext<Poly> ctext, LPEvalKey<Poly> rekey, LPPrivateKey<Poly> priv_key) {
	Ciphertext<Poly> re_ctext = chka::ReEnc(cc, rekey, ctext);
	Plaintext ptext = chka::Dec(cc, priv_key, re_ctext);
	string fileName = m_ptext[ptext];
	makefileRev(fileName);
}

// Writes to an Object (AONT Transform to text file)
Ciphertext<Poly> Subject::writeObject(Object *obj, std::string fileName, CryptoContext<Poly> cc, LPPublicKey<Poly> pub_key) {
	obj->file = fileName;
	makefileTrans(fileName);
	Plaintext ptext = cc->MakeStringPlaintext(fileName);
	m_ptext[ptext] = fileName;
	Ciphertext<Poly> ctext = chka::Enc(cc, pub_key, ptext);
	return ctext;
}

int Subject::getLevel() {
	return this->level;
}