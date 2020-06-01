#include "myCurl.h"
#include "base64.h"

myCurl curl2;

struct upload_status {
	int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct upload_status *upload_ctx = (struct upload_status *)userp;
	static char *data;

	char *payload_text[] = {

		"Subject: " ,curl2.subjectStr,
		"\r\n", /* empty line to divide headers from body, see RFC5322 */
		curl2.bodyStr,//the body of the message
		"\r\n",
		"\r\n\r\nSent by kursProject AlekseiPotapovich",



		NULL
	};

	if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}

	data = payload_text[upload_ctx->lines_read];

	if (data) {
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;

		return len;
	}

	return 0;
}

struct TxtFile {
	const char *filename;
	FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct TxtFile *out = (struct TxtFile *)stream;

	if (!out->stream) {
		out->stream = fopen(out->filename, "w+b");
		if (!out->stream)
			return -1; /* failure, can't open file to write */
	}

	return fwrite(buffer, size, nmemb, out->stream);

}


int myCurl::authorizeUser(char *logstr1, char *logstr2) {
		
	CURL *curl;
	CURLcode res = CURLE_OK;
	CURLINFO res1;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_USERNAME, logstr1);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, logstr2);



		curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.mail.ru");
		curl_easy_setopt(curl, CURLOPT_PORT, 465);



		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		res = curl_easy_perform(curl);



		if (res != CURLE_RECV_ERROR) {
			//cout << stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res);
			MessageBox(NULL, curl_easy_strerror(res), "Error", MB_OK);
			return 1;
		}
		else {
			MessageBox(NULL, "AuthorizeSuccsesful!!", "OK", MB_OK);
			return 0;
		}
		curl_easy_cleanup(curl);
	}
}


void myCurl::sent(char *logstr1, char *logstr2, char *FROM_ADDR, char *TO_ADDR, char *str2, char *str) {

	
	CURL *curl;
	
	curl2.subjectStr = str2;
	curl2.bodyStr = str;

	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	struct upload_status upload_ctx;
	upload_ctx.lines_read = 0;

	/*curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);*/
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_USERNAME, logstr1);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, logstr2);
	curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.mail.ru");
	curl_easy_setopt(curl, CURLOPT_PORT, 465);
	curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_ADDR);


	recipients = curl_slist_append(recipients, TO_ADDR);

	curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
	curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	/* Send the message */
	res = curl_easy_perform(curl);
	//cout << "\n" << res;


	/* Check for errors */
	if (res != CURLE_OK)
		//cout << stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res);
		MessageBox(NULL, curl_easy_strerror(res), "Error", MB_OK);
	else
		MessageBox(NULL, "Message was sented sucssecfuly", "OK", MB_OK);



	/* Free the list of recipients */
	curl_slist_free_all(recipients);

	curl_easy_cleanup(curl);
	//}else
	//MessageBox(NULL, "ERRRRROOOORRR", "Error", MB_OK);
}

int myCurl::getQuantityOfMessage() {
	int quantity = 0;
	std::ifstream in("text1.txt"); // îêðûâàåì ôàéë äëÿ ÷òåíèÿ
	if (in.is_open())
	{
		while (!in.eof()) {
			in.getline(str5, 1024, '\n');
			quantity++;
		}
	}
	in.close();
	return quantity;
}

void shiftStr(int s, char *str) {
	int sizeStr = strlen(str);
	int n = s % sizeStr;
	int si = strlen(str);
	str[si] = '\0';
	char *tail = new char[n];
	for (int i = 0; i < n; i++)
		tail[i] = str[i];
	for (int i = n; i < si; i++)
		str[i - n] = str[i];
	str[si - n] = '\0';
	/*for (int i = si - n; i < si; i++)
	{
		str[i] = tail[i + n - si];
	}*/
	delete[] tail;
}


