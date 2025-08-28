#ifndef PRIZE_H
#define PRIZE_H


#include "object.hpp"


class Prize {

    public:
        Prize(Window &window, Object &object, std::vector<Score> &scores, std::string mario_mode);
        int get_x();
        int get_y();
        int get_vx();
        int get_vy();
        std::string get_name();
        std::string get_status();
        void die();
        bool is_dead();
        void update_position(std::vector<std::string> &map);
        void draw_yourself(Window &window, int camera_x);

    private:
        int x;
        int y;
        int vx;
        int vy;
        char type;
        std::string name;
        std::string status;
        int climbing_time;

};


#endif
