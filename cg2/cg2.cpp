// cg2.cpp : コンソールアプリケーションのエントリポイントを定義します。
//
/*敵と接触したときの状態によって勝敗が変わるゲームを実装するプログラムです.
初期画面はタイトル画面が描画され、spaceキーを押すことでゲーム画面の描画に遷移します.
ゲーム画面ではplayerキャラは方向キーで移動します.
敵の状態は、10秒毎に変化し、赤い状態で接触するとゲームオーバ、
青い状態で接触するとゲームクリアになります.
*/

#include "stdafx.h"
#include "stage.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "enemy.h"

#include "stage1.cpp"
#include "stage2.cpp"
#include "stage3.cpp"
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <gl/glut.h>

#define FIRST_STAGECLASS stage1() //初期ステージクラス

int winw, winh;
const int samplingTime = 50; //msで指定
const int samplingTime2 = 1000; //1s
const int samplingTime3 = 10; //SubTimer用

int key; //2つ同時にキーが押されているか

bool gameStart = false; //ゲーム中にtrue
bool gameEnd = false; //ゲーム終了時にtrue
bool gameClear = false; //ゲームクリア時にtrue

int stage_num = 0; //ステージが進む毎にカウント
char str_statime[10];

bool cameraMode = false; //カメラがプレイヤーに近づく(エリア移動演出),演出中にtrue
int cameraModeTime = 30; //cameraModeの時間(200で1秒)
int ctimer = 0; //cameraModeの時間カウント用変数

bool enemyMode = true; //true:追跡モード false:逃走モード
int changetime = 10; //敵のモード変更時間

//ゲームが開始してから終了するまでのタイムを保存([s]でカウント)
int second = 0; 
int minute = 0;

//ゲーム画面中の文字表示用変数
char str_chtime[20];
char str_e_num[20];
char str_gtime[30];

const int t_enemy_num = 5; //enemyの総出現数
int enemy_num; //実際にenemyがフィールドに出現する数の一時変数
double ecounter = 0; //敵を何体倒したかをカウントする変数

bool animMode = false; //true:歩行モーション再生 false:停止
bool walkMode = false; //true:移動中 false:移動停止
double speedx, speedy;
double WALKSPEED[2] = { 0, 0.05 };
double dir, posx, posy;
double cposx = 0.0, cposy = -5.0, cposz = 4.0; //カメラの座標
float light0_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //光源色の初期値
int timestep, cycle[2];

stage *cuStage, *nextStage;
enemy ene[t_enemy_num];

//関数のプロトタイプ宣言
void myReshape(int width, int height);
void myInit(char *progname);
void areaStartInit();
void gameStartInit();
void toNextStage();

