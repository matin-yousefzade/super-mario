#include "object.hpp"


Object::Object(int row, int col, std::vector<std::string> &map) {

	x = col * OBJECT_LENGTH;
	y = row * OBJECT_WIDTH;
	type = map[row][col];
	question_status = 0;
	jump_value = 0;

	up_free = !(row > 0 && Object::is_object(map[row - 1][col]));
	down_free = !(row < (map.size() - 1) && Object::is_object(map[row + 1][col]));
	right_free = !(col < (map[0].size() - 1) && Object::is_object(map[row][col + 1]));
	left_free = !(row > 0 && Object::is_object(map[row][col - 1]));

	if(map[row][col] == 'b')
		name = "brick";

	else if(map[row][col] == '@')
		name = "block";

	else if(map[row][col] == '#')
		name = "clay";

	else if(map[row][col] == '?' || map[row][col] == 'm' || map[row][col] == 'h')
		name = "question";

	else if(map[row][col] == '|') {

		if(map[row - 1][col] == '|') {

			if(map[row][col - 1] == '|')
				name = "pipe-body-right";

			else
				name = "pipe-body-left";

		} else {

			if(map[row][col - 1] == '|')
				name = "pipe-head-right";

			else
				name = "pipe-head-left";

		}

	}

	else if(map[row][col] == 'f') {

		if(map[row - 1][col] == 'f')
			name = "flag-body";

		else
			name = "flag-head";

	}

}


int Object::get_x() {

	return x;

}


int Object::get_y() {

	return y;

}


char Object::get_type() {

	return type;

}


std::string Object::get_name() {

	return name;

}


void Object::set_name(std::string _name) {

	name = _name;

}


bool Object::is_object(char _type) {

	return (_type == 'b' || _type == '@' || _type == '#' || _type == '?' || _type == 'm' || _type == 'h' || _type == '|' || _type == 'f');

}


bool Object::is_dead(int camera_x) {

	return ((x + OBJECT_LENGTH) < camera_x);

}


char Object::get_overlap(int _x, int _y, int _l, int _w, int _vx, int _vy) {

    int previous_up_overlap = (_y - _vy) + _w - y;
    int previous_down_overlap = y + OBJECT_WIDTH - (_y - _vy);
    int previous_right_overlap = x + OBJECT_LENGTH - (_x - _vx);
    int previous_left_overlap = (_x - _vx) + _l - x;

    int current_up_overlap = _y + _w - y;
    int current_down_overlap = y + OBJECT_WIDTH - _y;
    int current_right_overlap = x + OBJECT_LENGTH - _x;
    int current_left_overlap = _x + _l - x;

    if(current_up_overlap <= 0 || current_down_overlap <= 0 || current_right_overlap <= 0 || current_left_overlap <= 0)
        return 'n';

    else if(up_free && previous_up_overlap <= 0)
        return 'u';

    else if(down_free && previous_down_overlap <= 0)
        return 'd';

    else if(right_free && previous_right_overlap <= 0)
        return 'r';

    else if(left_free && previous_left_overlap <= 0)
        return 'l';

    else
        return '?';

}


void Object::update_position() {

	if(jump_value > 0)
		jump_value += OBJECT_JUMP_SPEED;

}


void Object::draw_yourself(Window &window, int camera_x) {

	std::string address = "assets/sprites/objects/" + name;

	if(name == "question") {

		question_status = (question_status + 1) % OBJECT_QUESTION_STATUS_MAX;
		address += "-" + std::to_string((int) (question_status / OBJECT_QUESTION_STATUS_NUM + 1));

	}

	int dy = abs(jump_value, OBJECT_JUMP_VALUE) - OBJECT_JUMP_VALUE;

	window.draw_img(address + ".png", Rectangle(Point(x - camera_x, y + dy), OBJECT_LENGTH, OBJECT_WIDTH), Rectangle(Point(0, 0), OBJECT_LENGTH, OBJECT_WIDTH));

}
