#include "MoveGeneration.h"

int MoveGeneration::get_position_User() const {
	for (int i=0; i < strlen(mgMap_Sokoban); i++) {
		if(mgMap_Sokoban[i] == '@' || mgMap_Sokoban[i] == 'a') {
			return i;
		}
	}
	return -1;
}

void MoveGeneration::permutation_boxes(const int position) {
	if(mgMap_Sokoban[position] == 'a') {
		mgMap_Sokoban[position] = '.';
		mgSteps++;
	}

	if(mgMap_Sokoban[position] == '@') {
		mgMap_Sokoban[position] = ' '; 
		mgSteps++;
	}
}

void MoveGeneration::move_generation(int direction) {
	int indexPositionUser = get_position_User();

	if(indexPositionUser != -1) {
		int step = 0;
		switch(direction) {
		case ID_LEFT:
			step = -1;
			break;
		case ID_RIGHT:
			step = 1;
			break;
		case ID_UP:
			step = -mgWidth_map;
			break;
		case ID_DOWN:
			step = mgWidth_map;
			break;
		}


		if(mgMap_Sokoban[indexPositionUser+step] == ' ') {
			mgMap_Sokoban[indexPositionUser+step] = '@';
			permutation_boxes(indexPositionUser);
		}

		if(mgMap_Sokoban[indexPositionUser+step] == '.') {
			mgMap_Sokoban[indexPositionUser+step] = 'a';
			permutation_boxes(indexPositionUser);
		}


		if(mgMap_Sokoban[indexPositionUser+step] == '$') {
			if(mgMap_Sokoban[indexPositionUser+2*step] == ' ') {
				mgMap_Sokoban[indexPositionUser+2*step] = '$';
				mgMap_Sokoban[indexPositionUser+step] = '@';
				permutation_boxes(indexPositionUser);
			}

			if(mgMap_Sokoban[indexPositionUser+2*step] == '.') {
				mgMap_Sokoban[indexPositionUser+2*step] = '*';
				mgMap_Sokoban[indexPositionUser+step] = '@';
				permutation_boxes(indexPositionUser);
			}
		}

		if(mgMap_Sokoban[indexPositionUser+step] == '*') {
			if(mgMap_Sokoban[indexPositionUser+2*step] == ' ') {
				mgMap_Sokoban[indexPositionUser+2*step] = '$';
				mgMap_Sokoban[indexPositionUser+step] = 'a';
				permutation_boxes(indexPositionUser);
			}

			if(mgMap_Sokoban[indexPositionUser+2*step] == '.') {
				mgMap_Sokoban[indexPositionUser+2*step] = '*';
				mgMap_Sokoban[indexPositionUser+step] = 'a';
				permutation_boxes(indexPositionUser);
			}
		}
	}
}