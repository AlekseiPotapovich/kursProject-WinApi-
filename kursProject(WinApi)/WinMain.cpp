#include <windows.h>
#include <time.h>
#include <string>
#include "myCurl.h"

#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <shellapi.h>
#include <process.h>

#include <fstream>
using std::ofstream;
using std::ifstream;


#pragma comment(lib, "Ws2_32.lib")

//#include <curl.h>
//#pragma comment(lib, "libcurl.lib")

#include "winsock.h"
#include <iostream>

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctime>

# define ID_BUTTON_SENT 3000
# define ID_BUTTON_G_P 2500
# define ID_BUTTON_ENTER 2300
//# define ID_BUTTON_GET 2100
# define ID_BUTTON_GET 2200
#define ID_BUTTON_MENU 500
#define ID_BUTTON_EXIT 600
#define ID_BUTTON_DELETE 700


# define ID_EDIT 2000
# define ID_LIST 1000
# define ID_COMBO 1500
#define TIMER_SEC 1
#define IDC_COUNT 1010

#define FROM_ADDR    "<zxcasdqwe_54@mail.ru>"
//#define TO_ADDR      "<leshik-garoshik@mail.ru>"

char str[200];
char str1[50];
char str2[50];
char logstr1[50];
char logstr2[50];
//int l = 0;
int sizeLStr1 = 0;
int sizeLStr2 = 0;
int sizeLStr3 = 0;
char size[5];
//#define FROM_ADDR str2
#define TO_ADDR str1


static HWND hCombo, hwndDlg;
static HWND hList;
static HWND hList1;

static HWND hListBox;


HWND hEdit, hStr1, hStr2, hStr3, LogStr1, LogStr2;
int num = 0;
int kursor = 0;
int x11 = 950;
int y11 = 600;
bool par = false;


////////////////////////////////////////////////////////////////////
//
//int Count;
//int i;
//HICON hIcon;
//HANDLE h;


myCurl mycurl;

//CURL *curl;
//CURLcode res = CURLE_OK;
//struct curl_slist *recipients = NULL;
//struct upload_status upload_ctx;

int kX, kY, kX2, kY2;


