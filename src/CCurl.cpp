#include "CCurl.h"

//------------- Funkcja dla CURLa
static size_t write_data(void *buffer, size_t size, size_t nmem) {
    myBuffer += (char*)buffer;

    const int n = size * nmem;
    
if (segDownload == true)
segfile->write(static_cast<char*>(buffer), n);
    return n;
}
//-------------

CCurl::CCurl() {
    myCurl = curl_easy_init();
    curl_easy_setopt(myCurl, CURLOPT_AUTOREFERER, 1);
    curl_easy_setopt(myCurl, CURLOPT_HEADER, 0);
    curl_easy_setopt(myCurl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(myCurl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(myCurl, CURLOPT_USERAGENT, "User-Agent: Mozilla/5.0 (compatible;)");
    curl_easy_setopt(myCurl, CURLOPT_COOKIEFILE, "cookie");
    curl_easy_setopt(myCurl, CURLOPT_COOKIEJAR, "-");
    
    curl_easy_setopt(myCurl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(myCurl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(myCurl, CURLOPT_SSL_VERIFYPEER, 0);    
    myBuffer.clear();
}
CCurl::~CCurl() {
    curl_easy_cleanup(myCurl);
}

std::string CCurl::doHttpGet(std::string url) {
    myBuffer.clear(); // czyscimy bufor
    curl_easy_setopt(myCurl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(myCurl, CURLOPT_URL, url.c_str());
    curl_easy_perform(myCurl);
    return myBuffer;
}

std::string CCurl::doFileGet(std::string url, std::string filename) {
    segfile = new std::ofstream();
    segfile->open(filename.c_str(), std::ios::out | std::ios::binary );
    if (segfile->is_open()) {
	curl_easy_setopt(myCurl, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(myCurl, CURLOPT_URL, url.c_str());

	curl_easy_perform(myCurl);
	} else {

    }
    segfile->close();
}

std::string CCurl::doHttpPost(std::string url, std::string data) {
//    Logger::Log("[CCurl] doPost");
    myBuffer.clear(); // czyscimy bufor
    
    curl_easy_setopt(myCurl, CURLOPT_POST, 1);
    curl_easy_setopt(myCurl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(myCurl, CURLOPT_URL, url.c_str());
    
    curl_easy_perform(myCurl); 
    return myBuffer;
}

std::string CCurl::escape(std::string data) {
    std::string newdata = curl_easy_escape(myCurl, data.c_str(), 0);
    return newdata;
}

std::string CCurl::unescape(std::string data) {
    std::string newdata = curl_easy_unescape(myCurl, data.c_str(), 0, 0);
    return newdata;
}

void CCurl::setCookie(std::string cookie) {
    curl_easy_setopt(myCurl, CURLOPT_COOKIE, cookie.c_str());
}

double CCurl::getDownloadSpeed() {
    double val;
    res = curl_easy_getinfo(myCurl, CURLINFO_SPEED_DOWNLOAD, &val);
    if(CURLE_OK == res)
	return val; 
    else
	return 0;
}

double CCurl::getUploadSpeed() {
    double val;
    res = curl_easy_getinfo(myCurl, CURLINFO_SPEED_UPLOAD, &val);
    if(CURLE_OK == res)
	return val; 
    else
	return 0;
}

