#include "game.h"

#define DIGIT_TO_WCHAR(d) (0xFF10|(d))

#define LEVEL_ID_AIRSHIP 37
#define LEVEL_ID_GHOST_HOUSE 20
#define LEVEL_ID_TOWER 21
#define LEVEL_ID_CASTLE 23
#define LEVEL_ID_CANNON 35
#define LEVEL_ID_AMBUSH_1 32
#define LEVEL_ID_AMBUSH_2 33
#define LEVEL_ID_AMBUSH_3 34

#define CHAR_AIRSHIP L'2'
#define CHAR_GHOST_HOUSE L'0'
#define CHAR_TOWER L'/'
#define CHAR_CASTLE L'.'
#define CHAR_BOWSERS_CASTLE L'='
#define CHAR_CANNON L'1'
#define CHAR_AMBUSH L'>'

#define ATTEMPTS_STR_WIDTH 20


u16 last_world_and_level = 0;
u32 attempt = 0;


void increment_or_reset_attempt_counter() {
    u16 current_world_and_level = (dInfo_c::m_startGameInfo.level_2 * 0x10) + dInfo_c::m_startGameInfo.world_2;
    if (current_world_and_level != last_world_and_level) {
        // We entered a different level
        last_world_and_level = current_world_and_level;
        attempt = 1;
    } else {
        // We re-entered the same level
        attempt++;
    }
}

void set_level_name_text() {
    // "World X-Y" is 9 chars, plus 1 for null terminator
    wchar_t temp_str_wide[10] = {L'W', L'o', L'r', L'l', L'd', L' '};

    nw4r::lyt::TextBox *box_1 = dGameDisplay_c::m_instance->layout.findTextBoxByName("T_WorldID");
    nw4r::lyt::TextBox *box_2 = dGameDisplay_c::m_instance->layout.findTextBoxByName("T_LevelID");

    u8 level_id = dScStage_c::m_instance->curLevel;
    u8 world_id = dScStage_c::m_instance->curWorld;

    // Set box 1 to "World (world)-"...
    temp_str_wide[6] = DIGIT_TO_WCHAR(world_id + 1);
    temp_str_wide[7] = L'-';

    if (level_id > 9) {  // Special level number
        // Just set box 1's string like that, without the level number
        temp_str_wide[8] = L'\0';
        box_1->SetString(temp_str_wide, 0);

        // Prepare the level icon string for box 2
        if (level_id == LEVEL_ID_AIRSHIP) {
            temp_str_wide[0] = CHAR_AIRSHIP;
        } else if (level_id == LEVEL_ID_GHOST_HOUSE) {
            temp_str_wide[0] = CHAR_GHOST_HOUSE;
        } else if (level_id == LEVEL_ID_TOWER) {
            temp_str_wide[0] = CHAR_TOWER;
        } else if (level_id == LEVEL_ID_CASTLE) {
            temp_str_wide[0] = (world_id == 7) ? CHAR_BOWSERS_CASTLE : CHAR_CASTLE;
        } else if (level_id == LEVEL_ID_CANNON) {
            temp_str_wide[0] = CHAR_CANNON;
        } else if (level_id == LEVEL_ID_AMBUSH_1
            or level_id == LEVEL_ID_AMBUSH_2
            or level_id == LEVEL_ID_AMBUSH_3
        ) {
            temp_str_wide[0] = CHAR_AMBUSH;
        }

        temp_str_wide[1] = L'\0';

    } else {  // Regular level
        // Add the level number to box 1, and set it
        temp_str_wide[8] = DIGIT_TO_WCHAR(level_id + 1);
        temp_str_wide[9] = L'\0';
        box_1->SetString(temp_str_wide, 0);

        // Set box 2 string to empty
        temp_str_wide[0] = L'\0';
    }

    box_2->SetString(temp_str_wide, 0);
}

void set_attempts_text() {
    char temp_str[ATTEMPTS_STR_WIDTH];
    wchar_t temp_str_wide[ATTEMPTS_STR_WIDTH];

    nw4r::lyt::TextBox *box = dGameDisplay_c::m_instance->layout.findTextBoxByName("T_Attempts");

    sprintf(temp_str, "Attempt: %d", attempt);
    int i;
    for (i = 0; i < ATTEMPTS_STR_WIDTH && temp_str[i]; i++){
        temp_str_wide[i] = temp_str[i];
    }
    temp_str_wide[i] = 0;

    box->SetString(temp_str_wide, 0);

}

