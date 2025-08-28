#ifndef FIREBALL_H
#define FIREBALL_H


#include "enemy.hpp"


class Fireball {

    public:
        Fireball(int _x, int _y, int _vx);
        void die(Window &window);
        bool is_dead();
        void update_position(Window &window, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map);
        void draw_yourself(Window &window, int camera_x);

    private:
        int x;
        int y;
        int vx;
        int vy;
        std::string status;

};


#endif
