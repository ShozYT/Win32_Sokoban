#include "MoveGeneration.h"
#include "vector"


int CustomMessageBox(HWND, LPCTSTR, LPCTSTR, UINT); 

class Sokoban {
private:
	bool safe_load, win;

	int width_map, height_map;
	int steps, number_map;
	int sizeArray_map;
	int seconds;

	HWND hwndS;
	HWND hwBUTTON_PREVIOUS, hwBUTTON_NEXT, hwBUTTON_RESTART, hwBUTTON_LAST_MOVE;
	HWND hwStatic_Steps, hwStatic_Time;
	HBITMAP hBmpWall, hBmpBox, hBmpZero, hBmpPlayer,hBmpLabelPlayer,hBmpPlayerWin, hBmpLabel, hBmpLabelBox;

	HBRUSH hbrush;

	HWND *hPictureControl;
	char *MAP_Sokoban;

	struct str_MapSokoban {
		char *strMap;

		str_MapSokoban (char *_strMap) : strMap(_strMap) {
			int size = strlen(_strMap);
			strMap = new char [size+1];
			strcpy(strMap, _strMap);
		}

		str_MapSokoban(const str_MapSokoban &obj2) {
			int size = strlen(obj2.strMap);
			strMap = new char [size+1];
			strcpy(strMap, obj2.strMap);
		}

		~str_MapSokoban() {
			delete[] strMap;
		}
	};

	
	std::vector<str_MapSokoban> arraysMap_Sokoban;
	MoveGeneration Move;

public:
	static Sokoban* ptr;
	HINSTANCE hInst;

	Sokoban::Sokoban() : safe_load(0), win(0), width_map(0),height_map(0), steps(0), number_map(0), sizeArray_map(0), seconds(0) {
		ptr = this;
	}


	Sokoban::~Sokoban() {
		if(safe_load) {
			delete[] MAP_Sokoban;

			for(int i=0; i < sizeArray_map; i++) {
				DestroyWindow (hPictureControl[i]);
			}
			delete[] hPictureControl;
			arraysMap_Sokoban.clear();
		}
	}

public:
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);

private:
	//Win Api
	void Cls_OnTimer(HWND hwnd, UINT id);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);

	HBRUSH Cls_OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type);


	//Metods for game
	void load_Map(const TCHAR *pathInMap, const HWND &lhwndS);
	void create_Coordinates(const char *TxtMap, const HWND &chwndS);
	void draw_map(const char *TxtMap);

	bool verification_of_winning();

	void clear_map();

	void switch_map(const int direction);
	void last_move();
};