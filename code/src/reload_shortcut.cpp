#include "game.h"


#define BUTTON_MINUS 0x1000
#define SE_SYS_BACK 122


bool is_title_screen_stage() {
    if (dScStage_c::m_instance == NULL) {
        return false;
    }

    // 01-40.arc
    return dScStage_c::m_instance->curWorldAndLevel == ((0 << 8) | 39);
}


void trigger_stage_reload() {
    if (dScStage_c::m_instance == NULL) {
        return;
    }

    // To make the transition as quick as possible, request a
    // "fade"-type fade (i.e. full-screen fade-to-black) with a time of
    // 0 (instant).
    // Note that the fade-out duration value persists after the
    // fade-out, and can affect other scenes such as crsin (aka level
    // banner), so it should be reset back to its default value of 30
    // later (we do that in a different hook).
    dFader_c::setFader(dFader_c::FADER_TYPE_FADE);
    dScene_c::setFadeOutFrame(0);

    // Force a background music fade-out
    // (note: "0" = the currently playing music, "1" = no music, and
    // higher values = specific music IDs)
    dAudio::hashname_a2bd17ff_6bcc38cc(1);

    // The game normally uses this function to reset some game state
    // upon death, including the spawn position if loading from
    // checkpoint
    dScStage_c::m_instance->restoreOldPlayerInfo();

    // Trigger the actual stage reload
    u8 world = dScStage_c::m_instance->curWorld;
    u8 level = dScStage_c::m_instance->curLevel;
    dInfo_c::m_instance->startGame((dInfo_c::StartGameInfo_s) {
        /* replay_duration */ 0,
        /* hint_movie_type */ 0,
        /* entrance */ 0xff,  // (default entrance)
        /* area */ 0,
        /* is_replay */ false,
        /* screen_type */ dInfo_c::SCREEN_TYPE_NORMAL,
        /* world_1 */ world,
        /* level_1 */ level,
        /* world_2 */ world,
        /* level_2 */ level
    });
}


// Hook at the end of dScCrsin_c::create() to reset the fade-out length
// to its default/vanilla value of 30 frames
kmBranchDefCpp(0x8091f16c, NULL, u32, ) {
    dScene_c::setFadeOutFrame(30);
    return 1;
}


u32 dGameKeyCore_c_intercept_input(void *this_, u32 bitfield) {
    if ((bitfield & BUTTON_MINUS)          // minus pressed
        && dScStage_c::m_instance != NULL  // we're in a stage
        && !dScStage_c::m_isCourseOut      // the stage isn't already being exited
        && !is_title_screen_stage()        // not in the title screen stage
    ) {
        SndAudioMgr::sInstance->startSystemSe(SE_SYS_BACK, 1ul);
        trigger_stage_reload();
    }

    return bitfield;
}


kmBranchDefAsm(0x800b5e30, 0x800b5e34) {
    nofralloc         // no prologue/epilogue
    clrlwi r0, r0, 1  // original instruction

    // SAVE
    subi r1, r1, 0x30
    stw r3, 0x00(r1)
    stw r4, 0x04(r1)
    stw r5, 0x08(r1)
    stw r6, 0x0c(r1)
    stw r7, 0x10(r1)
    stw r8, 0x14(r1)
    stw r9, 0x18(r1)
    stw r10, 0x1c(r1)
    stw r11, 0x20(r1)
    stw r12, 0x24(r1)

    mr r4, r0
    bl dGameKeyCore_c_intercept_input
    mr r0, r3

    // RESTORE
    lwz r3, 0x00(r1)
    lwz r4, 0x04(r1)
    lwz r5, 0x08(r1)
    lwz r6, 0x0c(r1)
    lwz r7, 0x10(r1)
    lwz r8, 0x14(r1)
    lwz r9, 0x18(r1)
    lwz r10, 0x1c(r1)
    lwz r11, 0x20(r1)
    lwz r12, 0x24(r1)
    addi r1, r1, 0x30

    blr  // replaced with branch to exit point by Kamek
}
