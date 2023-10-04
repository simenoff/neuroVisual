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

sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
const unsigned int screenWidth = desktop.width;
const unsigned int screenHeight = desktop.height;

//unsigned int screenWidth = 1200;
//unsigned int screenHeight = 800;

float screenWidthF = (float)screenWidth + 0.0f;
float screenHeightF = (float)screenHeight + 0.0f;

const unsigned int countBall = 100;
const float sizeBall = 5.5f;
const float maxSpeed = 3;
const float distance = 255;

const unsigned int countStar = 500;

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
		setOutlineThickness(3);

		setPosition(sf::Vector2f(rnd(0, screenWidthF), rnd(0, screenHeightF)));

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

		unsigned int alpha = (int)round(speed*2*255);

		setFillColor(sf::Color(255, 255, 255, alpha));

		setPosition(sf::Vector2f(rndi(0, screenWidth), rndi(0, screenHeight)));

	}
};

int main() {
	sf::ContextSettings settings;
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenWidth, screenHeight), "Neuro", sf::Style::None, sf::ContextSettings(0,0,8));
	window.clear(sf::Color::Black);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	window.setMouseCursorVisible(false);

	//settings.antialiasingLevel = 8;

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
				};

				sf::Color c1 = myBalls[i].getOutlineColor();
				sf::Color c2 = myBalls[j].getOutlineColor();

				c1.a = 255 - dist;
				c2.a = 255 - dist;

				line[0].color = c1;
				line[1].color = c2;

				window.draw(line, 2, sf::Lines);
				//

			}
			//

			if (x > screenWidthF) {
				//x = screenWidthF;
				speedX = -speedX;
			}

			if (x < 0) {
				//x = -20;
				speedX = -speedX;
			}

			if (y > screenHeightF) {
				//y = screenHeightF;
				speedY = -speedY;
			}

			if (y < 0) {
				//y = -20;
				speedY = -speedY;
			}

			x += speedX;
			y += speedY;
			//

			window.draw(arrayFrom[i]);

			arrayTo[i].setPosition(x, y);
			arrayTo[i].speedX = speedX;
			arrayTo[i].speedY = speedY;

		}

		window.display();

	}

}
