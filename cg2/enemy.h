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
	bool exist; //敵がエリアに存在しているかどうか
	double dir, posx, posy; //敵用の座標、方向
};
