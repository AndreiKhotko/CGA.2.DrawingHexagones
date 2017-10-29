//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include "Point.h"

#define PI 3.14159265358979323846

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int MID_X = SCREEN_WIDTH / 2;
int MID_Y = SCREEN_HEIGHT / 2;

double X_SCALE = 1.1;
double Y_SCALE = 1.1;

const double A = 120;
const double SEPARATOR = 0.4;

double scale;
std::vector<Point> points;
std::vector<Point> all_points;

int rounding(double value)
{
	return (int)(value + 0.5);
}

void centering(double &x, double &y)
{
	x += MID_X;
	y += MID_Y;
}

void uncentering(double &x, double &y)
{
	x -= MID_X;
	y -= MID_Y;
}
void centering(int &x, int &y)
{
	x += MID_X;
	y += MID_Y;
}

void matrixMultiply(double src[2], double matrix[][2], double res[2])
{
	res[0] = matrix[0][0] * src[0] + matrix[0][1] * src[1];
	res[1] = matrix[1][0] * src[0] + matrix[1][1] * src[1];
}

void scaling(Point &point)
{
	uncentering(point.X, point.Y);

	double coords[2] =
	{
		point.X, //x
		point.Y  //y
	};

	double scale[2][2] =
	{
		{ X_SCALE, 0 },
		{ 0, Y_SCALE}
	};

	double newCoords[2];

	matrixMultiply(coords, scale, newCoords);

	point.set_values(newCoords[0], newCoords[1]);

	centering(point.X, point.Y);
}

void rotating(Point &point, double alpha)
{
	uncentering(point.X, point.Y);
	double coords[2] =
	{
		point.X, //x
		point.Y  //y
	};

	double rotate[2][2] =
	{
		{ cos(alpha), sin(alpha) },
		{ -sin(alpha), cos(alpha) }
	};

	double newCoords[2];

	matrixMultiply(coords, rotate, newCoords);

	point.set_values(newCoords[0], newCoords[1]);
	centering(point.X, point.Y);
}

struct color_t {
	int R;
	int G;
	int B;
} color;

void SetColor(color_t *color, int r, int g, int b)
{
	(*color).R = r;
	(*color).G = g;
	(*color).B = b;
}

void DrawLine(SDL_Renderer *r, int x1, int y1, int x2, int y2, color_t color)
{
	// Set color:
	SDL_SetRenderDrawColor(r, color.R, color.G, color.B, SDL_ALPHA_OPAQUE);

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x2 >= x1 ? 1 : -1;
	int sy = y2 >= y1 ? 1 : -1;
	if (dy <= dx) {
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;
		SDL_RenderDrawPoint(r, x1, y1);
		for (int x = x1 + sx, y = y1, i = 1; i <= dx; i++, x += sx) {
			if (d > 0) {
				d += d2; y += sy;
			}
			else
				d += d1;
			SDL_RenderDrawPoint(r, x, y);
		}
	}

	else {
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		SDL_RenderDrawPoint(r, x1, y1);
		for (int x = x1, y = y1 + sy, i = 1; i <= dy; i++, y += sy) {
			if (d > 0) {
				d += d2; x += sx;
			}
			else
				d += d1;
			SDL_RenderDrawPoint(r, x, y);
		}
	}
}

void drawMainLines(SDL_Renderer *r)
{
	SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SetColor(&color, 255, 255, 255);
	DrawLine(r, 0, MID_Y, SCREEN_WIDTH, MID_Y, color);
	DrawLine(r, MID_X, SCREEN_HEIGHT, MID_X, 0, color);
}

void drawHexagonNew(SDL_Renderer *r)
{
	all_points.clear();
	double alpha = PI / 3;

	int x0 = rounding(- A/ 2);
	int y0 = rounding(- A * sin(alpha));

	int x_prev = x0;
	int y_prev = y0;
	
	Point point;

	int centered_x = x0;
	int centered_y = y0;

	centering(centered_x, centered_y);
	int x_centered_prev = centered_x;
	int y_centered_prev = centered_y;

	point.set_values(centered_x, centered_y);

	points.push_back(point);
	all_points.push_back(point);

	alpha = 0;

	
	for (int i = 0; i < 5; i++)
	{
		int x = rounding(x_prev + A * cos(alpha));
		int y = rounding(y_prev + A * sin(alpha));

		centered_x = x;
		centered_y = y;
		centering(centered_x, centered_y);

		Point tmpPoint;
		tmpPoint.set_values(centered_x, centered_y);

		points.push_back(tmpPoint);
		all_points.push_back(tmpPoint);
		alpha += PI / 3;

		DrawLine(r, x_centered_prev, y_centered_prev, centered_x, centered_y, color);

		x_prev = x;
		y_prev = y;

		x_centered_prev = centered_x;
		y_centered_prev = centered_y;
	}

	DrawLine(r, rounding(x_centered_prev), rounding(y_centered_prev), rounding(points[0].X), rounding(points[0].Y), color);
	SDL_RenderPresent(r);
}

