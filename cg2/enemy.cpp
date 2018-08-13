#include "stdafx.h"
#include "enemy.h"
#include <gl/glut.h>

enemy::enemy()
{
	exist = false;
}

enemy::~enemy()
{
}

void enemy::setexi(bool e) {
	exist = e;
}
void enemy::setdir(double d) {
	dir = d;
}
void enemy::setpos(double px, double py) {
	posx = px;
	posy = py;
}

bool enemy::getexi() {
	return exist;
}
double enemy::getdir() {
	return dir;
}
double enemy::getposx() {
	return posx;
}
double enemy::getposy() {
	return posy;
}