/*This file is part of FFB Arcade Plugin.
FFB Arcade Plugin is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
FFB Arcade Plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with FFB Arcade Plugin.If not, see < https://www.gnu.org/licenses/>.
*/

// The Windows output protocol of MAME and Supermodel: MAME64.dll (MAME32.dll) calls these functions
// in the thread of a hidden window, MAMEInterop. The network outputs (MAMENetworkOutputs.cpp) call
// them too.
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "MAMESupermodel.h"
#include "MAMENetworkOutputs.h"
#include "../Core/Settings.h"

static HINSTANCE ProcDLL = NULL;
static HINSTANCE hInstance;
static HINSTANCE hPrevInstance;
static LPSTR lpCmdLine;
static int nCmdShow;

typedef int(__stdcall* MAME_START)(int hWnd);
typedef int(__stdcall* MAME_STOP)(void);
typedef int(__stdcall* MAME_COPYDATA)(int id, const char* name);
typedef int(__stdcall* MAME_UPDATESTATE)(const char* id, int state);
typedef int(__stdcall* MAME_OUTPUT)(const char* name, int value);
typedef int(__stdcall* INIT_MAME)(int clientid, PWCHAR name, MAME_START start, MAME_STOP stop, MAME_COPYDATA copydata, MAME_UPDATESTATE updatestate, MAME_OUTPUT output);

MAME_START mame_start_ptr;
MAME_STOP mame_stop_ptr;
MAME_COPYDATA mame_copydata_ptr;
MAME_UPDATESTATE mame_updatestate_ptr;
MAME_OUTPUT mame_output_ptr;
INIT_MAME dll_init;

int __stdcall mame_start(int hWnd);
int __stdcall mame_stop(void);
int __stdcall mame_copydata(int id, const char* name);
int __stdcall mame_updatestate(const char* id, int state);
int __stdcall mame_output(const char* name, int value);

void CallTheOutputs()
{
#ifdef _WIN64  
	ProcDLL = LoadLibrary(TEXT("MAME64.dll"));
#else  
	ProcDLL = LoadLibrary(TEXT("MAME32.dll"));
#endif
	if (ProcDLL != NULL)
	{
		dll_init = (INIT_MAME)GetProcAddress(ProcDLL, "init_mame");
	}
	else
	{
#ifdef _WIN64
		MessageBoxA(NULL, "MAME64.dll is missing!", "FFBPluginError", NULL);
#else
		MessageBoxA(NULL, "MAME32.dll is missing!", "FFBPluginError", NULL);
#endif
	}
}
void AppendTextToEditCtrl(HWND hWnd, LPCTSTR lpString);

HWND hEdit;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void AppendTextToEditCtrl(HWND hWnd, LPCTSTR pszText);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CallTheOutputs();
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MAMEInterop";
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(L"MAMEInterop", L"MAMEInterop", WS_OVERLAPPEDWINDOW,
		0, 0, 512, 512, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
		MessageBox(hWnd, L"Could not create window", L"Error", MB_OK | MB_ICONERROR);

	hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
		8, 8, 488, 464, hWnd, NULL, GetModuleHandle(NULL), NULL);

	if (hEdit == NULL)
		MessageBox(hWnd, L"Could not create edit box.", L"Error", MB_OK | MB_ICONERROR);

	if (hWnd != NULL)
	{
		//ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}
	
	mame_start_ptr = mame_start;
	mame_stop_ptr = mame_stop;
	mame_copydata_ptr = mame_copydata;
	mame_updatestate_ptr = mame_updatestate;
	mame_output_ptr = mame_output;

	// without MAME64.dll (its message box has been shown), only the network outputs work: calling
	// a null dll_init crashed the game once the message box was closed
	if (dll_init != NULL)
		dll_init(0, L"Test", mame_start_ptr, mame_stop_ptr, mame_copydata_ptr, mame_updatestate_ptr, mame_output_ptr);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void AppendTextToEditCtrl(HWND hWnd, LPCTSTR lpString)
{
	int iPrevLen = GetWindowTextLength(hWnd);
	SendMessage(hWnd, EM_SETSEL, iPrevLen, iPrevLen);
	SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)lpString);
}

int __stdcall mame_start(int hWnd)
{
	WCHAR buf[256];

	wsprintf(buf, L"mame_start hwnd = 0x%08x\r\n", hWnd);

	AppendTextToEditCtrl(hEdit, buf);

	return 1;
}

int __stdcall mame_stop(void)
{
	AppendTextToEditCtrl(hEdit, TEXT("mame_stop\r\n"));

	MAMESupermodel::OnStop();

	return 1;
}

static void removeWord(char* str, char* toRemove)
{
	int i, j, stringLen, toRemoveLen;
	int found;

	stringLen = (int)strlen(str);
	toRemoveLen = (int)strlen(toRemove);


	for (i = 0; i <= stringLen - toRemoveLen; i++)
	{
		found = 1;
		for (j = 0; j < toRemoveLen; j++)
		{
			if (str[i + j] != toRemove[j])
			{
				found = 0;
				break;
			}
		}

		if (str[i + j] != ' ' && str[i + j] != '\t' && str[i + j] != '\n' && str[i + j] != '\0')
		{
			found = 0;
		}


		if (found == 1)
		{
			for (j = i; j <= stringLen - toRemoveLen; j++)
			{
				str[j] = str[j + toRemoveLen];
			}

			stringLen = stringLen - toRemoveLen;

			i--;
		}
	}
}

int __stdcall mame_copydata(int id, const char* name)
{
	// The names are cut to fit buf: the network outputs pass lines of up to 4 KB
	WCHAR buf[256];

	wsprintf(buf, L"id %d = '%.200S'\r\n", id, name);

	if (id == 0)
	{
		char GameName[256];
		sprintf_s(GameName, "%.255s", name);

		removeWord(GameName, "game =");

		UINT whitespace = 0;

		for (UINT i = 0; i < strlen(GameName); i++)
		{
			if (GameName[i] == ' ' || GameName[i] == '\t')
				whitespace++;
			else
				break;
		}

		for (UINT i = 0; i < strlen(GameName); i++)
		{
			if (i + whitespace < strlen(GameName))
				GameName[i] = GameName[i + whitespace];
			else
				GameName[i] = 0;
		}

		MAMESupermodel::OnGameName(GameName);
	}

	AppendTextToEditCtrl(hEdit, buf);

	return 1;
}

int __stdcall mame_updatestate(const char* id, int state)
{
	WCHAR buf[256];

	LPCSTR name = id;

	wsprintf(buf, L"updatestate: id=%ld (%.200S) state=%d\r\n", (INT_PTR)id, IS_INTRESOURCE(name) ? "" : name, state);
	AppendTextToEditCtrl(hEdit, buf);

	MAMESupermodel::OnOutput(IS_INTRESOURCE(name) ? "" : name, state);

	return 1;
}

int __stdcall mame_output(const char* name, int value)
{
	WCHAR buf[256];

	wsprintf(buf, L"output: name=%.200S value=%d\r\n", name, value);
	AppendTextToEditCtrl(hEdit, buf);

	return 1;
}

DWORD WINAPI ThreadForOutputs(LPVOID lpParam)
{
	// also read the network outputs (MAME: "-output network"), unless MAMENetworkOutputs=0
	if (ffb::Settings::Int(L"MAMENetworkOutputs", 1) != 0)
		StartMameNetworkOutputs(ffb::Settings::Int(L"GameId", 0));
	WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	return 0;
}
