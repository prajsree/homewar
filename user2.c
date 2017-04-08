#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "demon_user_api.h"

#define GAME_THRESH 250
uint32_t num_game;
uint32_t count_arr[3] = {0};
uint8_t  opt_arr[1000];
uint32_t opt_arr_index = 0;
uint32_t cur_pat_index = 0;
uint32_t pat_det_this_iteration = false;

bool pattern_detected = false;

uint32_t pat_arr[3] = {0};

bool is_pat_arr_zeroed ()
{
    return ((pat_arr[0] || pat_arr[1] || pat_arr[2]) == 0);
}

uint8_t get_least_used (void)
{
    int index;

    if (count_arr[0] > count_arr[1]) {
        index = 0;
    } else {
        index = 1;
    }

    if (count_arr[index] < count_arr[2]) {
        index = 2;
    }

    return (index);
}

uint8_t get_user_option (void)
{
    uint8_t least_used = 0;
    uint8_t next_opt_expected = 0;


    if (pattern_detected) {

        next_opt_expected = opt_arr[cur_pat_index];
        if (cur_pat_index == opt_arr_index - 1) {
            cur_pat_index = 0;
        } else {
            cur_pat_index++;
        }

        return ((next_opt_expected + 1) % 3);

    } else {

        if (num_game < GAME_THRESH) { 
            return (rand() % 3);
        } else {

            least_used = get_least_used();
            return (least_used);
        }
    }
}

void set_other_user_option (uint8_t opt)
{
    count_arr[opt]++;

    if (!pattern_detected) {

        opt_arr[opt_arr_index] = opt;
        if (opt_arr_index == 999) {
            opt_arr_index = 0;
        } else {
            opt_arr_index++;
        }

        pat_arr[opt] = pat_arr[opt] ^ 1;
        pattern_detected = is_pat_arr_zeroed();

        if (pattern_detected) {
            pat_det_this_iteration = true;;
        }
    }

    return;
}

void set_result (int8_t opt)
{
    if (!opt && !pat_det_this_iteration) {
        if (pattern_detected) {
            memset(opt_arr, 0, 1000);
            opt_arr_index = 0;
            cur_pat_index = 0;
        }
        pattern_detected = false;
    }

    pat_det_this_iteration = false;

    return;
}

int main (void)
{
    return 0;
}