std::string myCurl::getText(char *sizeText, char *str5, char *str6, char *str7, char *str8, char *str9, std::string textOfMessage) {
	l = 0;
	//std::string line;
	/*char *str5 = new char[1024];
	char *str6 = new char[200];
	char *str7 = new char[6];*/
	char size[20];
	int getStr = 0;
	BOOL base64 = false;
	BOOL exit = false;
	textOfMessage = "";
	std::ifstream in("text1.txt"); // îêðûâàåì ôàéë äëÿ ÷òåíèÿ
	if (in.is_open())
	{
		while (!in.eof()) {

			sizeText = '\0';
			in.getline(str5, 1024, '\n');
			strncpy(str7, str5, 14);
			str7[14] = '\0';
			l++;
			if (strcmp(str7, "From: =?utf-8?") == 0 | strcmp(str7, "From: =?UTF-8?") == 0) {
				base64 = true;
			}
			strncpy(str7, str5, 6);
			str7[6] = '\0';
			if (strcmp(str7, "Date: ") == 0) {

				strncpy(str6, str5, 31);
				str6[31] = '\0';
				//MessageBox(NULL, str6, "OK", MB_OK);
			}
			
			if (strcmp(str7, "Subjec") == 0) {

				int sizeStr5 = strlen(str5);
				strncpy(str9, str5, sizeStr5);
				str9[sizeStr5] = '\0';

				char *str10 = new char[20];
				strncpy(str10, str9, 18);
				str10[17] = '\0';
				if (strcmp(str10, "Subject: ") == 0) {
					char *str99 = new char[1024];
					strcpy(str99, "Subject: =?UTF-8?b?");
					in.getline(str5, 1024, '\n');

					shiftStr(11, str5);
					//str5[strlen(str5) - 2] = '\0';

					strcat(str99, str5);
					//str99[strlen(str99) - 2] = '\0';

					in.getline(str5, 1024, '\n');
					strncpy(str7, str5, 10);
					str7[9] = '\0';
					if (strcmp(str7, " =?UTF-8?b?") == 0 | strcmp(str7, " =?utf-8?") == 0) {
						str99[strlen(str99) - 2] = '\0';
						shiftStr(11, str5);
						strcat(str99, str5);
					}
					
					//shiftStr(11, str5);
					//str5[strlen(str5) + 1] = '=';
					//str5[strlen(str5) - 1] = '\0';

					//strcat(str99, str5);
					//str99[strlen(str99) - 2] = '\0';
					memset(str5, 0, strlen(str5));
					strcat(str5, str99);
					memset(str10, 0, strlen(str10));
					strncat(str10, str5, 17);
					//strncpy(str10, str5, 8);
					str10[18] = '\0';
				}
				if (strcmp(str10, "Subject: =?UTF-8?") == 0 | strcmp(str10, "Subject: =?utf-8?") == 0 ) {
					std::string buff;                                    
					base64 = true;
					for (int f = 0; f < strlen(str5); f++) {
						buff.append(1, str5[f]);
					}
					
					buff = buff.substr(19, strlen(str5) - 19);
					buff.resize(buff.length() - 2);
					buff = base64_decode(buff, false);
					memset(str9, 0, 150);
					strcat(str9, "Subject: ");
					for (int f = 0; f < buff.length(); f++) {
						str9[9 + f] += buff.c_str()[f];
					}
					str9[buff.length() + 9] = '\0';
					char *stre = new char[100];
					convert_utf8_to_windows1251(str9, str9, strlen(str9));
				}
			}

			if (strcmp(str7, "Return") == 0) {
				int n = 14 % strlen(str5);
				int si = strlen(str5);
				str5[si - 1] = '\0';
				char *tail = new char[n];
				for (int i = 0; i < n; i++)
					tail[i] = str5[i];
				for (int i = n; i < si; i++)
					str5[i - n] = str5[i];
				for (int i = si - n; i < si; i++)
				{
					str5[i] = tail[i + n - si];
				}
				delete[] tail;
				strcpy(str8, "From: ");
				strcat(str8, str5);
			}
			
			strncpy(str7, str5, 33);
			str7[33] = '\0';

			if (strcmp(str7, "Content-Transfer-Encoding: base64") == 0 & getStr == 0) {
				in.getline(str5, 1024, '\n');
				strncpy(str7, str5, 2);
				str7[2] = '\0';
				do {
					in.getline(str5, 1024, '\n');
					strncpy(str7, str5, 2);
					str7[2] = '\0';
					if (strcmp(str7, "--") != 0)
						for (int f = 0; f < strlen(str5); f++) {
							textOfMessage.append(1, str5[f]);
						}
					//textOfMessage = insert_linebreaks(textOfMessage, strlen(str5));
				} while (strcmp(str7, "--") != 0);
					/*if (strcmp(str7, "--") != 0) {
						in.getline(str5, 1024, '\n');
						for (int f = 0; f < strlen(str5); f++) {
							textOfMessage.append(1, str5[f]);
						}
					}*/

				textOfMessage = base64_decode(textOfMessage, false);
				
				char *buff1 = new char[4096];
				convert_utf8_to_windows1251(textOfMessage.c_str(), buff1, textOfMessage.length());
				/*int i = 0;
					while (buff1[i] != '\0') {
						char fr = 0; char sc = 0;
						fr = buff1[i]; sc = buff1[i + 5];
						if (fr == -51 & sc == -51)
							buff1[i] = '\0';
						i++;
					}
					buff1[textOfMessage.length() / 2] = '\0';*/
				textOfMessage.resize(0);
				for (int f = 0; f < strlen(buff1); f++) {
					textOfMessage.append(1, buff1[f]);
				}

				getStr = 1;
				exit = true;
				
			}
			//}
			//if (strcmp(str7, "") == 0 & base64 == true) {
			//	in.getline(str5, 1024, '\n');
			//	strncpy(str7, str5, 2);
			//	str7[2] = '\0';
			//	do {
			//		in.getline(str5, 1024, '\n');
			//		strncpy(str7, str5, 2);
			//		str7[2] = '\0';
			//		if (strcmp(str7, "--") != 0)
			//			for (int f = 0; f < strlen(str5); f++) {
			//				textOfMessage.append(1, str5[f]);
			//			}
			//	} while (!in.eof());
			//	textOfMessage = base64_decode(textOfMessage, false);
			//	char *buff1 = new char[2048];
			//	convert_utf8_to_windows1251(textOfMessage.c_str(), buff1, textOfMessage.length());
			//	int i = 0;
			//	/*while (buff1[i] != '\0') {
			//	char fr = 0; char sc = 0;
			//	fr = buff1[i]; sc = buff1[i + 5];
			//	if (fr == -51 & sc == -51)
			//	buff1[i] = '\0';
			//	i++;
			//	}*/
			//	//buff1[textOfMessage.length() / 2] = '\0';
			//	textOfMessage.resize(0);
			//	for (int f = 0; f < strlen(buff1); f++) {
			//		textOfMessage.append(1, buff1[f]);
			//	}

			strncpy(str7, str5, 31);
			str7[31] = '\0';
			if (strcmp(str7, "Content-Transfer-Encoding: 8bit") == 0 & getStr == 0) {
				//in.getline(str5, 1024, '\n');
				//strncpy(str7, str5, 2);
				//str7[2] = '\0';
				//do {
				//	in.getline(str5, 1024, '\n');
				//	strncpy(str7, str5, 2);
				//	str7[2] = '\0';
				//	if (strcmp(str7, "--") != 0)
				//		for (int f = 0; f < strlen(str5); f++) {
				//			textOfMessage.append(1, str5[f]);
				//		}
				//} while (strcmp(str7, "--") != 0);
				////textOfMessage = base64_decode(textOfMessage, false);
				////char *buff1 = new char[2048];
				////convert_utf8_to_windows1251(textOfMessage.c_str(), buff1, textOfMessage.length());
				////textOfMessage.resize(0);
				///*for (int f = 0; f < strlen(buff1); f++) {
				//	textOfMessage.append(1, buff1[f]);
				//}*/
				in.getline(str5, 1024, '\n');
				 do{
					/*in.getline(str5, 1024, '\n');
					strncpy(str7, str5, 2);
					str7[2] = '\0';*/
					for (int f = 0; f < strlen(str5); f++) {
						textOfMessage.append(1, str5[f]);
					}

					//textOfMessage.append(strlen(str5), *str5);
					textOfMessage.append(1, '\r');
					textOfMessage.append(1, '\n');
					in.getline(str5, 1024, '\n');
					strncpy(str7, str5, 2);
					str7[2] = '\0';
				}while (strcmp(str7, "--") != 0);
				//base64 = false;
				char *buff1 = new char[4096];
				convert_utf8_to_windows1251(textOfMessage.c_str(), buff1, textOfMessage.length());			
				textOfMessage.resize(0);
				for (int f = 0; f < strlen(buff1); f++) {
					textOfMessage.append(1, buff1[f]);
				}
				getStr = 1;
				exit = true;
			}
			
			if (strcmp(str7, "") == 0 & base64 == false) {
				
				while (!in.eof()) {
					in.getline(str5, 1024, '\n');
					/*strncpy(str7, str5, 7);
					str7[7] = '\0';*/
					
					//while (strcmp(str7, "--") != 0 & !in.eof()) {
					do{
						
						//size += str5;
						//strcat(sizeText, str5);
						//smatch sm;

						for (int f = 0; f < strlen(str5); f++) {
							textOfMessage.append(1, str5[f]);
						}

						//textOfMessage.append(strlen(str5), *str5);
						textOfMessage.append(1, '\r');
						textOfMessage.append(1, '\n');
						in.getline(str5, 1024, '\n');	
						strncpy(str7, str5, 2);
						str7[2] = '\0';
											
					} while (strcmp(str7, "--") != 0 & !in.eof());
				}
				exit = true;
				getStr = 1;
			}
			//textOfMessage += '\0';
			//textOfMessage.append(1, '\0');
			//sizeText += '\0';
			//buf1 += line;

		}
		/*while (getline(in, line))
		{
		l++;

		}*/
	}
	else
		MessageBox(NULL, "Error of openning file", "Error", MB_OK);

	//MessageBox(NULL, itoa((l-1), size, 10), "OK", MB_OK);
	//MessageBox(NULL, size, "OK", MB_OK);

	in.close();
	return textOfMessage;
}


