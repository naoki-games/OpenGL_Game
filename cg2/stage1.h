#pragma once
#include "stage.h"
#include "stage2.h"

class stage1 : public stage {
private:
	int enemy_num; //実際にenemyがフィールドに出現する数
	int groundDistancex; //接地可能距離x
	int groundDistancey = 5; //接地可能距離y
	double obposx[3] = { NULL,NULL,NULL }; //障害物座標x
	double obposy[3] = { NULL,NULL,NULL }; //障害物座標y
	bool damageWall = false;
	bool st_clear; //現ステージクリア判定
	stage* nextStage; //次ステージを指定

public:
	stage1();
	~stage1();
	virtual void myDisplay();
	virtual void setclear(bool c);
	virtual bool getclear();
	virtual void setnum(int n);
	virtual int getnum();
	virtual void setdis(int d);
	virtual double getdisx();
	virtual int getdisy();
	virtual bool getdw();
	virtual double* getObstaclePosx();
	virtual double* getObstaclePosy();
	virtual stage* getst();
	void myWall();
	void myGround();

};
