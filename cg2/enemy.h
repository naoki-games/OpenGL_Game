#pragma once

class enemy
{
public:
	enemy();
	~enemy();
	void setexi(bool e);
	void setdir(double d);
	void setpos(double px, double py);
	bool getexi();
	double getdir();
	double getposx();
	double getposy();

private:
	bool exist; //�G���G���A�ɑ��݂��Ă��邩�ǂ���
	double dir, posx, posy; //�G�p�̍��W�A����
};
