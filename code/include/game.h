#pragma once

#include <kamek.h>


class dScStage_c {
public:
    u8 pad[0x120c];
    // Using a union so we can do some optimizations in certain places
    union {
        struct {
            /* 0x120c */ u8 curWorld;
            /* 0x120d */ u8 curLevel;
            /* 0x120e */ u8 curArea;
            /* 0x120f */ u8 curZone;
        };
        struct {
            /* 0x120c */ u16 curWorldAndLevel;
            /* 0x120e */ u16 curAreaAndZone;
        };
        /* 0x120c */ u32 curWorldLevelAreaAndZone;
    };

    static dScStage_c *m_instance;
    static bool m_isCourseOut;
    static u32 mCollectionCoin[3];

    void restoreOldPlayerInfo();
};


class dInfo_c {
public:
    typedef enum hint_movie_type_e {
        HINT_MOVIE_TYPE_SUPER_SKILLS = 0,
        HINT_MOVIE_TYPE_1UP,
        HINT_MOVIE_TYPE_STAR_COIN,
        HINT_MOVIE_TYPE_SECRET_EXIT,
    } hint_movie_type_e;

    typedef enum screen_type_e {
        SCREEN_TYPE_NORMAL = 0,
        SCREEN_TYPE_SUPER_GUIDE,
        SCREEN_TYPE_TITLE,
        SCREEN_TYPE_TITLE_REPLAY,
        SCREEN_TYPE_HINT_MOVIE,
    } screen_type_e;

    typedef struct StartGameInfo_s {
        /* 0x00 */ u32 replay_duration;
        /* 0x04 */ u8 hint_movie_type;  // hint_movie_type_e
        /* 0x05 */ u8 entrance;
        /* 0x06 */ u8 area;
        /* 0x07 */ bool is_replay;
        /* 0x08 */ u32 screen_type;  // screen_type_e
        /* 0x0c */ u8 world_1;
        /* 0x0d */ u8 level_1;
        /* 0x0e */ u8 world_2;
        /* 0x0f */ u8 level_2;
    } StartGameInfo_s;

    void startGame(const dInfo_c::StartGameInfo_s&);

    static dInfo_c *m_instance;
};


class dFader_c {
public:
    typedef enum fader_type_e {
        FADER_TYPE_FADE = 0,
        FADER_TYPE_CIRCLE_1,
        FADER_TYPE_BOWSER,
        FADER_TYPE_WAVY,
        FADER_TYPE_MARIO_HEAD,
        FADER_TYPE_CIRCLE_5,
    } fader_type_e;

    static bool setFader(fader_type_e type);
};


class dAudio {
public:
    static void hashname_a2bd17ff_6bcc38cc(s32);
};


class SndAudioMgr {
public:
    int startSystemSe(unsigned int, unsigned long);
    
    static SndAudioMgr *sInstance;
};

int sprintf(char*, const char*, ...);

namespace nw4r {
    namespace lyt {
        class Pane {
        public:
            /* 0x00 */ u8 pad_00[0xbb];
            /* 0xbb */ u8 flag;
            /* 0xbc */ u8 pad_bc[0x1c];

            bool IsVisible() {
                return flag & 1;
            }

            void SetVisible(bool value) {
                if (value)
                    flag |= 1;
                else
                    flag &= ~1;
            }
        };

        class TextBox : public Pane {
        public:
            /* 0x0d8 */ u8 pad[0x28];
            /* 0x100 */ u8 mTextPosition;

            void SetString(const wchar_t*, u16);
        };

        class Picture : public Pane {
        };
    }
}

namespace d2d {
    class Multi_c {
    public:
        nw4r::lyt::TextBox* findTextBoxByName(const char*);
        nw4r::lyt::Picture* findPictureByName(const char*);
    };
}

class LytBase_c : public d2d::Multi_c {
};

class dGameDisplay_c {
public:
    /* 0x00 */ u8 pad[0x70];
    /* 0x70 */ LytBase_c layout;

    static dGameDisplay_c *m_instance;
};

class dStageTimer_c {
public:
    /* 0x00 */ u8 pad[0x04];
    /* 0x04 */ u32 preciseTime;

    static dStageTimer_c *m_instance;
};
