#ifndef _ASCELL_MANAGER_H_
#define _ASCELL_MANAGER_H_

#include "ASCell.h"
#include <vector>
enum class E_State
{
	NONE, OPEN, CLOSE
};
//�L�����N�^�[����������}�b�v��������N���X
class ASCellManager
{

public:
	ASCellManager();
	ASCellManager(std::vector<ASCell> c,int width,int height,Position startPos, Position endPos);
	~ASCellManager();
	//��������
	void Initialize();
	//�T���J�n�n�_�̐ݒ�
	void SetStartPosition(Position pos);
	//�T���I���n�_�̐ݒ�
	void SetGoalPosition(Position pos);
	//�w�肵���Z���ɗאڂ����Z���̂���NONE��Ԃ̂��̂�OPEN�ɂ��R�X�g�̐ݒ蓙���s��
	void OpenNeighCells(ASCell* cell);
	//OPEN��Ԃ̃Z�����`�F�b�N�B�����ɉ�����CLOSE��ԂɕύX�ɂ������ƁA��Z���̑I����s��
	void CheckOpenedCell();
	//�w�肵���Z���̃R�X�g�����̎��̃S�[���Z���Ɗ�Z������ɐݒ肷��
	void SetCost(ASCell* cells, ASCellSearch* cSearch);
	//���[�g�̃f�[�^��n��
	std::vector<Position> GetRoute();
	//�w�肵�����W�̃Z����Ԃ�
	ASCell* GetCell(Position pos);
	void StartSearch();

	Position GetStartPosition();

	Position GetGoalPosition();

	std::vector<E_State> cellStates;
	//Map����擾����Cell���
	std::vector<ASCell> cells;
	//��L�ɑΉ������T���p�ϐ�
	std::vector<ASCellSearch> cellSearches;

	std::vector<ASCell*> openedCells;
	std::vector<Position> routes;
private: 
	ASCell* startCell;
	ASCell* goalCell;
	ASCell* currentCell;


private:
	int cellsWidth=22, cellsHeight=22;
};

#endif

