#include "utils.hpp"


char overlap(int x1, int y1, int l1, int w1, int vx1, int vy1, int x2, int y2, int l2, int w2, int vx2, int vy2) {

    int previous_up_overlap = (y1 - vy1) + w1 - (y2 - vy2);
    int previous_down_overlap = (y2 - vy2) + w2 - (y1 - vy1);
    int previous_right_overlap = (x2 - vx2) + l2 - (x1 - vx1);
    int previous_left_overlap = (x1 - vx1) + l1 - (x2 - vx2);

    int current_up_overlap = y1 + w1 - y2;
    int current_down_overlap = y2 + w2 - y1;
    int current_right_overlap = x2 + l2 - x1;
    int current_left_overlap = x1 + l1 - x2;

    if(current_up_overlap <= 0 || current_down_overlap <= 0 || current_right_overlap <= 0 || current_left_overlap <= 0)
        return 'n';

    else if(previous_up_overlap <= 0)
        return 'u';

    else if(previous_down_overlap <= 0)
        return 'd';

    else if(previous_right_overlap <= 0)
        return 'r';

    else if(previous_left_overlap <= 0)
        return 'l';

    else
        return '?';

}


int abs(int a, int b) {

    int diff = a - b;

    return (diff >= 0) ? diff : -diff;

}
