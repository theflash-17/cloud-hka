#include <fstream>

void makefileReq(std::string &temp1, std::string &temp2) {
    temp1 = "all: run\n\ntest.exe: aont.cs\n\t@ mcs aont.cs -out:test\n\nrun: test.exe\n\t./test ";
    
    temp2 = "\n\nclean:\n\t@ -rm -f test.exe\n\nremake :\n\t@ $(MAKE) clean\n\t@ $(MAKE)";
}

void makefileTrans(const std::string str) {
    std::string temp1, temp2;
    makefileReq(temp1, temp2);

    std::ofstream ofs("aont/Makefile", std::ofstream::trunc);
    ofs << temp1;
    ofs << "/n:1 " << str;
    ofs << temp2;
    ofs.close();
    system("cd aont; make");
}

void makefileRev(const std::string str) {
    std::string temp1, temp2;
    makefileReq(temp1, temp2);

    std::ofstream ofs("aont/Makefile", std::ofstream::trunc);
    ofs << temp1;
    ofs << "/r " << str << ".aont";
    ofs << temp2;
    ofs.close();
    system("cd aont; make");
}