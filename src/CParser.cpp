#include "CParser.h"

CParser::CParser() {}

void CParser::openTemplate(std::string filename) {
    myFile.open(filename.c_str());
    if (!myFile.is_open()) 
		std::cout << "Szablon nie istnieje..." << std::endl;
}

void CParser::parse() {
    if (myLogin.size() <= 0)
		std::cout << "Login is not set" << std::endl;
	else {
		std::string line;
		while ( myFile.good() ) {
			getline(myFile, line);
			myData.push_back(line);
		}
		
		pcrecpp::RE mainLink("mainLink\\((.*)\\)");
		mainLink.PartialMatch(myData.at(0), &myMainLink);
		if (myMainLink.size() > 0) {
			std::cout << "myMainLink: " <<  myMainLink << std::endl;
			myCommand.setMainLink(myMainLink);
		}
		else 
			std::cout << "mainLink() is not set!" << std::endl;
		
		std::string command, parameters;
		for (int line = 1; line < myData.size(); line++) {
			command.clear();
			parameters.clear();
			//std::cout << myData.at(line) << std::endl;
			pcrecpp::RE re("(.*)\\((.*)\\)");
			re.PartialMatch(myData.at(line), &command, &parameters);
			//std::cout << "Komenda: " << command << " - parametry: " << parameters << std::endl;	
			
			// replacing
			pcrecpp::RE("\\[\\[login\\]\\]").GlobalReplace(myLogin, &parameters);
			pcrecpp::RE("\\[\\[password\\]\\]").GlobalReplace(myPassword, &parameters);
			pcrecpp::RE("\\[\\[domain\\]\\]").GlobalReplace(myDomain, &parameters);

			std::cout << parameters << std::endl;
			if (command.size() > 0)
			    myCommand.addCommand(command, parameters);
		}
	}
}

void CParser::execute() {
    myCommand.executePreLoop();
    myCommand.executeSearchLoop();
}
