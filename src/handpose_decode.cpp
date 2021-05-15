// Modified for EPS-DIC 2D Project

#include "handpose_decode.h"
#include <math.h>
#include <iostream>

/**
* @brief Constructor
*/
HandposeDecode::HandposeDecode(){}

/**
* @brief Destructor
*/
//~HandposeDecode::~HandposeDecode(){}

/**
* @brief: validate keypoints
* @return: true: valid
*          false: invalid
*/
int HandposeDecode::validate(){
    if((max(x_arr) > x_max_threshold) || (min(x_arr) < x_min_threshold)){
        return 0;
    }
    else if(min_index(y_arr) != 8 && min_index(y_arr) != 12 && min_index(y_arr) != 16 && min_index(y_arr) != 20){
        return 0;
    }
    return 1;
}

/**
* @brief: calculate top of finger keypoint relative to bottom
* @return: value proportional to angle of fingers
*/
int HandposeDecode::get_fingers_angle_scale(){
    int finger_top_total = x_arr[8] + x_arr[12] + x_arr[16] + x_arr[20];
    int finger_bottom_total = x_arr[5] + x_arr[9] + x_arr[13] + x_arr[17];
    int angle_scale = finger_top_total - finger_bottom_total;
    return angle_scale;
}

/**
* @brief: thumb pointing inward or outward
* @return: true: inward
*          false: outward
*/
int HandposeDecode::thumb_status(){
    // get vector of thumb
    int vec_thumb_x = x_arr[4] - x_arr[3];
    int vec_thumb_y = y_arr[4] - y_arr[3];
    // get vector of index finger
    int vec_index_x = x_arr[8] - x_arr[7];
    int vec_index_y = y_arr[8] - y_arr[7];
    // length of thumb and index finger vector
    float len_thumb = sqrt(pow(vec_thumb_x,2)+pow(vec_thumb_y,2));
    float len_index = sqrt(pow(vec_index_x,2)+pow(vec_index_y,2));
    // thumb inward (direction = -1) or outward (direction = 1), using cross product of vectors
    int direction = (vec_thumb_x*vec_index_y-vec_index_x*vec_thumb_y)>0 ? -1:1;
    // get angle between thumb and index figure vector
    float thumb_index_angle =direction*acos((vec_thumb_x*vec_index_x+vec_thumb_y*vec_index_y)/len_thumb/len_index)*180.0/3.1415926;

    if(thumb_index_angle < thumb_threshold){
        return 1;
    }
    else {
        return 0;
    }
}
/**
* @brief: compute rc_command based on keypoint coordinates
* @return: 0: null (no conclusion)
*          1: one
*          2: two
*          3: three
*          4: four
*          5: five
*          6: bad (thumbs down)
*          7: good (thumbs up)
*/

unsigned char HandposeDecode::get_rc_command(){
    int thumb_bool = thumb_status();
    if (max_index(y_arr) == 4) // if the tip of the thumb is the highest point
    {
        return cmd_good;
    } else if (min_index(y_arr) == 4) // if the tip of the thumb is the lowest point
    {
        return cmd_bad;
    } else if (y_arr[8] < y_arr[7]) // if the index finger is down
    {
        return cmd_null;
    } else if (y_arr[12] < y_arr[11]) // if the middle finger is down
    {
        return cmd_one;
    } else if (y_arr[16] < y_arr[15]) // if the ring finger is down
    {
        return cmd_two;
    } else if (y_arr[20] < y_arr[19]) // if the pinky is down
    {
        return cmd_three;
    } else if (y_arr[4] < y_arr[3]) // if the thumb is down
    {
        return cmd_four;
    } else
    {
        return cmd_five;
    } 
}

/**
* @brief: find index of maximum value in array
*/
int HandposeDecode::max_index(int arr[]){
    int max_index = 0;
    for(int k=0; k<21; k++){
        if(arr[k] > arr[min_index]){
            min_index = k;
        }
    }
    return min_index;
}

/**
* @brief: find index of minimum value in array
*/
int HandposeDecode::min_index(int arr[]){
    int min_index = 0;
    for(int k=0; k<21; k++){
        if(arr[k] < arr[min_index]){
            min_index = k;
        }
    }
    return min_index;
}

/**
* @brief: find maximum value in array
*/
int HandposeDecode::max(int arr[]){
    int max = 0;
    for(int k=0; k<21; k++){
        if(arr[k] > max){
            max = arr[k];
        }
    }
    return max;
}

/**
* @brief: find minimum value in array
*/
int HandposeDecode::min(int arr[]){
    int min = 1280;
    for(int k=0; k<21; k++){
        if(arr[k] < min){
            min = arr[k];
        }
    }
    return min;
}

