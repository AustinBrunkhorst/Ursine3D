/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** KeyboardKey.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    enum KeyboardKey
    {
        // KEY_BACK (08) BACKSPACE key
        KEY_BACK = 0x08,

        // KEY_TAB (09) TAB key
        KEY_TAB = 0x09,

        // KEY_CLEAR (0C) CLEAR key
        KEY_CLEAR = 0x0C,

        // KEY_RETURN (0D)
        KEY_RETURN = 0x0D,

        // KEY_SHIFT (10) SHIFT key
        KEY_SHIFT = 0x10,

        // KEY_CONTROL (11) CTRL key
        KEY_CONTROL = 0x11,

        // KEY_MENU (12) ALT key
        KEY_MENU = 0x12,

        // KEY_PAUSE (13) PAUSE key
        KEY_PAUSE = 0x13,

        // KEY_CAPITAL (14) CAPS LOCK key
        KEY_CAPITAL = 0x14,

        // KEY_KANA (15) Input Method Editor (IME) Kana mode
        KEY_KANA = 0x15,

        // KEY_HANGUL (15) IME Hangul mode
        KEY_HANGUL = 0x15,

        // KEY_JUNJA (17) IME Junja mode
        KEY_JUNJA = 0x17,

        // KEY_FINAL (18) IME final mode
        KEY_FINAL = 0x18,

        // KEY_HANJA (19) IME Hanja mode
        KEY_HANJA = 0x19,

        // KEY_KANJI (19) IME Kanji mode
        KEY_KANJI = 0x19,

        // KEY_ESCAPE (1B) ESC key
        KEY_ESCAPE = 0x1B,

        // KEY_CONVERT (1C) IME convert
        KEY_CONVERT = 0x1C,

        // KEY_NONCONVERT (1D) IME nonconvert
        KEY_NONCONVERT = 0x1D,

        // KEY_ACCEPT (1E) IME accept
        KEY_ACCEPT = 0x1E,

        // KEY_MODECHANGE (1F) IME mode change request
        KEY_MODECHANGE = 0x1F,

        // KEY_SPACE (20) SPACEBAR
        KEY_SPACE = 0x20,

        // KEY_PRIOR (21) PAGE UP key
        KEY_PRIOR = 0x21,

        // KEY_NEXT (22) PAGE DOWN key
        KEY_NEXT = 0x22,

        // KEY_END (23) END key
        KEY_END = 0x23,

        // KEY_HOME (24) HOME key
        KEY_HOME = 0x24,

        // KEY_LEFT (25) LEFT ARROW key
        KEY_LEFT = 0x25,

        // KEY_UP (26) UP ARROW key
        KEY_UP = 0x26,

        // KEY_RIGHT (27) RIGHT ARROW key
        KEY_RIGHT = 0x27,

        // KEY_DOWN (28) DOWN ARROW key
        KEY_DOWN = 0x28,

        // KEY_SELECT (29) SELECT key
        KEY_SELECT = 0x29,

        // KEY_PRINT (2A) PRINT key
        KEY_PRINT = 0x2A,

        // KEY_VEXECUTE (2B) EXECUTE key (linux ?)
        KEY_VEXECUTE = 0x2B,

        // KEY_SNAPSHOT (2C) PRINT SCREEN key
        KEY_SNAPSHOT = 0x2C,

        // KEY_INSERT (2D) INS key
        KEY_INSERT = 0x2D,

        // KEY_DELETE (2E) DEL key
        KEY_DELETE = 0x2E,

        // KEY_HELP (2F) HELP key
        KEY_HELP = 0x2F,

        // (30) 0 key
        KEY_0 = 0x30,

        // (31) 1 key
        KEY_1 = 0x31,

        // (32) 2 key
        KEY_2 = 0x32,

        // (33) 3 key
        KEY_3 = 0x33,

        // (34) 4 key
        KEY_4 = 0x34,

        // (35) 5 key;
        KEY_5 = 0x35,

        // (36) 6 key
        KEY_6 = 0x36,

        // (37) 7 key
        KEY_7 = 0x37,

        // (38) 8 key
        KEY_8 = 0x38,

        // (39) 9 key
        KEY_9 = 0x39,

        // (41) A key
        KEY_A = 0x41,

        // (42) B key
        KEY_B = 0x42,

        // (43) C key
        KEY_C = 0x43,

        // (44) D key
        KEY_D = 0x44,

        // (45) E key
        KEY_E = 0x45,

        // (46) F key
        KEY_F = 0x46,

        // (47) G key
        KEY_G = 0x47,

        // (48) H key
        KEY_H = 0x48,

        // (49) I key
        KEY_I = 0x49,

        // (4A) J key
        KEY_J = 0x4A,

        // (4B) K key
        KEY_K = 0x4B,

        // (4C) L key
        KEY_L = 0x4C,

        // (4D) M key
        KEY_M = 0x4D,

        // (4E) N key
        KEY_N = 0x4E,

        // (4F) O key
        KEY_O = 0x4F,

        // (50) P key
        KEY_P = 0x50,

        // (51) Q key
        KEY_Q = 0x51,

        // (52) R key
        KEY_R = 0x52,

        // (53) S key
        KEY_S = 0x53,

        // (54) T key
        KEY_T = 0x54,

        // (55) U key
        KEY_U = 0x55,

        // (56) V key
        KEY_V = 0x56,

        // (57) W key
        KEY_W = 0x57,

        // (58) X key
        KEY_X = 0x58,

        // (59) Y key
        KEY_Y = 0x59,

        // (5A) Z key
        KEY_Z = 0x5A,

        // KEY_LWIN (5B) Left Windows key (Microsoft Natural keyboard)
        KEY_LWIN = 0x5B,

        // KEY_RWIN (5C) Right Windows key (Natural keyboard)
        KEY_RWIN = 0x5C,

        // KEY_APPS (5D) Applications key (Natural keyboard)
        KEY_APPS = 0x5D,

        // KEY_SLEEP (5F) Computer Sleep key
        KEY_SLEEP = 0x5F,

        // KEY_NUMPAD0 (60) Numeric keypad 0 key
        KEY_NUMPAD0 = 0x60,

        // KEY_NUMPAD1 (61) Numeric keypad 1 key
        KEY_NUMPAD1 = 0x61,

        // KEY_NUMPAD2 (62) Numeric keypad 2 key
        KEY_NUMPAD2 = 0x62,

        // KEY_NUMPAD3 (63) Numeric keypad 3 key
        KEY_NUMPAD3 = 0x63,

        // KEY_NUMPAD4 (64) Numeric keypad 4 key
        KEY_NUMPAD4 = 0x64,

        // KEY_NUMPAD5 (65) Numeric keypad 5 key
        KEY_NUMPAD5 = 0x65,

        // KEY_NUMPAD6 (66) Numeric keypad 6 key
        KEY_NUMPAD6 = 0x66,

        // KEY_NUMPAD7 (67) Numeric keypad 7 key
        KEY_NUMPAD7 = 0x67,

        // KEY_NUMPAD8 (68) Numeric keypad 8 key
        KEY_NUMPAD8 = 0x68,

        // KEY_NUMPAD9 (69) Numeric keypad 9 key
        KEY_NUMPAD9 = 0x69,

        // KEY_MULTIPLY (6A) Multiply key
        KEY_MULTIPLY = 0x6A,

        // KEY_ADD (6B) Add key
        KEY_ADD = 0x6B,

        // KEY_SEPARATOR (6C) Separator key
        KEY_SEPARATOR = 0x6C,

        // KEY_SUBTRACT (6D) Subtract key
        KEY_SUBTRACT = 0x6D,

        // KEY_DECIMAL (6E) Decimal key
        KEY_DECIMAL = 0x6E,

        // KEY_DIVIDE (6F) Divide key
        KEY_DIVIDE = 0x6F,

        // KEY_F1 (70) F1 key
        KEY_F1 = 0x70,

        // KEY_F2 (71) F2 key
        KEY_F2 = 0x71,

        // KEY_F3 (72) F3 key
        KEY_F3 = 0x72,

        // KEY_F4 (73) F4 key
        KEY_F4 = 0x73,

        // KEY_F5 (74) F5 key
        KEY_F5 = 0x74,

        // KEY_F6 (75) F6 key
        KEY_F6 = 0x75,

        // KEY_F7 (76) F7 key
        KEY_F7 = 0x76,

        // KEY_F8 (77) F8 key
        KEY_F8 = 0x77,

        // KEY_F9 (78) F9 key
        KEY_F9 = 0x78,

        // KEY_F10 (79) F10 key
        KEY_F10 = 0x79,

        // KEY_F11 (7A) F11 key
        KEY_F11 = 0x7A,

        // KEY_F12 (7B) F12 key
        KEY_F12 = 0x7B,

        // KEY_F13 (7C) F13 key
        KEY_F13 = 0x7C,

        // KEY_F14 (7D) F14 key
        KEY_F14 = 0x7D,

        // KEY_F15 (7E) F15 key
        KEY_F15 = 0x7E,

        // KEY_F16 (7F) F16 key
        KEY_F16 = 0x7F,

        // KEY_F17 (80H) F17 key
        KEY_F17 = 0x80,

        // KEY_F18 (81H) F18 key
        KEY_F18 = 0x81,

        // KEY_F19 (82H) F19 key
        KEY_F19 = 0x82,

        // KEY_F20 (83H) F20 key
        KEY_F20 = 0x83,

        // KEY_F21 (84H) F21 key
        KEY_F21 = 0x84,

        // KEY_F22 (85H) F22 key
        KEY_F22 = 0x85,

        // KEY_F23 (86H) F23 key
        KEY_F23 = 0x86,

        // KEY_F24 (87H) F24 key
        KEY_F24 = 0x87,

        // KEY_NUMLOCK (90) NUM LOCK key
        KEY_NUMLOCK = 0x90,

        // KEY_SCROLL (91) SCROLL LOCK key
        KEY_SCROLL = 0x91,

        // KEY_LSHIFT (A0) Left SHIFT key
        KEY_LSHIFT = 0xA0,

        // KEY_RSHIFT (A1) Right SHIFT key
        KEY_RSHIFT = 0xA1,

        // KEY_LCONTROL (A2) Left CONTROL key
        KEY_LCONTROL = 0xA2,

        // KEY_RCONTROL (A3) Right CONTROL key
        KEY_RCONTROL = 0xA3,

        // KEY_LMENU (A4) Left MENU key
        KEY_LMENU = 0xA4,

        // KEY_RMENU (A5) Right MENU key
        KEY_RMENU = 0xA5,

        // KEY_BROWSER_BACK (A6) Windows 2000/XP: Browser Back key
        KEY_BROWSER_BACK = 0xA6,

        // KEY_BROWSER_FORWARD (A7) Windows 2000/XP: Browser Forward key
        KEY_BROWSER_FORWARD = 0xA7,

        // KEY_BROWSER_REFRESH (A8) Windows 2000/XP: Browser Refresh key
        KEY_BROWSER_REFRESH = 0xA8,

        // KEY_BROWSER_STOP (A9) Windows 2000/XP: Browser Stop key
        KEY_BROWSER_STOP = 0xA9,

        // KEY_BROWSER_SEARCH (AA) Windows 2000/XP: Browser Search key
        KEY_BROWSER_SEARCH = 0xAA,

        // KEY_BROWSER_FAVORITES (AB) Windows 2000/XP: Browser Favorites key
        KEY_BROWSER_FAVORITES = 0xAB,

        // KEY_BROWSER_HOME (AC) Windows 2000/XP: Browser Start and Home key
        KEY_BROWSER_HOME = 0xAC,

        // KEY_VOLUME_MUTE (AD) Windows 2000/XP: Volume Mute key
        KEY_VOLUME_MUTE = 0xAD,

        // KEY_VOLUME_DOWN (AE) Windows 2000/XP: Volume Down key
        KEY_VOLUME_DOWN = 0xAE,

        // KEY_VOLUME_UP (AF) Windows 2000/XP: Volume Up key
        KEY_VOLUME_UP = 0xAF,

        // KEY_MEDIA_NEXT_TRACK (B0) Windows 2000/XP: Next Track key
        KEY_MEDIA_NEXT_TRACK = 0xB0,

        // KEY_MEDIA_PREV_TRACK (B1) Windows 2000/XP: Previous Track key
        KEY_MEDIA_PREV_TRACK = 0xB1,

        // KEY_MEDIA_STOP (B2) Windows 2000/XP: Stop Media key
        KEY_MEDIA_STOP = 0xB2,

        // KEY_MEDIA_PLAY_PAUSE (B3) Windows 2000/XP: Play/Pause Media key
        KEY_MEDIA_PLAY_PAUSE = 0xB3,

        // KEY_LAUNCH_MAIL (B4) Windows 2000/XP: Start Mail key
        KEY_MEDIA_LAUNCH_MAIL = 0xB4,

        // KEY_LAUNCH_MEDIA_SELECT (B5) Windows 2000/XP: Select Media key
        KEY_MEDIA_LAUNCH_MEDIA_SELECT = 0xB5,

        // KEY_LAUNCH_APP1 (B6) Windows 2000/XP: Start Application 1 key
        KEY_MEDIA_LAUNCH_APP1 = 0xB6,

        // KEY_LAUNCH_APP2 (B7) Windows 2000/XP: Start Application 2 key
        KEY_MEDIA_LAUNCH_APP2 = 0xB7,

        // KEY_OEM_1 (BA) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ';:' key
        KEY_OEM_1 = 0xBA,

        // KEY_OEM_PLUS (BB) Windows 2000/XP: For any country/region, the '+' key
        KEY_OEM_PLUS = 0xBB,

        // KEY_OEM_COMMA (BC) Windows 2000/XP: For any country/region, the ',' key
        KEY_OEM_COMMA = 0xBC,

        // KEY_OEM_MINUS (BD) Windows 2000/XP: For any country/region, the '-' key
        KEY_OEM_MINUS = 0xBD,

        // KEY_OEM_PERIOD (BE) Windows 2000/XP: For any country/region, the '.' key
        KEY_OEM_PERIOD = 0xBE,

        // KEY_OEM_2 (BF) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
        KEY_OEM_2 = 0xBF,

        // KEY_OEM_3 (C0) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
        KEY_TILDE = 0xC0,

        // KEY_OEM_4 (DB) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
        KEY_OEM_4 = 0xDB,

        // KEY_OEM_5 (DC) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
        KEY_OEM_5 = 0xDC,

        // KEY_OEM_6 (DD) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
        KEY_OEM_6 = 0xDD,

        // KEY_OEM_7 (DE) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
        KEY_OEM_7 = 0xDE,

        // KEY_OEM_8 (DF) Used for miscellaneous characters; it can vary by keyboard.
        KEY_OEM_8 = 0xDF,

        // KEY_OEM_102 (E2) Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
        KEY_OEM_102 = 0xE2,

        // KEY_PROCESSKEY (E5) Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key
        KEY_PROCESSKEY = 0xE5,

        // KEY_PACKET (E7) Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes. The KEY_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT,SendInput, WM_KEYDOWN, and WM_KEYUP
        KEY_PACKET = 0xE7,

        // KEY_ATTN (F6) Attn key
        KEY_ATTN = 0xF6,

        // KEY_CRSEL (F7) CrSel key
        KEY_CRSEL = 0xF7,

        // KEY_EXSEL (F8) ExSel key
        KEY_EXSEL = 0xF8,

        // KEY_EREOF (F9) Erase EOF key
        KEY_EREOF = 0xF9,

        // KEY_PLAY (FA) Play key
        KEY_PLAY = 0xFA,

        // KEY_ZOOM (FB) Zoom key
        KEY_ZOOM = 0xFB,

        // KEY_NONAME (FC) Reserved for future use
        KEY_NONAME = 0xFC,

        // KEY_PA1 (FD) PA1 key
        KEY_PA1 = 0xFD,

        // KEY_OEM_CLEAR (FE) Clear key
        KEY_OEM_CLEAR = 0xFE,

        // The number of keys
        KEY_NUM_KEYS = KEY_OEM_CLEAR,

        // KEY_UNKNOWN default
        KEY_UNKNOWN = 0
    };
}