LPCTSTR LStr1 = "Enter your login:";
LPCTSTR LStr2 = "Enter your password:";
LPCTSTR LStr3 = "Enter your message:";


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	

	static char szAppName[] = "Email_Client";
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	//wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIcon = (HICON)LoadImage(NULL, "icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	//if (hIcon) { SendMessage(NULL, WM_SETICON, ICON_BIG, (LPARAM)hIcon); }
	//else { MessageBoxW(NULL, L"ico not found", L"ico not found", MB_OK | MB_ICONERROR); }
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = "IDR_MENU1";
	wndclass.lpszClassName = szAppName;
	//wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = (HICON)LoadImage(NULL, "icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);

	if (!RegisterClassEx(&wndclass)) {
		MessageBox(NULL, "Cannot register class", "Error", MB_OK);
		return 0;
	}

	//RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, "Email_Client", WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	
	kX = 350; kY = 300;
	kX2 = 350; kY2 = 400;
	sizeLStr1 = 22;
	sizeLStr2 = 22;
	LStr1 = "Enter your login:";
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	//MoveWindow(hwnd, 10, 10, 950, 800, true);
	
	//создание таймера
	SetTimer(hwnd, TIMER_SEC, 10000, NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyWindow(hwnd); // ”ничтожаем основное окно
	UnregisterClass(szAppName, NULL); // ќсвобождаем пам€ть, отмен€€ регистрацию класса основного окна.
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	//определение идентификаторов button и edit
	static HWND hButton_SENT, hButton_G_P, hButton_ENTER, hButton_GET, hButton_MENU, hButton_EXIT, hButton_DELETE;

	
	HBRUSH hBrush;
	char chBuff[3];
	int list_num;
	//char buf1[3], buf2[3], buf3[3];
	//LPARAM text1[4]="red";
	char url[25] = "pop3s://pop3.mail.ru/";
	static int cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	

	switch (iMsg)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		//MoveWindow(hCombo,10,40,450-x11+cxClient,100-y11+cyClient,par);
		
		return 0;

	case WM_CREATE:
		switch (wParam)
		{
		case ID_BUTTON_G_P:
			hButton_GET = CreateWindow("button", "GET", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 570, 250, 40, hwnd, (HMENU)ID_BUTTON_GET, hInstance, NULL);
			hStr3 = CreateWindow("Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 20, 140, 530, 20, hwnd, (HMENU)ID_LIST, hInstance, 0);
			
			break;
		case ID_BUTTON_GET:
			hButton_MENU = CreateWindow("button", "MENU", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 570, 250, 40, hwnd, (HMENU)ID_BUTTON_MENU, hInstance, NULL);
			hButton_DELETE = CreateWindow("button", "Delete", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 350, 570, 250, 40, hwnd, (HMENU)ID_BUTTON_DELETE, hInstance, NULL);

			break;
		case ID_BUTTON_ENTER:
			hButton_EXIT = CreateWindow("button", "Exit", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 200, 630, 250, 40, hwnd, (HMENU)ID_BUTTON_EXIT, hInstance, NULL);

			hButton_SENT = CreateWindow("button", "Sent", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 350, 570, 250, 40, hwnd, (HMENU)ID_BUTTON_SENT, hInstance, NULL);
			hButton_G_P = CreateWindow("button", "Get Mess", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 570, 250, 40, hwnd, (HMENU)ID_BUTTON_G_P, hInstance, NULL);

			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"Edit", NULL,       // button text
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP,  // styles
				20, 200, 895, 280, hwnd, (HMENU)ID_LIST, hInstance, 0);

			hStr1 = CreateWindow("Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 20, 100, 250, 20, hwnd, (HMENU)ID_LIST, hInstance, 0);
			hStr2 = CreateWindow("Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 300, 100, 250, 20, hwnd, (HMENU)ID_LIST, hInstance, 0);
			UpdateWindow(hwnd);
			//MessageBox(hwnd, "ID_BUTTON_ENTER", "OK", MB_OK);
			break;
		case 0:
			MoveWindow(hwnd, 10, 10, 950, 800, true);
			//hBitmap = (HBITMAP)LoadImage(NULL, "1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			
		/*	GetObject(hBitmap, sizeof(Bitmap), &Bitmap);
			hdc = GetDC(hwnd);
			memBit1 = CreateCompatibleDC(hdc);
			SelectObject(memBit1, hBitmap);*/


			hButton_ENTER = CreateWindow("button", "Enter", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 350, 480, 250, 40, hwnd, (HMENU)ID_BUTTON_ENTER, hInstance, NULL);
			LogStr1 = CreateWindow("Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 350, 330, 250, 20, hwnd, (HMENU)ID_LIST, hInstance, 0);
			LogStr2 = CreateWindow("Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 350, 430, 250, 20, hwnd, (HMENU)ID_LIST, hInstance, 0);
			//hButton_G_P = CreateWindow("button", "Get Mess", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 470, 250, 40, hwnd, (HMENU)ID_BUTTON_G_P, hInstance, NULL);

			break;
		}

		//MoveWindow(hwnd, 10, 10, 950, 800, true);

		break;
		//return 0;
		//изображение меню
	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		switch (wParam)
		{
		case 0:
			
			//MessageBox(hwnd, "WM_PAINTRRRRRRRRRRRRRRR", "OK", MB_OK);
			TextOut(hdc, kX, kY, LStr1, sizeLStr1);//300
			TextOut(hdc, kX2, kY2, LStr2, sizeLStr2);//400
			TextOut(hdc, 20, 180, LStr3, sizeLStr3);
			
			//BitBlt(hdc, 0, 0, 265, 175, memBit1, 0, 0, SRCCOPY);//вывод изображени€
			return 0;
		case ID_BUTTON_ENTER:
		
			//MessageBox(hwnd, "WM_PAINT", "OK", MB_OK);
			TextOut(hdc, 250, 170, "Enter your message", 19);
			return 0;
		}
		//DeleteObject(hBrush);
		//MessageBox(hwnd, "WM_PAINT", "OK", MB_OK);
		//TextOut(hdc, 250, 170, "Enter your message", 19);

		
		EndPaint(hwnd, &ps);
		//break;
		return 0;


	case WM_COMMAND:

		switch (LOWORD(wParam))	// нажатие кнопки
		{

		case ID_BUTTON_SENT:

			GetWindowText(hEdit, str, 200);
			GetWindowText(hStr1, str1, 50);
			GetWindowText(hStr2, str2, 50);

			DestroyWindow(hEdit);
			DestroyWindow(hStr1);
			DestroyWindow(hStr2);
			DestroyWindow(hButton_G_P);
			DestroyWindow(hButton_SENT);
			SendMessage(hwnd, WM_CREATE, ID_BUTTON_ENTER, lParam);
			InvalidateRect(hwnd, NULL, true);
			mycurl.sent(logstr1,logstr2, FROM_ADDR, TO_ADDR, str2, str);
			mycurl.sentFile(logstr1, logstr2, FROM_ADDR, TO_ADDR);
			break;

			MessageBox(hwnd, "Button pressed", "Message from button", MB_OK);
			return 0;

		case ID_BUTTON_G_P:

			mycurl.getMessage(url,logstr1,logstr2);
			mycurl.l = mycurl.getQuantityOfMessage();
			DestroyWindow(hButton_G_P);
			DestroyWindow(hButton_SENT);
			LStr1 = "You have messages: ";
			//char size[5];
			sizeLStr2 = 45;
			sizeLStr3 = 0;
			//SetWindowText(hStr2, "");
			
			SendMessage(hwnd, WM_CREATE, ID_BUTTON_G_P, lParam);
		
			LStr2 = "Enter number of message what you want to see:";
			//InvalidateRect(hStr2, NULL, true);
		
			itoa((mycurl.l-1), size, 10);
			SetWindowText(hStr1, size);
			InvalidateRect(hwnd, NULL, true);
			return 0;

		case ID_BUTTON_DELETE:
			//GetWindowText(hStr2, size, 5);
			strcat(url, size);
			mycurl.delMess(url, logstr1, logstr2);
			//DestroyWindow(hStr1);
			//DestroyWindow(hStr2);
			DestroyWindow(hButton_DELETE);
			DestroyWindow(hButton_MENU);
			mycurl.str8[0] = 0;
			//DestroyWindow(hStr2);

			sizeLStr1 = 22;
			sizeLStr2 = 22;
			sizeLStr3 = 20;
			LStr1 = "Enter e-mail recipent";
			LStr2 = "Subject of message";
			LStr3 = "Enter your message:";

			DestroyWindow(hEdit);
			DestroyWindow(hStr1);
			DestroyWindow(hStr2);
			DestroyWindow(hStr3);
			//DestroyWindow(hButton_GET);
			SendMessage(hwnd, WM_CREATE, ID_BUTTON_ENTER, lParam);
			InvalidateRect(hwnd, NULL, true);
			return 0;
		case ID_BUTTON_GET:

			GetWindowText(hStr2, size, 5);
			if (strcmp(size, "") == 0) {
				MessageBox(hwnd, "You don't enter number of message!", "Error", MB_OK);
				//SendMessage(hwnd, WM_CREATE, ID_BUTTON_G_P, lParam);
				InvalidateRect(hwnd, NULL, true);
				return 0;
			}
			
			strcat(url, size);
			mycurl.getMessage(url, logstr1, logstr2);
			DestroyWindow(hButton_GET);
			DestroyWindow(hButton_DELETE);
			LStr1 = "";
			sizeLStr1 = 0;
			sizeLStr2 = 0;
			LStr3 = "Text of message";
			sizeLStr3 = 15;
			
			

			SetWindowText(hStr1, mycurl.str6);
			SetWindowText(hStr2, mycurl.str8);
			SetWindowText(hStr3, mycurl.str9);
			SetWindowText(hEdit, mycurl.textOfMessage.c_str());
			
			SendMessage(hwnd, WM_CREATE, ID_BUTTON_GET, lParam);
			InvalidateRect(hwnd, NULL, true);
			
			return 0;

		case ID_BUTTON_MENU:

			//SendMessage(hStr2, WM_CLEAR, 0, 0);
			mycurl.str8[0] = 0;
			//DestroyWindow(hStr2);
			
			sizeLStr1 = 22;
			sizeLStr2 = 22;
			sizeLStr3 = 20;
			LStr1 = "Enter e-mail recipent";
			LStr2 = "Subject of message";
			LStr3 = "Enter your message:";
			
			DestroyWindow(hEdit);
			DestroyWindow(hStr1);
			DestroyWindow(hStr2);
			DestroyWindow(hStr3);
			SendMessage(hwnd, WM_CREATE, ID_BUTTON_ENTER, lParam);
			//InvalidateRect(hwnd, NULL, true);
			DestroyWindow(hButton_MENU);
			DestroyWindow(hButton_DELETE);
			SendMessage(hwnd, WM_PAINT, wParam, lParam);
			SendMessage(hwnd, WM_CREATE, wParam, lParam);
			InvalidateRect(hwnd, NULL, true);
			return 0;

		case ID_BUTTON_ENTER:

			GetWindowText(LogStr1, logstr1, 50);
			GetWindowText(LogStr2, logstr2, 50);



			if (mycurl.authorizeUser(logstr1, logstr2) == 0) {
				DestroyWindow(LogStr1);
				DestroyWindow(LogStr2);
				DestroyWindow(hButton_ENTER);
				//SendMessage(hButton_ENTER, WM_DESTROY, 0, 0);

				sizeLStr3 = 20;
				kX = 20; kY = 80;
				kX2 = 300; kY2 = 80;
				LStr1 = "Enter e-mail recipent";
				LStr2 = "Subject of message";
				SendMessage(hwnd, WM_PAINT, ID_BUTTON_ENTER, lParam);
				SendMessage(hwnd, WM_CREATE, wParam, lParam);
				InvalidateRect(hwnd, NULL, true);
			}
			
			return 0;

		case ID_BUTTON_EXIT:
			PostQuitMessage(NULL);
			return 0;

		}
		return 0;
	/*case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 950;
			lpMMI->ptMinTrackSize.y = 800;

			lpMMI->ptMaxTrackSize.x = 950;
			lpMMI->ptMaxTrackSize.y = 800;
		}
		break;*/
	case WM_DESTROY:
		PostQuitMessage(NULL); // отправл€ем WinMain() сообщение WM_QUIT
		break;
	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam);

	}
	return 0;
	//return DefWindowProc(hwnd, iMsg, wParam, lParam);
}