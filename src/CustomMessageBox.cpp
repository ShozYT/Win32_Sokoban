#include "Sokoban.h"

static LRESULT CALLBACK CbtHookProc(int, WPARAM, LPARAM);
LRESULT CALLBACK CustomMessageBoxProc(HWND, UINT, WPARAM, LPARAM);

struct CustomMessageBoxValues {
	HHOOK hHook;         
	HWND  hWnd;         
	WNDPROC lpMsgBoxProc; 
	HBRUSH hBrush;      
} static __declspec(thread) cmbv;

int CustomMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) {
	cmbv.hHook = NULL;
	cmbv.hWnd = NULL;
	cmbv.lpMsgBoxProc = NULL;

	cmbv.hHook = SetWindowsHookEx(WH_CBT, CbtHookProc, GetModuleHandle(NULL), 
		GetCurrentThreadId());

	int nRet = MessageBox(hWnd, lpText, lpCaption, uType);

	UnhookWindowsHookEx(cmbv.hHook);
	return nRet;
}

LRESULT CALLBACK CustomMessageBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	switch(uMsg) {
	case WM_INITDIALOG:
		cmbv.hBrush = CreateSolidBrush (RGB(54, 44, 33));
		break;
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC:
		hDC = (HDC)wParam;
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(255, 215, 0));
		return (LRESULT)cmbv.hBrush;
	case WM_COMMAND:
		DeleteObject(cmbv.hBrush);
		break;
	}
	return CallWindowProc(cmbv.lpMsgBoxProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CbtHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if(nCode < 0)
		return CallNextHookEx(cmbv.hHook, nCode, wParam, lParam); 

	LPCBT_CREATEWND lpCbtCreate;

	switch(nCode) {
	case HCBT_CREATEWND:
		lpCbtCreate = (LPCBT_CREATEWND)lParam;
		if(WC_DIALOG == lpCbtCreate->lpcs->lpszClass) {
			cmbv.hWnd = (HWND)wParam;
		}
		else {
			if((NULL == cmbv.lpMsgBoxProc) && (NULL != cmbv.hWnd)) {
				cmbv.lpMsgBoxProc = (WNDPROC)::SetWindowLong(cmbv.hWnd, GWL_WNDPROC, 
					(LONG)CustomMessageBoxProc);
			}
		}
		break;
	case HCBT_DESTROYWND: 
		if(cmbv.hWnd == (HWND)wParam) {
			SetWindowLong(cmbv.hWnd, GWL_WNDPROC, (LONG)cmbv.lpMsgBoxProc);
		}
	}
	return 0;
}