/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

// Ported from https://github.com/xkbcommon/libxkbcommon/blob/master/scripts/makekeys

import { readFileSync, writeFileSync } from "fs"
import path, { dirname } from "path"
import { fileURLToPath } from "url"
import { GetUnicodeCharacterFromXKeySym } from "./keysym_to_unicode.mjs"

const __dirname = dirname(fileURLToPath(import.meta.url))
const XKB_KEY = {}
const XKB_NAME = {}

// https://github.com/chromium/chromium/blob/main/ui/events/keycodes/dom/dom_key_data.inc
const DomKey = {
    DeadKeyFromCombiningCharacter() {
        return "Dead"
    },
    NONE: "NONE",
    BACKSPACE: "Backspace",
    TAB: "Tab",
    ENTER: "Enter",
    ESCAPE: "Escape",
    DEL: "Delete",
    UNIDENTIFIED: "Unidentified",
    ACCEL: "Accel",
    ALT: "Alt",
    ALT_GRAPH: "AltGraph",
    CAPS_LOCK: "CapsLock",
    CONTROL: "Control",
    FN: "Fn",
    FN_LOCK: "FnLock",
    HYPER: "Hyper",
    META: "Meta",
    NUM_LOCK: "NumLock",
    SCROLL_LOCK: "ScrollLock",
    SHIFT: "Shift",
    SUPER: "Super",
    SYMBOL: "Symbol",
    SYMBOL_LOCK: "SymbolLock",
    SHIFT_LEVEL5: "ShiftLevel5",
    ALT_GRAPH_LATCH: ".AltGraphLatch",
    ARROW_DOWN: "ArrowDown",
    ARROW_LEFT: "ArrowLeft",
    ARROW_RIGHT: "ArrowRight",
    ARROW_UP: "ArrowUp",
    END: "End",
    HOME: "Home",
    PAGE_DOWN: "PageDown",
    PAGE_UP: "PageUp",
    CLEAR: "Clear",
    COPY: "Copy",
    CR_SEL: "CrSel",
    CUT: "Cut",
    ERASE_EOF: "EraseEof",
    EX_SEL: "ExSel",
    INSERT: "Insert",
    PASTE: "Paste",
    REDO: "Redo",
    UNDO: "Undo",
    ACCEPT: "Accept",
    AGAIN: "Again",
    ATTN: "Attn",
    CANCEL: "Cancel",
    CONTEXT_MENU: "ContextMenu",
    EXECUTE: "Execute",
    FIND: "Find",
    HELP: "Help",
    PAUSE: "Pause",
    PLAY: "Play",
    PROPS: "Props",
    SELECT: "Select",
    ZOOM_IN: "ZoomIn",
    ZOOM_OUT: "ZoomOut",
    BRIGHTNESS_DOWN: "BrightnessDown",
    BRIGHTNESS_UP: "BrightnessUp",
    EJECT: "Eject",
    LOG_OFF: "LogOff",
    POWER: "Power",
    POWER_OFF: "PowerOff",
    PRINT_SCREEN: "PrintScreen",
    HIBERNATE: "Hibernate",
    STANDBY: "Standby",
    WAKE_UP: "WakeUp",
    ALL_CANDIDATES: "AllCandidates",
    ALPHANUMERIC: "Alphanumeric",
    CODE_INPUT: "CodeInput",
    COMPOSE: "Compose",
    CONVERT: "Convert",
    FINAL_MODE: "FinalMode",
    GROUP_FIRST: "GroupFirst",
    GROUP_LAST: "GroupLast",
    GROUP_NEXT: "GroupNext",
    GROUP_PREVIOUS: "GroupPrevious",
    MODE_CHANGE: "ModeChange",
    NEXT_CANDIDATE: "NextCandidate",
    NON_CONVERT: "NonConvert",
    PREVIOUS_CANDIDATE: "PreviousCandidate",
    PROCESS: "Process",
    SINGLE_CANDIDATE: "SingleCandidate",
    HANGUL_MODE: "HangulMode",
    HANJA_MODE: "HanjaMode",
    JUNJA_MODE: "JunjaMode",
    EISU: "Eisu",
    HANKAKU: "Hankaku",
    HIRAGANA: "Hiragana",
    HIRAGANA_KATAKANA: "HiraganaKatakana",
    KANA_MODE: "KanaMode",
    KANJI_MODE: "KanjiMode",
    KATAKANA: "Katakana",
    ROMAJI: "Romaji",
    ZENKAKU: "Zenkaku",
    ZENKAKU_HANKAKU: "ZenkakuHankaku",
    F1: "F1",
    F2: "F2",
    F3: "F3",
    F4: "F4",
    F5: "F5",
    F6: "F6",
    F7: "F7",
    F8: "F8",
    F9: "F9",
    F10: "F10",
    F11: "F11",
    F12: "F12",
    F13: "F13",
    F14: "F14",
    F15: "F15",
    F16: "F16",
    F17: "F17",
    F18: "F18",
    F19: "F19",
    F20: "F20",
    F21: "F21",
    F22: "F22",
    F23: "F23",
    F24: "F24",
    SOFT1: "Soft1",
    SOFT2: "Soft2",
    SOFT3: "Soft3",
    SOFT4: "Soft4",
    SOFT5: "Soft5",
    SOFT6: "Soft6",
    SOFT7: "Soft7",
    SOFT8: "Soft8",
    CHANNEL_DOWN: "ChannelDown",
    CHANNEL_UP: "ChannelUp",
    CLOSE: "Close",
    MAIL_FORWARD: "MailForward",
    MAIL_REPLY: "MailReply",
    MAIL_SEND: "MailSend",
    MEDIA_CLOSE: "MediaClose",
    MEDIA_FAST_FORWARD: "MediaFastForward",
    MEDIA_PAUSE: "MediaPause",
    MEDIA_PLAY: "MediaPlay",
    MEDIA_PLAY_PAUSE: "MediaPlayPause",
    MEDIA_RECORD: "MediaRecord",
    MEDIA_REWIND: "MediaRewind",
    MEDIA_STOP: "MediaStop",
    MEDIA_TRACK_NEXT: "MediaTrackNext",
    MEDIA_TRACK_PREVIOUS: "MediaTrackPrevious",
    NEW: "New",
    OPEN: "Open",
    PRINT: "Print",
    SAVE: "Save",
    SPELL_CHECK: "SpellCheck",
    KEY11: "Key11",
    KEY12: "Key12",
    AUDIO_BALANCE_LEFT: "AudioBalanceLeft",
    AUDIO_BALANCE_RIGHT: "AudioBalanceRight",
    AUDIO_BASS_BOOST_DOWN: "AudioBassBoostDown",
    AUDIO_BASS_BOOST_TOGGLE: "AudioBassBoostToggle",
    AUDIO_BASS_BOOST_UP: "AudioBassBoostUp",
    AUDIO_FADER_FRONT: "AudioFaderFront",
    AUDIO_FADER_REAR: "AudioFaderRear",
    AUDIO_SURROUND_MODE_NEXT: "AudioSurroundModeNext",
    AUDIO_TREBLE_DOWN: "AudioTrebleDown",
    AUDIO_TREBLE_UP: "AudioTrebleUp",
    AUDIO_VOLUME_DOWN: "AudioVolumeDown",
    AUDIO_VOLUME_UP: "AudioVolumeUp",
    AUDIO_VOLUME_MUTE: "AudioVolumeMute",
    MICROPHONE_TOGGLE: "MicrophoneToggle",
    MICROPHONE_VOLUME_DOWN: "MicrophoneVolumeDown",
    MICROPHONE_VOLUME_UP: "MicrophoneVolumeUp",
    MICROPHONE_VOLUME_MUTE: "MicrophoneVolumeMute",
    SPEECH_CORRECTION_LIST: "SpeechCorrectionList",
    SPEECH_INPUT_TOGGLE: "SpeechInputToggle",
    LAUNCH_MY_COMPUTER: "LaunchApplication1",
    LAUNCH_CALCULATOR: "LaunchApplication2",
    LAUNCH_CALENDAR: "LaunchCalendar",
    LAUNCH_CONTACTS: "LaunchContacts",
    LAUNCH_MAIL: "LaunchMail",
    LAUNCH_MEDIA_PLAYER: "LaunchMediaPlayer",
    LAUNCH_MUSIC_PLAYER: "LaunchMusicPlayer",
    LAUNCH_PHONE: "LaunchPhone",
    LAUNCH_SCREEN_SAVER: "LaunchScreenSaver",
    LAUNCH_SPREADSHEET: "LaunchSpreadsheet",
    LAUNCH_WEB_BROWSER: "LaunchWebBrowser",
    LAUNCH_WEB_CAM: "LaunchWebCam",
    LAUNCH_WORD_PROCESSOR: "LaunchWordProcessor",
    LAUNCH_ASSISTANT: "LaunchAssistant",
    LAUNCH_CONTROL_PANEL: "LaunchControlPanel",
    BROWSER_BACK: "BrowserBack",
    BROWSER_FAVORITES: "BrowserFavorites",
    BROWSER_FORWARD: "BrowserForward",
    BROWSER_HOME: "BrowserHome",
    BROWSER_REFRESH: "BrowserRefresh",
    BROWSER_SEARCH: "BrowserSearch",
    BROWSER_STOP: "BrowserStop",
    APP_SWITCH: "AppSwitch",
    CALL: "Call",
    CAMERA: "Camera",
    CAMERA_FOCUS: "CameraFocus",
    END_CALL: "EndCall",
    GO_BACK: "GoBack",
    GO_HOME: "GoHome",
    HEADSET_HOOK: "HeadsetHook",
    LAST_NUMBER_REDIAL: "LastNumberRedial",
    NOTIFICATION: "Notification",
    MANNER_MODE: "MannerMode",
    VOICE_DIAL: "VoiceDial",
    TV: "TV",
    TV_3D_MODE: "TV3DMode",
    TV_ANTENNA_CABLE: "TVAntennaCable",
    TV_AUDIO_DESCRIPTION: "TVAudioDescription",
    TV_AUDIO_DESCRIPTION_MIX_DOWN: "TVAudioDescriptionMixDown",
    TV_AUDIO_DESCRIPTION_MIX_UP: "TVAudioDescriptionMixUp",
    TV_CONTENTS_MENU: "TVContentsMenu",
    TV_DATA_SERVICE: "TVDataService",
    TV_INPUT: "TVInput",
    TV_INPUT_COMPONENT1: "TVInputComponent1",
    TV_INPUT_COMPONENT2: "TVInputComponent2",
    TV_INPUT_COMPOSITE1: "TVInputComposite1",
    TV_INPUT_COMPOSITE2: "TVInputComposite2",
    TV_INPUT_HDMI1: "TVInputHDMI1",
    TV_INPUT_HDMI2: "TVInputHDMI2",
    TV_INPUT_HDMI3: "TVInputHDMI3",
    TV_INPUT_HDMI4: "TVInputHDMI4",
    TV_INPUT_VGA1: "TVInputVGA1",
    TV_MEDIA_CONTEXT: "TVMediaContext",
    TV_NETWORK: "TVNetwork",
    TV_NUMBER_ENTRY: "TVNumberEntry",
    TV_POWER: "TVPower",
    TV_RADIO_SERVICE: "TVRadioService",
    TV_SATELLITE: "TVSatellite",
    TV_SATELLITE_BS: "TVSatelliteBS",
    TV_SATELLITE_CS: "TVSatelliteCS",
    TV_SATELLITE_TOGGLE: "TVSatelliteToggle",
    TV_TERRESTRIAL_ANALOG: "TVTerrestrialAnalog",
    TV_TERRESTRIAL_DIGITAL: "TVTerrestrialDigital",
    TV_TIMER: "TVTimer",
    AVR_INPUT: "AVRInput",
    AVR_POWER: "AVRPower",
    COLOR_F0_RED: "ColorF0Red",
    COLOR_F1_GREEN: "ColorF1Green",
    COLOR_F2_YELLOW: "ColorF2Yellow",
    COLOR_F3_BLUE: "ColorF3Blue",
    COLOR_F4_GREY: "ColorF4Grey",
    COLOR_F5_BROWN: "ColorF5Brown",
    CLOSED_CAPTION_TOGGLE: "ClosedCaptionToggle",
    DIMMER: "Dimmer",
    DISPLAY_SWAP: "DisplaySwap",
    DVR: "DVR",
    EXIT: "Exit",
    FAVORITE_CLEAR0: "FavoriteClear0",
    FAVORITE_CLEAR1: "FavoriteClear1",
    FAVORITE_CLEAR2: "FavoriteClear2",
    FAVORITE_CLEAR3: "FavoriteClear3",
    FAVORITE_RECALL0: "FavoriteRecall0",
    FAVORITE_RECALL1: "FavoriteRecall1",
    FAVORITE_RECALL2: "FavoriteRecall2",
    FAVORITE_RECALL3: "FavoriteRecall3",
    FAVORITE_STORE0: "FavoriteStore0",
    FAVORITE_STORE1: "FavoriteStore1",
    FAVORITE_STORE2: "FavoriteStore2",
    FAVORITE_STORE3: "FavoriteStore3",
    GUIDE: "Guide",
    GUIDE_NEXT_DAY: "GuideNextDay",
    GUIDE_PREVIOUS_DAY: "GuidePreviousDay",
    INFO: "Info",
    INSTANT_REPLAY: "InstantReplay",
    LINK: "Link",
    LIST_PROGRAM: "ListProgram",
    LIVE_CONTENT: "LiveContent",
    LOCK: "Lock",
    MEDIA_APPS: "MediaApps",
    MEDIA_AUDIO_TRACK: "MediaAudioTrack",
    MEDIA_LAST: "MediaLast",
    MEDIA_SKIP_BACKWARD: "MediaSkipBackward",
    MEDIA_SKIP_FORWARD: "MediaSkipForward",
    MEDIA_SKIP: "MediaSkip",
    MEDIA_STEP_BACKWARD: "MediaStepBackward",
    MEDIA_STEP_FORWARD: "MediaStepForward",
    MEDIA_TOP_MENU: "MediaTopMenu",
    NAVIGATE_IN: "NavigateIn",
    NAVIGATE_NEXT: "NavigateNext",
    NAVIGATE_OUT: "NavigateOut",
    NAVIGATE_PREVIOUS: "NavigatePrevious",
    NEXT_FAVORITE_CHANNEL: "NextFavoriteChannel",
    NEXT_USER_PROFILE: "NextUserProfile",
    ON_DEMAND: "OnDemand",
    PAIRING: "Pairing",
    PINP_DOWN: "PinPDown",
    PINP_MOVE: "PinPMove",
    PINP_TOGGLE: "PinPToggle",
    PINP_UP: "PinPUp",
    PLAY_SPEED_DOWN: "PlaySpeedDown",
    PLAY_SPEED_RESET: "PlaySpeedReset",
    PLAY_SPEED_UP: "PlaySpeedUp",
    RANDOM_TOGGLE: "RandomToggle",
    RC_LOW_BATTERY: "RcLowBattery",
    RECORD_SPEED_NEXT: "RecordSpeedNext",
    RF_BYPASS: "RfBypass",
    SCAN_CHANNELS_TOGGLE: "ScanChannelsToggle",
    SCREEN_MODE_NEXT: "ScreenModeNext",
    SETTINGS: "Settings",
    SPLIT_SCREEN_TOGGLE: "SplitScreenToggle",
    STB_INPUT: "STBInput",
    STB_POWER: "STBPower",
    SUBTITLE: "Subtitle",
    TELETEXT: "Teletext",
    VIDEO_MODE_NEXT: "VideoModeNext",
    WINK: "Wink",
    ZOOM_TOGGLE: "ZoomToggle",
}

