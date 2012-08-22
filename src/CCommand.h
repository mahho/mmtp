#ifndef __CCOMMAND_H_
#define __CCOMMAND_H_

#include <iostream>
#include <pcre.h>
#include <vector>
#include <sstream>

#include "CCurl.h"

struct s_cmd {
    std::string myCommand;
    std::string myParameters;
};

struct s_message {
	std::string myLink;
	std::string myHeader;
	std::string myDownloadLink;
};

enum errorCodes {
    ERROR = 0,
};

class CCommand {
	std::string myMainLink;
	std::string myPage;
	std::vector <s_cmd> myCommandsList;
    std::vector <s_message> myMessages;
	CCurl *myCurl;
    std::string myNextPageLink;
    std::string myMessageLink;
    int myOffset;

    
public:
	CCommand();
	void setMainLink(std::string mainLink) { myMainLink = mainLink; };
	void addCommand(std::string command, std::string parameters);
	int executePreLoop();
	int executeSearchLoop();
};

#endif

