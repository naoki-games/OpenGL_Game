#include "stage2.h"

stage2::stage2()
{
	st_clear = false;
	enemy_num = 3;
	groundDistancex = 6;
	obposx[0] = -groundDistancex / 2;
	obposx[1] = groundDistancex / 2;
	obposy[0] = 0.0;
	obposy[1] = 0.0;
	nextStage = new stage3();
}

stage2::~stage2()
{
}

void stage2::setclear(bool c) {
	st_clear = c;
}
bool stage2::getclear() {
	return st_clear;
}
void stage2::setnum(int n) {
	enemy_num = n;
}
int stage2::getnum() {
	return enemy_num;
}

void stage2::setdis(int d) {
	groundDistancex = d;
}
double stage2::getdisx() {
	return groundDistancex;
}
int stage2::getdisy() {
	return groundDistancey;
}
bool stage2::getdw() {
	return damageWall;
}
double* stage2::getObstaclePosx() {
	return obposx;
}
double* stage2::getObstaclePosy() {
	return obposy;
}
stage* stage2::getst() {
	return nextStage;
}

void stage2::myPillar(double x, double y, double z)
{
	glBegin(GL_QUADS);
	glNormal3d(0.0, 0.0, 1.0);
	glVertex3d(-x / 2.0, -y / 2.0, 0.0);
	glVertex3d(x / 2.0, -y / 2.0, 0.0);
	glVertex3d(x / 2.0, y / 2.0, 0.0);
	glVertex3d(-x / 2.0, y / 2.0, 0.0);
	glNormal3d(1.0, 0.0, 0.0);
	glVertex3d(x / 2.0, -y / 2.0, 0.0);
	glVertex3d(x / 2.0, -y / 2.0, -z);
	glVertex3d(x / 2.0, y / 2.0, -z);
	glVertex3d(x / 2.0, y / 2.0, 0.0);
	glNormal3d(0.0, 1.0, 0.0);
	glVertex3d(x / 2.0, y / 2.0, 0.0);
	glVertex3d(x / 2.0, y / 2.0, -z);
	glVertex3d(-x / 2.0, y / 2.0, -z);
	glVertex3d(-x / 2.0, y / 2.0, 0.0);
	glNormal3d(-1.0, 0.0, 0.0);
	glVertex3d(-x / 2.0, y / 2.0, 0.0);
	glVertex3d(-x / 2.0, y / 2.0, -z);
	glVertex3d(-x / 2.0, -y / 2.0, -z);
	glVertex3d(-x / 2.0, -y / 2.0, 0.0);
	glNormal3d(0.0, -1.0, 0.0);
	glVertex3d(-x / 2.0, -y / 2.0, 0.0);
	glVertex3d(-x / 2.0, -y / 2.0, -z);
	glVertex3d(x / 2.0, -y / 2.0, -z);
	glVertex3d(x / 2.0, -y / 2.0, 0.0);
	glEnd();
}

void stage2::myWall()
{
	float wall[2][4] = {
		{ 0.6f, 0.6f, 0.6f, 1.0f },
		{ 0.3f, 0.3f, 0.3f, 0.5f }
	};
	float door[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	int i, j;
	glBegin(GL_QUADS);
	glNormal3d(0.0, 0.0, 1.0);
	//‰E‚Ì•Ç
	for (j = -3; j < 3; j++) {
		for (i = -groundDistancey; i < groundDistancey; i++) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, wall[abs(i + j) % 2]);
			glVertex3d(groundDistancex, (double)i, (double)j);
			glVertex3d(groundDistancex, (double)(i + 1), (double)j);
			glVertex3d(groundDistancex, (double)(i + 1), (double)(j + 1));
			glVertex3d(groundDistancex, (double)i, (double)(j + 1));
		}
	}
	//¶‚Ì•Ç
	glNormal3d(0.0, 0.0, 1.0);
	for (j = -3; j < 3; j++) {
		for (i = -groundDistancey; i < groundDistancey; i++) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, wall[abs(i + j) % 2]);
			glVertex3d(-groundDistancex, (double)i, (double)j);
			glVertex3d(-groundDistancex, (double)(i + 1), (double)j);
			glVertex3d(-groundDistancex, (double)(i + 1), (double)(j + 1));
			glVertex3d(-groundDistancex, (double)i, (double)(j + 1));
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

void stage2::myGround()
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

void stage2::myDisplay()
{
	float gray[4] = { 0.5f, 0.5f, 0.5f, 0.8f };
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_FOG);

	myGround();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myWall();
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
	glTranslated(obposx[0], obposy[0], 2.5);
	myPillar(2.0, 2.0, 5.0);
	glPopMatrix();
	glPushMatrix();
	glTranslated(obposx[1], obposy[1], 2.5);
	myPillar(2.0, 2.0, 5.0);
	glPopMatrix();

	glDisable(GL_BLEND);
}