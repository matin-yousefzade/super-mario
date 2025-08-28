#include "score.hpp"


Score::Score(int _x, int _y, std::string _name) {

	x = _x;
	y = _y;
	name = _name;
	vy = SCORE_VY;
	status = "fresh";
	climbing_time = 0;

	if(_name == "brick")
		score = SCORE_BRICK;

	else if(_name == "kill")
		score = SCORE_KILL;

	else if(_name == "coin")
		score = SCORE_COIN;

	else if(_name == "mushroom-health")
		score = SCORE_MUSHROOM_HEALTH;

	else if(_name == "mushroom-red")
		score = SCORE_MUSHROOM_RED;

	else if(_name == "flower")
		score = SCORE_FLOWER;

	else
		score = 0;

}


bool Score::is_fresh() {

	return (status == "fresh");

}



bool Score::is_dead() {

	return (climbing_time > SCORE_CLIMBING_TIME);

}


int Score::get_score() {

	if(status != "fresh")
		return 0;

	status = "taken";
	return score;

}


void Score::update_position() {

	y += vy;
	climbing_time++;

}


void Score::draw_yourself(Window &window, int camera_x) {

	if(name == "kill" || name == "mushroom-health" || name == "mushroom-red" || name == "flower")
		window.show_text(std::to_string(score), Point(x - camera_x, y), WHITE, "assets/fonts/FreeSansBold.otf", FONT_SIZE_SMALL);

}
