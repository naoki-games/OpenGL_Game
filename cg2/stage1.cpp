#include "stage1.h"

stage1::stage1()
{
	st_clear = false;
	groundDistancex = 5;
	enemy_num = 2;
	nextStage = new stage2();
}

stage1::~stage1()
{
}

void stage1::setclear(bool c) {
	st_clear = c;
}
bool stage1::getclear() {
	return st_clear;
}
void stage1::setnum(int n) {
	enemy_num = n;
}
int stage1::getnum() {
	return enemy_num;
}

void stage1::setdis(int d) {
	groundDistancex = d;
}
double stage1::getdisx() {
	return groundDistancex;
}
int stage1::getdisy() {
	return groundDistancey;
}
bool stage1::getdw() {
	return damageWall;
}
double* stage1::getObstaclePosx() {
	return obposx;
}
double* stage1::getObstaclePosy() {
	return obposy;
}
stage* stage1::getst() {
	return nextStage;
}

void stage1::myWall()
{
	float wall[2][4] = {
		{ 0.6f, 0.6f, 0.6f, 1.0f },
		{ 0.3f, 0.3f, 0.3f, 1.0f }
	};
	float door[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	int i, j;
	glBegin(GL_QUADS);
	glNormal3d(0.0, 0.0, 1.0);
	//‰E‚Ì•Ç
	for (j = -3; j < 3; j++) {
		for (i = -groundDistancey; i < groundDistancey; i++) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, wall[abs(i + j) % 2]);
			glVertex3d(5.0, (double)i, (double)j);
			glVertex3d(5.0, (double)(i + 1), (double)j);
			glVertex3d(5.0, (double)(i + 1), (double)(j + 1));
			glVertex3d(5.0, (double)i, (double)(j + 1));
		}
	}
	//¶‚Ì•Ç
	glNormal3d(0.0, 0.0, 1.0);
	for (j = -3; j < 3; j++) {
		for (i = -groundDistancey; i < groundDistancey; i++) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, wall[abs(i + j) % 2]);
			glVertex3d(-5.0, (double)i, (double)j);
			glVertex3d(-5.0, (double)(i + 1), (double)j);
			glVertex3d(-5.0, (double)(i + 1), (double)(j + 1));
			glVertex3d(-5.0, (double)i, (double)(j + 1));
		}
	}
	//‰æ–Ê‰œ‚Ì•Ç
	glNormal3d(0.0, 0.0, 1.0);
	for (j = -3; j < 3; j++) {
		for (i = -groundDistancex; i < groundDistancex; i++) {
			if (i >= -1 && i <= 1) {
				if (!st_clear) {
					glMaterialfv(GL_FRONT, GL_DIFFUSE, door);
					glVertex3d((double)i, groundDistancey, (double)j);
					glVertex3d((double)(i + 1), groundDistancey, (double)j);
					glVertex3d((double)(i + 1), groundDistancey, (double)(j + 1));
					glVertex3d((double)i, groundDistancey, (double)(j + 1));
				}
			}
			else {
				glMaterialfv(GL_FRONT, GL_DIFFUSE, wall[abs(i + j) % 2]);
				glVertex3d((double)i, groundDistancey, (double)j);
				glVertex3d((double)(i + 1), groundDistancey, (double)j);
				glVertex3d((double)(i + 1), groundDistancey, (double)(j + 1));
				glVertex3d((double)i, groundDistancey, (double)(j + 1));
			}

		}
	}
	glEnd();
}

void stage1::myGround()
{
	float ground[2][4] = {
		{ 0.6f, 0.6f, 0.6f, 1.0f },
		{ 0.3f, 0.3f, 0.3f, 1.0f }
	};
	int i, j;
	glBegin(GL_QUADS);
	glNormal3d(0.0, 0.0, 1.0);
	for (j = -groundDistancey; j < groundDistancey; j++) {
		for (i = -groundDistancex; i < groundDistancex; i++) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[abs(i + j) % 2]);
			glVertex3d((double)i, (double)j, 0.0);
			glVertex3d((double)(i + 1), (double)j, 0.0);
			glVertex3d((double)(i + 1), (double)(j + 1), 0.0);
			glVertex3d((double)i, (double)(j + 1), 0.0);
		}
	}
	glEnd();
}

void stage1::myDisplay()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_FOG);
	myGround();
	myWall();
	
}