void hide_star_coins() {
    dGameDisplay_c::m_instance->layout.findPictureByName("P_Coin1")->SetVisible(false);
    dGameDisplay_c::m_instance->layout.findPictureByName("P_Coin2")->SetVisible(false);
    dGameDisplay_c::m_instance->layout.findPictureByName("P_Coin3")->SetVisible(false);
    dGameDisplay_c::m_instance->layout.findPictureByName("P_NoCoin1")->SetVisible(false);
    dGameDisplay_c::m_instance->layout.findPictureByName("P_NoCoin2")->SetVisible(false);
    dGameDisplay_c::m_instance->layout.findPictureByName("P_NoCoin3")->SetVisible(false);
}

void show_star_coins() {
    char *pic_1, *pic_2, *pic_3;

    if (dScStage_c::mCollectionCoin[0] == 4) {
        pic_1 = "P_NoCoin1";
    } else {
        pic_1 = "P_Coin1";
    }

    if (dScStage_c::mCollectionCoin[1] == 4) {
        pic_2 = "P_NoCoin2";
    } else {
        pic_2 = "P_Coin2";
    }

    if (dScStage_c::mCollectionCoin[2] == 4) {
        pic_3 = "P_NoCoin3";
    } else {
        pic_3 = "P_Coin3";
    }

    dGameDisplay_c::m_instance->layout.findPictureByName(pic_1)->SetVisible(true);
    dGameDisplay_c::m_instance->layout.findPictureByName(pic_2)->SetVisible(true);
    dGameDisplay_c::m_instance->layout.findPictureByName(pic_3)->SetVisible(true);
}

void hide_precise_time() {
    dGameDisplay_c::m_instance->layout.findTextBoxByName("T_PreciseTime")->SetVisible(false);
}

void show_precise_time() {
    nw4r::lyt::TextBox *box = dGameDisplay_c::m_instance->layout.findTextBoxByName("T_PreciseTime");
    if (box->IsVisible()) {
        return;
    }

    // Get original timer value
    int value = dStageTimer_c::m_instance->preciseTime;
    if (dScStage_c::m_instance->curLevel < 9) {
        // On normal levels, subtract 92 since it reads the IGT a frame before it actually stops
        value -= 92;
    }

    // Precise IGT -> string calculation copied from NSMBW Practice Mode
    // https://github.com/RoadrunnerWMC/NSMBW-Practice-Mode

    // Integer part is calculated identically to how the game normally does it
    int integerPart = (value + 0xfff) >> 12;

    // Fractional part is clamped to 00-99
    int fractionPart = (((value + 0xfff) * 100) >> 12) - (integerPart * 100);
    if (fractionPart < 0) fractionPart = 0;
    if (fractionPart > 99) fractionPart = 99;

    // Print the precise time this way so that it uses monospace
    wchar_t time[7] = {
        DIGIT_TO_WCHAR(integerPart / 100),
        DIGIT_TO_WCHAR((integerPart / 10) % 10),
        DIGIT_TO_WCHAR(integerPart % 10),
        L'.',
        DIGIT_TO_WCHAR(fractionPart / 10),
        DIGIT_TO_WCHAR(fractionPart % 10),
        L'\0'
    };

    // Write the string to the textbox
    box->SetVisible(true);
    box->SetString(time, 0);
}

void run_at_end_of_level() {
    show_precise_time();
    show_star_coins();
}


// Runs at the start of each level
kmBranch(0x800BB8C8, increment_or_reset_attempt_counter);

// Runs at the start of each room
kmBranchDefCpp(0x80158820, NULL, void, ) {
    set_level_name_text();
    set_attempts_text();
    hide_precise_time();
    hide_star_coins();
}

// Runs when a flagpole is grabbed
kmBranch(0x80051348, run_at_end_of_level);

// Runs when the timer starts counting down (for castle levels)
kmBranch(0x80159098, run_at_end_of_level);
