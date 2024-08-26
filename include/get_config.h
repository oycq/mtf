#ifndef GET_CONFIG_H
#define GET_CONFIG_H

#define MAX_STR_LEN 128

typedef struct {
    int w;
    int h;
    int roi_w;
    int roi_h;
    int top_margin;
    int thresh;
    char input_img_path[MAX_STR_LEN];
    char output_img_path[MAX_STR_LEN];
    int field_a_size;
    float field_a_throat;
    float field_b_ratio;
    int field_b_size;
    float field_b_throat;
} config_t;

config_t get_config(void);

#endif // GET_CONFIG_H
