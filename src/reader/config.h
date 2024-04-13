#ifndef CONFIG_H_
#define CONFIG_H_

#define TARGET_FPS 20

#define IDLE_SAVE_TIME_SEC 60

#ifndef USER_FONTS
#define FONT_DIR            "resources/fonts"
#define EXTRA_FONT_DIR      ""
#define CUSTOM_FONT_DIR     ""
#else
#define FONT_DIR            "/usr/share/fonts/dejavu"
#define EXTRA_FONT_DIR      "/usr/share/fonts/liberation"
#define CUSTOM_FONT_DIR     "fonts"
#endif
#define DEFAULT_FONT_NAME   FONT_DIR "/DejaVuSans.ttf"
#define SYSTEM_FONT         FONT_DIR "/DejaVuSansMono.ttf"

#ifdef MIYOO
#define MIN_FONT_SIZE      6
#define MAX_FONT_SIZE      22
#define DEFAULT_FONT_SIZE  14
#define FONT_SIZE_STEP     2

#define DEFAULT_COLOR_THEME "light_contrast"
#else
#define MIN_FONT_SIZE      18
#define MAX_FONT_SIZE      32
#define DEFAULT_FONT_SIZE  26
#define FONT_SIZE_STEP     2

#define DEFAULT_COLOR_THEME "night_contrast"
#endif

#define CONFIG_FILE_PATH "reader.cfg"
#define FALLBACK_STORE_PATH ".pixel_reader_store"

#if PLATFORM_MIYOO_MINI
    #define DEFAULT_BROWSE_PATH "/mnt/SDCARD/Media/Books/"
    #define EXTRA_FONTS_LIST    {"/customer/app/wqy-microhei.ttc"}
#elif defined(MIYOO)
    #define DEFAULT_BROWSE_PATH "/mnt/books/"
    #define EXTRA_FONTS_LIST    {"/mnt/gmenu2x/skins/Default/font.ttf"}
#else
    #define DEFAULT_BROWSE_PATH std::filesystem::current_path() / ""
    #define EXTRA_FONTS_LIST    {}
#endif

#define DEFAULT_SHOW_PROGRESS true
#define DEFAULT_SHOULDER_KEYMAP "LR"

#define DEFAULT_PROGRESS_REPORTING ProgressReporting::GLOBAL_PERCENT

#endif
