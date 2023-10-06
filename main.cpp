#include <iostream>
#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
#include <thread>
//#include <mutex>
#include <chrono>
#include <vector>
#include <algorithm>

//#define _USE_MATH_DEFINES
#include <math.h>

//#include <Windows.h>
//HDC hDC = GetDC(nullptr);
//const float refreshCorr = 60.0f / (float)GetDeviceCaps(hDC, VREFRESH);

//sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

const int screenWidth = 1920;
const int screenHeight = 1080;

const int cFrames = 60 * (6 * 60 + 36); // 23760
int nFrame = 0;

const int countBall = 150;
const int countStar = 500;
const float sizeBall = 6.5;
const float maxSpeed = 2;
const float distance = 255;

const float screenWidthF = screenWidth + distance;
const float screenHeightF = screenHeight + distance;

const float screenWidthF0 = -distance;
const float screenHeightF0 = -distance;

bool step = true;

const float pi180 = 180.0f / 3.141592f;

//

float rnd(float min, float max) {
	return min + (max - min) * static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX));
}

int rndi(int min, int max) {
	return min + std::rand() % (max - min + 1);
}

class CustomCircle : public sf::CircleShape {
	float speed = rnd(maxSpeed / 10, maxSpeed);
	float angle = rnd(0, 90) * pi180;
public:
	float speedX = speed * sin(angle);
	float speedY = speed * cos(angle);

public:
	CustomCircle()
		: sf::CircleShape(sizeBall) {

		int r = rndi(32, 255);
		int g = rndi(32, 255);
		int b = rndi(32, 255);

		setFillColor(sf::Color(r, g, b, 128));
		setOutlineColor(sf::Color(r, g, b, 255));
		setOutlineThickness(4);

		setPosition(sf::Vector2f(rnd(screenWidthF0, screenWidthF), rnd(screenWidthF0, screenHeightF)));

		setOrigin(getRadius(), getRadius());

		if (rndi(0, 1) == 0)
			speedX = -speedX;

		if (rndi(0, 1) == 0)
			speedY = -speedY;
	}
};

class Star : public sf::CircleShape {
public:
	float speed = rnd(0.1, 0.5);

public:
	Star()
		: sf::CircleShape(0.5) {

		unsigned int alpha = (int)round(speed * 2 * 255);

		setFillColor(sf::Color(255, 255, 255, alpha));

		setPosition(sf::Vector2f(rndi(0, screenWidth), rndi(0, screenHeight)));

	}
};


int main() {
	sf::RenderWindow window;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(screenWidth, screenHeight), "Neuro", sf::Style::None, settings);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	window.setMouseCursorVisible(false);

	sf::Texture tex;
	tex.create(screenWidth, screenHeight);

	//sf::RenderTexture renderTexture;
	//renderTexture.create(screenWidth, screenHeight);
	//renderTexture.setSmooth(false);
	//

	std::srand(std::time(nullptr));

	CustomCircle myBalls[countBall];
	CustomCircle myBalls2[countBall];
	for (int i = 0; i < countBall; ++i)
		myBalls2[i] = myBalls[i];

	Star myStars[countStar];

	//std::vector<CustomCircle> myBalls;
	//for (int i = 0; i < countBall; ++i)
	//	myBalls.push_back(CustomCircle());

	//
	float x, y, x2, y2, speedX, speedY;
	float dx, dy, dist;
	sf::Vector2f position;
	sf::Vector2f position2;

	const sf::Color Black = sf::Color::Black;

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();

			/*
			if (event.type == sf::Event::Resized) {

				screenWidth = event.size.width;
				screenHeight = event.size.height;

				screenWidthF = (float)screenWidth - 1.0f;
				screenHeightF = (float)screenHeight - 1.0f;

				sf::FloatRect view(0, 0, screenWidth, screenHeight);
				window.setView(sf::View(view));

			}
			*/
		}


		window.clear(Black);
		//renderTexture.clear(Black);

		for (size_t i = 0; i < countStar; ++i) {

			position = myStars[i].getPosition();

			y = position.y;
			x = position.x;

			y -= myStars[i].speed;

			if (y < 0) {
				y = screenHeight;
				x = rndi(0, screenWidth);
			}

			myStars[i].setPosition(x, y);

			window.draw(myStars[i]);
			//renderTexture.draw(myStars[i]);
		}

		CustomCircle(&arrayFrom)[countBall] = step ? myBalls : myBalls2;
		CustomCircle(&arrayTo)[countBall] = step ? myBalls2 : myBalls;
		step = !step;

		for (size_t i = 0; i < countBall; ++i) {

			position = arrayFrom[i].getPosition();
			x = position.x;
			y = position.y;

			speedX = arrayFrom[i].speedX;
			speedY = arrayFrom[i].speedY;

			//
			for (int j = i; j < countBall; ++j) {

				if (i == j)
					continue;

				position2 = arrayFrom[j].getPosition();
				x2 = position2.x;
				y2 = position2.y;

				dx = x2 - x;
				if (abs(dx) > distance)
					continue;
				dy = y2 - y;
				if (abs(dy) > distance)
					continue;

				dist = sqrt(dx * dx + dy * dy);

				if (dist > distance)
					continue;

				//
				sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f(x, y)),
					sf::Vertex(sf::Vector2f(x2, y2)),


					sf::Vertex(sf::Vector2f(x + 1, y)),
					sf::Vertex(sf::Vector2f(x2 - 1, y2)),

					sf::Vertex(sf::Vector2f(x - 1, y)),
					sf::Vertex(sf::Vector2f(x2 + 1, y2)),


					sf::Vertex(sf::Vector2f(x, y + 1)),
					sf::Vertex(sf::Vector2f(x2, y2 - 1)),

					sf::Vertex(sf::Vector2f(x, y - 1)),
					sf::Vertex(sf::Vector2f(x2, y2 + 1)),

				};

				sf::Color c1 = myBalls[i].getOutlineColor();
				sf::Color c2 = myBalls[j].getOutlineColor();

				c1.a = (int)(255 - dist);
				c2.a = (int)(255 - dist);

				for (int k = 0; k < 10; k += 2) {
					line[k].color = c1;
					line[k + 1].color = c2;
				}

				window.draw(line, 10, sf::Lines);
				//renderTexture.draw(&line[0], 10, sf::Lines);

			}
			//

			if (x > screenWidthF) {
				x = screenWidthF0;
				//speedX = -speedX;
			}

			if (x < screenWidthF0) {
				x = screenWidthF;
				//speedX = -speedX;
			}

			if (y > screenHeightF) {
				y = screenHeightF0;
				//speedY = -speedY;
			}

			if (y < screenHeightF0) {
				y = screenHeightF;
				//speedY = -speedY;
			}

			x += speedX;
			y += speedY;
			//

			window.draw(arrayFrom[i]);
			//renderTexture.draw(arrayFrom[i]);

			arrayTo[i].setPosition(x, y);
			arrayTo[i].speedX = speedX;
			arrayTo[i].speedY = speedY;

		}

		window.display();

		//tex.update(window);
		//tex.copyToImage().saveToFile("frames/" + std::to_string(nFrame) + ".png");

		//++nFrame;
		//if (nFrame > cFrames)
		//	window.close();

	}

}
