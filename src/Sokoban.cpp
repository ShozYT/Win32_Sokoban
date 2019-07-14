#include "Sokoban.h"

Sokoban* Sokoban::ptr = NULL; 

void Sokoban::load_Map(const TCHAR *pathInMap, const HWND &lhwndS) {
	HANDLE hFile = CreateFile (pathInMap, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile != INVALID_HANDLE_VALUE) {
		WIN32_FIND_DATA Find_data;
		HANDLE hf = FindFirstFile (pathInMap, &Find_data);
		FindClose (hf);

		int sizeFile = Find_data.nFileSizeHigh + Find_data.nFileSizeLow;

		if(sizeFile > 334) {
			sizeFile = 334;
		}

		char strBuf[335];
		strBuf[0] = '\0';

		DWORD dwRead = 0;
		SetFilePointer (hFile, 0, 0, FILE_BEGIN);
		if (ReadFile (hFile, strBuf, sizeFile, &dwRead, NULL)) {
			strBuf[sizeFile] = '\0';

			for(int i=0; i < sizeFile+1; i++) {
				if(strBuf[i] == '\n') {
					width_map = i-1;
					break;
				}
			}

			if(width_map > 22 || width_map < 1) {
				safe_load = 0;
				CustomMessageBox(NULL, TEXT("No big width = 22"), TEXT("ERROR"), MB_OK | MB_ICONERROR);

			}
			else {
				create_Coordinates(strBuf, lhwndS);
				safe_load = 1;
			}
		}
		else {
			safe_load = 0;
			CustomMessageBox(NULL, TEXT("No read file"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		}
	}
	else {
		safe_load = 0;
		CustomMessageBox(NULL, TEXT("No open file"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
	}

	CloseHandle (hFile);
}


void Sokoban::create_Coordinates(const char *TxtMap,const HWND &chwndS) {
	int i, sizeTxtMap = strlen(TxtMap);

	for(i=0; i < sizeTxtMap; i++) {
		if(TxtMap[i] == '\r' || TxtMap[i] == '\n') {
			continue;
		}
		sizeArray_map++;
	}

	hPictureControl= new HWND [sizeArray_map]; 

	int startPositionX = 40;
	if((22 - width_map) > 0) {
		startPositionX = 5 + ((22 - width_map)/2)*40;
	}
	else {
		startPositionX = 5;
	}


	int x = startPositionX, y = 40, countStatic = 0;

	for(i=0; i < sizeTxtMap; i++) {
		if(TxtMap[i] == '\n') {
			continue;
		}

		if(TxtMap[i] == '\r' && TxtMap[i+1] == '\n') {
			x = startPositionX;
			y+= 40;
		} 
		else {
			hPictureControl[countStatic] = CreateWindowEx(WS_EX_CONTROLPARENT, TEXT("Static"), 0, WS_CHILD | WS_VISIBLE | SS_BITMAP, 
				x, y, 40, 40, chwndS, 0, hInst, 0);
			countStatic++;
			x+=40;
		}
	}

	MAP_Sokoban = new char [sizeArray_map+1];
	MAP_Sokoban[0] = '\0';

	int k=0;
	for(i=0; i < sizeTxtMap+1; i++) {
		if(TxtMap[i] == '\r' || TxtMap[i] == '\n') {
			continue;
		}
		MAP_Sokoban[k] = TxtMap[i];
		k++;
	}

	MAP_Sokoban[sizeArray_map] = '\0';
	arraysMap_Sokoban.push_back(str_MapSokoban(MAP_Sokoban));
	draw_map(MAP_Sokoban);
}


void Sokoban::draw_map(const char *TxtMap) {
	for(int i=0; i < strlen(TxtMap); i++) {
		switch(TxtMap[i]) {
		case '#':
			SendMessage(hPictureControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmpWall));
			break;
		case '$':
			SendMessage(hPictureControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmpBox));
			break;
		case ' ':
			SendMessage(hPictureControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmpZero));
			break;
		case '@':
			SendMessage(hPictureControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmpPlayer));
			break;
		case 'a':
			SendMessage(hPictureControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmpLabelPlayer));
			break;
		case '.':
			SendMessage(hPictureControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmpLabel));
			break;
		case '*':
			SendMessage(hPictureControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmpLabelBox));
			break;
		}
	}

	if(verification_of_winning()) {
		for(int i=0; i < strlen(TxtMap); i++) {
			if(TxtMap[i] == '@') {
				SendMessage(hPictureControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmpPlayerWin));
				CustomMessageBox(hwndS, TEXT("   Вы победили!   \n\n\n"), TEXT("             Win!"), MB_OK);
				break;
			}
		}
	}
}


void Sokoban::clear_map() {
	delete[] MAP_Sokoban;

	for(int i=0; i < sizeArray_map; i++) {
		DestroyWindow (hPictureControl[i]);
	}

	delete[] hPictureControl;
	arraysMap_Sokoban.clear();
}


bool Sokoban::verification_of_winning() {
	for(int i=0; i < strlen(MAP_Sokoban); i++) {
		if(MAP_Sokoban[i] ==  '$' || MAP_Sokoban[i] ==  '.' || MAP_Sokoban[i] ==  'a') {
			win = 0;
			return 0;
		}
	}

	win = 1;
	return 1;
}


void Sokoban::switch_map(const int direction) {
	switch(direction) {
	case IDC_BUTTON_PREVIOUS:
		if(number_map > 1){
			number_map--; 
		}
		else {return;}
		break;

	case IDC_BUTTON_NEXT:
		if(number_map < 30) {
			number_map++; 

		}
		else {return;}
		break;

	case IDC_BUTTON_RESTART:
		strcpy(MAP_Sokoban, arraysMap_Sokoban[0].strMap);
		arraysMap_Sokoban.clear();
		arraysMap_Sokoban.push_back(str_MapSokoban(MAP_Sokoban));
		steps = 0;
		seconds = 0;
		draw_map(MAP_Sokoban);
		return;
		break;
	}


	if(safe_load == 1) {
		clear_map();
	}

	wchar_t map_path[20];
	lstrcpyW(map_path, L"maps\\");

	wchar_t wnumber_map[10];
	wsprintf(wnumber_map, L"%d.txt", number_map);
	lstrcatW(map_path, wnumber_map);

	load_Map(map_path, hwndS);
}


void Sokoban::last_move() {
	if(arraysMap_Sokoban.size() > 1 && verification_of_winning() != 1 && safe_load == 1) {
		int lastMove = arraysMap_Sokoban.size()-2;
		strcpy(MAP_Sokoban, arraysMap_Sokoban[lastMove].strMap);
		draw_map(arraysMap_Sokoban[lastMove].strMap);

		arraysMap_Sokoban.pop_back();
		--steps;
	}
}