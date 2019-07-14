#include "Sokoban.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow) {
	Sokoban dlg;
	dlg.hInst = hInstance;

	MSG msg;
	HWND hDialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Sokoban::DlgProc);
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1)); 
	ShowWindow(hDialog, nCmdShow);
	while(GetMessage(&msg, 0, 0, 0)) {
		if(!TranslateAccelerator(hDialog, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	_CrtDumpMemoryLeaks();
	return msg.wParam;
}


BOOL CALLBACK Sokoban::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);

		HANDLE_MSG(hwnd, WM_CTLCOLORSTATIC, ptr->Cls_OnCtlColor);
		HANDLE_MSG(hwnd, WM_CTLCOLORDLG, ptr->Cls_OnCtlColor);
	}
	return FALSE;
}