#include "mario.hpp"


Mario::Mario() {

	lives = MARIO_LIVES;
	victory = false;

}


void Mario::init(std::vector<std::string> &map) {

	vx = 0;
	vy = 0;
	width = MARIO_WIDTH_NORMAL;
	mode = "normal";
	status = "standing";
	direction = "right";
	ghost = false;
	ghost_timer = 0;
	death_timer = 0;
	walking_status = -1;
	sliding_status = 0;
	coins = 0;
	score = 0;
	fire_delay = 0;

	for(int row = 0; row < map.size(); row++) {

		for(int col = 0; col < map[row].size(); col++) {

			if(map[row][col] == 'M') {

				x = col * OBJECT_LENGTH;
				y = row * OBJECT_WIDTH;

				return;

			}

		}

	}

}


int Mario::get_x() {

	return x;

}


int Mario::get_y() {

	return y;

}


void Mario::stand(std::string _direction) {

	if(direction == _direction) {

		if(status == "walking") {

			vx = 0;
			status = "standing";
			walking_status = -1;

		}

		else if(status == "jumping") {

			walking_status = -1;

		}

	}

}


void Mario::land() {

	if(status == "jumping") {

		if(walking_status == -1) {

			vx = 0;
			status = "standing";

		}

		else {

			status = "walking";

		}

	}

}


void Mario::walk(std::string _direction) {

	if(status == "standing" || (status == "sliding" && vx == 0)) {

		vx = (_direction == "right") ? MARIO_VX : -MARIO_VX;
		status = "walking";
		direction = _direction;
		walking_status = 0;

	}

	else if(status == "walking" && direction != _direction) {

		status = "sliding";
		sliding_status = 0;

	}

}


void Mario::jump(Window &window) {

	if(status == "standing" || status == "walking") {

		vy = MARIO_VY;
		status = "jumping";
		window.play_sound_effect("assets/sounds/jump-small.wav");

	}

}


void Mario::fire(Window &window, std::vector<Fireball> &fireballs) {

	if(mode == "white" && fire_delay > MARIO_FIRE_DELAY) {

		fire_delay = 0;
		window.play_sound_effect("assets/sounds/fireball.wav");

		if(direction == "right")
			fireballs.push_back(Fireball(x + MARIO_LENGTH, y + (MARIO_WIDTH_BIG_WHITE - MARIO_WIDTH_NORMAL), FIREBALL_VX));

		else
			fireballs.push_back(Fireball(x - FIREBALL_LENGTH, y + (MARIO_WIDTH_BIG_WHITE - MARIO_WIDTH_NORMAL), -FIREBALL_VX));

	}

}


void Mario::die(Window &window) {

	if(mode == "big" || mode == "white") {

		y += (MARIO_WIDTH_BIG_WHITE - MARIO_WIDTH_NORMAL);
		width = MARIO_WIDTH_NORMAL;
		mode = "normal";
		ghost = true;
		ghost_timer = 0;

	}

	else {

		vx = 0;
		vy = MARIO_VY;
		status = "dead";
		death_timer = 0;
		lives--;
		window.stop_music();
		window.play_sound_effect("assets/sounds/mario-death.wav");

	}

}


bool Mario::is_dead() {

	return (status == "dead" && death_timer > MARIO_DEATH_TIME);

}


bool Mario::has_live() {

	return (lives > 0);

}


bool Mario::has_victory() {

	return victory;

}


void Mario::take_prize(Window &window, Prize &prize, std::vector<Score> &scores) {

	if(prize.get_name() == "mushroom-health") {

		if(lives < MARIO_LIVES)
			lives++;

	}

	else if(prize.get_name() == "mushroom-red") {

		y -= (MARIO_WIDTH_BIG_WHITE - MARIO_WIDTH_NORMAL);
		width = MARIO_WIDTH_BIG_WHITE;
		mode = "big";

	}

	else if(prize.get_name() == "flower") {

		mode = "white";
		fire_delay = 0;

	}

	if(prize.get_name() != "coin") {

		scores.push_back(Score(prize.get_x(), prize.get_y(), prize.get_name()));
		window.play_sound_effect("assets/sounds/powerup.wav");
		prize.die();

	}

}


void Mario::take_new_scores(std::vector<Score> &scores) {

	int i = scores.size() - 1;

	while(i >= 0 && scores[i].is_fresh()) {

		score += scores[i--].get_score();

	}

}


