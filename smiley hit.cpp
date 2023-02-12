#include<iostream>

#include<graphics.h>

#include<conio.h>

#include<dos.h>

#include<time.h>

#include<stdio.h>

#define ScWidth 1400
#define ScHeight 700
#define OpWidth 400

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define SPACEBAR 32

using namespace std;

// Dispaly Gameover screen and reset score
void GameOver();
// Draw game board and score / control area
void DrawBoard();
// Drawing Nail
void DrawEnemies();
// Drawing Player
void DrawPlayer();
// Checking if player hit red nail or blue nail
int PlayerHitEnemy();

// Global Variables
char option, txtScore[5];
int score = 0;

// Enemy Variables
const int NoEnemies = 3;
// Enemy (x, y, height, type)
int Enemy[NoEnemies][4] = {
  {
    300,
    500,
    0,
    0
  },
  {
    500,
    500,
    1,
    0
  },
  {
    700,
    500,
    0,
    0
  }
};
// eGap-Enemy Gap, eSize-Enemy Width, eStep-Enemy Step
int eSize = 50, eStep = -10, ei = 0;

// Player Variables
// px-Player X, py-Player Y, pSize-Player Size, pStep-Player Step
int pw = 70, ph = 100, px = 500 - pw / 2, py = 150;

// Line variables
int lx = 200, ly = 400, lw = 800;

// Other variables
int i = 0, timer;

int main() {
  // Generation of random number
  srand((unsigned) time(0));

  timer = 5 + rand() % 5; // initializing timer for first time

  //initializing Graphics
  int gd = DETECT, gm, err, tmp;
  initgraph( & gd, & gm, "C:\\tc\\bgi");

  while (1) {
    setcolor(WHITE);
    // Draw game board and score/controls 
    DrawBoard();

    // Draw reference line
    line(lx, ly, lw, ly);

    //Draw Enemies
    for (int i = 0; i < NoEnemies; i++) {
      setcolor(YELLOW);
      DrawEnemies();
    }

    // Update enemy
    Enemy[ei][1] += eStep;
    if (Enemy[ei][1] < 350) // if enemy reach upper bound change direction
      eStep = -eStep;
    if (Enemy[ei][1] > 500) { // if enemy reach lower bound
      ei = rand() % 3; // choose next enemy randomly
      eStep = -10; // udpate enemy position
      Enemy[0][2] = Enemy[1][2] = Enemy[2][2] = 0; // reset all enemies
    }
    if (i == timer) { // if i(counter) reaches max timer
      Enemy[ei][2] = (Enemy[ei][2] == 0 ? 1 : 0); // change enemey type
      i = 0; // reset counter
      timer = 10 + rand() % 5; // randomly generate next timer
    }

    if (kbhit()) { // if any key is pressed
      option = getch(); // catch character in options variable
      if (option == KEY_LEFT) { // if left arrow key is pressed
        if (px + pw / 2 > 300)
          px -= 200; // move hammer to left
      } else if (option == KEY_RIGHT) { // if right arrow key is pressed
        if (px + pw / 2 < 700)
          px += 200; // move hammer to right
      } else if (option == SPACEBAR) { // if spacebar is pressed

        py += 150; // hit hammer down
        DrawPlayer();
        delay(200);

        if (PlayerHitEnemy() == 1) {
          score++;
        } else if (PlayerHitEnemy() == 2) {
          GameOver();
        }

        py -= 150; // reset hammer position
      }
    }

    DrawPlayer();

    i++;
    delay(40);
    cleardevice();
  }

  closegraph();
}

int PlayerHitEnemy() {
  if (px + pw / 2 == Enemy[ei][0]) {
    if (py + ph >= Enemy[ei][1] - eSize) {
      if (Enemy[ei][2] == 0)
        return 1;
      else
        return 2;
    }
  }
  return 0;
}

// Drawing Nail
void DrawEnemies() {
  for (int i = 0; i < NoEnemies; i++) {
    if (Enemy[i][2] == 0) {
      setcolor(YELLOW);
    } else {
      setcolor(LIGHTRED);
    }
    circle(Enemy[i][0], Enemy[i][1], eSize);
    circle(Enemy[i][0] - eSize / 3, Enemy[i][1] - eSize / 3, 5);
    circle(Enemy[i][0] + eSize / 3, Enemy[i][1] - eSize / 3, 5);
    arc(Enemy[i][0], Enemy[i][1], 180, 360, eSize / 2);
  }
}

// Draw Player - Hammer
void DrawPlayer() {
  setcolor(WHITE);
  setfillstyle(1, WHITE);

  // drawing head
  bar(px, py, px + pw, py + ph);

  // drawing handle
  bar(px + pw, py + ph / 2 - 20, px + pw + 20, py + ph / 2 + 20);
  bar(px + pw + 25, py + ph / 2 - 20, px + pw + 45, py + ph / 2 + 20);
  bar(px + pw + 50, py + ph / 2 - 20, px + pw + 75, py + ph / 2 + 20);
  bar(px + pw + 80, py + ph / 2 - 20, px + pw + 105, py + ph / 2 + 20);
}

void GameOver() {
  cleardevice(); // clear screen

  setcolor(WHITE);
  // Print Game Over Text
  settextstyle(4, 0, 9);
  outtextxy(300, 300, "Game Over");

  settextstyle(4, 0, 2);
  outtextxy(500, 420, "Press any key to restart...");
  getch();
  score = 0; // reset score
}

void DrawBoard() {

  setcolor(WHITE);

  // draw board
  rectangle(0, 0, ScWidth - OpWidth, ScHeight);
  rectangle(ScWidth - OpWidth, 0, ScWidth, ScHeight);

  // draw title
  settextstyle(4, 0, 5);
  outtextxy(ScWidth - 375, 50, "Smiley Hit");

  // print score
  // itoa function converts number to character array
  settextstyle(4, 0, 9);
  if (score < 10)
    outtextxy(ScWidth - 270, 250, itoa(score, txtScore, 10));
  else if (score >= 10)
    outtextxy(ScWidth - 320, 250, itoa(score, txtScore, 10));

  settextstyle(4, 0, 1);
  outtextxy(ScWidth - 375, 500, "Controls: ");
  outtextxy(ScWidth - 300, 550, "Spacebar to Hit ");
  outtextxy(ScWidth - 300, 600, "Left Arrow Key ");
  outtextxy(ScWidth - 300, 650, "Right Arrow Key ");
}
