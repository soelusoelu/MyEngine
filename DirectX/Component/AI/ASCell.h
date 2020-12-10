#pragma once
#include <vector>
#include"../../Math/Vector2.h"

struct Position {
	int x=0; int y=0;
};
struct ASCell
{
	//�}�b�v���i�}�b�v������̏����������s�v�ȃN���X
	Position position;
	int posNum;
	int cost = 1;
	bool canMove = true;
	std::vector<ASCell*> neighCells;

	//�T���p�̏��i�����������K�v�ȕϐ�
	//����Cell�O�Ɉڂ�
	int scoreE = 0, scoreR = 0, score = 0;
	ASCell* parent = nullptr;
};