void myCurl::getMessage(char url[25], char *logstr1, char *logstr2) {

	CURL *curl;
	
	CURLcode res = CURLE_OK;
	//FILE *fd;
	struct TxtFile txtfile;
	//struct FtpFile 
	txtfile = {
		"text1.txt", NULL
	};
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_USERNAME, logstr1/*"zxcasdqwe_54@mail.ru"*/);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, logstr2/*"1810092Alex"*/);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_PORT, 995);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);



	//curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

	//curl_easy_setopt(curl, CURLFORM_);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &txtfile);

	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


	res = curl_easy_perform(curl);
	//MessageBox(NULL, str, "OK", MB_OK);
	/* Check for errors */
	if (res != CURLE_OK) {
		MessageBox(NULL, curl_easy_strerror(res), "Error", MB_OK);

	}
	else {


	}

	curl_easy_cleanup(curl);
	fcloseall();
	textOfMessage = curl2.getText(sizeText, str5, str6, str7, str8, str9, textOfMessage);

}

size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *readhere = (FILE *)userdata;
	curl_off_t nread;

	/* copy as much data as possible into the 'ptr' buffer, but no more than
	'size' * 'nmemb' bytes! */
	size_t retcode = fread(ptr, size, nmemb, readhere);
	
	nread = (curl_off_t)retcode;

	fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T" bytes from file\n", nread);
	return retcode;
}

