#ifndef MARIO_H
#define MARIO_H


#include "prize.hpp"
#include "fireball.hpp"


class Mario {

    public:
        Mario();
        void init(std::vector<std::string> &map);
        int get_x();
        int get_y();
        void stand(std::string _direction);
        void land();
        void walk(std::string direction);
        void jump(Window &window);
        void fire(Window &window, std::vector<Fireball> &fireballs);
        void die(Window &window);
        bool is_dead();
        bool has_live();
        bool has_victory();
        void take_prize(Window &window, Prize &prize, std::vector<Score> &scores);
        void take_new_scores(std::vector<Score> &scores);
        void update_position(Window &window, int camera_x, std::vector<Object> &objects, std::vector<Prize> &prizes, std::vector<Enemy> &enemies, std::vector<Score> &scores, std::vector<std::string> &map);
        void draw_yourself(Window &window, int camera_x);
        void show_info(Window &window);

    private:
        int x;
        int y;
        int vx;
        int vy;
        int width;
        std::string mode;
        std::string status;
        std::string direction;
        bool victory;
        bool ghost;
        int ghost_timer;
        int death_timer;
        int walking_status;
        int sliding_status;
        int lives;
        int coins;
        int score;
        int fire_delay;

};


#endif
