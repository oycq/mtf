#ifndef GET_CONFIG_H
#define GET_CONFIG_H

#define MAX_STR_LEN 128

typedef struct {
    int w;
    int h;
    char input_img_path[MAX_STR_LEN];
    char output_img_path[MAX_STR_LEN];
    float ratio;
    float throat;
} config_t;


config_t get_config(char *config_name);

#endif // GET_CONFIG_H
