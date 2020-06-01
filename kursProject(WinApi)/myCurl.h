#pragma once
#include "winsock.h"
#include <curl.h>
#pragma comment(lib, "libcurl.lib")

#include <string>
#include <fstream>
using std::ofstream;
using std::ifstream;

//
//struct upload_status {
//	int lines_read;
//};
//
//static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp, char *str2, char *str)
//{
//	struct upload_status *upload_ctx = (struct upload_status *)userp;
//	static char *data;
//
//	char *payload_text[] = {
//
//		"Subject: " ,str2,
//		"\r\n", /* empty line to divide headers from body, see RFC5322 */
//		str,//the body of the message
//		"\r\n",
//		"\r\n\r\nSent by kursProject AlekseiPotapovich",
//
//
//
//		NULL
//	};
//
//	if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
//		return 0;
//	}
//
//	data = payload_text[upload_ctx->lines_read];
//
//	if (data) {
//		size_t len = strlen(data);
//		memcpy(ptr, data, len);
//		upload_ctx->lines_read++;
//
//		return len;
//	}
//
//	return 0;
//}
//
//struct FtpFile {
//	const char *filename;
//	FILE *stream;
//};
//
////struct FtpFile ftpfile;
////char *buf;
//static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
//{
//	struct FtpFile *out = (struct FtpFile *)stream;
//
//
//
//	if (!out->stream) {
//		out->stream = fopen(out->filename, "w+b");
//		if (!out->stream)
//			return -1; /* failure, can't open file to write */
//	}
//
//	return fwrite(buffer, size, nmemb, out->stream);
//
//}


//CURL *curl;
//CURLcode res = CURLE_OK;

class myCurl {
public:
	/*CURL *curl;
	CURLcode res = CURLE_OK;*/
	//struct FtpFile ftpfile;
	std::string textOfMessage;
	int l = 0;
	char *str5 = new char[1024];
	char *str6 = new char[50];
	char *str7 = new char[50];
	char *str8 = new char[50];
	char *str9 = new char[150];

	char *sizeText = new char[2048];

	char *subjectStr = new char[50];
	char *bodyStr = new char[200];

	int authorizeUser(char *logstr1, char *logstr2);
	void sent(char *logstr1, char *logstr2, char *FROM_ADDR, char *TO_ADDR, char *str2, char *str);
	std::string getText(char *sizeText, char *str5, char *str6, char *str7, char *str8, char *str9,std::string textOfMessage);
	void getMessage(char url[25], char *logstr1, char *logstr2);
	int getQuantityOfMessage();
	void sentFile(char *logstr1, char *logstr2, char *FROM_ADDR, char *TO_ADDR);
	void delMess(char url[25], char *logstr1, char *logstr2);
	
	//void getMessageByIndex();
	
};