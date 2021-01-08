//
// Created by maba_kalox on 12/10/20.
//

#include "DistanceRollClass.h"
#include <iostream>

void DistanceRollClass::save_new(unsigned int new_distance) {
    saved_distances[get_next_i()] = new_distance;
}

DistanceRollClass::DistanceRollClass() = default;

unsigned DistanceRollClass::get_average() {
    unsigned summation = 0;
    int non_zero_counter = 0;
    for (unsigned int saved_distance : saved_distances) {
        if (saved_distance != 0) {
            summation += saved_distance;
            non_zero_counter++;
        }
    }
    return summation / non_zero_counter;
}

int DistanceRollClass::get_next_i() {
    if (next_i < roll_history_length) {
        return next_i++;
    } else {
        next_i = 1;
        return 0;
    }
}

void DistanceRollClass::print_history() {
    std::cout<< "---HISTORY_START---" << std::endl;
    for (unsigned a: saved_distances) {
        std::cout << a << ", ";
    }
    std::cout << "\n---HISTORY_END-" << next_i << "-" << std::endl;
}

void DistanceRollClass::empty_saved_distance() {
    std::fill(saved_distances, saved_distances+roll_history_length, 0);
}
