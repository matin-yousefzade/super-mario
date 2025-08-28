#include "prize.hpp"


Prize::Prize(Window &window, Object &object, std::vector<Score> &scores, std::string mario_mode) {

	x = object.get_x();
	y = object.get_y();
	type = object.get_type();
	status = "climbing";
	climbing_time = 0;

	if(type == '?') {

		vx = 0;
		vy = COIN_VY;
		name = "coin";
		scores.push_back(Score(x, y, "coin"));
		window.play_sound_effect("assets/sounds/coin.wav");

	}

	else if(type == 'h') {

		vx = 0;
		vy = PRIZE_VY;
		name = "mushroom-health";
		window.play_sound_effect("assets/sounds/powerup-appears.wav");

	}

	else if(type == 'm') {

		vx = 0;
		vy = PRIZE_VY;
		name = (mario_mode == "big") ? "flower" : "mushroom-red";
		window.play_sound_effect("assets/sounds/powerup-appears.wav");

	}

}


int Prize::get_x() {

	return x;

}


int Prize::get_y() {

	return y;

}


int Prize::get_vx() {

	return vx;

}


int Prize::get_vy() {

	return vy;

}


std::string Prize::get_name() {

	return name;

}


std::string Prize::get_status() {

	return status;

}


void Prize::die() {

	status = "dead";

}


bool Prize::is_dead() {

	return (status == "dead");

}


void Prize::update_position(std::vector<std::string> &map) {

	if(status == "climbing") {

		y += vy;
		climbing_time++;

		if(climbing_time > PRIZE_CLIMBING_TIME) {

			if(name == "coin") {

				status = "dead";

			}

			else {

				vx = (name == "flower") ? 0 : PRIZE_VX;
				vy = 0;
				status = "walking";

			}

		}

	}

	else if(status == "walking") {

		x += vx;
		y += vy;
		vy += GRAVITY;

		for(int col = x / OBJECT_LENGTH - 1; col <= (x + PRIZE_LENGTH) / OBJECT_LENGTH + 1; col++) {

			for(int row = y / OBJECT_WIDTH - 1; row <= (y + PRIZE_WIDTH) / OBJECT_WIDTH + 1; row++) {

				if(col >= 0 && col < map[0].size() && row >= 0 && row < map.size()) {

					if(Object::is_object(map[row][col])) {

						int object_x = col * OBJECT_LENGTH;
						int object_y = row * OBJECT_WIDTH;
						char overlap_status = overlap(x, y, PRIZE_LENGTH, PRIZE_WIDTH, vx, vy - GRAVITY, object_x, object_y, OBJECT_LENGTH, OBJECT_WIDTH, 0, 0);

						if(overlap_status == 'u') {

							y = object_y - PRIZE_WIDTH;
							vy = 0;

						}

						else if(overlap_status == 'd') {

							y = object_y + OBJECT_WIDTH;
							vy = 0;

						}

						else if(overlap_status == 'r') {

							x = object_x + OBJECT_LENGTH;
							vx *= -1;

						}

						else if(overlap_status == 'l') {

							x = object_x - PRIZE_LENGTH;
							vx *= -1;

						}

					}

				}

			}

		}

	}

}


void Prize::draw_yourself(Window &window, int camera_x) {

	std::string address = "assets/sprites/prizes/" + name;

	window.draw_img(address + ".png", Rectangle(Point(x - camera_x, y), PRIZE_LENGTH, PRIZE_WIDTH), Rectangle(Point(0, 0), PRIZE_LENGTH, PRIZE_WIDTH));

}
