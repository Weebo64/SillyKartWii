#include <MarioKartWii/UI/Section/SectionMgr.hpp>
#include <MarioKartWii/UI/Page/Menu/CupSelect.hpp>
#include <MarioKartWii/UI/Page/Menu/CourseSelect.hpp>
#include <MarioKartWii/UI/Page/Other/GhostSelect.hpp>
#include <MarioKartWii/UI/Page/Other/Votes.hpp>
#include <MarioKartWii/GlobalFunctions.hpp>
#include <MarioKartWii/Race/RaceInfo/RaceInfo.hpp>
#include <SlotExpansion/CupsConfig.hpp>
#include <SlotExpansion/UI/ExpCupSelect.hpp>
#include <SlotExpansion/UI/ExpansionUIMisc.hpp>


namespace Pulsar {
namespace UI {
//Change brctr names
kmWrite24(0x808a85ef, 'PUL'); //used by 807e5754

static void LoadCtrlMenuCourseSelectCupBRCTR(ControlLoader& loader, const char* folderName, const char* ctrName,
    const char* variant, const char** animNames) {
    loader.Load(UI::buttonFolder, "PULrseSelectCup", variant, animNames); //Move to button to avoid duplication of cup icon tpls
}
kmCall(0x807e4538, LoadCtrlMenuCourseSelectCupBRCTR);

static void LoadCorrectTrackListBox(ControlLoader& loader, const char* folder, const char* normalBrctr, const char* variant, const char** anims) {
    loader.Load(folder, "PULSelectNULL", variant, anims);
}
kmCall(0x807e5f24, LoadCorrectTrackListBox);

//BMG
int GetTrackBMGId(PulsarId pulsarId, bool useCommonName) {
    u32 bmgId;
    u32 realId = CupsConfig::ConvertTrack_PulsarIdToRealId(pulsarId);
    if (CupsConfig::IsReg(pulsarId)) bmgId = realId > 32 ? BMG_BATTLE : BMG_REGS;
    else {
        bmgId = BMG_TRACKS;
        const CupsConfig* cupsConfig = CupsConfig::sInstance;
        u8 variantIdx;
        if (useCommonName) {
            if (cupsConfig->GetTrack(pulsarId).variantCount > 0) variantIdx = 8;
            else variantIdx = 0;
        }
        else variantIdx = cupsConfig->GetCurVariantIdx();
        realId += variantIdx << 12;
    }
    return bmgId + realId;
}

int GetTrackBMGByRowIdx(u32 cupTrackIdx) {
    const Pages::CupSelect* cup = SectionMgr::sInstance->curSection->Get<Pages::CupSelect>();
    PulsarCupId curCupId;
    if (cup == nullptr) curCupId = PULSARCUPID_FIRSTREG;
    else curCupId = static_cast<PulsarCupId>(cup->ctrlMenuCupSelectCup.curCupID);
    return GetTrackBMGId(CupsConfig::sInstance->ConvertTrack_PulsarCupToTrack(curCupId, cupTrackIdx), true);
}
kmWrite32(0x807e6184, 0x7FA3EB78);
kmCall(0x807e6188, &GetTrackBMGByRowIdx);
kmWrite32(0x807e6088, 0x7F63DB78);
kmCall(0x807e608c, GetTrackBMGByRowIdx);

int GetCurTrackBMG() {
    return GetTrackBMGId(CupsConfig::sInstance->GetWinning(), false);
}

// Helper functions for track name and author formatting
static const wchar_t FONT_SIZE_ESCAPE_SMALL[] = {0x001A, 0x0800, 0x0000, 0x0050, 0x0000};

static void RemoveAllEscapeSequences(wchar_t* dest, const wchar_t* src) {
    while (*src != L'\0') {
        if (src[0] == 0x001A) {
            const u8* escapeBytes = reinterpret_cast<const u8*>(src);
            u8 escapeLength = escapeBytes[2];
            src = reinterpret_cast<const wchar_t*>(escapeBytes + escapeLength);
        } else {
            *dest++ = *src++;
        }
    }
    *dest = L'\0';
}

static const wchar_t* FindFirstColorEscape(const wchar_t* src) {
    const u8* srcBytes = reinterpret_cast<const u8*>(src);
    while (*reinterpret_cast<const wchar_t*>(srcBytes) != L'\0') {
        const wchar_t cur = *reinterpret_cast<const wchar_t*>(srcBytes);
        if (cur == 0x001A) {
            const u8 escapeLen = srcBytes[2];
            if ((escapeLen == 0) || (escapeLen & 1)) break;

            const wchar_t* escapeWchars = reinterpret_cast<const wchar_t*>(srcBytes);
            const u8 escapeType = srcBytes[3];
            if (escapeLen >= 8 && escapeType == 0 && escapeWchars[2] == 0x0001) return escapeWchars;

            srcBytes += escapeLen;
        } else {
            srcBytes += sizeof(wchar_t);
        }
    }
    return nullptr;
}

static u32 AppendTrackTextPreservingColor(wchar_t* dest, const wchar_t* src, u32 outLen, u32 maxLen) {
    if (src == nullptr || maxLen == 0 || outLen >= maxLen - 1) return outLen;

    u8* destBytes = reinterpret_cast<u8*>(dest);
    const u8* srcBytes = reinterpret_cast<const u8*>(src);
    u32 outBytes = outLen * sizeof(wchar_t);
    const u32 maxDataBytes = (maxLen - 1) * sizeof(wchar_t);

    while (outBytes < maxDataBytes) {
        const wchar_t cur = *reinterpret_cast<const wchar_t*>(srcBytes);
        if (cur == L'\0') break;

        if (cur == 0x001A) {
            const u8 escapeLen = srcBytes[2];
            if ((escapeLen == 0) || (escapeLen & 1)) break;

            bool keepEscape = false;
            if (escapeLen >= 8) {
                const wchar_t* escapeWchars = reinterpret_cast<const wchar_t*>(srcBytes);
                const u8 escapeType = srcBytes[3];
                keepEscape = (escapeType == 0) && (escapeWchars[2] == 0x0001);
            }

            if (keepEscape) {
                if (outBytes + escapeLen > maxDataBytes) break;
                for (u32 i = 0; i < static_cast<u32>(escapeLen); ++i) {
                    destBytes[outBytes + i] = srcBytes[i];
                }
                outBytes += escapeLen;
            }
            srcBytes += escapeLen;
            continue;
        }

        if (outBytes + sizeof(wchar_t) > maxDataBytes) break;
        *reinterpret_cast<wchar_t*>(destBytes + outBytes) = cur;
        outBytes += sizeof(wchar_t);
        srcBytes += sizeof(wchar_t);
    }

    dest[outBytes / sizeof(wchar_t)] = L'\0';
    return outBytes / sizeof(wchar_t);
}

static void BuildTrackNameAndAuthor(wchar_t* dest, const wchar_t* trackName, const wchar_t* authorName, u32 maxLen) {
    if (maxLen == 0) return;

    wchar_t cleanAuthor[0x100];
    RemoveAllEscapeSequences(cleanAuthor, authorName);

    u32 out = AppendTrackTextPreservingColor(dest, trackName, 0, maxLen);

    if (out < maxLen - 1) {
        dest[out++] = L'\n';
    }

    const u32 prefixLen = 4;
    for (u32 i = 0; i < prefixLen && out < maxLen - 1; ++i) {
        dest[out++] = FONT_SIZE_ESCAPE_SMALL[i];
    }

    const wchar_t* curAuthor = cleanAuthor;
    while (*curAuthor != L'\0' && out < maxLen - 1) {
        dest[out++] = *curAuthor++;
    }

    dest[out] = L'\0';
}

u32 GetTrackAuthorBMGId(PulsarId trackId, u32 trackBmgId) {
    if (CupsConfig::IsReg(trackId) || trackBmgId < BMG_TRACKS) return BMG_NINTENDO;
    
    const CupsConfig* cupsConfig = CupsConfig::sInstance;
    const bool hasVariants = cupsConfig->GetTrack(trackId).variantCount > 0;
    const u8 curVariant = cupsConfig->GetCurVariantIdx();
    const u32 realId = CupsConfig::ConvertTrack_PulsarIdToRealId(trackId);

    // For variants, use the base track's author
    if (hasVariants && curVariant > 0) {
        return BMG_AUTHORS + realId;
    }
    
    return BMG_AUTHORS + realId;
}

bool SetTrackNameAuthorMessage(LayoutUIControl& control, PulsarId trackId, u32 trackBmgId) {
    if (CupsConfig::IsReg(trackId)) return false;

    const CupsConfig* cupsConfig = CupsConfig::sInstance;
    const u32 trackNameBmgId = GetTrackBMGId(trackId, true);
    const u32 authorId = GetTrackAuthorBMGId(trackId, trackBmgId);
    const wchar_t* trackText = GetCustomMsg(trackNameBmgId);
    const wchar_t* authorText = GetCustomMsg(authorId);

    if (trackText == nullptr || authorText == nullptr) return false;

    static wchar_t s_trackAuthorBuffer[0x200];
    BuildTrackNameAndAuthor(s_trackAuthorBuffer, trackText, authorText, 0x200);
    Text::Info customInfo;
    customInfo.strings[0] = s_trackAuthorBuffer;
    control.SetMessage(BMG_TEXT, &customInfo);
    return true;
}

static void SetVSIntroBmgId(LayoutUIControl* trackName) {
    u32 bmgId = GetCurTrackBMG();
    const PulsarId winning = CupsConfig::sInstance->GetWinning();
    
    if (!CupsConfig::IsReg(winning)) {
        if (SetTrackNameAuthorMessage(*trackName, winning, bmgId)) return;
    }
    
    Text::Info info;
    info.bmgToPass[0] = bmgId;
    u32 authorId;
    if (bmgId < BMG_TRACKS) authorId = BMG_NINTENDO;
    else authorId = bmgId + BMG_AUTHORS - BMG_TRACKS;
    info.bmgToPass[1] = authorId;
    trackName->SetMessage(BMG_INFO_DISPLAY, &info);
}
kmCall(0x808552cc, SetVSIntroBmgId);

static void SetAwardsResultCupInfo(LayoutUIControl& awardType, const char* textBoxName, u32 bmgId, Text::Info& info) {
    PulsarCupId id = CupsConfig::sInstance->lastSelectedCup;
    if (!CupsConfig::IsRegCup(id)) {
        awardType.layout.GetPaneByName("cup_icon")->flag &= ~1;
        u32 realCupId = CupsConfig::ConvertCup_PulsarIdToRealId(id);
        u32 cupBmgId;
        u16 iconCount = System::sInstance->GetInfo().GetCupIconCount();
        if (realCupId > iconCount - 1) {
            wchar_t cupName[0x20];
            swprintf(cupName, 0x20, L"Cup %d", realCupId);
            info.strings[0] = cupName;
            cupBmgId = BMG_TEXT;
        }
        else cupBmgId = BMG_CUPS + realCupId;
        info.bmgToPass[1] = cupBmgId;
    }
    awardType.SetTextBoxMessage(textBoxName, bmgId, &info);
}
kmCall(0x805bcb88, SetAwardsResultCupInfo);

static void SetGPIntroInfo(LayoutUIControl& titleText, u32 bmgId, Text::Info& info) {

    PulsarCupId id = CupsConfig::sInstance->lastSelectedCup;
    if (!CupsConfig::IsRegCup(id)) {
        titleText.layout.GetPaneByName("cup_icon")->flag &= ~1;
        u32 realCupId = CupsConfig::ConvertCup_PulsarIdToRealId(id);
        u32 cupBmgId;
        u16 iconCount = System::sInstance->GetInfo().GetCupIconCount();
        if (realCupId > iconCount - 1) {
            wchar_t cupName[0x20];
            swprintf(cupName, 0x20, L"Cup %d", realCupId);
            info.strings[0] = cupName;
            cupBmgId = BMG_TEXT;
        }
        else cupBmgId = BMG_CUPS + realCupId;
        info.bmgToPass[1] = cupBmgId;

    }
    titleText.SetMessage(bmgId, &info);
}
kmCall(0x808553b4, SetGPIntroInfo);

static void SetGPBottomText(CtrlMenuInstructionText& bottomText, u32 bmgId, Text::Info& info) {
    register ExpCupSelect* cupPage;
    asm(mr cupPage, r31;);
    PulsarCupId id = static_cast<PulsarCupId>(cupPage->ctrlMenuCupSelectCup.curCupID);

    if (!CupsConfig::IsRegCup(id)) {
        u32 realCupId = CupsConfig::ConvertCup_PulsarIdToRealId(id);
        register u32 cc;
        asm(mr cc, r28;);
        u8 status = Settings::Mgr::GetGPStatus(realCupId, cc);
        u32 trophyBmg;
        u32 rankBmg;
        if (status == 0xFF) {
            trophyBmg = BMG_GP_BLANK;
            rankBmg= BMG_GP_BLANK;
        }
        else {
            trophyBmg = BMG_GP_GOLD_TROPHY + Settings::Mgr::ComputeTrophyFromStatus(status);
            rankBmg = BMG_GP_RANK_3STARS + Settings::Mgr::ComputeRankFromStatus(status);
        }
        info.bmgToPass[1] = trophyBmg;
        info.bmgToPass[2] = rankBmg;
    }
    bottomText.SetMessage(bmgId, &info);
}
kmCall(0x80841720, SetGPBottomText);

static void SetGhostInfoTrackBMG(GhostInfoControl* control, const char* textBoxName) {
    control->SetTextBoxMessage(textBoxName, GetCurTrackBMG());
}
kmCall(0x805e2a4c, SetGhostInfoTrackBMG);

kmWrite32(0x808406e8, 0x388000ff); //store 0xFF on timeout instead of -1
kmWrite32(0x808415ac, 0x388000ff);
kmWrite32(0x80643004, 0x3be000ff);
kmWrite32(0x808394e8, 0x388000ff);
kmWrite32(0x80644104, 0x3b5b0000);
static void CourseVoteBMG(VoteControl* vote, bool isCourseIdInvalid, PulsarId courseVote, MiiGroup& miiGroup, u32 playerId, bool isLocalPlayer, u32 team) {
    u32 bmgId = courseVote;
    if (bmgId != 0x1101 && bmgId < 0x2498) bmgId = GetTrackBMGId(courseVote, true);
    vote->Fill(isCourseIdInvalid, bmgId, miiGroup, playerId, isLocalPlayer, team);
}
kmCall(0x806441b8, CourseVoteBMG);

static bool BattleArenaBMGFix(SectionId sectionId) {
    register PulsarId id;
    asm(mr id, r28;);
    CupsConfig::sInstance->SetWinning(id, 0);
    return IsOnlineSection(sectionId);
}
kmCall(0x8083d02c, BattleArenaBMGFix);


//kmWrite32(0x80644340, 0x7F64DB78);
static void WinningTrackBMG(PulsarId winningCourse) {
    register Pages::Vote* vote;
    asm(mr vote, r27;);
    vote->trackBmgId = GetTrackBMGId(winningCourse, true);
}
kmCall(0x80644344, WinningTrackBMG);

//Rewrote InitSelf to start with correct TPLs
static void ExtCupSelectCupInitSelf(CtrlMenuCupSelectCup* cups) {
    const CupsConfig* cupsConfig = CupsConfig::sInstance;
    PulsarCupId selCup = cupsConfig->lastSelectedCup;
    cups->curCupID = selCup;
    PushButton** buttons = reinterpret_cast<PushButton**>(cups->childrenGroup.controlArray);

    for (int i = 0; i < 8; ++i) {
        const PulsarCupId id = cupsConfig->GetNextCupId(selCup, i - cupsConfig->lastSelectedCupButtonIdx);
        buttons[i]->buttonId = id;
        ExpCupSelect::UpdateCupData(id, *buttons[i]);
        buttons[i]->SetOnClickHandler(cups->onCupButtonClickHandler, 0);
        buttons[i]->SetOnSelectHandler(cups->onCupButtonSelectHandler);
        buttons[i]->SetPlayerBitfield(SectionMgr::sInstance->curSection->Get<Pages::CupSelect>()->GetPlayerBitfield());
    }
    buttons[cupsConfig->lastSelectedCupButtonIdx]->SelectInitial(0);
};
kmWritePointer(0x808d324c, ExtCupSelectCupInitSelf); //807e5894

static void ExtCourseSelectCupInitSelf(CtrlMenuCourseSelectCup* courseCups) {
    const CupsConfig* cupsConfig = CupsConfig::sInstance;
    for (int i = 0; i < 8; ++i) {
        CtrlMenuCourseSelectCupSub& cur = courseCups->cupIcons[i];
        const PulsarCupId id = cupsConfig->GetNextCupId(cupsConfig->lastSelectedCup, i - cupsConfig->lastSelectedCupButtonIdx);
        ExpCupSelect::UpdateCupData(id, cur);
        cur.animator.GetAnimationGroupById(0).PlayAnimationAtFrame(0, 0.0f);
        const bool clicked = cupsConfig->lastSelectedCupButtonIdx == i ? true : false;
        cur.animator.GetAnimationGroupById(1).PlayAnimationAtFrame(!clicked, 0.0f);
        cur.animator.GetAnimationGroupById(2).PlayAnimationAtFrame(!clicked, 0.0f);
        cur.animator.GetAnimationGroupById(3).PlayAnimationAtFrame(clicked, 0.0f);
        cur.selected = clicked;
        cur.SetRelativePosition(courseCups->positionAndscale[1]);
    }
    const Section* curSection = SectionMgr::sInstance->curSection;
    Pages::CupSelect* cup = curSection->Get<Pages::CupSelect>();
    NoteModelControl* positionArray = cup->modelPosition;

    switch (cup->extraControlNumber) {
    case(2):
        positionArray[0].positionAndscale[1].position.x = -52.0f;
        positionArray[0].positionAndscale[1].position.y = -8.0f;
        positionArray[0].positionAndscale[1].scale.x = 0.875f;
        positionArray[0].positionAndscale[1].scale.z = 0.875f;
        positionArray[1].positionAndscale[1].position.x = -52.0f;
        positionArray[1].positionAndscale[1].position.y = -13.0f;
        positionArray[1].positionAndscale[1].scale.x = 0.875f;
        positionArray[1].positionAndscale[1].scale.z = 0.875f;
        break;
    case(1):
        positionArray[0].positionAndscale[1].position.x = -32.0f;
        positionArray[0].positionAndscale[1].position.y = -32.0f;
        positionArray = curSection->Get<Pages::CourseSelect>()->modelPosition;
        positionArray[0].positionAndscale[1].position.x = -32.0f;
        positionArray[0].positionAndscale[1].position.y = -32.0f;
        break;
    case(4):
        positionArray[3].positionAndscale[1].position.x = 64.0f;
        positionArray[3].positionAndscale[1].position.y = -55.25f;
        positionArray[3].positionAndscale[1].scale.x = 0.6875f;
        positionArray[3].positionAndscale[1].scale.z = 0.6875f;
    case(3):
        positionArray[0].positionAndscale[1].position.x = 64.0f;
        positionArray[0].positionAndscale[1].position.y = -64.0f;
        positionArray[0].positionAndscale[1].scale.x = 0.6875f;
        positionArray[0].positionAndscale[1].scale.z = 0.6875f;
        positionArray[1].positionAndscale[1].position.x = 64.0f;
        positionArray[1].positionAndscale[1].position.y = -64.0f;
        positionArray[1].positionAndscale[1].scale.x = 0.6875f;
        positionArray[1].positionAndscale[1].scale.z = 0.6875f;
        positionArray[2].positionAndscale[1].position.x = 64.0f;
        positionArray[2].positionAndscale[1].position.y = -55.25f;
        positionArray[2].positionAndscale[1].scale.x = 0.6875f;
        positionArray[2].positionAndscale[1].scale.z = 0.6875f;
        break;
    }
};
kmWritePointer(0x808d3190, ExtCourseSelectCupInitSelf); //807e45c0

static void ExtCourseSelectCourseInitSelf(CtrlMenuCourseSelectCourse* course) {
    const CupsConfig* cupsConfig = CupsConfig::sInstance;
    const Section* curSection = SectionMgr::sInstance->curSection;
    const Pages::CupSelect* cupPage = curSection->Get<Pages::CupSelect>();
    Pages::CourseSelect* coursePage = curSection->Get<Pages::CourseSelect>();
    //channel ldb stuff ignored
    const u32 cupId = cupPage->clickedCupId;

    PushButton* toSelect = &course->courseButtons[0];
    for (int i = 0; i < 4; ++i) {
        PushButton& curButton = course->courseButtons[i];
        curButton.buttonId = i;
        const u32 bmgId = GetTrackBMGByRowIdx(i);
        curButton.SetMessage(bmgId);
        if (cupsConfig->ConvertTrack_PulsarCupToTrack(cupsConfig->lastSelectedCup, i) == cupsConfig->GetSelected()) {
            toSelect = &curButton;
        }
    };
    coursePage->SelectButton(*toSelect);
};
kmWritePointer(0x808d30d8, ExtCourseSelectCourseInitSelf); //807e5118

//Multiplayer Fix
kmWrite32(0x807e56d4, 0x60000000);
kmWrite32(0x807e5f04, 0x60000000);

//TPL
//CupSelectCup patch, disable picture panes
kmWrite32(0x807e57a4, 0x60000000);
kmWrite32(0x807e57bc, 0x60000000);
kmWrite32(0x807e57d4, 0x60000000);

//CourseSelectCup patch, disable picture panes
kmWrite32(0x807e4550, 0x60000000);
kmWrite32(0x807e4568, 0x60000000);
kmWrite32(0x807e4580, 0x60000000);
}//namespace UI
}//namespace Pulsar