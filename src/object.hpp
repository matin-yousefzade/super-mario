#ifndef OBJECT_H
#define OBJECT_H


#include "score.hpp"


class Enemy;


class Object {

    public:
        Object(int row, int col, std::vector<std::string> &map);
        int get_x();
        int get_y();
        char get_type();
        std::string get_name();
        void set_name(std::string _name);
        static bool is_object(char _type);
        bool is_dead(int camera_x);
        char get_overlap(int _x, int _y, int _l, int _w, int _vx, int _vy);
        void jump(Window &window, std::vector<Enemy> &enemies, std::vector<Score> &scores);
        void update_position();
        void draw_yourself(Window &window, int camera_x);

    private:
        int x;
        int y;
        char type;
        std::string name;
        int question_status;
        int jump_value;
        bool up_free;
        bool down_free;
        bool right_free;
        bool left_free;

};


#endif
