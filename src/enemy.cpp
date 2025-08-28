#include "enemy.hpp"


Enemy::Enemy(int row, int col, std::vector<std::string> &map) {

	x = col * OBJECT_LENGTH;
	vx = -ENEMY_VX;
	vy = 0;
	type = map[row][col];
	status = "walking";
	direction = "left";
	walking_status = 0;

	if(map[row][col] == 'l') {

		y = row * OBJECT_WIDTH;
		width = GOOMBA_WIDTH;
		name = "goomba";

	}

	else if(map[row][col] == 'k') {

		y = row * OBJECT_WIDTH - (KOOPA_WIDTH - OBJECT_WIDTH);
		width = KOOPA_WIDTH;
		name = "koopa";

	}

}


int Enemy::get_x() {

	return x;

}


int Enemy::get_y() {

	return y;

}


int Enemy::get_vx() {

	return vx;

}


int Enemy::get_vy() {

	return vy;

}


int Enemy::get_width() {

	return width;

}


std::string Enemy::get_status() {

	return status;

}


void Enemy::die(Window &window, std::string mode) {

	window.play_sound_effect("assets/sounds/enemy-stomp.wav");

	if(name == "goomba") {

		vx = 0;
		status = "dead";
		death_timer = 0;

	}

	else if(name == "koopa") {

		if(mode == "forced") {

			vx = 0;
			status = "dead";
			death_timer = 0;
			y += (KOOPA_WIDTH - OBJECT_WIDTH);
			width -= (KOOPA_WIDTH - OBJECT_WIDTH);

		}

		else if(status == "walking") {

			vx = 0;
			status = "halfdead";
			y += (KOOPA_WIDTH - OBJECT_WIDTH);
			width -= (KOOPA_WIDTH - OBJECT_WIDTH);

		}

		else if(status == "halfdead") {

			vx = KOOPA_ZOMBIE_VX;
			status = "zombie";

		}

		else if(status == "zombie") {

			vx = 0;
			status = "dead";
			death_timer = 0;

		}

	}

}


bool Enemy::is_enemy(char _type) {

	return (_type == 'l' || _type == 'k');

}


bool Enemy::is_dead() {

	return (status == "dead" && death_timer > ENEMY_DEATH_TIME);

}


void Enemy::update_position(Window &window, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map) {

	x += vx;
	y += vy;
	vy += GRAVITY;

	if(status == "walking")
		walking_status = (walking_status + 1) % ENEMY_WALKING_STATUS_MAX;

	else if(status == "dead")
		death_timer++;

	for(int col = x / OBJECT_LENGTH - 1; col <= (x + ENEMY_LENGTH) / OBJECT_LENGTH + 1; col++) {

		for(int row = y / OBJECT_WIDTH - 1; row <= (y + width) / OBJECT_WIDTH + 1; row++) {

			if(col >= 0 && col < map[0].size() && row >= 0 && row < map.size()) {

				if(Object::is_object(map[row][col])) {

					int object_x = col * OBJECT_LENGTH;
					int object_y = row * OBJECT_WIDTH;
					char overlap_status = overlap(x, y, ENEMY_LENGTH, width, vx, vy - GRAVITY, object_x, object_y, OBJECT_LENGTH, OBJECT_WIDTH, 0, 0);

					if(overlap_status == 'u') {

						y = object_y - width;
						vy = 0;

					}

					else if(overlap_status == 'd') {

						y = object_y + OBJECT_WIDTH;
						vy = 0;

					}

					else if(overlap_status == 'r') {

						x = object_x + OBJECT_LENGTH;
						vx *= -1;
						direction = "left";

					}

					else if(overlap_status == 'l') {

						x = object_x - ENEMY_LENGTH;
						vx *= -1;
						direction = "right";

					}

				}

			}

		}

	}

	if(name == "koopa" && status == "zombie") {

		for(int i = 0; i < enemies.size(); i++) {

			if(enemies[i].get_status() != "dead" && &(enemies[i]) != this) {

				char overlap_status = overlap(x, y, ENEMY_LENGTH, width, vx, vy - GRAVITY, enemies[i].get_x(), enemies[i].get_y(), ENEMY_LENGTH, enemies[i].get_width(), enemies[i].get_vx(), enemies[i].get_vy() - GRAVITY);

				if(overlap_status == 'u' || overlap_status == 'd' || overlap_status == 'r' || overlap_status == 'l') {

					scores.push_back(Score(enemies[i].get_x(), enemies[i].get_y(), "kill"));
					enemies[i].die(window, "forced");

				}

			}

		}

	}

}


void Enemy::draw_yourself(Window &window, int camera_x) {

	std::string address = "assets/sprites/enemies/" + name;

	if(status == "walking") {

		address += "-" + status;

		if(name == "koopa")
			address += "-" + direction;

		address += "-" + std::to_string((int) (walking_status / ENEMY_WALKING_STATUS_NUM + 1));

	}

	else {

		address += "-dead";

	}

	window.draw_img(address + ".png", Rectangle(Point(x - camera_x, y), ENEMY_LENGTH, width), Rectangle(Point(0, 0), ENEMY_LENGTH, width));

}


void Object::jump(Window &window, std::vector<Enemy> &enemies, std::vector<Score> &scores) {

	jump_value = 2 * OBJECT_JUMP_VALUE;

	for(int i = 0; i < enemies.size(); i++) {

		if(enemies[i].get_status() != "dead") {

			char overlap_status = overlap(x, y - 5, OBJECT_LENGTH, OBJECT_WIDTH, 0, -5, enemies[i].get_x(), enemies[i].get_y(), ENEMY_LENGTH, enemies[i].get_width(), enemies[i].get_vx(), enemies[i].get_vy() - GRAVITY);

			if(overlap_status == 'd') {

				scores.push_back(Score(enemies[i].get_x(), enemies[i].get_y(), "kill"));
				enemies[i].die(window);

			}

		}

	}

}
