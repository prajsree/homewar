#include <stdio.h> 
#include <dlfcn.h>
#include "demon_user_api.h"

#define NUM_GAME 10000

typedef uint8_t (*get_usr_opt_f_)(void);
typedef void (*set_usr_opt_f_) (uint8_t);
typedef void (*set_result_f_) (uint8_t);

typedef struct user_func_vect_t_ {
    get_usr_opt_f_ get_usr_opt;
    set_usr_opt_f_ set_usr_opt;
    set_result_f_  set_result;
} user_func_vect_t;

typedef struct demon_global_ctx_t_ {
    uint8_t game_count;
    uint8_t user1_win_count;
    uint8_t user2_win_count;
    user_func_vect_t vt[2];
} demon_global_ctx_t;

demon_global_ctx_t G_demon_global_ctx; 

void demon_init (void)
{
    void *user_dll_handle = NULL;

    user_func_vect_t *vt = G_demon_global_ctx.vt;

    user_dll_handle = dlopen("libuser1.so", RTLD_LAZY);
    G_demon_global_ctx.vt[0].get_usr_opt = 
        (get_usr_opt_f_)dlsym(user_dll_handle, "get_user_option");
    G_demon_global_ctx.vt[0].set_usr_opt =
        (set_usr_opt_f_)dlsym(user_dll_handle, "set_other_user_option");
    G_demon_global_ctx.vt[0].set_result =
        (set_result_f_)dlsym(user_dll_handle, "set_result");

    user_dll_handle = dlopen("libuser2.so", RTLD_LAZY);
    G_demon_global_ctx.vt[1].get_usr_opt = 
        (get_usr_opt_f_)dlsym(user_dll_handle, "get_user_option");
    G_demon_global_ctx.vt[1].set_usr_opt =
        (set_usr_opt_f_)dlsym(user_dll_handle, "set_other_user_option");
    G_demon_global_ctx.vt[1].set_result =
        (set_result_f_)dlsym(user_dll_handle, "set_result");

}

uint8_t get_usr_option (uint8_t user)
{
    uint8_t ret;

    ret = G_demon_global_ctx.vt[user].get_usr_opt();

    return (ret);
}

void set_usr_option (uint8_t user, uint8_t opt)
{
    uint8_t ret;

    G_demon_global_ctx.vt[user].set_usr_opt(opt);

    return;
}

void set_usr_result (uint8_t user, uint8_t res)
{
    uint8_t ret;

    G_demon_global_ctx.vt[user].set_result(res);

    return;
}

int8_t compare_opt (uint8_t u1_opt, uint8_t u2_opt)
{
    uint8_t cmp = u1_opt << 4 | u2_opt;


    if (u1_opt == u2_opt) {
        return -1;
    }

    switch (cmp) {
        case 0x01:
            return (1);
        case 0x02:
            return (0);
        case 0x10:
            return (0);
        case 0x12:
            return (1);
        case 0x20:
            return (1);
        case 0x21:
            return (0);
    }

    return (-1);

}

char * opt_2_str (uint8_t opt)
{
    switch (opt) {
        case 0:
            return ("rock");

        case 1:
            return ("paper");

        case 2:
            return ("scissor");
    }

    return ("unknown");
}

int main (void) 
{ 
    uint8_t user0_opt = 0;
    uint8_t user1_opt = 0;
    int8_t winner;
    uint32_t i;
    uint32_t user0_win = 0;
    uint32_t user1_win = 0;

    printf("--------------------------------------------------------------------------------\n");
    printf("welcome to rock paper scissor challenge\n");
    printf("--------------------------------------------------------------------------------\n");

    printf("user1_opt\tuser2_opt\twinner\n");

    demon_init();

    for (i = 0; i < NUM_GAME; i++) {

        printf("--------------------------------------------------------------------------------\n");
        user0_opt = get_usr_option(0);
        user1_opt = get_usr_option(1);

        winner = compare_opt(user0_opt,user1_opt);

        set_usr_option(0, user1_opt);
        set_usr_option(1, user0_opt);

        if (winner == 0) {
            printf("%s\t\t%s\t\tuser1\n", opt_2_str(user0_opt), opt_2_str(user1_opt));
            user0_win++;
            set_usr_result(0,1);
            set_usr_result(1,0);
        } else if (winner == 1) {
            printf("%s\t\t%s\t\tuser2\n", opt_2_str(user0_opt), opt_2_str(user1_opt));
            user1_win++;
            set_usr_result(0,0);
            set_usr_result(1,1);
        } else if (winner == -1) {
            printf("%s\t\t%s\t\tdraw\n", opt_2_str(user0_opt), opt_2_str(user1_opt));
            set_usr_result(0,-1);
            set_usr_result(1,-1);
        }

    }

    if (user0_win == user1_win) {
        printf("\nGAME DRAWN\n");
    } else if (user0_win > user1_win) {
        printf("\nMEENU WON\n");
    } else {
        printf("\nPRAJU WON\n");
    }

    printf("Meenu = %d, Praju = %d\n", user0_win, user1_win);

    return (0);
}

