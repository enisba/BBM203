#include "CelestialObject.h"
#include <iostream>
#include <sstream>

CelestialObject* CelestialObject::rotate_right() const {
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<bool>> new_shape(cols, vector<bool>(rows));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            new_shape[j][rows - 1 - i] = shape[i][j];
        }
    }
    CelestialObject *rotated = new CelestialObject(new_shape, object_type, starting_row, time_of_appearance);
    return rotated;
}

// Constructor to initialize CelestialObject with all rotations calculated in a circular manner
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row, int time_of_appearance)
    : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
        
}

void CelestialObject::calculateRotation() {
    CelestialObject *current = this;
    do {
        CelestialObject *new_right = current->rotate_right();

        current->right_rotation = new_right;
        new_right->left_rotation = current;
        new_right->next_celestial_object = this->next_celestial_object;

        if (new_right->shape == this->shape) {
            current->right_rotation = this;
            this->left_rotation = current;
            delete new_right;
            break;
        }
        current = new_right;
    } while (true);
}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
    : shape(other->shape), object_type(other->object_type), starting_row(other->starting_row), time_of_appearance(other->time_of_appearance) {

}

// Function to delete rotations of a given celestial object
void CelestialObject::delete_rotations(CelestialObject *target) {
    if (target == nullptr) return;

    CelestialObject *curr = target->right_rotation;
    
    while (curr != nullptr && curr != target) {
        CelestialObject *next = curr->right_rotation;
        delete curr;
        curr = next;
    }
    if (target->right_rotation)
    {   
        target->right_rotation = nullptr;
    }
    if (target->left_rotation)
    {
        target->left_rotation = nullptr;
    }
}
