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
} config_t;

config_t get_config(void);
