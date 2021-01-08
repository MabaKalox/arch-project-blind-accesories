//
// Created by maba_kalox on 12/10/20.
//

#ifndef ARCH_PROJECT_DISTANCEROLLCLASS_H
#define ARCH_PROJECT_DISTANCEROLLCLASS_H


class DistanceRollClass {
private:
    static const int roll_history_length = 4;
    unsigned saved_distances[roll_history_length] = { };
    int next_i = 0;
public:
    int get_next_i();
    void save_new(unsigned new_distance);
    DistanceRollClass();
    unsigned get_average();
    void print_history();
    void empty_saved_distance();
};


#endif //ARCH_PROJECT_DISTANCEROLLCLASS_H
