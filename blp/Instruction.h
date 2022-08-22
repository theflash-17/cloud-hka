#include "chka_req.h"

class Instruction {

public:
	int type; //0 for READ, 1 for WRITE
	CryptoContext<Poly> cc;
	vector<LPKeyPair<Poly>> v_kp;
	vector<vector<LPEvalKey<Poly>>> vv_relatK;

	char subjectName[30];
	char objectName[30];
	std::string fileName;
	
	Instruction(int mType, char mSname[30], char mObjName[30], CryptoContext<Poly> temp_cc, vector<LPKeyPair<Poly>> temp_v_kp, vector<vector<LPEvalKey<Poly>>> temp_vv_relatK) {
		this->type = mType;
		strcpy(this->subjectName, mSname);
		strcpy(this->objectName, mObjName);
		this->cc = temp_cc;
		this->v_kp = temp_v_kp;
		this->vv_relatK = temp_vv_relatK;
	}

	Instruction(int mType, char mSname[30], char mObjName[30], std::string temp_fileName, CryptoContext<Poly> temp_cc, vector<LPKeyPair<Poly>> temp_v_kp, vector<vector<LPEvalKey<Poly>>> temp_vv_relatK) {
		this->type = mType;
		strcpy(this->subjectName, mSname);
		strcpy(this->objectName, mObjName);
		this->fileName = temp_fileName;
		this->cc = temp_cc;
		this->v_kp = temp_v_kp;
		this->vv_relatK = temp_vv_relatK;
	}

};