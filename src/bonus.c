/* Copyright Team 119*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

int kbhit();
int getpos1(int y, char key);
int getpos2(int y, char key);
int field(int ball_hor, int ball_vert, int right, int left);
void getPosBall(int xBall, int yBall, int score, int vecX, int vecY,
                int racket1, int racket2);

int main() {
  getPosBall(40, 13, 0, 1, 1, 13, 13);
  return 0;
}

void getPosBall(int xBall, int yBall, int score, int vecX, int vecY,
                int racket1, int racket2) {
  // [0] - xball, [1] - yball, [2] - who win,
  // [3] - vecx, [4] - vecy, [5] - racket1, [6] - racket2;
  // [3] - если 0, то никто не забил, 1 - забил игрок1, 2 - забил игрок2
  int score1 = 0, score2 = 0;
  char key;
  field(40, 13, 13, 13);
  while (score1 != 21 && score2 != 21) {
    clock_t begin = clock();
    while ((clock() - begin) < 150000) {
      if (kbhit()) {
        system("stty raw");
        key = getchar();
        system("stty -raw");
        racket1 = getpos1(racket1, key);
        racket2 = getpos2(racket2, key);

        if (key == 26 || key == 27) {
          printf("\n\t     ╿ ATTENTION, GAME CLOSED, WOULD YOU LIKE TO "
                 "RESTART? ╿\n\n\n");
          return;
        }
      }
    }

    if (score == 1) {
      score1++;
    } else if (score == 2) {
      score2++;
    }
    score = 0;

    if (yBall == 24 || yBall == 1) {
      vecY = -vecY;
    }
    if (xBall == 2) {
      if (yBall == racket1 || yBall == racket1 + 1 || yBall == racket1 + 2) {
        vecX = -vecX;
      } else {
        xBall = 40;
        yBall = 13;
        score = 2;
      }
    } else if (xBall == 77) {
      if (yBall == racket2 || yBall == racket2 + 2 || yBall == racket2 + 1) {
        vecX = -vecX;
      } else {
        xBall = 40;
        yBall = 13;
        score = 1;
      }
    }
    xBall = xBall + vecX;
    yBall = yBall + vecY;

    field(xBall, yBall, racket2, racket1);
    printf("\n\t\tFIRST PLAYER: %2d       VS\tSECOND PLAYER: %2d\t\t\t\t\n\n",
           score1, score2);
  }
  if (score1 == 21) {
    printf("\n\t\t\tFIRST PLAYER HAS WONE!HURRAY!\n\n\n");
  } else if (score2 == 21) {
    printf("\n\t\t\tSECOND PLAYER HAS WON!HURRAY!\n\n\n");
  }
}

int field(int ball_hor, int ball_vert, int right, int left) {
  system("clear");
  int MIDDLE1 = 40;
  int MIDDLE2 = 39;
  int i, j;
  for (i = 0; i <= 25; i++) {
    for (j = 0; j < 80; j++) {
      if ((i == 0) && (j == 0)) {
        printf("╔");
      } else if ((i == 25) && (j == 0)) {
        printf("╚");
      } else if ((i == 0) && (j == 79)) {
        printf("╗");
      } else if ((i == 25) && (j == 79)) {
        printf("╝");
      } else if ((i == 0) || (i == 25)) {
        printf("═");
      } else if ((j == 1) && (i >= left) && (i < left + 3)) {
        printf("█");
      } else if ((j == 78) && (i >= right) && (i < right + 3)) {
        printf("█");
      } else if ((ball_hor == j) && (ball_vert == i)) {
        printf("®");
      } else if ((j == MIDDLE1) || (j == MIDDLE2)) {
        printf("░");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
  return 0;
}

int getpos1(int y, char key) {
  if (key == 'a' || key == 'A') {
    if (y > 1) {
      y--;
    }
  } else if (key == 'z' || key == 'Z') {
    if (y < 22) {
      y++;
    }
  }
  return y;
}

int getpos2(int y, char key) {
  if (key == 'k' || key == 'K') {
    if (y > 1) {
      y--;
    }
  } else if (key == 'm' || key == 'M') {
    if (y < 22) {
      y++;
    }
  }
  return y;
}

int kbhit() {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}
// detects if key is pressed
