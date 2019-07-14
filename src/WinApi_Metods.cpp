#include "Sokoban.h"

BOOL Sokoban::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	hwndS = hwnd;

	hBmpWall = (HBITMAP)LoadImage(NULL,L"texture\\wall.bmp",IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
	hBmpBox = (HBITMAP)LoadImage(NULL,L"texture\\box.bmp",IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
	hBmpZero = (HBITMAP)LoadImage(NULL,L"texture\\zero.bmp",IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
	hBmpPlayer = (HBITMAP)LoadImage(NULL,L"texture\\player.bmp",IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
	hBmpLabelPlayer = (HBITMAP)LoadImage(NULL,L"texture\\label_player.bmp",IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
	hBmpPlayerWin = (HBITMAP)LoadImage(NULL,L"texture\\\playerWin.bmp",IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
	hBmpLabel = (HBITMAP)LoadImage(NULL,L"texture\\label.bmp",IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
	hBmpLabelBox = (HBITMAP)LoadImage(NULL,L"texture\\labelbox.bmp",IMAGE_BITMAP,40,40,LR_LOADFROMFILE);

	hwBUTTON_LAST_MOVE =  GetDlgItem (hwndS, IDC_BUTTON_LAST_MOVE);
	hwBUTTON_PREVIOUS = GetDlgItem (hwndS, IDC_BUTTON_PREVIOUS);
	hwBUTTON_RESTART = GetDlgItem (hwndS, IDC_BUTTON_RESTART);
	hwStatic_Steps = GetDlgItem (hwndS, IDC_STATIC_STEPS);
	hwStatic_Time =  GetDlgItem (hwndS, IDC_STATIC_TIME);

	hbrush = CreateSolidBrush (RGB(54, 44, 33));

	SetTimer(hwndS, 0, 100, 0);
	SetTimer(hwndS, 1, 1000, 0);

	number_map = 1;
	load_Map(L"maps\\1.txt", hwndS);

	return TRUE;
}


HBRUSH Sokoban::Cls_OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type) {
	HDC hdcStatic = hdc;
	SetTextColor (hdcStatic, RGB (255, 0, 0));
	SetBkMode (hdcStatic, TRANSPARENT);

	return hbrush;
}


void Sokoban::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (win == 0 && safe_load == 1) {
		Move.SetMoveGeneration(width_map, steps, MAP_Sokoban);
		switch (id) {
		case ID_LEFT:
			Move.move_generation(ID_LEFT);
			break;
		case ID_RIGHT:
			Move.move_generation(ID_RIGHT);
			break;
		case ID_UP:
			Move.move_generation(ID_UP);
			break;
		case ID_DOWN:
			Move.move_generation(ID_DOWN);
			break;
		case IDC_BUTTON_RESTART:
			switch_map(IDC_BUTTON_RESTART);
			break;
		case IDC_BUTTON_LAST_MOVE:
			last_move();
			break;

		}


		if(id == ID_LEFT || id == ID_RIGHT || id == ID_UP || id == ID_DOWN) {
			int index = arraysMap_Sokoban.size() - 1;
			if(strcmp(arraysMap_Sokoban[index].strMap, Move.getmgMap_Sokoban()) != 0) {
				steps = Move.getmgSteps();
				strcpy(MAP_Sokoban, Move.getmgMap_Sokoban());
				arraysMap_Sokoban.push_back(str_MapSokoban(MAP_Sokoban));
				draw_map(MAP_Sokoban);
			}
		}
	}



	switch (id) {
	case IDC_BUTTON_PREVIOUS:
		switch_map(IDC_BUTTON_PREVIOUS);
		break;
	case IDC_BUTTON_NEXT:
		switch_map(IDC_BUTTON_NEXT);
		break;

	}
}	

void Sokoban::Cls_OnTimer(HWND hwnd, UINT id) {
	if(win == 0 && safe_load == 1) {
		switch (id) { 
		case 0:
			{
				static int value_steps = 0;
				if(value_steps != steps) {
					value_steps = steps;
					TCHAR count_steps[10];
					wsprintf(count_steps, L"STEPS %d", value_steps);
					SetWindowText (hwStatic_Steps, count_steps);
				}
			}
			break;

		case 1:
			seconds++;
			TCHAR tchar_seconds[10];
			wsprintf(tchar_seconds, L"%d", seconds);
			SetWindowText (hwStatic_Time, tchar_seconds);
			break;
		} 
	}
	else {
		steps = 0;
		seconds = 0;
	}
}

void Sokoban::Cls_OnClose(HWND hwnd){
	DestroyWindow(hwnd);
	PostQuitMessage(0);
}