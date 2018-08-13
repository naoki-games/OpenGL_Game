#pragma once

#include "stdafx.h"
#include "enemy.cpp"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <gl/glut.h>

class stage {
public:
	virtual void myDisplay() = 0;
	virtual void setclear(bool c) {}
	virtual bool getclear() = 0;
	virtual void setnum(int n){}
	virtual int getnum() = 0;
	virtual void setdis(int d){}
	virtual double getdisx() = 0;
	virtual int getdisy() = 0;
	virtual bool getdw() = 0;
	virtual double* getObstaclePosx() = 0;
	virtual double* getObstaclePosy() = 0;
	virtual stage* getst() = 0;

private:


};

