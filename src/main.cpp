#include "camera.hpp"


using namespace std;


vector<string> read_map(string address) {

	string line;
	vector<string> map;

	ifstream file (address);

	while(getline(file, line))
		map.push_back(line);

	file.close();

	return map;

}


void handle_event(Window &window, Mario &mario, vector<Fireball> &fireballs, bool &pause) {

	if(window.has_pending_event()) {

		Event event = window.poll_for_event();

		if(event.get_type() == Event::KEY_PRESS) {

			if(pause == false && (event.get_pressed_key() == 'd' || event.get_pressed_key() == 'O'))
				mario.walk("right");

			else if(pause == false && (event.get_pressed_key() == 'a' || event.get_pressed_key() == 'P'))
				mario.walk("left");

			else if(pause == false && (event.get_pressed_key() == 'w' || event.get_pressed_key() == 'R'))
				mario.jump(window);

			else if(pause == false && (event.get_pressed_key() == 'f'))
				mario.fire(window, fireballs);

			else if(event.get_pressed_key() == 'p')
				pause = !pause;

			else if(event.get_pressed_key() == 'q')
				exit(0);

		}

		else if(event.get_type() == Event::KEY_RELEASE) {

			if(event.get_pressed_key() == 'd' || event.get_pressed_key() == 'O')
				mario.stand("right");

			else if(event.get_pressed_key() == 'a' || event.get_pressed_key() == 'P')
				mario.stand("left");

		}

		else if(event.get_type() == Event::QUIT) {

			exit(0);

		}

	}

}


void game_loop(Window &window, Camera &camera, Mario &mario, vector<string> &map) {

	bool previous_pause = false;
	bool current_pause = false;
	vector<Score> scores;
	vector<Object> objects;
	vector<Prize> prizes;
	vector<Enemy> enemies;
	vector<Fireball> fireballs;

	window.play_music("assets/sounds/background.mp3");

	camera.init();
	mario.init(map);

	while(mario.is_dead() == false && mario.has_victory() == false) {

		previous_pause = current_pause;
		handle_event(window, mario, fireballs, current_pause);

		if(previous_pause == false && current_pause == true) {

			window.pause_music();
			window.play_sound_effect("assets/sounds/pause.wav");

		}

		else if(previous_pause == true && current_pause == false) {

			window.resume_music();

		}

		else if(previous_pause == false && current_pause == false) {

			camera.update_yourself(mario.get_x());
			camera.update_scores(scores);
			camera.update_objects(objects, map);
			camera.update_prizes(prizes, map);
			camera.update_enemies(window, enemies, scores, map);
			camera.update_fireballs(window, fireballs, mario, enemies, scores, map);
			camera.update_mario(window, mario, objects, prizes, enemies, scores, map);

			camera.draw_yourself(window);
			camera.draw_scores(window, scores);
			camera.draw_prizes(window, prizes);
			camera.draw_objects(window, objects);
			camera.draw_enemies(window, enemies);
			camera.draw_fireballs(window, fireballs);
			camera.draw_mario(window, mario);

		}

		window.update_screen();
		delay(PERIOD);
	}

}


int main() {

	vector<string> map = read_map("assets/maps/map.txt");
	Window window(WINDOW_LENGTH, map.size() * OBJECT_WIDTH, "Super Mario");
	Camera camera(WINDOW_LENGTH, map.size() * OBJECT_WIDTH);
	Mario mario;

	while(mario.has_live()) {

		game_loop(window, camera, mario, map);

		if(mario.has_victory())
			break;

	}

	if(mario.has_victory())
		camera.victory(window);

	else
		camera.gameover(window);

	return 0;

}
