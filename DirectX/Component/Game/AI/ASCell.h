#pragma once
#include <vector>
#include"../../../Math/Vector2.h"

struct Position {
	int x=0; int y=0;
	bool operator==(const Position p)const
	{
		if (x == p.x && y == p.y)return true;
		else return false;
	}
};
struct ASCell
{
	//�}�b�v���i�}�b�v������̏����������s�v�ȃN���X
	Position position;
	int posNum;//���X�g���ŉ��Ԗڂ�
	int cost = 1;//����Cell�ŗL�̃R�X�g
	bool canMove = true;//�ړ��\���s�\��
	std::vector<ASCell*> neighCells;//�אڂ���Cell
};

struct ASCellSearch
{
	//�T���p�̏��i�����������K�v�ȕϐ�
	//����Cell�O�Ɉڂ�
	int scoreE = 0, scoreR = 0, score = 0;
	ASCell* parent = nullptr;
};

