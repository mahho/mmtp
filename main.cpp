#include "src/CParser.h"

int main(int argc, char* argv[]) {
    if (argc < 2)
	std::cout << "Potrzebny parametr: ./mmtp szablon" << std::endl;
    else {
	CParser parser;
	parser.openTemplate(argv[1]); // otwieramy szablon
	parser.setLogin("testuje");
	parser.setPassword("mmtp");
	//parser.setDomain("hexnet.info");
	parser.parse();
	parser.execute();
    }
    return 0;
}
