#include <time.h>

/// @brief Structure used to store the data collected from the sonar sensor
struct Ray {
    int distance;
    time_t time;
    int angle;
};

/// @brief Dynamic storage structure (resizeable)
struct List {
    struct Ray* data;
    int size;
    int capacity;
};

//INT List structure
struct IntList {
    int* data;
    int size;
    int capacity;
};

void initRay(struct Ray* ray, int distance, int angle);

void init(struct List* list);

void put(struct List* list, struct Ray* value);

struct Ray* get(struct List* list, int index);

void remove_at(struct List* list, int index);

void clear(struct List* list);

int length(struct List* list);

void int_list_init(struct IntList* list);

void int_list_put(struct IntList* list, int value);

int int_list_remove_at(struct IntList* list, int index);

int int_list_get(struct IntList* list, int index);

int int_list_length(struct IntList* list);

void int_list_clear(struct IntList* list);

/// @brief Update a ray with the sensor offset translation
void update_with_offset(struct Ray* ray);

float cal_run_time(uint8_t sn, int distance, int speed);
    