//myHuman関数のパーツを生成する関数
void myBox(double x, double y, double z)
{
	glBegin(GL_QUADS);
	glNormal3d(0.0, 0.0, 1.0);
	glVertex3d(-x / 2.0, -y / 2.0, 0.0);
	glVertex3d(x / 2.0, -y / 2.0, 0.0);
	glVertex3d(x / 2.0, y / 2.0, 0.0);
	glVertex3d(-x / 2.0, y / 2.0, 0.0);
	glNormal3d(0.0, 0.0, -1.0);
	glVertex3d(-x / 2.0, y / 2.0, -z);
	glVertex3d(x / 2.0, y / 2.0, -z);
	glVertex3d(x / 2.0, -y / 2.0, -z);
	glVertex3d(-x / 2.0, -y / 2.0, -z);
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

//playerキャラのモーションを制御する関数
void myHuman(int t, int c) { 
	double leg1right, leg1left, leg2right, leg2left;
	double arm1right, arm1left, arm2right, arm2left;
	double a = (double)(t % c) / (double)c;
	leg1right = -30 * cos(2.0 * M_PI * a + 2.0 * M_PI);
	leg1left = 30 * cos(2.0 * M_PI * a + 2.0 * M_PI);
	leg2right = -40 * cos(2 * M_PI * a + 2.0 * M_PI) - 10;
	leg2left = 40 * cos(2.0 * M_PI * a + 2.0 * M_PI) - 10;
	arm1left = -30 * cos(2 * M_PI * a + 2.0 * M_PI);
	arm1right = 30 * cos(2 * M_PI * a + 2.0 * M_PI);
	arm2left = -40 * cos(2 * M_PI * a + 2.0 * M_PI) + 20;
	arm2right = 40 * cos(2 * M_PI * a + 2.0 * M_PI) + 20;

	if (animMode) { //移動中は歩行モーション再生
		myBox(0.15, 0.15, 0.25); // head
		glTranslated(0.0, 0.0, -0.3);
		myBox(0.45, 0.25, 0.65); // body
		glPushMatrix();
		glTranslated(0.3, 0.0, 0.0);
		glRotated(arm1right, 1.0, 0.0, 0.0);
		myBox(0.15, 0.15, 0.35); // arm right shoulder
		glTranslated(0.0, 0.0, -0.4);
		glRotated(arm2right, 1.0, 0.0, 0.0);
		myBox(0.15, 0.15, 0.35); // arm right elbow
		glPopMatrix();
		glPushMatrix();
		glTranslated(-0.3, 0.0, 0.0);
		glRotated(arm1left, 1.0, 0.0, 0.0);
		myBox(0.15, 0.15, 0.35); // arm left shoulder
		glTranslated(0.0, 0.0, -0.4);
		glRotated(arm2left, 1.0, 0.0, 0.0);
		myBox(0.15, 0.15, 0.35); // arm left elbow
		glPopMatrix();
		glPushMatrix();
		glTranslated(0.125, 0.0, -0.7);
		glRotated(leg1right, 1.0, 0.0, 0.0);
		myBox(0.2, 0.2, 0.4); // leg right hip
		glTranslated(0.0, 0.0, -0.45);
		glRotated(leg2right, 1.0, 0.0, 0.0);
		myBox(0.2, 0.2, 0.4); // leg right knee
		glPopMatrix();
		glTranslated(-0.125, 0.0, -0.7);
		glRotated(leg1left, 1.0, 0.0, 0.0);
		myBox(0.2, 0.2, 0.4); // leg left hip
		glTranslated(0.0, 0.0, -0.45);
		glRotated(leg2left, 1.0, 0.0, 0.0);
		myBox(0.2, 0.2, 0.4); // leg left knee
	}
	else { //歩行モーション停止
		myBox(0.15, 0.15, 0.25); // head
		glTranslated(0.0, 0.0, -0.3);
		myBox(0.45, 0.25, 0.65); // body
		glPushMatrix();
		glTranslated(0.3, 0.0, 0.0);
		myBox(0.15, 0.15, 0.35); // arm right shoulder
		glTranslated(0.0, 0.0, -0.4);
		myBox(0.15, 0.15, 0.35); // arm right elbow
		glPopMatrix();
		glPushMatrix();
		glTranslated(-0.3, 0.0, 0.0);
		myBox(0.15, 0.15, 0.35); // arm left shoulder
		glTranslated(0.0, 0.0, -0.4);
		myBox(0.15, 0.15, 0.35); // arm left elbow
		glPopMatrix();
		glPushMatrix();
		glTranslated(0.125, 0.0, -0.7);
		myBox(0.2, 0.2, 0.4); // leg right hip
		glTranslated(0.0, 0.0, -0.45);
		myBox(0.2, 0.2, 0.4); // leg right knee
		glPopMatrix();
		glTranslated(-0.125, 0.0, -0.7);
		myBox(0.2, 0.2, 0.4); // leg left hip
		glTranslated(0.0, 0.0, -0.45);
		myBox(0.2, 0.2, 0.4); // leg left knee
	}

}

//enemyキャラのモーションを制御する関数
void myHuman2(int t, int c) { 
	double leg1right, leg1left, leg2right, leg2left;
	double arm1right, arm1left, arm2right, arm2left;
	double a = (double)(t % c) / (double)c;
	leg1right = -30 * cos(2.0 * M_PI * a + 2.0 * M_PI);
	leg1left = 30 * cos(2.0 * M_PI * a + 2.0 * M_PI);
	leg2right = -40 * cos(2 * M_PI * a + 2.0 * M_PI) - 10;
	leg2left = 40 * cos(2.0 * M_PI * a + 2.0 * M_PI) - 10;
	arm1left = -30 * cos(2 * M_PI * a + 2.0 * M_PI);
	arm1right = 30 * cos(2 * M_PI * a + 2.0 * M_PI);
	arm2left = -40 * cos(2 * M_PI * a + 2.0 * M_PI) + 20;
	arm2right = 40 * cos(2 * M_PI * a + 2.0 * M_PI) + 20;

	myBox(0.15, 0.15, 0.25); // head
	glTranslated(0.0, 0.0, -0.3);
	myBox(0.45, 0.25, 0.65); // body
	glPushMatrix();
	glTranslated(0.3, 0.0, 0.0);
	glRotated(arm1right, 1.0, 0.0, 0.0);
	myBox(0.15, 0.15, 0.35); // arm right shoulder
	glTranslated(0.0, 0.0, -0.4);
	glRotated(arm2right, 1.0, 0.0, 0.0);
	myBox(0.15, 0.15, 0.35); // arm right elbow
	glPopMatrix();
	glPushMatrix();
	glTranslated(-0.3, 0.0, 0.0);
	glRotated(arm1left, 1.0, 0.0, 0.0);
	myBox(0.15, 0.15, 0.35); // arm left shoulder
	glTranslated(0.0, 0.0, -0.4);
	glRotated(arm2left, 1.0, 0.0, 0.0);
	myBox(0.15, 0.15, 0.35); // arm left elbow
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.125, 0.0, -0.7);
	glRotated(leg1right, 1.0, 0.0, 0.0);
	myBox(0.2, 0.2, 0.4); // leg right hip
	glTranslated(0.0, 0.0, -0.45);
	glRotated(leg2right, 1.0, 0.0, 0.0);
	myBox(0.2, 0.2, 0.4); // leg right knee
	glPopMatrix();
	glTranslated(-0.125, 0.0, -0.7);
	glRotated(leg1left, 1.0, 0.0, 0.0);
	myBox(0.2, 0.2, 0.4); // leg left hip
	glTranslated(0.0, 0.0, -0.45);
	glRotated(leg2left, 1.0, 0.0, 0.0);
	myBox(0.2, 0.2, 0.4); // leg left knee
}

