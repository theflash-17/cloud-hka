# cloud-hka
This project uses Proxy Re-Encryption (PRE), and All-Or-Nothing-Transform (AONT) along with Bell-LaPadula scheme to implement a secure method for hierarchical key access access control.

This project is based on the paper - '**CloudHKA: A Cryptographic Approach for Hierachical Access Control in Cloud Computing**', by Yi-Ruei Chen, Cheng-Kang Chu, Wen-Guey Tzeng, and Jianying Zhou. In *11th International Conference on Applied Cryptography and Network Security (ACNS'13)*.
<br/><br/>
## Working of the project
The file 'chka.cpp' creates a cryptocontext. It takes the files - inputSub.txt, inputObj.txt, and inputIns.txt respectively as parameters and parses them.

The file 'inputSub.txt' contains a list of names of users and their respective security classes (levels).
The file 'inputObj.txt' contains a list of names of data and their respective security classes (levels).
The file 'inputIns.txt' contains a list of instructions that allows users to read or write data.

**Note:** All these three text files can be edited and modified as per need.

A user can read or write data following the properties of Bell-LaPadula scheme. To observe the Bell-LaPadula model, the following properties are required:
- Simple Security Property:
  
  User 'U' cannot read datum 'D' if security class level of U is *less than* the security class level of D
- Star Property:
  
  User 'U' cannot write datum 'D' if security class level of D is *less than* the security class level of U

If the user satisfies the above properties during read/write, then he/she can access the contents of the respective data's text file, present in the 'aont' folder.

**Note:** The name of text files containing data must be same as the name of the data in the files 'inputObj.txt' and 'inputIns.txt'.

When the user is granted access to write, the respective new text file of data is encrypted and AONT transformed.
Similarly, when the user is granted access to read, the respective text file of data is AONT reversed, and then decrypted using Proxy Re-Encryption to get the original text file.<br/><br/>
## Directory structure of the project
The file 'chka.cpp' is the main file of the project. 

The folder 'blp' contains all the files required for constructing the cryptosystem, and the Bell-LaPadula scheme.

The folder 'build' contains the three text files required as input for 'chka.cpp', along with a folder named 'aont'.

The folder 'aont' contains the file 'aont.cs' for constructing AONT, the 'Makefile' for performing AONT operations on files, and text files containing data of users.
<br/><br/>
## Instruction for running the project
The source files required to implement AONT, and Bell-LaPadula scheme are included in this repository. The PRE scheme has been implemented using the [PALISADE](https://gitlab.com/palisade/palisade-release) library.

* Clone and build the PALISADE library as shown [here](https://gitlab.com/palisade/palisade-release/-/wikis/Instructions-for-building-PALISADE-in-Linux).
* Clone this repository.
* Copy 'CMakeLists.User.txt' from the root directory of the PALISADE repository to the folder of this repository. Then rename 'CMakeLists.User.txt' to 'CMakeLists.txt'.
* In the 'CMakeLists.txt' file, after line 31, add the following:
```
include_directories( ${PALISADE_INCLUDE}/binfhe )
include_directories( "your_path"/aont )
include_directories( "your_path"/blp )
```
Now, it must look as follows:
```
include_directories( ${OPENMP_INCLUDES} )
include_directories( ${PALISADE_INCLUDE} )
include_directories( ${PALISADE_INCLUDE}/third-party/include )
include_directories( ${PALISADE_INCLUDE}/core )
include_directories( ${PALISADE_INCLUDE}/pke )
include_directories( ${PALISADE_INCLUDE}/binfhe )
include_directories( "your_path"/aont )
include_directories( "your_path"/blp )
### add directories for other PALISADE modules as needed for your project
```
* At the end of 'CMakeLists.txt' file, add the name of executable (say, "test") and the source file (here, 'chka.cpp'). It must look as follows:
```
add_executable( test chka.cpp )
```
* Open the terminal in the 'build' folder and then run:
```
cmake ..
```
* Then run "make" to build the executable
```
make
```
* You should be able to see the executable file (in this case, "test") in the build folder.
* Run the "test" executable file by passing the 3 text files - inputSub.txt, inputObj.txt, and inputIns.txt respectively as parameters.
```
./test inputSub.txt inputObj.txt inputIns.txt
```
<br/><br/>
**Note: This project has been made for educational purposes, as a part of internship at Indian Institute of Information Technology, Guwahati**
