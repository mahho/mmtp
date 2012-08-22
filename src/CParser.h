#ifndef __CPARSER_H_
#define __CPARSER_H_

#include <algorithm>
#include <iostream>
#include <pcre.h>
#include <string>
#include <fstream>
#include <vector>

#include "CCommand.h"

class CParser {
    std::ifstream myFile;
    std::vector<std::string> myData;
    CCommand myCommand;
    std::string myDomain;
    std::string myLogin;
    std::string myPassword;

	// wyciagniete z szablonu
	std::string myMainLink;
public:
    CParser();
    void openTemplate(std::string filename);
    void setMainLink(std::string mainLink) { this->myMainLink = mainLink; }
    std::string getMailLink() { return myMainLink; }
    void setDomain(std::string domain) { this->myDomain = domain; }
    std::string getDomain() { return myDomain; }
    void setLogin(std::string login) { this->myLogin = login; }
    std::string getLogin() { return myLogin; }
    void setPassword(std::string password) { this->myPassword = password; }
    std::string getPassword() { return myPassword; }
    void strReplace( std::string& source, const char* find, const char* replace );

    void parse();
    void execute();

};

#endif