void drawHexagon(SDL_Renderer *r)
{
	all_points.clear();
	double alpha = PI / 3;

	int x0 = rounding((SCREEN_WIDTH - A) / 2);
	int y0 = rounding((SCREEN_HEIGHT - 2 * (A * sin(alpha))) / 2);
	
	int x_prev = x0;
	int y_prev = y0;

	Point point;

	point.set_values(x0, y0);

	points.push_back(point);
	all_points.push_back(point);

	alpha = 0;

	for (int i = 0; i < 5; i++)
	{
		int x = rounding(x_prev + A * cos(alpha));
		int y = rounding(y_prev + A * sin(alpha));

		

		Point tmpPoint;
		tmpPoint.set_values(x, y);

		points.push_back(tmpPoint);
		all_points.push_back(tmpPoint);
		alpha += PI / 3;

		DrawLine(r, x_prev, y_prev, x, y, color);

		x_prev = x;
		y_prev = y;
	}

	DrawLine(r, rounding(x_prev), rounding(y_prev), rounding(points[0].X), rounding(points[0].Y), color);
	SDL_RenderPresent(r);
}

void saveVector(std::vector<Point> *dest, std::vector<Point> src)
{
	(*dest).clear();

	for (int i = 0; i < (int)src.size(); i++)
	{
		(*dest).push_back(src[i]);
	}
}

void rotateAndDrawHexagon(SDL_Renderer *r, int count)
{
	std::vector<Point> old_points;
	std::vector<Point> new_points;

	saveVector(&old_points, points);

	for (int i = 0; i < count; i++)
	{		
		new_points.clear();

		int i_next;
		for (int i = 0; i < 6; i++)
		{
			if (i == 5)
				i_next = 0;
			else
				i_next = i + 1;

			Point tmp;
			tmp.X = rounding((1 - SEPARATOR) * old_points[i].X + SEPARATOR * old_points[i_next].X);
			tmp.Y = rounding((1 - SEPARATOR) * old_points[i].Y + SEPARATOR * old_points[i_next].Y);

			new_points.push_back(tmp);
			all_points.push_back(tmp);
		}

		saveVector(&old_points, new_points);

		for (int i = 0; i < 6; i++)
		{
			if (i == 5)
				i_next = 0;
			else
				i_next = i + 1;

			DrawLine(r, rounding(old_points[i].X), rounding(old_points[i].Y), 
				rounding(old_points[i_next].X), rounding(old_points[i_next].Y), color);
		}
	}
}

void RotateRightAndZoomIn()
{
	X_SCALE = 1.1;
	Y_SCALE = 1.1;

	for (int i = 0; i < (int)(all_points.size()); i++)
	{
		scaling(all_points[i]);
		rotating(all_points[i], -(PI / 30));
	}
}

void RotateLeftAndZoomOut()
{
	X_SCALE = 0.9;
	Y_SCALE = 0.9;

	for (int i = 0; i < (int)(all_points.size()); i++)
	{
		scaling(all_points[i]);
		rotating(all_points[i], PI / 30);
	}
}

void drawAllPoints(SDL_Renderer *r)
{
	for (int i = 0; i < (int)(all_points.size()) - 1; i += 6)
	{
		int i_next = 0;
		
		for (int j = 0; j <= 5; j++)
		{
			if (j == 5)
				i_next = 0;
			else
				i_next = j + 1;

			DrawLine(r, rounding(all_points[i + j].X), rounding(all_points[i + j].Y),
				rounding(all_points[i + i_next].X), rounding(all_points[i + i_next].Y), color);
		}
	}
}

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	SDL_Renderer* renderer = NULL;

	bool isFirst = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) == 0) 
		{
			SDL_bool done = SDL_FALSE;

			while (!done) 
			{
				SDL_Event event;

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

				if (isFirst)
				{
					drawMainLines(renderer);
					SetColor(&color, 255, 255, 255);
					drawHexagonNew(renderer);

					int count = 10;
					rotateAndDrawHexagon(renderer, count);
					isFirst = false;
				}				
				else 
				{
					
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
					SDL_RenderClear(renderer);
					drawMainLines(renderer);
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
					drawAllPoints(renderer);					
				}
					
				SDL_RenderPresent(renderer);

				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						done = SDL_TRUE;
					}

					if (event.type == SDL_KEYDOWN) 
					{ 
						switch (event.key.keysym.scancode)
						{
							case SDL_SCANCODE_LEFT:
								// Rotate left and scale down
								
								RotateLeftAndZoomOut();
								break;

							case SDL_SCANCODE_RIGHT:
								// Rotate right and scale up
								RotateRightAndZoomIn();
								break;
						}
					}
				}
			}
		}
	}

	SDL_DestroyRenderer(renderer);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}