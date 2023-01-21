#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

#define INITIAL_CAPACITY 10 //Initial capacity for the Dynamic storage structure (List)
#define WHEEL_RADIUS 27.5
#define WHEEL_DIAM 55
#define PI 3.142857

/// @brief Structure used to store the data collected from the sonar sensor
struct Ray {
    int distance;
    time_t time;
    int angle;
};

void initRay(struct Ray* ray, int distance, int angle) {
    ray->distance = distance;
    ray->time = time(NULL); //TODO: check if this gives time is ms, otherwise fix it
    ray->angle = angle;
}

/// @brief Dynamic storage structure (resizeable)
struct List {
    struct Ray* data;
    int size;
    int capacity;
};

void init(struct List* list) {
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    list->data = malloc(sizeof(struct Ray) * INITIAL_CAPACITY);
}

void put(struct List* list, struct Ray* value) {
    if (list->size == list->capacity) {
        int new_capacity = list->capacity * 2;
        struct Ray* new_data = malloc(sizeof(struct Ray) * new_capacity);
        for (int i = 0; i < list->size; i++) {
            new_data[i] = list->data[i];
        }
        free(list->data);
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size] = *value;
    list->size++;
}

struct Ray* get(struct List* list, int index) {
    if (index >= list->size || index < 0) {
        return NULL;
    }
    return &list->data[index];
}

void remove_at(struct List* list, int index) {
    if (index < 0 || index >= list->size) {
        return;
    }
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
}

void clear(struct List* list) {
    free(list->data);
    list->size = 0;
    list->capacity = 0;
}

int length(struct List* list) {
    return list->size;
}

//INT List structure
struct IntList {
    int* data;
    int size;
    int capacity;
};

void int_list_init(struct IntList* list) {
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    list->data = malloc(sizeof(int) * INITIAL_CAPACITY);
}

void int_list_put(struct IntList* list, int value) {
    if (list->size == list->capacity) {
        int new_capacity = list->capacity * 2;
        int* new_data = malloc(sizeof(int) * new_capacity);
        for (int i = 0; i < list->size; i++) {
            new_data[i] = list->data[i];
        }
        free(list->data);
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size] = value;
    list->size++;
}

int int_list_remove_at(struct int_list* list, int index) {
    if (index >= list->size || index < 0) {
        printf("Error: Index out of bounds\n");
        return -1;
    }
    int removed = list->data[index];
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
    return removed;
}

int int_list_get(struct IntList* list, int index) {
    if (index >= list->size || index < 0) {
        printf("Error: Index out of bounds\n");
        return -1;
    }
    return list->data[index];
}

int int_list_length(struct int_List* list) {
    return list->size;
}

void int_list_clear(struct int_list* list) {
    free(list->data);
    int_list_init(list);
}

float cal_run_time(uint8_t sn, int distance, int speed) {
    int count_per_rot;
    get_tacho_count_per_rot(sn, &count_per_rot);
    return (float) (1000 * count_per_rot * abs(distance) )/(speed * PI * WHEEL_RADIUS) ;
}
