#include "palisade.h"

#include "cryptocontextgen.h"
#include "cryptocontextparametersets.h"

#include "binfhecontext.h"

CryptoContext<Poly> GeneratePREContext() {
  shared_ptr<Poly::Params> ep;
  CryptoContext<Poly> cc;
  unsigned int m = 2048;
  PlaintextModulus ptm = 256;                                 // #Can use your own
  cc = GenTestCryptoContext<Poly>("BFV_rlwe", m, ptm);
  return cc;
}

namespace chka {

  // psi.Setup - (k) -> {sp, MK}
  // (CryptoContext) -> {LPKeyPair}
  LPKeyPair<Poly> Setup(CryptoContext<Poly> cc) {              
    LPKeyPair<Poly> kp = cc->KeyGen();
    if(!kp.good()) {
      std::cout << "Key generation failed" << std::endl;
      exit(1);
    }
    return kp; 
  }

  // psi.KeyGen - (MK, index) -> {ek_i, dk_i}
  // (CryptoContext, LPPrivatKey, vector of index)  -> {vector of LPKeyPair}
  vector<LPKeyPair<Poly>> KeyGen(CryptoContext<Poly> cc, int countSC) {    
    vector<LPKeyPair<Poly>> vkp;
    for(auto i=0; i<countSC; i++) {
      LPKeyPair<Poly> kp = chka::Setup(cc);
      vkp.push_back(kp);
    }
    return vkp;
  }

  // psi.Enc - (ek_i, m) -> {c_i}
  // (CryptoContext, LPPublicKey, Plaintext) -> {Ciphertext}
  Ciphertext<Poly> Enc(CryptoContext<Poly> cc, LPPublicKey<Poly> pub_key, Plaintext m) {
    Ciphertext<Poly> c = cc->Encrypt(pub_key, m);
    return c;
  }

  // psi.ReKeyGen - ((ek_i, dk_i), (ek_j, dk_j)) -> {rk_ij}
  // (CryptoContext, LPPublicKey, LPPrivatKey) -> {ReKey}
  LPEvalKey<Poly> ReKeyGen(CryptoContext<Poly> cc, LPPublicKey<Poly> pub_key_j, LPPrivateKey<Poly> priv_key_i) {
    LPEvalKey<Poly> rekey_ij = cc->ReKeyGen(pub_key_j, priv_key_i);
    return rekey_ij;
  }

  // psi.ReEnc - (rk_ij, c_i) -> {c_ij} 
  // (CryptoContext, ReKey, Ciphertext) -> {Ciphertext}
  Ciphertext<Poly> ReEnc(CryptoContext<Poly> cc, LPEvalKey<Poly> rekey_ij, Ciphertext<Poly> c_i) {
    Ciphertext<Poly> c_ij = cc->ReEncrypt(rekey_ij, c_i);
    return c_ij;
  }

  // psi.Dec - ()
  // (CryptoContext, LPPrivatKey, Ciphertext) -> {Plaintext}
  Plaintext Dec(CryptoContext<Poly> cc, LPPrivateKey<Poly> priv_key_j, Ciphertext<Poly> c_ij) {
    Plaintext m_new;
    DecryptResult result = cc->Decrypt(priv_key_j, c_ij, &m_new);
    if(!result.isValid) {
      std::cout << "Decryption failed" << std::endl;
      exit(1);
    }
    return m_new;
  }
}
