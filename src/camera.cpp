#include "camera.hpp"


Camera::Camera(int _length, int _width) {

	length = _length;
	width = _width;

}


void Camera::init() {

	x = 0;
	objects_map_column = 0;
	enemies_map_column = 0;

}


void Camera::update_yourself(int mario_x) {

	if((mario_x - x) > ((WINDOW_LENGTH - MARIO_LENGTH) / 2))
		x = mario_x - ((WINDOW_LENGTH - MARIO_LENGTH) / 2);

}


void Camera::update_scores(std::vector<Score> &scores) {

	for(int i = 0; i < scores.size(); i++) {

		if(scores[i].is_dead())
			scores.erase(scores.begin() + i--);

	}

	for(int i = 0; i < scores.size(); i++) {

		scores[i].update_position();

	}

}


void Camera::update_objects(std::vector<Object> &objects, std::vector<std::string> &map) {

	for(int i = 0; i < objects.size(); i++) {

		if(objects[i].is_dead(x))
			objects.erase(objects.begin() + i--);

	}

	for(int col = objects_map_column; col <= (x + WINDOW_LENGTH) / OBJECT_LENGTH; col++) {

		for(int row = 0; row < map.size(); row++) {

			if(Object::is_object(map[row][col]))
				objects.push_back(Object(row , col, map));

		}

	}

	objects_map_column = (x + WINDOW_LENGTH) / OBJECT_LENGTH + 1;

	for(int i = 0; i < objects.size(); i++) {

		objects[i].update_position();

	}

}


void Camera::update_prizes(std::vector<Prize> &prizes, std::vector<std::string> &map) {

	for(int i = 0; i < prizes.size(); i++) {

		if(prizes[i].is_dead())
			prizes.erase(prizes.begin() + i--);

	}

	for(int i = 0; i < prizes.size(); i++) {

		prizes[i].update_position(map);

	}

}


void Camera::update_enemies(Window &window, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map) {

	for(int i = 0; i < enemies.size(); i++) {

		if(enemies[i].is_dead())
			enemies.erase(enemies.begin() + i--);

	}

	for(int col = enemies_map_column; col <= (x + WINDOW_LENGTH) / OBJECT_LENGTH; col++) {

		for(int row = 0; row < map.size(); row++) {

			if(Enemy::is_enemy(map[row][col]))
				enemies.push_back(Enemy(row , col, map));

		}

	}

	enemies_map_column = (x + WINDOW_LENGTH) / OBJECT_LENGTH + 1;

	for(int i = 0; i < enemies.size(); i++) {

		enemies[i].update_position(window, enemies, scores, map);

	}

}


void Camera::update_fireballs(Window &window, std::vector<Fireball> &fireballs, Mario &mario, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map) {

	for(int i = 0; i < fireballs.size(); i++) {

		if(fireballs[i].is_dead())
			fireballs.erase(fireballs.begin() + i--);

	}

	for(int i = 0; i < fireballs.size(); i++) {

		fireballs[i].update_position(window, enemies, scores, map);

	}

}


void Camera::update_mario(Window &window, Mario &mario, std::vector<Object> &objects, std::vector<Prize> &prizes, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map) {

	mario.update_position(window, x, objects, prizes, enemies, scores, map);
	mario.take_new_scores(scores);

}


void Camera::draw_yourself(Window &window) {

	int x_begin = x % BACKGROUND_LENGTH;
	int y_begin = BACKGROUND_WIDTH - width;

	if((x_begin + length) <= BACKGROUND_LENGTH) {

		window.draw_img("assets/background/background.png", Rectangle(Point(0, 0), length, width), Rectangle(Point(x_begin, y_begin), length, width));

	}

	else {

		window.draw_img("assets/background/background.png", Rectangle(Point(0, 0), BACKGROUND_LENGTH - x_begin, width), Rectangle(Point(x_begin, y_begin), BACKGROUND_LENGTH - x_begin, width));
		window.draw_img("assets/background/background.png", Rectangle(Point(BACKGROUND_LENGTH - x_begin, 0), (x + length) % BACKGROUND_LENGTH, width), Rectangle(Point(0, y_begin), (x + length) % BACKGROUND_LENGTH, width));

	}

}


void Camera::draw_scores(Window &window, std::vector<Score> &scores) {

	for(int i = 0; i < scores.size(); i++) {

		scores[i].draw_yourself(window, x);

	}

}


void Camera::draw_objects(Window &window, std::vector<Object> &objects) {

	for(int i = 0; i < objects.size(); i++) {

		objects[i].draw_yourself(window, x);

	}

}


void Camera::draw_prizes(Window &window, std::vector<Prize> &prizes) {

	for(int i = 0; i < prizes.size(); i++) {

		prizes[i].draw_yourself(window, x);

	}

}


void Camera::draw_enemies(Window &window, std::vector<Enemy> &enemies) {

	for(int i = 0; i < enemies.size(); i++) {

		enemies[i].draw_yourself(window, x);

	}

}


void Camera::draw_fireballs(Window &window, std::vector<Fireball> &fireballs) {

	for(int i = 0; i < fireballs.size(); i++) {

		fireballs[i].draw_yourself(window, x);

	}

}


void Camera::draw_mario(Window &window, Mario &mario) {

	mario.draw_yourself(window, x);
	mario.show_info(window);

}


void Camera::victory(Window &window) {

	window.clear();
	window.show_text("VICTORY", Point(length / 2 - 310, width / 2 - 100), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_BIG);
	window.play_sound_effect("assets/sounds/victory.wav");
	window.update_screen();
	delay(DELAY_VICTORY);

}


void Camera::gameover(Window &window) {

	window.clear();
	window.show_text("GAMEOVER", Point(length / 2 - 420, width / 2 - 100), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_BIG);
	window.play_sound_effect("assets/sounds/gameover.wav");
	window.update_screen();
	delay(DELAY_GAMEOVER);

}
