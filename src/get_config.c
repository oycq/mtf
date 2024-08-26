#include "ini.h"
#include "get_config.h"
#include "stdlib.h"
#include "stdio.h"

static int handler(void* user, const char* section, const char* name, const char* value) {
    config_t* pconfig = (config_t*)user;

    if (strcmp(section, "settings") == 0) {
        if (strcmp(name, "w") == 0) {
            pconfig->w = atoi(value);
        } else if (strcmp(name, "h") == 0) {
            pconfig->h = atoi(value);
        } else if (strcmp(name, "roi_w") == 0) {
            pconfig->roi_w = atoi(value);
        } else if (strcmp(name, "roi_h") == 0) {
            pconfig->roi_h = atoi(value);
        } else if (strcmp(name, "top_margin") == 0) {
            pconfig->top_margin = atoi(value);
        } else if (strcmp(name, "thresh") == 0) {
            pconfig->thresh = atoi(value);
        } else if (strcmp(name, "input_img_path") == 0) {
            strncpy(pconfig->input_img_path, value, MAX_STR_LEN - 1);
            pconfig->input_img_path[MAX_STR_LEN - 1] = '\0'; 
        } else if (strcmp(name, "output_img_path") == 0) {
            strncpy(pconfig->output_img_path, value, MAX_STR_LEN - 1);
            pconfig->output_img_path[MAX_STR_LEN - 1] = '\0'; 
        } else if (strcmp(name, "field_a_size") == 0) {
            pconfig->field_a_size = atoi(value);
        } else if (strcmp(name, "field_a_throat") == 0) {
            pconfig->field_a_throat = atof(value);
        } else if (strcmp(name, "field_b_ratio") == 0) {
            pconfig->field_b_ratio = atof(value);
        } else if (strcmp(name, "field_b_size") == 0) {
            pconfig->field_b_size = atoi(value);
        } else if (strcmp(name, "field_b_throat") == 0) {
            pconfig->field_b_throat = atof(value);
        }
    }
    return 1;
}

config_t get_config() {
    config_t cfg;
    if (ini_parse("config.ini", handler, &cfg) < 0) {
        printf("Can't load 'config.ini'\n");
        exit(EXIT_FAILURE);
    }

    printf("w: %d\n", cfg.w);
    printf("h: %d\n", cfg.h);
    printf("roi_w: %d\n", cfg.roi_w);
    printf("roi_h: %d\n", cfg.roi_h);
    printf("top_margin: %d\n", cfg.top_margin);
    printf("thresh: %d\n", cfg.thresh);
    printf("input_img_path: %s\n", cfg.input_img_path);
    printf("output_img_path: %s\n", cfg.output_img_path);
    printf("field_a_size: %d\n", cfg.field_a_size);
    printf("field_a_throat: %.2f\n", cfg.field_a_throat);
    printf("field_b_ratio: %.2f\n", cfg.field_b_ratio);
    printf("field_b_size: %d\n", cfg.field_b_size);
    printf("field_b_throat: %.2f\n", cfg.field_b_throat);

    return cfg;
}