void myCurl::sentFile(char *logstr1, char *logstr2, char *FROM_ADDR, char *TO_ADDR) {
	CURL *curl;
	CURLcode res;

	curl_mime *form = NULL;
	curl_mimepart *field = NULL;
	struct curl_slist *headerlist = NULL;
	static const char buf[] = "Expect:";

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	
		/* Create the form */
		form = curl_mime_init(curl);

		/* Fill in the file upload field */
		field = curl_mime_addpart(form);
		curl_mime_name(field, "sendfile");
		curl_mime_filedata(field, "text.txt");

		/* Fill in the filename field */
		field = curl_mime_addpart(form);
		curl_mime_name(field, "filename");
		curl_mime_data(field, "text.txt", CURL_ZERO_TERMINATED);

		

		/* initialize custom header list (stating that Expect: 100-continue is not wanted */
		headerlist = curl_slist_append(headerlist, buf);
		/* what URL that receives this POST */
		curl_easy_setopt(curl, CURLOPT_USERNAME, logstr1/*"zxcasdqwe_54@mail.ru"*/);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, logstr2/*"1810092Alex"*/);
		curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.mail.ru");
		curl_easy_setopt(curl, CURLOPT_PORT, 465);
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_ADDR);


		//recipients = curl_slist_append(recipients, TO_ADDR);
		
		

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the form */
		curl_mime_free(form);
		/* free slist */
		curl_slist_free_all(headerlist);
	
}

void myCurl::delMess(char url[25], char *logstr1, char *logstr2) {

	CURL *curl;
	CURLcode res = CURLE_OK;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_USERNAME, logstr1);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, logstr2);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_PORT, 995);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELE");
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		MessageBox(NULL, curl_easy_strerror(res), "Error", MB_OK);
	}
	else {
		MessageBox(NULL, "Message deleted sucssesful", "OK", MB_OK);
	}
	/* Always cleanup */
	curl_easy_cleanup(curl);

}
