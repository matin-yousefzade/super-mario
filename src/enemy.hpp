#ifndef ENEMY_H
#define ENEMY_H


#include "object.hpp"


class Enemy {

    public:
        Enemy(int row, int col, std::vector<std::string> &map);
        int get_x();
        int get_y();
        int get_vx();
        int get_vy();
        int get_width();
        std::string get_status();
        void die(Window &window, std::string mode="normal");
        static bool is_enemy(char _type);
        bool is_dead();
        void update_position(Window &window, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map);
        void draw_yourself(Window &window, int camera_x);

    private:
        int x;
        int y;
        int vx;
        int vy;
        int width;
        char type;
        std::string name;
        std::string status;
        std::string direction;
        int walking_status;
        int death_timer;

};


#endif
