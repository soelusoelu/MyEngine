#pragma once
#include <vector>
#include "ASCell.h"
#include "ASCellManager.h"
#include "../Component.h"
#include"../../Math/Vector3.h"
#include "../Character/ICharacterManager.h"
class CharacterCommonComponents;


class ASAI :public Component
{
public:
	ASAI(GameObject& obj);
	~ASAI();
	void Initialize();
	virtual void start()override;
	void originalUpdate();

	//�ŋ߂̓G�̏����擾����
	Vector3 GetNearEnemy();

	//float JudgeCharacter(CharacterPtr chara, float distance);


private:
	Vector3 CalcPosition(int phase);

	Position VectorToPosition(const Vector3& v);

	std::unique_ptr<ASCellManager> cellManager;
	std::unique_ptr < ASCellManager> target;
	std::vector<Position> routes;
	int routePhase=0;
	float mapWidth = 200;
	float mapHeight = 200;
	int cellCountW= 10;
	int cellCountH = 10;
	Vector3 routePoint;
	int startX=0, startY=0;
	int goalX = 0, goalY = 0;
	//ICharacterManager manager;
	const ICharacterManager* manager;
	//Position start;
	//Position goal;
	//std::vector<ASCell>&cells;
};

