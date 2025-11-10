#pragma once

#include <kamek.h>


/* 802e1acc */ int sprintf(char*, const char*, ...);


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

            /* 802af000 */ void SetString(const wchar_t*, u16);
        };

        class Picture : public Pane {
        };
    }
}

namespace d2d {
    class Multi_c {
    public:
        /* 80007320 */ nw4r::lyt::TextBox* findTextBoxByName(const char*);
        /* 800073d0 */ nw4r::lyt::Picture* findPictureByName(const char*);
    };
}


class dScene_c {
public:
    /* 800e2050 */ static void setFadeOutFrame(u16 duration);
};


class dScStage_c {
public:
    /* 0x0000 */ u8 pad[0x120c];

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

    /* 8042a4a8 */ static dScStage_c *m_instance;
    /* 8042a4fd */ static bool m_isCourseOut;
    /* 803744b0 */ static u32 mCollectionCoin[3];

    /* 809253e0 */ void restoreOldPlayerInfo();
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

    /* 8042a25c */ static dInfo_c *m_instance;
    /* 80315e90 */ static StartGameInfo_s m_startGameInfo;

    /* 800bb7d0 */ void startGame(const dInfo_c::StartGameInfo_s&);
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

    /* 800b0db0 */ static bool setFader(fader_type_e type);
};


class dAudio {
public:
    /* 8006a6a0 */ static void hashname_a2bd17ff_6bcc38cc(s32);
};


class SndAudioMgr {
public:
    /* 8042a768 */ static SndAudioMgr *sInstance;

    /* 801954c0 */ int startSystemSe(unsigned int, unsigned long);
};


class LytBase_c : public d2d::Multi_c {
};


class dGameDisplay_c {
public:
    /* 0x00 */ u8 pad[0x70];
    /* 0x70 */ LytBase_c layout;

    /* 8042a608 */ static dGameDisplay_c *m_instance;
};


class dStageTimer_c {
public:
    /* 0x00 */ u8 pad[0x04];
    /* 0x04 */ u32 preciseTime;

    /* 8042a350 */ static dStageTimer_c *m_instance;
};
