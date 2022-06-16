#include <iostream>
#include <raylib.h>
#include <ctime>

/**********************************
* SNAKE GAME V 1.0
* Developed by Allan97
* Begin: 20/05/22
* End: 27/05/22
*
* BUG FIXES
* TODO: check if food is still being created on rotten food's location.
* TODO: fix rapid movement bug after score 2
***********************************/

#define SIZE_SPRITES 10

int score = 0, timer = 0;
bool wrap = false;

void GenerateFood(Rectangle* food, Rectangle* snake, int tam)
{
	food->x = (float)(rand() % 50) * SIZE_SPRITES;
	food->y = (float)(rand() % 50) * SIZE_SPRITES;

	for (int i = 0; i < tam; i++) // check if food was generated inside of snake
	{
		if (snake[i].x == food->x && snake[i].y == food->y)
		{
			GenerateFood(food, snake, tam);
		}
	}
}

void DrawGameOver()
{
	char temp[20];
	DrawText("GAME OVER", 200, 250, 30, RED);

	sprintf_s(temp, "Your score was %d", score);
	DrawText(temp, 200, 300, 30, RED);

	sprintf_s(temp, "Your time was %d", timer);
	DrawText(temp, 200, 350, 30, RED);

	DrawText("Press ENTER to restart", 200, 400, 30, RED);
}

void DrawHUD()
{
	char temp[20];

	sprintf_s(temp, "Snake, %d fps", GetFPS());
	SetWindowTitle(temp);

	sprintf_s(temp, "Score: %d", score);
	DrawText(temp, 510, 10, 30, RED);

	sprintf_s(temp, "Time: %d", timer);
	DrawText(temp, 510, 60, 30, RED);

	sprintf_s(temp, "Wrap mode: ");
	wrap ? strcat_s(temp, "on") : strcat_s(temp, "off");
	DrawText(temp, 510, 110, 30, RED);

	DrawText("Game made in C with Raylib\n\t\t\t by Marmota 2022", 510, 200, 20, YELLOW);
}

int main()
{
	InitWindow(800, 500, "Snake");

	Rectangle rec{ 0, 0, 500, 500 };
	Rectangle player[500]{ 250, 250, SIZE_SPRITES, SIZE_SPRITES };
	Rectangle food{ 0, 0, SIZE_SPRITES, SIZE_SPRITES };
	Rectangle rotten_food[500]{ 0, 0, SIZE_SPRITES, SIZE_SPRITES };

	srand((unsigned int)time(nullptr));

	int fps = 0, move_speed = 0, player_segments = 5, count_rotten_food = 1;
	int key_pressed = KEY_RIGHT;
	int choose_scene = 1;

	for (int i = 1; i < player_segments; i++)
	{
		player[i].x -= i * SIZE_SPRITES;
	}

	SetTargetFPS(60);

	GenerateFood(&food, &player[0], player_segments);
	GenerateFood(&rotten_food[0], &player[0], player_segments);

	while (!WindowShouldClose())
	{
		// --- UPDATE
		if (choose_scene == 1)
		{
			if (fps == 60)
			{
				timer++;
				fps = 0;
			}

			if (move_speed == 5)
			{

				switch (key_pressed)
				{
				case KEY_UP:
					if (wrap && player[0].y <= 0) // If wrap is on
					{
						player[0].y = rec.height - SIZE_SPRITES;
					}
					else if (!(player[0].y <= 0)) // If wall collision is on
					{
						player[0].y -= SIZE_SPRITES;
					}
					else // If it is Game Over
					{
						choose_scene = 2;
					}
					break;
				case KEY_DOWN:
					if (wrap && player[0].y >= rec.height - SIZE_SPRITES)
					{
						player[0].y = 0;
					}
					else if (!(player[0].y >= rec.height - SIZE_SPRITES))
					{
						player[0].y += SIZE_SPRITES;
					}
					else
					{
						choose_scene = 2;
					}
					break;
				case KEY_LEFT:
					if (wrap && player[0].x <= 0)
					{
						player[0].x = rec.width - SIZE_SPRITES;
					}
					else if (!(player[0].x <= 0))
					{
						player[0].x -= SIZE_SPRITES;
					}
					else
					{
						choose_scene = 2;
					}
					break;
				case KEY_RIGHT:
					if (wrap && player[0].x >= rec.width - SIZE_SPRITES)
					{
						player[0].x = 0;
					}
					else if (!(player[0].x >= rec.width - SIZE_SPRITES))
					{
						player[0].x += SIZE_SPRITES;
					}
					else
					{
						choose_scene = 2;
					}
					break;
				}

				// Check itself collision
				for (int i = 1; i < player_segments; i++)
				{
					if (player[0].x == player[i].x && player[0].y == player[i].y)
					{
						choose_scene = 2;
					}
				}

				for (int i = player_segments; i > 0; i--)
				{
					player[i] = player[i - 1];
				}

				move_speed = 0;
			}

			move_speed++;

			if (IsKeyPressed(KEY_UP) && key_pressed != KEY_DOWN)
			{
				key_pressed = KEY_UP;
			}
			else if (IsKeyPressed(KEY_DOWN) && key_pressed != KEY_UP)
			{
				key_pressed = KEY_DOWN;
			}
			else if (IsKeyPressed(KEY_LEFT) && key_pressed != KEY_RIGHT)
			{
				key_pressed = KEY_LEFT;
			}
			else if (IsKeyPressed(KEY_RIGHT) && key_pressed != KEY_LEFT)
			{
				key_pressed = KEY_RIGHT;
			}

			if (IsKeyPressed(KEY_SPACE)) wrap = !wrap;

			// Check food collision
			if (food.x == player[0].x && food.y == player[0].y)
			{
				GenerateFood(&food, &player[0], player_segments);

				player_segments++;

				for (int i = 0; i < count_rotten_food; i++)
				{

					if (food.x == rotten_food[i].x && food.y == rotten_food[i].y)
					{
						GenerateFood(&food, &player[0], player_segments);
					}
				}

				GenerateFood(&rotten_food[count_rotten_food], &player[0], player_segments);

				if (rotten_food[count_rotten_food].x == food.x && rotten_food[count_rotten_food].y == food.y)
				{
					GenerateFood(&rotten_food[count_rotten_food], &player[0], player_segments);
				}

				score++;
				count_rotten_food++;
			}

			// Check rotten food collision
			for (int i = 0; i < count_rotten_food; i++)
			{
				if (rotten_food[i].x == player[0].x && rotten_food[i].y == player[0].y)
				{
					choose_scene = 2;
				}
			}

			fps++; // increment for timer
			// --- END UPDATE

			// --- DRAW
			BeginDrawing();
			ClearBackground(BLACK);
			DrawRectangleLinesEx(rec, 2.f, RED); // draw gameplay box

			for (int i = 0; i < player_segments; i++)
			{
				DrawRectangle((int)player[i].x, (int)player[i].y, SIZE_SPRITES, SIZE_SPRITES, RED);
			}

			DrawRectangle((int)food.x, (int)food.y, SIZE_SPRITES, SIZE_SPRITES, GREEN); // draw food

			for (int i = 0; i < count_rotten_food; i++)
			{
				DrawRectangle((int)rotten_food[i].x, (int)rotten_food[i].y, SIZE_SPRITES, SIZE_SPRITES, PURPLE);
			}

			DrawHUD();
			EndDrawing();
			// --- END DRAW
		}
		else if (choose_scene == 2)
		{
			BeginDrawing();
			ClearBackground(BLACK);
			DrawGameOver();
			EndDrawing();
		}
	}

	CloseWindow();
	return 0;
}