// https://github.com/chromium/chromium/blob/main/ui/events/keycodes/keyboard_code_conversion_xkb.cc
function NonPrintableXKeySymToDomKey(keysym) {
    switch (keysym) {
        case XKB_KEY.BackSpace:
            return DomKey.BACKSPACE
        case XKB_KEY.Tab:
        case XKB_KEY.KP_Tab:
        case XKB_KEY.ISO_Left_Tab:
            return DomKey.TAB
        case XKB_KEY.Clear:
        case XKB_KEY.KP_Begin:
        case XKB_KEY.XF86Clear:
            return DomKey.CLEAR
        case XKB_KEY.Return:
        case XKB_KEY.KP_Enter:
            return DomKey.ENTER
        case XKB_KEY.Linefeed:
            return DomKey.ENTER
        case XKB_KEY.Pause:
            return DomKey.PAUSE
        case XKB_KEY.Scroll_Lock:
            return DomKey.SCROLL_LOCK
        case XKB_KEY.Escape:
            return DomKey.ESCAPE
        case XKB_KEY.Multi_key:
            return DomKey.COMPOSE
        case XKB_KEY.Kanji:
            return DomKey.KANJI_MODE
        case XKB_KEY.Muhenkan:
            return DomKey.NON_CONVERT
        case XKB_KEY.Henkan_Mode:
            return DomKey.CONVERT
        case XKB_KEY.Romaji:
            return DomKey.ROMAJI
        case XKB_KEY.Hiragana:
            return DomKey.HIRAGANA
        case XKB_KEY.Katakana:
            return DomKey.KATAKANA
        case XKB_KEY.Hiragana_Katakana:
            return DomKey.HIRAGANA_KATAKANA
        case XKB_KEY.Zenkaku:
            return DomKey.ZENKAKU
        case XKB_KEY.Hankaku:
            return DomKey.HANKAKU
        case XKB_KEY.Zenkaku_Hankaku:
            return DomKey.ZENKAKU_HANKAKU
        case XKB_KEY.Kana_Lock:
            return DomKey.KANA_MODE
        case XKB_KEY.Eisu_Shift:
        case XKB_KEY.Eisu_toggle:
            return DomKey.ALPHANUMERIC
        case XKB_KEY.Hangul:
            return DomKey.HANGUL_MODE
        case XKB_KEY.Hangul_Hanja:
            return DomKey.HANJA_MODE
        case XKB_KEY.Codeinput:
            return DomKey.CODE_INPUT
        case XKB_KEY.SingleCandidate:
            return DomKey.SINGLE_CANDIDATE
        case XKB_KEY.MultipleCandidate:
            return DomKey.ALL_CANDIDATES
        case XKB_KEY.PreviousCandidate:
            return DomKey.PREVIOUS_CANDIDATE
        case XKB_KEY.Home:
        case XKB_KEY.KP_Home:
            return DomKey.HOME
        case XKB_KEY.Left:
        case XKB_KEY.KP_Left:
            return DomKey.ARROW_LEFT
        case XKB_KEY.Up:
        case XKB_KEY.KP_Up:
            return DomKey.ARROW_UP
        case XKB_KEY.Right:
        case XKB_KEY.KP_Right:
            return DomKey.ARROW_RIGHT
        case XKB_KEY.Down:
        case XKB_KEY.KP_Down:
            return DomKey.ARROW_DOWN
        case XKB_KEY.Prior:
        case XKB_KEY.KP_Prior:
            return DomKey.PAGE_UP
        case XKB_KEY.Next:
        case XKB_KEY.KP_Next:
        case XKB_KEY.XF86ScrollDown:
            return DomKey.PAGE_DOWN
        case XKB_KEY.End:
        case XKB_KEY.KP_End:
        case XKB_KEY.XF86ScrollUp:
            return DomKey.END
        case XKB_KEY.Select:
            return DomKey.SELECT
        case XKB_KEY.Print:
            // For legacy reasons XKB and Linux treat Print and PrintScreen as
            // PrintScreen. See https://crbug.com/683097.
            return DomKey.PRINT_SCREEN
        case XKB_KEY["3270_PrintScreen"]:
            return DomKey.PRINT_SCREEN
        case XKB_KEY.Execute:
            return DomKey.EXECUTE
        case XKB_KEY.Insert:
        case XKB_KEY.KP_Insert:
            return DomKey.INSERT
        case XKB_KEY.Undo:
            return DomKey.UNDO
        case XKB_KEY.Redo:
            return DomKey.REDO
        case XKB_KEY.Menu:
            return DomKey.CONTEXT_MENU
        case XKB_KEY.Find:
            return DomKey.FIND
        case XKB_KEY.Cancel:
            return DomKey.CANCEL
        case XKB_KEY.Help:
            return DomKey.HELP
        case XKB_KEY.Break:
        case XKB_KEY["3270_Attn"]:
            return DomKey.ATTN
        case XKB_KEY.Mode_switch:
            return DomKey.MODE_CHANGE
        case XKB_KEY.Num_Lock:
            return DomKey.NUM_LOCK
        case XKB_KEY.F1:
        case XKB_KEY.KP_F1:
            return DomKey.F1
        case XKB_KEY.F2:
        case XKB_KEY.KP_F2:
            return DomKey.F2
        case XKB_KEY.F3:
        case XKB_KEY.KP_F3:
            return DomKey.F3
        case XKB_KEY.F4:
        case XKB_KEY.KP_F4:
            return DomKey.F4
        case XKB_KEY.F5:
            return DomKey.F5
        case XKB_KEY.F6:
            return DomKey.F6
        case XKB_KEY.F7:
            return DomKey.F7
        case XKB_KEY.F8:
            return DomKey.F8
        case XKB_KEY.F9:
            return DomKey.F9
        case XKB_KEY.F10:
            return DomKey.F10
        case XKB_KEY.F11:
            return DomKey.F11
        case XKB_KEY.F12:
            return DomKey.F12
        case XKB_KEY.XF86Tools: // XKB 'inet' mapping of F13
        case XKB_KEY.F13:
            return DomKey.F13
        case XKB_KEY.F14:
        case XKB_KEY.XF86Launch5: // XKB 'inet' mapping of F14
            return DomKey.F14
        case XKB_KEY.F15:
        case XKB_KEY.XF86Launch6: // XKB 'inet' mapping of F15
            return DomKey.F15
        case XKB_KEY.F16:
        case XKB_KEY.XF86Launch7: // XKB 'inet' mapping of F16
            return DomKey.F16
        case XKB_KEY.F17:
        case XKB_KEY.XF86Launch8: // XKB 'inet' mapping of F17
            return DomKey.F17
        case XKB_KEY.F18:
        case XKB_KEY.XF86Launch9: // XKB 'inet' mapping of F18
            return DomKey.F18
        case XKB_KEY.F19:
            return DomKey.F19
        case XKB_KEY.F20:
            return DomKey.F20
        case XKB_KEY.F21:
            return DomKey.F21
        case XKB_KEY.F22:
            return DomKey.F22
        case XKB_KEY.F23:
            return DomKey.F23
        case XKB_KEY.F24:
            return DomKey.F24
        case XKB_KEY.Shift_L:
        case XKB_KEY.Shift_R:
            return DomKey.SHIFT
        case XKB_KEY.Control_L:
        case XKB_KEY.Control_R:
            return DomKey.CONTROL
        case XKB_KEY.Caps_Lock:
            return DomKey.CAPS_LOCK
        case XKB_KEY.Meta_L:
        case XKB_KEY.Meta_R:
            return DomKey.META
        case XKB_KEY.Alt_L:
        case XKB_KEY.Alt_R:
            return DomKey.ALT
        case XKB_KEY.Super_L:
        case XKB_KEY.Super_R:
            return DomKey.META
        case XKB_KEY.Hyper_L:
        case XKB_KEY.Hyper_R:
            return DomKey.HYPER
        case XKB_KEY.Delete:
            return DomKey.DEL
        case XKB_KEY.SunProps:
            return DomKey.PROPS
        case XKB_KEY.XF86Next_VMode:
            return DomKey.VIDEO_MODE_NEXT
        case XKB_KEY.XF86MonBrightnessUp:
            return DomKey.BRIGHTNESS_UP
        case XKB_KEY.XF86MonBrightnessDown:
            return DomKey.BRIGHTNESS_DOWN
        case XKB_KEY.XF86Standby:
        case XKB_KEY.XF86Sleep:
        case XKB_KEY.XF86Suspend:
            return DomKey.STANDBY
        case XKB_KEY.XF86AudioLowerVolume:
            return DomKey.AUDIO_VOLUME_DOWN
        case XKB_KEY.XF86AudioMute:
            return DomKey.AUDIO_VOLUME_MUTE
        case XKB_KEY.XF86AudioRaiseVolume:
            return DomKey.AUDIO_VOLUME_UP
        case XKB_KEY.XF86AudioPlay:
            return DomKey.MEDIA_PLAY_PAUSE
        case XKB_KEY.XF86AudioStop:
            return DomKey.MEDIA_STOP
        case XKB_KEY.XF86AudioPrev:
            return DomKey.MEDIA_TRACK_PREVIOUS
        case XKB_KEY.XF86AudioNext:
            return DomKey.MEDIA_TRACK_NEXT
        case XKB_KEY.XF86HomePage:
            return DomKey.BROWSER_HOME
        case XKB_KEY.XF86Mail:
            return DomKey.LAUNCH_MAIL
        case XKB_KEY.XF86Search:
            return DomKey.BROWSER_SEARCH
        case XKB_KEY.XF86AudioRecord:
            return DomKey.MEDIA_RECORD
        case XKB_KEY.XF86Calculator:
            return DomKey.LAUNCH_CALCULATOR
        case XKB_KEY.XF86Calendar:
            return DomKey.LAUNCH_CALENDAR
        case XKB_KEY.XF86Back:
            return DomKey.BROWSER_BACK
        case XKB_KEY.XF86Forward:
            return DomKey.BROWSER_FORWARD
        case XKB_KEY.XF86Stop:
            return DomKey.BROWSER_STOP
        case XKB_KEY.XF86Refresh:
        case XKB_KEY.XF86Reload:
            return DomKey.BROWSER_REFRESH
        case XKB_KEY.XF86PowerOff:
            return DomKey.POWER_OFF
        case XKB_KEY.XF86WakeUp:
            return DomKey.WAKE_UP
        case XKB_KEY.XF86Eject:
            return DomKey.EJECT
        case XKB_KEY.XF86ScreenSaver:
            return DomKey.LAUNCH_SCREEN_SAVER
        case XKB_KEY.XF86WWW:
            return DomKey.LAUNCH_WEB_BROWSER
        case XKB_KEY.XF86Favorites:
            return DomKey.BROWSER_FAVORITES
        case XKB_KEY.XF86AudioPause:
            return DomKey.MEDIA_PAUSE
        case XKB_KEY.XF86AudioMedia:
        case XKB_KEY.XF86Music:
            return DomKey.LAUNCH_MUSIC_PLAYER
        case XKB_KEY.XF86MyComputer:
        case XKB_KEY.XF86Explorer:
            return DomKey.LAUNCH_MY_COMPUTER
        case XKB_KEY.XF86AudioRewind:
            return DomKey.MEDIA_REWIND
        case XKB_KEY.XF86CD:
        case XKB_KEY.XF86Video:
            return DomKey.LAUNCH_MEDIA_PLAYER
        case XKB_KEY.XF86Close:
            return DomKey.CLOSE
        case XKB_KEY.XF86Copy:
        case XKB_KEY.SunCopy:
            return DomKey.COPY
        case XKB_KEY.XF86Cut:
        case XKB_KEY.SunCut:
            return DomKey.CUT
        case XKB_KEY.XF86Display:
            return DomKey.DISPLAY_SWAP
        case XKB_KEY.XF86Excel:
            return DomKey.LAUNCH_SPREADSHEET
        case XKB_KEY.XF86LogOff:
            return DomKey.LOG_OFF
        case XKB_KEY.XF86New:
            return DomKey.NEW
        case XKB_KEY.XF86Open:
        case XKB_KEY.SunOpen:
            return DomKey.OPEN
        case XKB_KEY.XF86Paste:
        case XKB_KEY.SunPaste:
            return DomKey.PASTE
        case XKB_KEY.XF86Reply:
            return DomKey.MAIL_REPLY
        case XKB_KEY.XF86Save:
            return DomKey.SAVE
        case XKB_KEY.XF86Send:
            return DomKey.MAIL_SEND
        case XKB_KEY.XF86Spell:
            return DomKey.SPELL_CHECK
        case XKB_KEY.XF86SplitScreen:
            return DomKey.SPLIT_SCREEN_TOGGLE
        case XKB_KEY.XF86Word:
        case XKB_KEY.XF86OfficeHome:
            return DomKey.LAUNCH_WORD_PROCESSOR
        case XKB_KEY.XF86ZoomIn:
            return DomKey.ZOOM_IN
        case XKB_KEY.XF86ZoomOut:
            return DomKey.ZOOM_OUT
        case XKB_KEY.XF86WebCam:
            return DomKey.LAUNCH_WEB_CAM
        case XKB_KEY.XF86MailForward:
            return DomKey.MAIL_FORWARD
        case XKB_KEY.XF86AudioForward:
            return DomKey.MEDIA_FAST_FORWARD
        case XKB_KEY.XF86AudioRandomPlay:
            return DomKey.RANDOM_TOGGLE
        case XKB_KEY.XF86Subtitle:
            return DomKey.SUBTITLE
        case XKB_KEY.XF86Hibernate:
            return DomKey.HIBERNATE
        case XKB_KEY["3270_EraseEOF"]:
            return DomKey.ERASE_EOF
        case XKB_KEY["3270_Play"]:
            return DomKey.PLAY
        case XKB_KEY["3270_ExSelect"]:
            return DomKey.EX_SEL
        case XKB_KEY["3270_CursorSelect"]:
            return DomKey.CR_SEL
        case XKB_KEY.ISO_Level3_Shift:
            return DomKey.ALT_GRAPH
        case XKB_KEY.ISO_Level3_Latch:
            return DomKey.ALT_GRAPH_LATCH
        case XKB_KEY.ISO_Level5_Shift:
            return DomKey.SHIFT_LEVEL5
        case XKB_KEY.ISO_Next_Group:
            return DomKey.GROUP_NEXT
        case XKB_KEY.ISO_Prev_Group:
            return DomKey.GROUP_PREVIOUS
        case XKB_KEY.ISO_First_Group:
            return DomKey.GROUP_FIRST
        case XKB_KEY.ISO_Last_Group:
            return DomKey.GROUP_LAST
        case XKB_KEY.dead_grave:
            // combining grave accent
            return DomKey.DeadKeyFromCombiningCharacter(0x0300)
        case XKB_KEY.dead_acute:
            // combining acute accent
            return DomKey.DeadKeyFromCombiningCharacter(0x0301)
        case XKB_KEY.dead_circumflex:
            // combining circumflex accent
            return DomKey.DeadKeyFromCombiningCharacter(0x0302)
        case XKB_KEY.dead_tilde:
            // combining tilde
            return DomKey.DeadKeyFromCombiningCharacter(0x0303)
        case XKB_KEY.dead_macron:
            // combining macron
            return DomKey.DeadKeyFromCombiningCharacter(0x0304)
        case XKB_KEY.dead_breve:
            // combining breve
            return DomKey.DeadKeyFromCombiningCharacter(0x0306)
        case XKB_KEY.dead_abovedot:
            // combining dot above
            return DomKey.DeadKeyFromCombiningCharacter(0x0307)
        case XKB_KEY.dead_diaeresis:
            // combining diaeresis
            return DomKey.DeadKeyFromCombiningCharacter(0x0308)
        case XKB_KEY.dead_abovering:
            // combining ring above
            return DomKey.DeadKeyFromCombiningCharacter(0x030a)
        case XKB_KEY.dead_doubleacute:
            // combining double acute accent
            return DomKey.DeadKeyFromCombiningCharacter(0x030b)
        case XKB_KEY.dead_caron:
            // combining caron
            return DomKey.DeadKeyFromCombiningCharacter(0x030c)
        case XKB_KEY.dead_cedilla:
            // combining cedilla
            return DomKey.DeadKeyFromCombiningCharacter(0x0327)
        case XKB_KEY.dead_ogonek:
            // combining ogonek
            return DomKey.DeadKeyFromCombiningCharacter(0x0328)
        case XKB_KEY.dead_iota:
            // combining greek ypogegrammeni
            return DomKey.DeadKeyFromCombiningCharacter(0x0345)
        case XKB_KEY.dead_voiced_sound:
            // combining voiced sound mark
            return DomKey.DeadKeyFromCombiningCharacter(0x3099)
        case XKB_KEY.dead_semivoiced_sound:
            // combining semi-voiced sound mark
            return DomKey.DeadKeyFromCombiningCharacter(0x309a)
        case XKB_KEY.dead_belowdot:
            // combining dot below
            return DomKey.DeadKeyFromCombiningCharacter(0x0323)
        case XKB_KEY.dead_hook:
            // combining hook above
            return DomKey.DeadKeyFromCombiningCharacter(0x0309)
        case XKB_KEY.dead_horn:
            // combining horn
            return DomKey.DeadKeyFromCombiningCharacter(0x031b)
        case XKB_KEY.dead_stroke:
            // combining long solidus overlay
            return DomKey.DeadKeyFromCombiningCharacter(0x0338)
        case XKB_KEY.dead_abovecomma:
            // combining comma above
            return DomKey.DeadKeyFromCombiningCharacter(0x0313)
        case XKB_KEY.dead_abovereversedcomma:
            // combining reversed comma above
            return DomKey.DeadKeyFromCombiningCharacter(0x0314)
        case XKB_KEY.dead_doublegrave:
            // combining double grave accent
            return DomKey.DeadKeyFromCombiningCharacter(0x030f)
        case XKB_KEY.dead_belowring:
            // combining ring below
            return DomKey.DeadKeyFromCombiningCharacter(0x0325)
        case XKB_KEY.dead_belowmacron:
            // combining macron below
            return DomKey.DeadKeyFromCombiningCharacter(0x0331)
        case XKB_KEY.dead_belowcircumflex:
            // combining circumflex accent below
            return DomKey.DeadKeyFromCombiningCharacter(0x032d)
        case XKB_KEY.dead_belowtilde:
            // combining tilde below
            return DomKey.DeadKeyFromCombiningCharacter(0x0330)
        case XKB_KEY.dead_belowbreve:
            // combining breve below
            return DomKey.DeadKeyFromCombiningCharacter(0x032e)
        case XKB_KEY.dead_belowdiaeresis:
            // combining diaeresis below
            return DomKey.DeadKeyFromCombiningCharacter(0x0324)
        case XKB_KEY.dead_invertedbreve:
            // combining inverted breve
            return DomKey.DeadKeyFromCombiningCharacter(0x0311)
        case XKB_KEY.dead_belowcomma:
            // combining comma below
            return DomKey.DeadKeyFromCombiningCharacter(0x0326)
        case XKB_KEY.dead_currency:
            // currency sign
            return DomKey.DeadKeyFromCombiningCharacter(0x00a4)
        case XKB_KEY.dead_greek:
            // greek question mark
            return DomKey.DeadKeyFromCombiningCharacter(0x037e)
        default:
            return DomKey.NONE
    }
}

