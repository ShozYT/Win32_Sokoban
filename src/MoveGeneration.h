#include <windows.h>
#include <windowsX.h>

#include "resource.h"


class MoveGeneration {
	char *mgMap_Sokoban;
	int mgWidth_map, mgSteps;

	int get_position_User() const;
	void permutation_boxes(const int position);

public:
	void SetMoveGeneration(const int _mgWidth_map, const int _mgSteps, char* _mgMap_Sokoban) {
		mgWidth_map = _mgWidth_map; 
		mgSteps = _mgSteps;
		mgMap_Sokoban = _mgMap_Sokoban;
	}

	char* getmgMap_Sokoban() const {
		return mgMap_Sokoban;
	}

	int getmgSteps() const {
		return mgSteps;
	}

	void move_generation(int direction);
};