#pragma once
#include "stage.h"

class stage3 : public stage {
private:
	int enemy_num; //���ۂ�enemy���t�B�[���h�ɏo�����鐔
	double groundDistancex; //�ڒn�\����x
	int groundDistancey = 5; //�ڒn�\����y
	double obposx[4] = { NULL,NULL,NULL,NULL }; //��Q�����Wx
	double obposy[4] = { NULL,NULL,NULL,NULL }; //��Q�����Wy
	bool gdChange = false;
	bool damageWall = true;
	bool st_clear; //���X�e�[�W�N���A����
	stage* nextStage; //���X�e�[�W���w��

public:
	stage3();
	~stage3();
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
	void myPillar(double x, double y, double z);
};