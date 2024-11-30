#include <graphics.h>
#include <conio.h>
#include <dos.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 80
#define BALL_SIZE 10
#define MARGIN 50


class Paddle {
public:
    int x, y;

    Paddle(int startX, int startY) {
        x = startX;
        y = startY;
    }

    void draw() {
        bar(x, y, x + PADDLE_WIDTH, y + PADDLE_HEIGHT);
    }

    void moveUp() {
        if (y > MARGIN) y -= 10;
    }

    void moveDown() {
        if (y < SCREEN_HEIGHT - PADDLE_HEIGHT - MARGIN) y += 10;
    }

    void moveLeft() {
        if (x > MARGIN) x -= 10;
    }

    void moveRight(int boundary) {
        if (x + PADDLE_WIDTH < boundary) x += 10;
    }

    void restrictToHalf(int midX, int isLeft) {
        if (isLeft) {
            if (x + PADDLE_WIDTH > midX) x = midX - PADDLE_WIDTH;
        } else {
            if (x < midX) x = midX;
        }
    }
};


class Ball {
public:
    int x, y;
    int dx, dy;

    Ball(int startX, int startY) {
        x = startX;
        y = startY;
        dx = 4;
        dy = 4;
    }

    void draw() {

       setcolor(RED);
        fillellipse(x + BALL_SIZE / 2, y + BALL_SIZE / 2, BALL_SIZE / 2, BALL_SIZE / 2);
    }


    void move() {
        x += dx;
        y += dy;


        if (y <= MARGIN || y + BALL_SIZE >= SCREEN_HEIGHT - MARGIN) {
            dy = -dy;
        }
    }

    void reset(int startX, int startY, int direction) {
        x = startX;
        y = startY;
        if (direction == 1) {
            dx = 4;
        } else {
            dx = -4;
        }
        dy = 4;
    }


    void checkCollision(Paddle player1, Paddle player2) {

        if (x <= player1.x + PADDLE_WIDTH && y + BALL_SIZE >= player1.y && y <= player1.y + PADDLE_HEIGHT) {
            dx = -dx;
            x = player1.x + PADDLE_WIDTH;
        }

        if (x + BALL_SIZE >= player2.x && y + BALL_SIZE >= player2.y && y <= player2.y + PADDLE_HEIGHT) {
            dx = -dx;
            x = player2.x - BALL_SIZE;
        }
    }

    int checkScore(int leftEdge, int rightEdge) {
        if (x <= leftEdge) return 2;
        if (x + BALL_SIZE >= rightEdge) return 1;
        return 0;
    }
};


void drawCourt() {
    rectangle(MARGIN, MARGIN, SCREEN_WIDTH - MARGIN, SCREEN_HEIGHT - MARGIN);
    line(SCREEN_WIDTH / 2, MARGIN, SCREEN_WIDTH / 2, SCREEN_HEIGHT - MARGIN);
}


void intToStr(int num, char* str) {
    int i = 0;
    int temp = num;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }


    while (temp > 0) {
        str[i++] = (temp % 10) + '0';
        temp /= 10;
    }

    str[i] = '\0';


    int start = 0;
    int end = i - 1;
    while (start < end) {
        char tempChar = str[start];
        str[start] = str[end];
        str[end] = tempChar;
        start++;
        end--;
    }
}


void displayScore(int player1Score, int player2Score) {
    char scoreText[50];
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 1);


    intToStr(player1Score, scoreText);
    outtextxy(SCREEN_WIDTH / 4 - 40, 10, "Player 1: ");
    setcolor(RED);
    outtextxy(SCREEN_WIDTH / 4 + 60, 10, scoreText);
    setcolor(WHITE);


    intToStr(player2Score, scoreText);
    outtextxy(SCREEN_WIDTH * 3 / 4 - 40, 10, "Player 2: ");
    setcolor(GREEN);
    outtextxy(SCREEN_WIDTH * 3 / 4 + 60, 10, scoreText);
    setcolor(WHITE);
}


void resetGame(Paddle &player1, Paddle &player2, Ball &ball, int &player1Score, int &player2Score) {
    player1Score = 0;
    player2Score = 0;
    player1.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    player2.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    ball.reset(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1);
}

void displayMenu(int &curr) {
    cleardevice();
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2);


    outtextxy(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 4, "1. New Game");
    outtextxy(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 4 + 40, "2. Score");
    outtextxy(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 4 + 80, "3. Exit");


	setcolor(RED);
    if (curr == 1) {

        outtextxy(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 4, "1. New Game");
    } else if (curr == 2) {

        outtextxy(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 4 + 40, "2. Score");
    } else if (curr == 3) {

        outtextxy(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 4 + 80, "3. Exit");
    }
}




int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "D:/BGI/");  // Ensure correct path to your BGI folder

    Paddle player1(MARGIN + 30, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2);
    Paddle player2(SCREEN_WIDTH - MARGIN - PADDLE_WIDTH - 30, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2);
    Ball ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    int player1Score = 0;
    int player2Score = 0;
    int curr = 1;

    while (1) {
        displayMenu(curr);
        char ch = getch();

        if (ch == 27) break;

        if (ch == 72) {
            curr--;
            if (curr < 1) curr = 3;
        } else if (ch == 80) {
            curr++;
            if (curr > 3) curr = 1;
        }

        if (ch == 13) {
            if (curr == 1) {
                while (1) {
                    if (kbhit()) {
                        char ch = getch();
                        if (ch == 27) break;


                        if (ch == 72) player1.moveUp();
                        else if (ch == 80) player1.moveDown();
                        else if (ch == 75) player1.moveLeft();
                        else if (ch == 77) player1.moveRight(SCREEN_WIDTH / 2);


                        if (ch == 'w' || ch == 'W') player2.moveUp();
                        else if (ch == 's' || ch == 'S') player2.moveDown();
                        else if (ch == 'a' || ch == 'A') player2.moveLeft();
                        else if (ch == 'd' || ch == 'D') player2.moveRight(SCREEN_WIDTH - MARGIN);


                        player1.restrictToHalf(SCREEN_WIDTH / 2, 1);
                        player2.restrictToHalf(SCREEN_WIDTH / 2, 0);
                    }

                    ball.move();
                    ball.checkCollision(player1, player2);

                    int scored = ball.checkScore(MARGIN, SCREEN_WIDTH - MARGIN);
                    if (scored == 1) {
                        ++player1Score;
                        ball.reset(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2);
                    } else if (scored == 2) {
                        ++player2Score;
                        ball.reset(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1);
                    }


if (player1Score == 5) {
    setcolor(RED);
    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
    int textWidth = textwidth("Player 1 Wins!");
    int textHeight = textheight("Player 1 Wins!");
    outtextxy((SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, "Player 1 Wins!");
    delay(3000);
    resetGame(player1, player2, ball, player1Score, player2Score);
    break;
} else if (player2Score == 5) {
    setcolor(YELLOW);
    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
    int textWidth = textwidth("Player 2 Wins!");
    int textHeight = textheight("Player 2 Wins!");
    outtextxy((SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, "Player 2 Wins!");
    delay(3000);
    resetGame(player1, player2, ball, player1Score, player2Score);
    break;
}




                    cleardevice();
                    drawCourt();
                    player1.draw();
                    player2.draw();
                    ball.draw();
                    displayScore(player1Score, player2Score);

                    delay(20);
                }
            } else if (curr == 2) {
    cleardevice();
    displayScore(player1Score, player2Score);


    while (1) {
        char ch = getch();
        if (ch == 27) {
            break;
        }
	} }else if (curr == 3) {
                break;
            }
        }
    }

    closegraph();
}
