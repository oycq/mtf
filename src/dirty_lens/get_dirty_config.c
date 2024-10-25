#include "ini.h"
#include "get_dirty_config.h"
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
        } else if (strcmp(name, "bottom_ratio") == 0) {
            pconfig->bottom_ratio = atof(value);
        } else if (strcmp(name, "bottom_throat") == 0) {
            pconfig->bottom_throat = atof(value);
        } else if (strcmp(name, "top_ratio") == 0) {
            pconfig->top_ratio = atof(value);
        } else if (strcmp(name, "top_throat") == 0) {
            pconfig->top_throat = atof(value);
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
    printf("bottom_ratio: %.2f\n", cfg.bottom_ratio);
    printf("bottom_throat: %.2f\n", cfg.bottom_throat);
    printf("top_ratio: %.2f\n", cfg.top_ratio);
    printf("top_throat: %.2f\n", cfg.top_throat);

    return cfg;
}
