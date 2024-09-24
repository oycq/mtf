#include "ini.h"
#include "get_config.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

static int handler(void* user, const char* section, const char* name, const char* value) {
    config_t* pconfig = (config_t*)user;

    if (strcmp(section, "settings") == 0) {
        if (strcmp(name, "w") == 0) {
            pconfig->w = atoi(value);
        } else if (strcmp(name, "h") == 0) {
            pconfig->h = atoi(value);
        } else if (strcmp(name, "input_img_path") == 0) {
            strncpy(pconfig->input_img_path, value, MAX_STR_LEN - 1);
            pconfig->input_img_path[MAX_STR_LEN - 1] = '\0'; 
        } else if (strcmp(name, "output_img_path") == 0) {
            strncpy(pconfig->output_img_path, value, MAX_STR_LEN - 1);
            pconfig->output_img_path[MAX_STR_LEN - 1] = '\0'; 
        } else if (strcmp(name, "ratio") == 0) {
            pconfig->ratio = atof(value);
        } else if (strcmp(name, "throat") == 0) {
            pconfig->throat = atof(value);
        }
    }
    return 1;
}

config_t get_config(char *config_name) {
    config_t cfg;
    if (ini_parse(config_name, handler, &cfg) < 0) {
        printf("Can't load %s\n", config_name);
        exit(EXIT_FAILURE);
    }
    printf("w: %d\n", cfg.w);
    printf("h: %d\n", cfg.h);
    printf("input_img_path: %s\n", cfg.input_img_path);
    printf("output_img_path: %s\n", cfg.output_img_path);
    printf("ratio: %.2f\n", cfg.ratio);
    printf("throat: %.2f\n", cfg.throat);

    return cfg;
}