//文字を描画する関数
void render_string(float x, float y, const char* string, void* font) 
{
	float z = -1.0f;
	glRasterPos3f(x, y, z);
	char* p = (char*)string;
	while (*p != '\0') glutBitmapCharacter(font, *p++);
}

//タイトル画面を描画する関数
void TitleDisplay()
{
	//初期化
	enemyMode = true;
	gameClear = false;
	changetime = 10;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, (double)winw / (double)winh, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, -7.0, 7.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1, 1, 1);
	render_string(-1.5, 1.0, "Escape from red human", GLUT_BITMAP_TIMES_ROMAN_24);
	render_string(-0.4f, -1.0f, "press space key", GLUT_BITMAP_HELVETICA_18);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glutSwapBuffers();

}

//ゲーム画面を描画する関数
void myDisplay()
{
	double dir2, posx2, posy2; //敵用の座標、方向を一時保存する変数

	float white[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float red[4] = { 0.8f, 0.2f, 0.2f, 1.0f };
	float blue[4] = { 0.2f, 0.2f, 0.8f, 1.0f };
	float light1_position[4] = { 0.0f, 0.0f, 2.0f, 1.0f };
	float light2_position[4] = { 2.0f, 5.0f, -7.0f, 1.0f }; //扉左の光源
	float light3_position[4] = { -2.0f, 5.0f, -7.0f, 1.0f }; //扉右の光源
	//float light4_position[4] = { -cposx+posx, posy, -cposz, 1.0f }; 未実装
	float light1_scolor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float light2_3_color[4] = { 0.5f, 0.5f, 0.0f, 1.0f };

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, (double)winw / (double)winh, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(cameraMode) {
		cposx = posx;
		if(cposy != posy) cposy += 0.2;
		if (cposz >= 0.8) cposz -= 0.2;
		gluLookAt(cposx, cposy, cposz, posx, posy, 0.0, 0.0, 0.0, 1.0);
	}
	else {
		cposx = posx/4;
		gluLookAt(cposx, cposy, cposz, posx / 4, posy / 4, 0.0, 0.0, 0.0, 1.0);
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glPushMatrix();
	glTranslated(posx, posy, 1.9);
	glRotated(dir, 0.0, 0.0, 1.0);
	myHuman(timestep, cycle[0]);
	glPopMatrix();
	//enemyModeによって色を変更する.
	if (enemyMode) {
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light1_scolor);
	}
	else {
		glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, blue);
		glLightfv(GL_LIGHT1, GL_SPECULAR, blue);
	}

	for (int i = 0; i < enemy_num; i++) {
		if (ene[i].getexi()) {
			glPushMatrix();
			dir2 = ene[i].getdir();
			posx2 = ene[i].getposx();
			posy2 = ene[i].getposy();
			glTranslated(posx2, posy2, 1.9);
			glRotated(dir2, 0.0, 0.0, 1.0);
			myHuman2(timestep, cycle[1]);
			glPopMatrix();
		}
	}

	cuStage->myDisplay();

	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	if (enemyMode) render_string(-1.8f, -1.3f, "EnemyMode:Pursuit", GLUT_BITMAP_HELVETICA_18);
	else render_string(-1.8f, -1.3f, "EnemyMode:Escape", GLUT_BITMAP_HELVETICA_18);
	render_string(-1.8f, -1.5f, str_chtime, GLUT_BITMAP_HELVETICA_18);
	render_string(-1.0f, -1.3f, str_statime, GLUT_BITMAP_HELVETICA_18);
	render_string(-1.0f, -1.5f, str_e_num, GLUT_BITMAP_HELVETICA_18);
	render_string(0.0f, -1.5f, str_gtime, GLUT_BITMAP_HELVETICA_18);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	if (cuStage->getclear()) {
		glDisable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
	}
	else {
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
	}
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_3_color);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light2_3_color);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glutSwapBuffers();
}

