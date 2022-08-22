#include <iterator>
#include "blp/SecureSystem.h"
using namespace std;
using namespace lbcrypto;

int main(int argc, char* argv[]) {

	////////////////////////////////////////////////////////////
  // SYSTEM SETUP
  ////////////////////////////////////////////////////////////


	//////////////////////////// [BLP] - SETUP
  // ReferenceMonitor is created and the number of Security Classes is counted

	FILE *fp, *fp1;
	char line[80];

	if(argc != 4) {
		std::cout << "Usage: ./chka inputSub inputObj inputIns";
		exit(0);
	}

	refMon = new ReferenceMonitor(); // Create ReferenceMonitor for our SecuritySystem

	// Find the number of Security Classes (classCount)
	fp1 = fopen(argv[1], "r");
	while(!feof(fp1)) {
		fgets(line, 80, fp1);	// assume a line to have maximum of 80 characters
		refMon->classCount = parseCal(line);
	}

	if(refMon->classCount==0) {
		std::cout << "Error: No Security Classes found";
		exit(0);
	}


	//////////////////////////// [PRE] - PRE SETUP
  // CryptoContext is created and initialized with BFV

  CryptoContext<Poly> cc = GeneratePREContext();

  if (cc == 0) return 0;

  std::cout << "Crypto system for BFV initialized with parameters:" << std::endl;
  std::cout << *cc->GetCryptoParameters() << std::endl;

  // enable features that you wish to use
  cc->Enable(ENCRYPTION);
  cc->Enable(SHE);
  cc->Enable(PRE);


  //////////////////////////// [PRE] - SETUP
  // Master Secret Key (MK) is generated

  std::cout << "Running setup" << std::endl;

  LPKeyPair<Poly> kp = chka::Setup(cc);


	//////////////////////////// [PRE] - KEY GENERATION
  // For each SC_i, {WriteK_i, ReadK_i} is generated using KeyGen(MK)

  int countSC = refMon->classCount;		// Number of Security Classes (from SecureSystem)

  std::cout << "Running key generation for each level of Security Class" << std::endl;

  vector<LPKeyPair<Poly>> v_kp = chka::KeyGen(cc, countSC);  


	//////////////////////////// [AES] - KEY GENERATION
  // For each SC_i, DistK_i = {dist_k_pub, dist_k_priv} is generated

  std::cout << "Running key generation for AES" << std::endl;

	vector<LPKeyPair<Poly>> v_distK = chka::KeyGen(cc, countSC);


	//////////////////////////// [PRE] - REKEY GENERATION
  // For each pair of {SC_i, SC_j}, a rekey RelatK_ij is generated

  std::cout << "Running re-key generation for each pair of levels of Security Class" << std::endl;

	vector<vector<LPEvalKey<Poly>>> vv_relatK;

	for(int i=0; i<countSC; i++) {
    vector<LPEvalKey<Poly>> temp;
    for(int j=0; j<countSC; j++) {
      if(i==j)
        temp.push_back(NULL);
      else
        temp.push_back(chka::ReKeyGen(cc, v_kp[j].publicKey, v_kp[i].secretKey));
    }
    vv_relatK.push_back(temp);
	}

  //////////////////////////// [BLP] - Parse subject input file
  
  fp = fopen(argv[1], "r");

  std::cout << "Parsing subject input file" << std::endl;

	// Parse the subject input file
	while(!feof(fp)) {
		fgets(line, 80, fp); // assume a line to have maximum of 80 characters
		parseSub(line);
	}

  //////////////////////////// [BLP] - Parse object input file
  
  fp = fopen(argv[2], "r");

  std::cout << "Parsing object input file" << std::endl;

	// Parse the object input file
	while(!feof(fp)) {
		fgets(line, 80, fp); // assume a line to have maximum of 80 characters
		parseObj(line);
	}

  //////////////////////////// [BLP] - Parse instruction input file
  
  fp = fopen(argv[3], "r");

  std::cout << "Parsing instruction input file" << std::endl;

	// Parse the instruction input file
	while(!feof(fp)) {
		fgets(line, 80, fp); // assume a line to have maximum of 80 characters
		parseIns(line, cc, v_kp, vv_relatK);
	}  

  return 0;
}