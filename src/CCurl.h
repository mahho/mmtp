#ifndef __CCURL_H_
#define __CURL_H_

#include <curl/curl.h>
#include <vector>
#include <string>
#include <pcrecpp.h>
#include <fstream>

static std::string myBuffer;
static bool segDownload;
static std::ofstream *segfile;

class CCurl {
    std::string myPage, myResult;
    CURL *myCurl;
    CURLcode res;

public:
	CCurl();
	~CCurl();
    std::string doHttpGet(std::string url); // HTTP GET
    std::string doFileGet(std::string url, std::string filename);
	std::string doHttpPost(std::string url, std::string data); // HTTP POST
    std::string escape(std::string data);
    std::string unescape(std::string data);
    void setCookie(std::string cookie);
    double getDownloadSpeed(); // predkosc pobierania
    double getUploadSpeed(); // predkosc wysylania
};

#endif