//リザルト画面を描画する関数
void EndDisplay() //ゲーム終了時の画面を描画する関数
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, (double)winw / (double)winh, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, -7.0, 7.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	if (gameClear) {
		glColor3f(1, 1, 0); //文字色：黄
		render_string(-0.4f, 0.5f, "Game Clear!!", GLUT_BITMAP_TIMES_ROMAN_24);
		render_string(-0.4f, 0.0f, str_gtime, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else {
		glColor3f(1, 0, 0); //文字色：赤
		render_string(-0.4f, 0.5f, "Game Over", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	render_string(-0.4f, -1.0f, "press space key", GLUT_BITMAP_HELVETICA_18);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glutSwapBuffers();

}

//次ステージ遷移時に処理される関数
void toNextStage() {
	delete cuStage;
	cuStage = nextStage;
	if (cuStage == NULL) {
		gameEnd = true;
		gameClear = true;
		snprintf(str_gtime, 30, "ClearTime  %d:%d", minute, second);
		glutDisplayFunc(EndDisplay);
	}
}

//エリア開始時に必要な初期化を行う関数
void areaStartInit() {
	timestep = 0;

	stage_num++;
	snprintf(str_statime, 30, "Stage : %d", stage_num);
	nextStage = cuStage->getst(); //次ステージを取得
	enemy_num = cuStage->getnum();
	ecounter = 0;

	for (int i = 0; i < enemy_num; i++) {
		ene[i].setexi(true);
		ene[i].setdir(0.0);
		ene[i].setpos(i -3.0, 4.0);
	}

	dir = -90.0;
	posx = 0.0;
	posy = -cuStage->getdisy()+1;
	cycle[0] = 20;

	//printf("%f ", posx); debug用

	cposx = 0.0;
	cposy = -cuStage->getdisy();
	cposz = 4.0;
	cycle[1] = 40;
}

//ゲーム開始時に必要な初期化を行う変数
void gameStartInit() {
	second = 0;
	minute = 0;
	stage_num = 0;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
	cuStage = new FIRST_STAGECLASS;
	areaStartInit();
	gameClear = false;
	gameEnd = false;
	gameStart = false;
}

//エリアの移動制限をする関数
double arealimit(double a ,double b) {
	if (a > b - 0.5) {
		a = b - 0.5;
	}
	if (a < -b + 0.5) {
		a = -b + 0.5;
	}

	return a;
}

//ゲーム画面中にある全ての3Dオブジェクトを計算する関数
void myMainTimer(int value)
{
	const double SMALLVAL = 0.01;
	double speedx2, speedy2;
	double dir2, posx2, posy2; //敵用の座標、方向を一時保存する変数

	double vectorx, vectory;
	double rad;

	double *Obposx;
	double *Obposy;

	double limitx, limity;

	if (value == 1) {
		timestep++;
		Obposx = cuStage->getObstaclePosx();
		Obposy = cuStage->getObstaclePosy();
		limitx = cuStage->getdisx();
		limity = cuStage->getdisy();

		//ここからplayer処理

		//playerの移動処理
		if (walkMode || animMode) {
			speedx = WALKSPEED[0] * cos((dir + 90.0) / 180.0 * M_PI);
			speedy = WALKSPEED[0] * sin((dir + 90.0) / 180.0 * M_PI);
			posx += speedx;
			posy += speedy;
			walkMode = false;
		}
		

		//printf("pl:%f ", posx); debug用
		//printf("gd:%f ", limitx); debug用

		//接地可能エリアを指定
		if (!cuStage->getdw()) {
			posx = arealimit(posx, limitx);
		}
		else {
			if (posx > limitx) {
				gameEnd = true;
				glutDisplayFunc(EndDisplay);
			}
			if (posx < -limity) {
				gameEnd = true;
				glutDisplayFunc(EndDisplay);
			}
		}
		posy = arealimit(posy, limity);

		//ここまでplayer処理

		//ここからenemy処理

		for (int i = 0; i < enemy_num; i++) {
			if (ene[i].getexi()) {
				//enemyクラスの各パラメータを一時変数に格納
				dir2 = ene[i].getdir();
				posx2 = ene[i].getposx();
				posy2 = ene[i].getposy();

				//enemyの移動処理
				speedx2 = WALKSPEED[1] * cos((dir2 + 90.0) / 180.0 * M_PI);
				speedy2 = WALKSPEED[1] * sin((dir2 + 90.0) / 180.0 * M_PI);
				posx2 += speedx2;
				posy2 += speedy2;

				//ここからenemyの移動方向算出

				vectorx = posx2 - posx; //playerの座標に対してのxベクトル
				vectory = posy - posy2; //playerの座標に対してのyベクトル

				//printf("%f %f",posx2,posy2); debug用

				if (enemyMode) rad = atan2(vectorx, vectory); //角度(ラジアン)
				else rad = atan2(-vectorx, -vectory); //逃走モードならそれぞれのベクトルに-1を乗算する.
				dir2 = rad * 180.0 / (atan(1.0) * 4.0); //角度(度) 

				//ここまでがenemyの移動方向算出

				//playerがenemyに接触時の処理
				if (abs(vectorx) <= 0.3) {
					if (abs(vectory) <= 0.3) {
						if (enemyMode) {
							gameEnd = true;
							glutDisplayFunc(EndDisplay);
						}
						else {
							ene[i].setexi(false);
							ecounter++;
						}
					}
				}

				//enemy同士の当たり判定の処理
				for (int j = i + 1; j < enemy_num; j++) {
					vectorx = posx2 - ene[j].getposx(); //他のenemyの座標に対してのxベクトル
					vectory = ene[j].getposy() - posy2; //他のenemyの座標に対してのyベクトル

					if (abs(vectorx) <= 0.3) {
						if (abs(vectory) <= 0.3) {
							posx2 -= speedx2;
							posy2 -= speedy2;
						}
					}
				}

				//障害物接触時のenemy処理
				while (*Obposx != NULL)
				{
					vectorx = *Obposx - posx2; //xベクトル
					vectory = posy2 - *Obposy; //yベクトル

					if (abs(vectorx) <= 1.0) {
						if (abs(vectory) <= 1.0) {
							posx2 -= speedx2;
							posy2 -= speedy2;
						}
					}
					Obposx++;
					Obposy++;
				}
				//初期化
				Obposx = cuStage->getObstaclePosx();
				Obposy = cuStage->getObstaclePosy();

				//接地可能エリアを指定
				posx2 = arealimit(posx2, cuStage->getdisx());
				posy2 = arealimit(posy2, cuStage->getdisy());

				//一時変数の計算結果をenemyクラスに返す
				ene[i].setdir(dir2);
				ene[i].setpos(posx2, posy2);
			}

		}

		if (ecounter >= enemy_num) { //出現している全ての敵が倒されたとき
			cuStage->setclear(true);
		}

		//ここまでenemy処理

		//障害物接触時のplayer処理
		while (*Obposx != NULL)
		{
			vectorx = *Obposx - posx; //xベクトル
			vectory = posy - *Obposy; //yベクトル

			if (abs(vectorx) <= 1.0) {
				if (abs(vectory) <= 1.0) {
					posx -= speedx;
					posy -= speedy;
				}
			}
			Obposx++;
			Obposy++;
		}

		//敵を全て倒したとき
		if (cuStage->getclear()) {
			vectorx = 0.0 - posx; //xベクトル
			vectory = posy - 5.0; //yベクトル

			//ゴールポイント接触時の処理
			if (abs(vectorx) <= 1.5) {
				if (abs(vectory) <= 1.0) {
					if (!cameraMode) cameraMode = true;
					else ctimer++;
					printf("%d", ctimer);
				}
			}
		}
		//エリア移動時処理
		if (cameraMode && ctimer > cameraModeTime) {
			ctimer = 0;
			cameraMode = false;
			toNextStage();
			if (!gameEnd) {
				areaStartInit();
				glutDisplayFunc(myDisplay);
			}
		}

	}
	if (gameStart && !gameEnd) {
		glutTimerFunc(samplingTime, myMainTimer, 1);

	}
	else glutTimerFunc(samplingTime, myMainTimer, 0);

	glutPostRedisplay();
}

//ゲーム画面中に表示される文字を管理する関数
void TimeCount(int value) {
	if (value == 1) {
		if (changetime > 0) {
			changetime--;
		}
		else {
			changetime = 10;
			enemyMode = !enemyMode;
		}
		snprintf(str_chtime, 20, "ChangeTime : %d", changetime);

		if (!gameClear) {
			second++;
			if (second == 60) {
				second = 0;
				minute++;
			}
		}
		if (!gameClear) snprintf(str_gtime, 30, "GameTime  %d:%d", minute, second);
		else snprintf(str_gtime, 30, "ClearTime  %d:%d", minute, second);

		int num = enemy_num - ecounter;
		snprintf(str_e_num, 20, "enemy_number : %d", num);
	}

	if (gameStart) {
		glutTimerFunc(samplingTime2, TimeCount, 1);
	}
	else glutTimerFunc(samplingTime, TimeCount, 0);

}

void inputup(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		animMode = false;
		
	}
	else if (key == GLUT_KEY_DOWN) {
		animMode = false;
		
	}
	else if (key == GLUT_KEY_LEFT) {
		animMode = false;
		
	}
	else if (key == GLUT_KEY_RIGHT) {
		animMode = false;
	}
}

void input(int key, int x, int y) {
	if (!cameraMode) {
		if (key == GLUT_KEY_UP) {
			animMode = true;
			walkMode = true;
			dir = 0;
			WALKSPEED[0] = 0.1;
		}
		if (key == GLUT_KEY_DOWN) {
			animMode = true;
			walkMode = true;
			dir = 180;
			WALKSPEED[0] = 0.1;
		}
		if (key == GLUT_KEY_LEFT) {
			animMode = true;
			walkMode = true;
			dir = 90;
			WALKSPEED[0] = 0.1;
		}
		if (key == GLUT_KEY_RIGHT) {
			animMode = true;
			walkMode = true;
			dir = 270;
			WALKSPEED[0] = 0.1;
		}
	}
}

void myReshape(int width, int height)
{
	winw = width;
	winh = height;
	glViewport(0, 0, winw, winh);
}

void myKeyboard(unsigned char key, int x, int y) //キー入力
{
	if (!gameStart) {
		if (key == 0x20) { //spaceキー
			glutDisplayFunc(myDisplay);
			gameStart = true;
		}
	}
	if (gameEnd) {
		if (key == 0x20) { //spaceキー
			gameStartInit();
			glutDisplayFunc(TitleDisplay);
		}
	}
	if (key == 0x1B) exit(0); //escキー
}

void myInit(char *progname) //初期化
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(progname);
	gameStartInit();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutSpecialFunc(input);
	glutSpecialUpFunc(inputup);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(samplingTime, TimeCount, 0);
	glutTimerFunc(samplingTime, myMainTimer, 0);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(TitleDisplay);
	glutMainLoop();
	return 0;
}