void Mario::update_position(Window &window, int camera_x, std::vector<Object> &objects, std::vector<Prize> &prizes, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map) {

	x += vx;
	y += vy;
	vy += GRAVITY;

	if(x < camera_x)
		x = camera_x;

	if(status == "walking")
		walking_status = (walking_status + 1) % MARIO_WALKING_STATUS_MAX;

	else if(status == "sliding" && vx != 0) {

		if(((sliding_status++) % MARIO_SLIDING_STATUS_PERIOD) == 0)
			vx = (direction == "right") ? (vx - 1) : (vx + 1);

	}

	else if(status == "dead") {

		death_timer++;
		return;

	}

	if(mode == "white")
		fire_delay++;

	for(int i = 0; i < objects.size(); i++) {

		char overlap_status = objects[i].get_overlap(x, y, MARIO_LENGTH, width, vx, vy - GRAVITY);

		if(overlap_status == 'u' || overlap_status == 'd' || overlap_status == 'r' || overlap_status == 'l') {

			if(objects[i].get_name().substr(0, 4) == "flag") {

				victory = true;
				window.stop_music();
				return;

			}

		}

		if(overlap_status == 'u') {

			y = objects[i].get_y() - width;
			vy = 0;
			this->land();

		}

		else if(overlap_status == 'd') {

			y = objects[i].get_y() + OBJECT_WIDTH;
			vy = 0;

			if(objects[i].get_name() == "brick" || objects[i].get_name() == "question") {

				objects[i].jump(window, enemies, scores);

			}

			if(objects[i].get_name() == "brick" && (mode == "big" || mode == "white")) {

				objects[i].set_name("brick-debris");
				window.play_sound_effect("assets/sounds/brick-smash.wav");
				scores.push_back(Score(objects[i].get_x(), objects[i].get_y(), "brick"));

			}

			else if(objects[i].get_name() == "question") {

				if(objects[i].get_type() == '?')
					coins++;

				prizes.push_back(Prize(window, objects[i], scores, mode));
				objects[i].set_name("question-empty");

			}

		}

		else if(overlap_status == 'r') {

			x = objects[i].get_x() + OBJECT_LENGTH;

		}

		else if(overlap_status == 'l') {

			x = objects[i].get_x() - MARIO_LENGTH;

		}

	}

	if(ghost == true) {

		if((y + width) > (map.size() * OBJECT_WIDTH)) {

			y = (map.size() * OBJECT_WIDTH) - width;
			vy = 0;
			this->land();

		}

		ghost = ((ghost_timer++) <= MARIO_GHOST_TIME);
		return;

	}

	else {

		if((y + width) > (map.size() * OBJECT_WIDTH)) {

			this->die(window);
			return;

		}

	}

	for(int i = 0; i < prizes.size(); i++) {

		char overlap_status = overlap(x, y, MARIO_LENGTH, width, vx, vy - GRAVITY, prizes[i].get_x(), prizes[i].get_y(), PRIZE_LENGTH, PRIZE_WIDTH, prizes[i].get_vx(), prizes[i].get_vy() - GRAVITY);

		if(overlap_status == 'u' || overlap_status == 'd' || overlap_status == 'r' || overlap_status == 'l') {

			this->take_prize(window, prizes[i], scores);

		}

	}

	for(int i = 0; i < enemies.size(); i++) {

		if(enemies[i].get_status() != "dead") {

			char overlap_status = overlap(x, y, MARIO_LENGTH, width, vx, vy - GRAVITY, enemies[i].get_x(), enemies[i].get_y(), ENEMY_LENGTH, enemies[i].get_width(), enemies[i].get_vx(), enemies[i].get_vy() - GRAVITY);

			if(overlap_status == 'u') {

				scores.push_back(Score(enemies[i].get_x(), enemies[i].get_y(), "kill"));
				enemies[i].die(window);

			}

			else if(overlap_status == 'd' || overlap_status == 'r' || overlap_status == 'l') {

				this->die(window);

			}

		}

	}

}


void Mario::draw_yourself(Window &window, int camera_x) {

	if(ghost == true && ((int) (ghost_timer / MARIO_GHOST_BLINKING_PERIOD)) % 2 == 0)
		return;

	std::string address = "assets/sprites/mario/" + mode + "/" + status;

	if(status != "dead")
		address += "-" + direction;

	if(status == "walking")
		address += "-" + std::to_string((int) (walking_status / MARIO_WALKING_STATUS_NUM + 1));

	window.draw_img(address + ".png", Rectangle(Point(x - camera_x, y), MARIO_LENGTH, width), Rectangle(Point(0, 0), MARIO_LENGTH, width));

}


void Mario::show_info(Window &window) {

	window.show_text("SCORE", Point(100, 0), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_MEDIUM);
	window.show_text(std::to_string(score), Point(100, 50), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_MEDIUM);

	window.show_text("COINS", Point(520, 0), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_MEDIUM);
	window.show_text(std::to_string(coins), Point(520, 50), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_MEDIUM);

	window.show_text("LIVES", Point(950, 0), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_MEDIUM);
	window.show_text(std::to_string(lives), Point(950, 50), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_MEDIUM);

}
