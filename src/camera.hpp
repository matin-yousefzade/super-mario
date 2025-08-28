#ifndef CAMERA_H
#define CAMERA_H


#include "mario.hpp"


class Camera {

    public:
        Camera(int _length, int _width);
        void init();
        void update_yourself(int mario_x);
        void update_scores(std::vector<Score> &scores);
        void update_objects(std::vector<Object> &objects, std::vector<std::string> &map);
        void update_prizes(std::vector<Prize> &prizes, std::vector<std::string> &map);
        void update_enemies(Window &window, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map);
        void update_fireballs(Window &window, std::vector<Fireball> &fireballs, Mario &mario, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map);
        void update_mario(Window &window, Mario &mario, std::vector<Object> &objects, std::vector<Prize> &prizes, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map);
        void draw_yourself(Window &window);
        void draw_scores(Window &window, std::vector<Score> &scores);
        void draw_objects(Window &window, std::vector<Object> &objects);
        void draw_prizes(Window &window, std::vector<Prize> &prizes);
        void draw_enemies(Window &window, std::vector<Enemy> &enemies);
        void draw_fireballs(Window &window, std::vector<Fireball> &fireballs);
        void draw_mario(Window &window, Mario &mario);
        void victory(Window &window);
        void gameover(Window &window);

    private:
        int x;
        int length;
        int width;
        int objects_map_column;
        int enemies_map_column;

};


#endif
