#include "stage3.h"

stage3::stage3()
{
	st_clear = false;
	groundDistancex = 7;
	enemy_num = 4;
	obposx[0] = (-groundDistancex / 2) + 1;
	obposx[1] = (groundDistancex / 2) - 1;
	obposx[2] = 1.0;
	obposy[0] = 0.0;
	obposy[1] = 0.0;
	obposy[2] = -3.0;
	nextStage = NULL;
}

stage3::~stage3()
{
}

void stage3::setclear(bool c) {
	st_clear = c;
}
bool stage3::getclear() {
	return st_clear;
}
void stage3::setnum(int n) {
	enemy_num = n;
}
int stage3::getnum() {
	return enemy_num;
}

void stage3::setdis(int d) {
	groundDistancex = d;
}
double stage3::getdisx() {
	return groundDistancex;
}
int stage3::getdisy() {
	return groundDistancey;
}
bool stage3::getdw() {
	return damageWall;
}
double* stage3::getObstaclePosx() {
	return obposx;
}
double* stage3::getObstaclePosy() {
	return obposy;
}
stage* stage3::getst() {
	return nextStage;
}

void stage3::myPillar(double x, double y, double z)
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

void stage3::myWall()
{
	float wall[2][4] = {
		{ 0.6f, 0.6f, 0.6f, 1.0f },
		{ 0.3f, 0.3f, 0.3f, 1.0f }
	};
	float Dwall[2][4] = {
		{ 0.8f, 0.2f, 0.2f, 1.0f },
		{ 0.6f, 0.0f, 0.0f, 1.0f }
	};
	float door[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	int i, j;
	glBegin(GL_QUADS);
	glNormal3d(0.0, 0.0, 1.0);
	//‰E‚Ì•Ç
	for (j = -3; j < 3; j++) {
		for (i = -groundDistancey; i < groundDistancey; i++) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Dwall[abs(i + j) % 2]); //wall[0]‚Å”’,wall[1]‚Å•
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
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Dwall[abs(i + j) % 2]);
			glVertex3d(-groundDistancex, (double)i, (double)j);
			glVertex3d(-groundDistancex, (double)(i + 1), (double)j);
			glVertex3d(-groundDistancex, (double)(i + 1), (double)(j + 1));
			glVertex3d(-groundDistancex, (double)i, (double)(j + 1));
		}
	}
	//‰æ–Ê‰œ‚Ì•Ç
	glNormal3d(0.0, 0.0, 1.0);
	for (j = -3; j < 3; j++) {
		for (i = -7.0; i < 7.0; i++) {
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

void stage3::myGround()
{
	float ground[2][4] = {
		{ 0.6f, 0.6f, 0.6f, 1.0f },
		{ 0.3f, 0.3f, 0.3f, 1.0f }
	};
	int i, j;
	glBegin(GL_QUADS);
	glNormal3d(0.0, 0.0, 1.0);
	for (j = -groundDistancey; j < groundDistancey; j++) {
		for (i = -7.0; i < 7.0; i++) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[abs(i + j) % 2]);
			glVertex3d((double)i, (double)j, 0.0);
			glVertex3d((double)(i + 1), (double)j, 0.0);
			glVertex3d((double)(i + 1), (double)(j + 1), 0.0);
			glVertex3d((double)i, (double)(j + 1), 0.0);
		}
	}
	glEnd();
}

void stage3::myDisplay()
{
	float gray[4] = { 0.5f, 0.5f, 0.5f, 0.8f };
	float white[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mtrl_shininess = 76.8f;

	glClearColor(0.0, 0.0, 0.0, 1.0);

	if (!st_clear) {
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogi(GL_FOG_START, 2);
		glFogi(GL_FOG_END, 10);
	}
	else glDisable(GL_FOG);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gray);

	if (!gdChange) {
		if (groundDistancex > 5) {
			groundDistancex -= 0.05;
		}
		else gdChange = true;
		
	}
	else {
		if (groundDistancex < 7) {
			groundDistancex += 0.05;
		}
		else gdChange = false;
	}

	glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
	myGround();
	myWall();
	glPushMatrix();

	glMaterialfv(GL_FRONT,GL_SHININESS, &mtrl_shininess);
	glTranslated(obposx[0], obposy[0], 2.5);
	myPillar(2.0, 2.0, 5.0);
	glPopMatrix();
	glPushMatrix();
	glTranslated(obposx[1], obposy[1], 2.5);
	myPillar(2.0, 2.0, 5.0);
	glPopMatrix();
	glPushMatrix();
	glTranslated(obposx[2], obposy[2], 2.5);
	myPillar(2.0, 2.0, 5.0);
	glPopMatrix();

}