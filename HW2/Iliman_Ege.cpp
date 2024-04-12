/*********
   CTIS164 - Template Source Program
----------
STUDENT :Ege Iliman
SECTION :1
HOMEWORK:2
----------
PROBLEMS:-
----------
ADDITIONAL FEATURES:User can use only 40 bullets in the gametime. If user presses F2 key while playing the game ends. When user hits the target gets 5 points. 
If user can't shoot the target before it leaves loses -1 score.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


#define START 0
#define PLAY 1
#define PAUSE 2
#define END 3

#define MAX_SHIP 5
#define MAX_BULLET 40


typedef struct {
	int xB;
	int yB;
	bool active;

}bullet_t;

typedef struct {
	int xS;
	int yS;

}ship_t;


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, f1 = false, f2 = false;
int  winWidth, winHeight; // current Window width and height
int xC = -700, yC = 0;// coordinates of the cannon
int xB, yB;// coordinates of the bullet
int xS = 450;// coordinates of the ship
double time = 20;
int appState = START;
int score = 0;
int numBullet = 0;
int numShip = 0;
bullet_t bulletInfo[MAX_BULLET];
ship_t shipInfo[MAX_SHIP];

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void displayCannon() {

	//border of the cannon
	//glColor3f(112 / 255., 128 / 255., 144 / 255.);
	glColor3f(0, 0, 0);
	glRectf(xC, yC + 20, xC + 10, yC - 20);
	glRectf(xC + 10, yC + 10, xC + 60, yC - 10);
	glRectf(xC + 35, yC - 10, xC + 55, yC - 15);
	glRectf(xC + 40, yC - 15, xC + 55, yC - 40);
	glRectf(xC + 60, yC + 15, xC + 120, yC - 15);
	glRectf(xC + 120, yC + 10, xC + 215, yC - 10);
	glRectf(xC + 215, yC + 15, xC + 230, yC - 15);
	glRectf(xC + 135, yC, xC + 160, yC + 35);
	glRectf(xC + 125, yC + 15, xC + 135, yC + 40);
	glRectf(xC + 160, yC + 15, xC + 170, yC + 40);
	glRectf(xC + 180, yC + 10, xC + 195, yC + 15);
	glRectf(xC + 180, yC + 15, xC + 190, yC + 20);
	glRectf(xC + 215, yC + 15, xC + 230, yC - 15);
	glRectf(xC + 135, yC - 10, xC + 145, yC - 30);
	glRectf(xC + 130, yC - 20, xC + 140, yC - 35);
	glRectf(xC + 145, yC - 10, xC + 155, yC - 20);
	glRectf(xC + 155, yC - 15, xC + 160, yC - 20);
	glRectf(xC + 160, yC - 10, xC + 165, yC - 15);
	glRectf(xC + 170, yC - 10, xC + 185, yC - 40);
	glRectf(xC + 185, yC - 10, xC + 190, yC - 15);

	//dark green part of the cannon
	glColor3f(0, 100 / 255., 0);
	glRectf(xC + 5, yC + 15, xC + 10, yC - 15);
	glRectf(xC + 10, yC + 5, xC + 55, yC - 5);
	glRectf(xC + 60, yC, xC + 120, yC + 10);
	glRectf(xC + 65, yC, xC + 75, yC - 5);
	glRectf(xC + 105, yC, xC + 115, yC - 5);
	glRectf(xC + 125, yC + 5, xC + 135, yC - 5);
	glRectf(xC + 135, yC, xC + 160, yC - 5);
	glRectf(xC + 160, yC + 5, xC + 215, yC - 5);
	glRectf(xC + 215, yC + 10, xC + 220, yC - 10);
	glRectf(xC + 130, yC + 20, xC + 135, yC + 35);
	glRectf(xC + 140, yC + 20, xC + 155, yC + 30);
	glRectf(xC + 160, yC + 20, xC + 165, yC + 35);

	//light green part of the cannon
	glColor3f(0, 128 / 255., 0);
	glRectf(xC + 60, yC, xC + 65, yC - 5);
	glRectf(xC + 115, yC, xC + 120, yC - 5);
	glRectf(xC + 75, yC, xC + 105, yC - 5);
	glRectf(xC + 60, yC - 5, xC + 120, yC - 10);
	glRectf(xC + 135, yC + 5, xC + 160, yC + 15);
	glRectf(xC + 135, yC + 20, xC + 140, yC + 30);
	glRectf(xC + 155, yC + 20, xC + 160, yC + 30);
	glRectf(xC + 185, yC + 10, xC + 190, yC + 15);
	glRectf(xC + 150, yC - 10, xC + 155, yC - 15);
	glRectf(xC + 140, yC - 10, xC + 145, yC - 20);
	glRectf(xC + 135, yC - 20, xC + 140, yC - 30);
	glRectf(xC + 180, yC - 10, xC + 185, yC - 15);
	glRectf(xC + 175, yC - 10, xC + 180, yC - 35);
	glRectf(xC + 40, yC - 10, xC + 45, yC - 15);
	glRectf(xC + 45, yC - 10, xC + 50, yC - 35);


	//red part of the cannon
	glColor3f(1, 0, 0);
	glRectf(xC + 220, yC + 10, xC + 225, yC - 10);

}

void displayShip(ship_t* shipInfo) {
	for (int i = 0; i < MAX_SHIP; i++) {
		glColor3f(192 / 255., 192 / 255., 192 / 255.);
		circle(shipInfo[i].xS, shipInfo[i].yS, 40);
		glColor3f(128 / 255., 128 / 255., 128 / 255.);
		circle(shipInfo[i].xS, shipInfo[i].yS, 28);
		glColor3f(50 / 255., 205 / 255., 50 / 255.);
		circle(shipInfo[i].xS - 33, shipInfo[i].yS - 3, 5);
		circle(shipInfo[i].xS + 3, shipInfo[i].yS - 33, 5);
		circle(shipInfo[i].xS + 33, shipInfo[i].yS + 3, 5);
		circle(shipInfo[i].xS - 3, shipInfo[i].yS + 33, 5);

	}

}

void displayBullet(bullet_t* bulletInfo) {
	for (int i = 0; i < numBullet; i++) {

		if (bulletInfo[i].active) {

			//flame effect
			glBegin(GL_TRIANGLES);
			glColor3f(1, 0, 0);
			glVertex2f(bulletInfo[i].xB + 5, bulletInfo[i].yB + 5);
			glVertex2f(bulletInfo[i].xB + 5, bulletInfo[i].yB - 10);
			glColor3f(1, 1, 0);
			glVertex2f(bulletInfo[i].xB - 15, bulletInfo[i].yB - 5);
			glEnd();

			//black parts of the bullet
			glColor3f(0, 0, 0);
			glRectf(bulletInfo[i].xB + 5, bulletInfo[i].yB, bulletInfo[i].xB + 10, bulletInfo[i].yB - 5);
			glRectf(bulletInfo[i].xB + 70, bulletInfo[i].yB, bulletInfo[i].xB + 75, bulletInfo[i].yB - 5);
			glRectf(bulletInfo[i].xB, bulletInfo[i].yB - 5, bulletInfo[i].xB + 5, bulletInfo[i].yB - 15);
			glRectf(bulletInfo[i].xB, bulletInfo[i].yB, bulletInfo[i].xB + 5, bulletInfo[i].yB + 10);
			glRectf(bulletInfo[i].xB + 5, bulletInfo[i].yB + 10, bulletInfo[i].xB + 25, bulletInfo[i].yB + 5);
			glRectf(bulletInfo[i].xB + 5, bulletInfo[i].yB - 15, bulletInfo[i].xB + 25, bulletInfo[i].yB - 10);
			glRectf(bulletInfo[i].xB + 25, bulletInfo[i].yB - 10, bulletInfo[i].xB + 45, bulletInfo[i].yB - 5);
			glRectf(bulletInfo[i].xB + 25, bulletInfo[i].yB, bulletInfo[i].xB + 45, bulletInfo[i].yB + 5);
			glRectf(bulletInfo[i].xB + 45, bulletInfo[i].yB, bulletInfo[i].xB + 50, bulletInfo[i].yB + 10);
			glRectf(bulletInfo[i].xB + 45, bulletInfo[i].yB - 5, bulletInfo[i].xB + 50, bulletInfo[i].yB - 15);
			glRectf(bulletInfo[i].xB + 50, bulletInfo[i].yB - 15, bulletInfo[i].xB + 60, bulletInfo[i].yB - 10);
			glRectf(bulletInfo[i].xB + 50, bulletInfo[i].yB + 10, bulletInfo[i].xB + 60, bulletInfo[i].yB + 5);
			glRectf(bulletInfo[i].xB + 60, bulletInfo[i].yB - 5, bulletInfo[i].xB + 70, bulletInfo[i].yB - 10);
			glRectf(bulletInfo[i].xB + 60, bulletInfo[i].yB, bulletInfo[i].xB + 70, bulletInfo[i].yB + 5);

			//white part of the bullet
			glColor3f(1, 1, 1);
			glRectf(bulletInfo[i].xB + 10, bulletInfo[i].yB, bulletInfo[i].xB + 65, bulletInfo[i].yB - 5);

			//red parts of the bullet
			glColor3f(1, 0, 0);
			glRectf(bulletInfo[i].xB + 65, bulletInfo[i].yB, bulletInfo[i].xB + 70, bulletInfo[i].yB - 5);
			glRectf(bulletInfo[i].xB + 5, bulletInfo[i].yB - 10, bulletInfo[i].xB + 25, bulletInfo[i].yB - 5);
			glRectf(bulletInfo[i].xB + 5, bulletInfo[i].yB + 5, bulletInfo[i].xB + 25, bulletInfo[i].yB);
			glRectf(bulletInfo[i].xB + 50, bulletInfo[i].yB - 5, bulletInfo[i].xB + 60, bulletInfo[i].yB - 10);
			glRectf(bulletInfo[i].xB + 50, bulletInfo[i].yB, bulletInfo[i].xB + 60, bulletInfo[i].yB + 5);
		}
	}
}

void displaySky() {
	glColor3f(0, 1, 0);

}

void display_start() {
	glColor3f(1, 1, 1);
	vprint2(-400, 10, 0.3, "PRESS F1 KEY TO START THE GAME");
	vprint2(-420, -80, 0.3, "PRESS ESC KEY TO CLOSE THE GAME");


}

void display_play() {
	displayBullet(bulletInfo);
	displayCannon();
	displayShip(shipInfo);


	glColor3f(50 / 255., 205 / 255., 50 / 255.);
	vprint(600, 320, GLUT_BITMAP_9_BY_15, "Time Left:");
	vprint(620, 300, GLUT_BITMAP_9_BY_15, "%.2f", time);
	vprint(600, 280, GLUT_BITMAP_9_BY_15, "Score:%d", score);
	vprint(560, 260, GLUT_BITMAP_9_BY_15, "Bullets Left:%d", 40 - numBullet);


	vprint(-550, -380, GLUT_BITMAP_9_BY_15, "Controls->Spacebar to shoot   Up/Down arrows to move   F1 to pause   F2 to end");


}

void display_pause() {
	glColor3f(1, 1, 1);
	glRectf(-100, 150, -20, -120);
	glRectf(100, 150, 20, -120);
	glColor3f(50 / 255., 205 / 255., 50 / 255.);
	vprint(600, 320, GLUT_BITMAP_9_BY_15, "Time Left:");
	vprint(620, 300, GLUT_BITMAP_9_BY_15, "%.2f", time);
	glColor3f(1, 1, 1);
	vprint2(-150, -180, 0.3, "GAME PAUSED");
	vprint2(-420, -250, 0.3, "PRESS F1 KEY TO RESUME THE GAME");

}

void display_end() {
	glColor3f(47 / 255., 249 / 255., 36 / 255.);
	vprint2(-230, 60, 0.5, "GAME OVER");
	vprint2(-220, 140, 0.5, "SCORE: %d", score);
	vprint2(-400, -10, 0.3, "PRESS F1 KEY TO RESTART THE GAME");
	vprint2(-400, -100, 0.3, "PRESS ESC KEY TO CLOSE THE GAME");

}


//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(96 / 255., 98 / 255., 99 / 255., 0);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (appState) {
	case START: display_start(); break;
	case PLAY: display_play(); break;
	case PAUSE: display_pause(); break;
	case END: display_end(); break;



	}

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);


	if (key == 32 && numBullet < MAX_BULLET) {
		if (numBullet < MAX_BULLET) {
			bulletInfo[numBullet].active = true;
			bulletInfo[numBullet].xB = xC + 190;
			bulletInfo[numBullet].yB = yC;
			numBullet++;
		}
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_F1: f1 = true; break;
	case GLUT_KEY_F2: f2 = true; break;
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	if (key == GLUT_KEY_F1) {
		switch (appState) {
		case START: appState = PLAY; break;
		case PLAY: appState = PAUSE; break;
		case PAUSE: appState = PLAY; break;
		case END: appState = START;  break;


		}
	}
	if (key == GLUT_KEY_F2 && appState == PLAY) {
		appState = END;
		time = 20;
		numBullet = 0;
		for (int i = 0; i < MAX_SHIP; i++) {
			shipInfo[i].yS = -((rand() % 10) * 100 + 400);
			shipInfo[i].xS = rand() % 150 + 400;
		}
	}


	if (key == GLUT_KEY_UP && yC < 360)
		yC += 8;
	if (key == GLUT_KEY_DOWN && yC > -360)
		yC -= 8;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

bool testCollision(bullet_t fr, ship_t t) {
	float dx = t.xS - fr.xB;
	float dy = t.yS - fr.yB;
	float d = sqrt(dx * dx + dy * dy);
	return d <= 40;
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (appState == PLAY) {
		if (time > 0.01) {
			time = time - (TIMER_PERIOD * 0.001);

			for (int k = 0; k < MAX_SHIP; k++)
				shipInfo[k].yS += 4;
			for (int i = 0; i < numBullet; i++) {
				bulletInfo[i].xB += 10;
				for (int j = 0; j < MAX_SHIP; j++) {
					if (testCollision(bulletInfo[i], shipInfo[j])) {
						if (bulletInfo[i].active)
							score += 5;
						bulletInfo[i].active = false;
						shipInfo[j].yS = -((rand() % 10) * 100 + 400);
						shipInfo[j].xS = rand() % 300 + 400;
					}


				}
				for (int k = 0; k < MAX_SHIP; k++)
					if (shipInfo[k].yS > 400) {
						shipInfo[k].yS = -500;
						score--;
					}
			}
		}
		else {
			appState = END;
			time = 20;
			numBullet = 0;
			for (int i = 0; i < MAX_SHIP; i++) {
				shipInfo[i].yS = -((rand() % 10) * 100 + 400);
				shipInfo[i].xS = rand() % 300 + 400;
			}
		}
	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}

#endif

void Init() {
	for (int i = 0; i < MAX_SHIP; i++) {
		shipInfo[i].yS = -((rand() % 10) * 100 + 400);
		shipInfo[i].xS = rand() % 300 + 300;
	}

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("UFO Shooting Game  Ege Iliman");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}