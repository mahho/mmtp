#include "CCommand.h"

CCommand::CCommand()  {
    myCurl = new CCurl();
}

void CCommand::addCommand(std::string command, std::string parameters) {
    s_cmd cmd;
    cmd.myCommand = command;
    cmd.myParameters = parameters;
    myCommandsList.push_back(cmd);
}

int CCommand::executePreLoop() {
	std::cout << "** Executing preLoop" << std::endl;
    for (int counter = 0; counter < myCommandsList.size(); counter++) {
		s_cmd cmd = myCommandsList.at(counter);
		if (cmd.myCommand == "get") {
			std::cout << "* Executing get" << std::endl;
			myPage = myCurl->doHttpGet(myMainLink);
			//std::cout << myPage << std::endl;
		} else if (cmd.myCommand == "post") {
			std::cout << "* Executing post" << std::endl;
			pcrecpp::RE explode("(.*),(.*)");
			std::string postForm, postData;
			explode.PartialMatch(cmd.myParameters, &postForm, &postData);
			myPage = myCurl->doHttpPost(myMainLink+postForm, postData);
			//std::cout << myPage << std::endl;
		} else if (cmd.myCommand == "search") {
			std::cout << "* Executing search" << std::endl;
			pcrecpp::RE re(cmd.myParameters+"(.*)");
			std::string result;
			int myresult = re.PartialMatch(myPage, &result);
			if (myresult)
				std::cout << "Found phrase..." << std::endl;
		} 
    }
}

int CCommand::executeSearchLoop() {
	std::cout << "** Executing mainLoop" << std::endl;
	for (int counter = 0; counter < myCommandsList.size(); counter++) {
		s_cmd cmd = myCommandsList.at(counter);
		if (cmd.myCommand == "nextPageLinkSearch") {
			std::cout << "* Parsing nextPageLinkSearch: " << cmd.myParameters << std::endl;
			pcrecpp::RE explode("(.*?),*.(\\d+)");
			explode.PartialMatch(cmd.myParameters, &myNextPageLink, &myOffset);
			std::cout << "nextpl " << myNextPageLink << " offset: " << myOffset << std::endl;
		} else if (cmd.myCommand == "messageLinkSearch") {
			//messageLinkSearch(<a href="showmessage.html?@@messageLink@@"><strong>@@partHeader@@</strong></a>)
			std::cout << "* Parsing messageLinkSearch" << std::endl;
			myMessageLink = cmd.myParameters;
		}
	}
	
	if (myNextPageLink.size() <= 0 || myOffset <= 0) {
		std::cout << "nextPageLink or offset is invalid!" << std::endl;
		return false;
	} else 	{
		int pageNumber = 0;
		bool cont = true;
		std::cout << "+Searching headers" << std::endl;
		do {
			// pobieramy aktualna strone
			std::stringstream ss;
			ss << pageNumber;
			std::string currentPageLink = myNextPageLink;
			pcrecpp::RE("@@pageNumber@@").GlobalReplace(ss.str(), &currentPageLink);
			std::cout << currentPageLink << std::endl;
			myPage = myCurl->doHttpGet(myMainLink+"/"+currentPageLink);

			// Szukamy wszystkich wiadomosci na stronie
			pcrecpp::RE("@@messageLink@@").GlobalReplace("(.*?)", &myMessageLink);
			pcrecpp::RE("@@messageHeader@@").GlobalReplace("(.*?)", &myMessageLink);
			pcrecpp::RE consumer(myMessageLink);
			std::cout << "myMessageLink: " << myMessageLink << std::endl;
			std::string link, header;
			pcrecpp::StringPiece input(myPage);
			while (consumer.FindAndConsume(&input, &link, &header)) {
				std::cout << "Link: " << link << " --Header: " << header << std::endl;
				s_message msg;
				msg.myLink = link;
				msg.myHeader = header;
				myMessages.push_back(msg);
			}
			
			// sprawdzamy czy jest kolejna strona dostępna
			pageNumber+=myOffset;
			std::stringstream ss2;
			ss2 << pageNumber;
			std::string nextPageLink = myNextPageLink;
			pcrecpp::RE("@@pageNumber@@").GlobalReplace(ss2.str(), &nextPageLink); // zamieniamy @@pageNumber@@ na offset nastepnej strony
			pcrecpp::RE("[?]").GlobalReplace("[?]", &nextPageLink); // zamieniami znak ? na [?] zeby pcre go nie parsowal
			std::cout << "search next: " << nextPageLink << std::endl;
			std::string result;
			pcrecpp::RE rere(nextPageLink+"(.*)");
			int res = rere.PartialMatch(myPage, &result);

			// jesli nie mamy zadnej strony, to konczymy
			if (!res)
				cont = false;
		} while (cont == true);
	}
}

