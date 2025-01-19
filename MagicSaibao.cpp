#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Magic.h"

float randFloat() {
	float ZeroToOne = (float)rand() * (1.0f / RAND_MAX);
	return (ZeroToOne - 0.5f) * 2.0f;
}

float sparse(float x, float sparser) {
	return x + randFloat() * sparser;
}

int main() {

	srand((unsigned int)time(NULL));

	Magic();

	Picture saibao, gameOver, paddle, background;
	saibao.Load(".\\BMP\\01-140px-PinkBottom.bmp");
	gameOver.Load(".\\BMP\\GameOver.bmp");
	paddle.Load(".\\BMP\\Paddle.bmp");
	background.Load(".\\BMP\\Background.bmp");

	int speed = 7;
	int saiX = 300, saiY = 100;
	int dtX = speed, dtY = -speed;
	int acceY = 1;  // Y轴加速度

	bool isGameOver = false;
	int paddleHeight = 600 - paddle.height - 20;
	int paddleCenter;

	while (true) {

		// Clear Screen
		//Clean(255, 255, 255);  // When Drawing BG, No Need to Clean Again.
		background.Draw(0, 0);

		// Draw Saibao
		//saibao.Draw(saiX, saiY);
		for (int y = 0; y < saibao.height; y++) {
			for (int x = 0; x < saibao.width; x++) {
				
				if (
					saibao.GetR(x, y) == 255 &&
					saibao.GetG(x, y) == 0 &&
					saibao.GetB(x, y) == 255
				) {
					// Pink Color, Transparent, Don't Draw
					;
				}
				else {
					// Draw
					MagicSetPixel(
						x + saiX, y + saiY,
						saibao.GetR(x, y),
						saibao.GetG(x, y),
						saibao.GetB(x, y)
					);
				}
			}
		}

		// Move Saibao
		saiX += dtX;
		saiY += dtY;
		dtY += acceY;

		// Border Bouncing
		if (saiX <= 0 || saiX >= 800 - saibao.width) {
			dtX = -dtX;
		}
		if (saiY <= 0) {
			dtY = -dtY;
		}

		// Paddle
		if (!isGameOver) {
			// Only Update Paddle Position when NOT Game Over.
			paddleCenter = mouseX;
		}
		paddle.Draw(paddleCenter - paddle.width / 2, paddleHeight);

		// Re-Bounce: Touching the Paddle
		if ((saiY + saibao.height) >= paddleHeight) {
			int saibaoCenter = saiX + saibao.width / 2;

			if (
				(saibaoCenter >= paddleCenter - paddle.width / 2)
				&&
				(saibaoCenter <= paddleCenter + paddle.width / 2)
			) {
				dtY = int(-speed * sparse(3.5, 0.5));  // Random Number from 3 to 4
				saiY -= speed * 2;  // Prevent Corner Case
			}
		}

		// Game Over: Touching the Bottom
		if (saiY >= 600 - saibao.height) {
			dtX = 0;
			dtY = 0;
			isGameOver = true;
		}

		if (isGameOver == true) {
			gameOver.Draw(
				(800 - gameOver.width) / 2,
				(600 - gameOver.height) / 2
			);
		}

		// Update Screen
		Show();
	}

	Quit();
}