function XKeySymToDomKey(keysym, character) {
    const dom_key = NonPrintableXKeySymToDomKey(keysym)
    // console.log(keysym, dom_key)
    if (dom_key !== DomKey.NONE) return dom_key
    if (character == 0) return DomKey.UNIDENTIFIED
    return String.fromCharCode(character)
}

function GetDomKeyFromXEvent(keyname, keysym) {
    const ch = GetUnicodeCharacterFromXKeySym(keysym)
    return XKeySymToDomKey(keysym, ch)
}

const regex = /#define\s+XKB_KEY.(\w+)\s+(0x[0-9a-fA-F]+)\s+/g
const content = readFileSync(
    "/usr/include/xkbcommon/xkbcommon-keysyms.h",
    "utf-8"
).toString()

let array1
while ((array1 = regex.exec(content)) !== null) {
    const name = array1[1]
    const value = parseInt(array1[2], 16)
    XKB_KEY[name] = value
    XKB_NAME[value] = name
}

let entries = Object.entries(XKB_KEY).map(([k, v]) => [
    GetDomKeyFromXEvent(k, v),
    v,
])

let entries_isorted = entries.sort(([a], [b]) => {
    const A = a[0].toLowerCase() + a.slice(1)
    const B = b[0].toLowerCase() + b.slice(1)
    if (A < B) {
        return -1
    }
    if (A > B) {
        return 1
    }
    return 0
})

let entries_kssorted = [...entries].sort(([, A], [, B]) => {
    return A - B
})

let entry_offsets = {}

let output = `#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverlength-strings"
#endif
static const char *keysym_names =
`

function decimalToHex(d, padding) {
    var hex = Number(d).toString(16)
    padding =
        typeof padding === "undefined" || padding === null
            ? (padding = 2)
            : padding

    while (hex.length < padding) {
        hex = "0" + hex
    }

    return hex
}

let offs = 0

for (const [name, value] of entries_isorted) {
    entry_offsets[name] = offs
    output += `    ${JSON.stringify(name)}"\\0"\n`
    offs += Buffer.byteLength(name, "utf8") + 1
}

output += `;\n`

output += `
struct name_keysym {
    xkb_keysym_t keysym;
    uint32_t offset;
};
`

output += `
static const struct name_keysym keysym_to_name[] = {
${entries_kssorted
    .map(
        ([k, v]) =>
            `    { 0x${decimalToHex(v, 8)}, ${entry_offsets[k]} }, /* ${k} */`
    )
    .join("\n")}
};
`
writeFileSync(path.join("include", "gen", "ks_table.h"), output, {
    encoding: "utf-8",
})
