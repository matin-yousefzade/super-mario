#ifndef SCORE_H
#define SCORE_H


#include <string>
#include <vector>
#include <fstream>
#include "rsdl.hpp"
#include "defines.hpp"
#include "utils.hpp"


class Score {

    public:
        Score(int _x, int _y, std::string _name);
        bool is_fresh();
        bool is_dead();
        int get_score();
        void update_position();
        void draw_yourself(Window &window, int camera_x);

    private:
        int x;
        int y;
        int vy;
        std::string name;
        std::string status;
        int climbing_time;
        int score;

};


#endif
