#include "fireball.hpp"


Fireball::Fireball(int _x, int _y, int _vx) {

	x = _x;
	y = _y;
	vx = _vx;
	vy = 0;
	status = "alive";

}


void Fireball::die(Window &window) {

	status = "dead";
	window.play_sound_effect("assets/sounds/bump.wav");

}


bool Fireball::is_dead() {

	return (status == "dead");

}


void Fireball::update_position(Window &window, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map) {

	x += vx;
	y += vy;
	vy += GRAVITY;

	for(int col = x / OBJECT_LENGTH - 1; col <= (x + FIREBALL_LENGTH) / OBJECT_LENGTH + 1; col++) {

		for(int row = y / OBJECT_WIDTH - 1; row <= (y + FIREBALL_WIDTH) / OBJECT_WIDTH + 1; row++) {

			if(col >= 0 && col < map[0].size() && row >= 0 && row < map.size()) {

				if(Object::is_object(map[row][col])) {

					int object_x = col * OBJECT_LENGTH;
					int object_y = row * OBJECT_WIDTH;
					char overlap_status = overlap(x, y, FIREBALL_LENGTH, FIREBALL_WIDTH, vx, vy - GRAVITY, object_x, object_y, OBJECT_LENGTH, OBJECT_WIDTH, 0, 0);

					if(overlap_status == 'u') {

						y = object_y - FIREBALL_WIDTH;
						vy = FIREBALL_VY;

					}

					else if(overlap_status == 'd' || overlap_status == 'r' || overlap_status == 'l') {

						this->die(window);

					}

				}

			}

		}

	}

	for(int i = 0; i < enemies.size(); i++) {

		if(enemies[i].get_status() != "dead") {

			char overlap_status = overlap(x, y, FIREBALL_LENGTH, FIREBALL_WIDTH, vx, vy - GRAVITY, enemies[i].get_x(), enemies[i].get_y(), ENEMY_LENGTH, enemies[i].get_width(), enemies[i].get_vx(), enemies[i].get_vy() - GRAVITY);

			if(overlap_status == 'u' || overlap_status == 'd' || overlap_status == 'r' || overlap_status == 'l') {

				scores.push_back(Score(enemies[i].get_x(), enemies[i].get_y(), "kill"));
				enemies[i].die(window);
				this->die(window);
				return;

			}

		}

	}

}


void Fireball::draw_yourself(Window &window, int camera_x) {

	window.fill_circle(Point(x + FIREBALL_LENGTH / 2 - camera_x, y + FIREBALL_WIDTH / 2), FIREBALL_LENGTH / 2, RED);

}
