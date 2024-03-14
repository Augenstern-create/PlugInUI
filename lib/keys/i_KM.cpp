#include "stdafx.h"
#include "i_KM.h"
#include "cmd.h"
#include "key.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define aVendorID 0x1c1f
#define aProductID 0xc18a

void i_KM::make_encrtpy(BYTE* dat) {
    if (using_encrypt) {
        volatile int i;
        for (i = 0; i < 11; ++i) {
            dat[i] ^= key[i];
        }
    }
}
UINT32 MAKE32(BYTE* dat) { return (((UINT32) * (dat)) << 24) | (((UINT32) * (dat + 1)) << 16) | (((UINT32) * (dat + 2)) << 8) | *(dat + 3); }
BOOL i_KM::open_device(const char* com_name) {
    char name[256] = "\\\\.\\";
    strcat(name, com_name);
    handle = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) return FALSE;
    SetupComm(handle, 64, 64);
    COMMTIMEOUTS commTimeouts;
    commTimeouts.ReadIntervalTimeout = MAXDWORD;
    commTimeouts.ReadTotalTimeoutMultiplier = 0;
    commTimeouts.ReadTotalTimeoutConstant = 0;
    commTimeouts.WriteTotalTimeoutConstant = 1;
    commTimeouts.WriteTotalTimeoutMultiplier = 1;
    SetCommTimeouts(handle, &commTimeouts);

    DCB dcb;
    GetCommState(handle, &dcb);
    dcb.BaudRate = 460800;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    BOOL b = SetCommState(handle, &dcb);
    if (!b) {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        return FALSE;
    }

    return handle != INVALID_HANDLE_VALUE;
}

int i_KM::read_data_timeout(BYTE* buf, int timeout) {
    if (!handle) return -1;
    BYTE buff[64];
    DWORD read_len;
    int copy_len = 0, ret;
    DWORD begin_time = timeGetTime();
    do {
        ret = ReadFile(handle, buff, 1, &read_len, NULL);
        if (!ret) {
            if (handle) {
                CloseHandle(handle);
                handle = INVALID_HANDLE_VALUE;
            }
            return -1;
        }
        if (ret == 0 && timeout >= 0) {
            DWORD now = timeGetTime();
            if (now - begin_time >= static_cast<DWORD>(timeout)) {
                return -1;
            }
        }

    } while (read_len == 0);
    buf[0] = buff[0];
    int total_len = buf[0];
    copy_len = 1;
    if (total_len > 63) {
        return -1;
    }
    while (total_len > 0) {
        ret = ReadFile(handle, buff, total_len, &read_len, NULL);
        if (!ret) {
            if (handle) {
                CloseHandle(handle);
                handle = INVALID_HANDLE_VALUE;
            }
            return -1;
        }
        if (read_len > 0) {
            memcpy(buf + copy_len, buff, read_len);
            copy_len += read_len;
            total_len -= read_len;
        }
    }
    ret = ReadFile(handle, buff, 2, &read_len, NULL);
    if (buff[0] == '\r' && buff[1] == '\n') {
        make_encrtpy(buf + 1);
    }

    return copy_len;
}
int i_KM::write_data(BYTE* dat, DWORD length) {
    if (!handle) return -1;
    if (length > MAX_LENGTH) {
        return -2;
    }
    BYTE buf[64];
    memset(buf, 0xff, 64);
    memcpy(buf, dat, length);
    make_encrtpy(buf + 1);
    DWORD result_len;
    BOOL ret = WriteFile(handle, buf, 15, &result_len, NULL);
    if (!ret) {
        return -2;
    }
    return 1;
}
int i_KM::write_cmd(BYTE cmd, BYTE* dat, DWORD length) {
    if (length > MAX_LENGTH - 1) return -2;
    BYTE buf[63];
    buf[0] = (length + 1) & 0xff;
    buf[1] = cmd;
    if (length > 0) memcpy(buf + 2, dat, length);
    return write_data(buf, length + 2);
}

i_KM::i_KM() {
    using_encrypt = FALSE;
    wait_respon = FALSE;
    version = model = 0;
    memset(key, 0, sizeof(key));
    handle = INVALID_HANDLE_VALUE;
    MAX_LENGTH = 15;
    using_encrypt = FALSE;
    screenX = 1920;
    screenY = 1080;
}
i_KM::~i_KM() { Close(); }
void i_KM::Close(void) {
    if (handle != INVALID_HANDLE_VALUE) {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
    }
}
void i_KM::SetWaitRespon(BOOL wait) {
    wait_respon = wait;
    BYTE cmd[1];
    cmd[0] = wait;
    write_cmd(SET_SYNCMODE, cmd, 1);
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            break;
        }
        if (!handle) {
            break;
        }
    } while (true);
}
void i_KM::Reboot(void) {
    write_cmd(REBOOT, NULL, 0);
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            break;
        }
        if (!handle) {
            break;
        }
    } while (true);
}
BOOL i_KM::IsOpen(void) { return handle != INVALID_HANDLE_VALUE; }
BOOL i_KM::OpenDevice(const char* com_name) {
    BOOL ret = open_device(com_name);
    if (ret) {
        write_cmd(GET_VERSION, NULL, 0);
        do {
            BYTE buf[64];
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_VERSION) {
                    continue;
                }
                model = buf[2];
                version = buf[3];
                return TRUE;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
    return FALSE;
}

BYTE i_KM::GetVersion(void) {
    if (!handle)
        return 0;
    else {
        return version;
    }
    return 0;
}

BYTE i_KM::GetModel(void) {
    if (!handle)
        return 0;
    else {
        return model;
    }
    return 0;
}

LONG i_KM::GetChipID(void) {
    if (!handle)
        return 0;
    else {
        write_cmd(GET_ID, NULL, 0);
        BYTE buf[64];
        long long_result;
        do {
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_GETID) {
                    continue;
                }
                memcpy(&long_result, buf + 2, 4);
                long_result += 113666;
                return long_result;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
    return 0xffff;
}

LONG i_KM::GetStorageSize(void) {
    if (!handle) {
        return 0;
    } else {
        write_cmd(GET_STORAGE, NULL, 0);
        BYTE buf[64];
        long long_result;
        do {
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_STORAGE) {
                    continue;
                }
                long_result = MAKE32(buf + 2);
                long_result -= 10;
                return long_result;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
    return -1;
}

void i_KM::DelayRandom(int min, int max) {
    int delay = 0;
    if (max >= min && max > 0 && min >= 0) {
        delay = rand() % (max - min + 1) + min;
    } else if (max == 0 && min > 0) {
        delay = min;
    }
    if (delay > 0) {
        Sleep(delay);
    }
}
void i_KM::EnableAccurateDelay() { timeBeginPeriod(1); }

void i_KM::RunScript(BYTE mode, BYTE index) {
    BYTE cmd[2];
    cmd[0] = mode;
    cmd[1] = index;
    write_cmd(RUN_SCRIPT, cmd, 2);
}

void i_KM::mouse_event(BYTE e, SHORT x, SHORT y, BYTE extra1, BYTE extra2) {
    BYTE cmd[12];
    memset(cmd, 0, 12);
    cmd[0] = e;
    if (e >= 1 && e <= 7) {
        write_cmd(MOVEEVENT, cmd, 5);
    } else if (e == 8) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x >= screenX) x = screenX - 1;
        if (y >= screenY) y = screenY - 1;
        long ax = x, ay = y;

        ax = (ax << 15) / screenX;
        ay = (ay << 15) / screenY;
        cmd[1] = (BYTE)((ax & 0xff00) >> 8);
        cmd[2] = ax & 0xff;
        cmd[3] = (BYTE)((ay & 0xff00) >> 8);
        cmd[4] = ay & 0xff;
        write_cmd(MOVEEVENT, cmd, 5);
    } else if (e == 9) {
        if (x < -128 || x > 127 || y < -128 || y > 127) return;
        cmd[1] = (BYTE)x;
        cmd[2] = (BYTE)y;
        write_cmd(MOVEEVENT, cmd, 5);
    } else if (e == 91) {
        if (x < -32768 || x > 32767 || y < -32768 || y > 32767) return;
        cmd[1] = (BYTE)((x & 0xff00) >> 8);
        cmd[2] = x & 0xff;
        cmd[3] = (BYTE)((y & 0xff00) >> 8);
        cmd[4] = y & 0xff;
        write_cmd(MOVEEVENT, cmd, 5);
    } else if (e == 10) {
        if (x < -128 || x > 127) return;
        cmd[1] = (BYTE)x;
        write_cmd(MOVEEVENT, cmd, 5);
    } else if (e == 11) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;

        cmd[1] = (BYTE)((x & 0xff00) >> 8);
        cmd[2] = x & 0xff;
        cmd[3] = (BYTE)((y & 0xff00) >> 8);
        cmd[4] = y & 0xff;
        cmd[5] = (BYTE)((screenX & 0xff00) >> 8);
        cmd[6] = screenX & 0xff;
        cmd[7] = (BYTE)((screenY & 0xff00) >> 8);
        cmd[8] = screenY & 0xff;
        cmd[9] = extra1;
        cmd[10] = extra2;
        write_cmd(MOVEEVENT, cmd, 12);
    } else if (e == 12) {
        cmd[1] = (BYTE)((x & 0xff00) >> 8);
        cmd[2] = x & 0xff;
        cmd[3] = (BYTE)((y & 0xff00) >> 8);
        cmd[4] = y & 0xff;
        cmd[5] = (BYTE)((screenX & 0xff00) >> 8);
        cmd[6] = screenX & 0xff;
        cmd[7] = (BYTE)((screenY & 0xff00) >> 8);
        cmd[8] = screenY & 0xff;
        cmd[9] = extra1;
        cmd[10] = extra2;
        write_cmd(MOVEEVENT, cmd, 12);
    }
    if (wait_respon) {
        do {
            BYTE buf[64];
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_EVENT) {
                    continue;
                }
                break;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
}
void i_KM::LeftDown() { mouse_event(1, 0, 0); }
void i_KM::LeftUp() { mouse_event(2, 0, 0); }
void i_KM::LeftClick(int min, int max) {
    LeftDown();
    DelayRandom(min, max);
    LeftUp();
}
void i_KM::LeftDoubleClick(int min, int max) {
    LeftDown();
    DelayRandom(min, max);
    LeftUp();
    DelayRandom(min, max);
    LeftDown();
    DelayRandom(min, max);
    LeftUp();
}
void i_KM::RightDown() { mouse_event(3, 0, 0); }
void i_KM::RightUp() { mouse_event(4, 0, 0); }
void i_KM::RightClick(int min, int max) {
    RightDown();
    DelayRandom(min, max);
    RightUp();
}
void i_KM::MiddleDown() { mouse_event(5, 0, 0); }
void i_KM::MiddleUp() { mouse_event(6, 0, 0); }
void i_KM::MiddleClick(int min, int max) {
    MiddleDown();
    DelayRandom(min, max);
    MiddleUp();
}

void i_KM::MouseWheel(int delta) { mouse_event(10, delta, 0); }
void i_KM::MoveTo(USHORT x, USHORT y) { mouse_event(8, x, y); }
void i_KM::MoveR(short x, short y) { mouse_event(91, x, y); }
void i_KM::MoveD(USHORT x, USHORT y, BYTE delay, BYTE delta) { mouse_event(11, x, y, delay, delta); }
void i_KM::MoveRD(short dx, short dy, BYTE delay, BYTE delta) { mouse_event(12, dx, dy, delay, delta); }

void i_KM::MouseAllUp() { mouse_event(7, 0, 0); }

void i_KM::key_event(BYTE e, const char* key) {
    BYTE cmd[2];
    cmd[0] = e;
    if (e == 1 || e == 2) {
        cmd[1] = getkeycode(key);
        write_cmd(KEYEVENT, cmd, 2);
    } else if (e == 3) {
        cmd[1] = 0;
        write_cmd(KEYEVENT, cmd, 2);
    }
    if (wait_respon) {
        do {
            BYTE buf[64];
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_EVENT) {
                    continue;
                }
                break;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
}
void i_KM::KeyDownName(const char* keyname) { key_event(1, keyname); }
void i_KM::KeyUpName(const char* keyname) { key_event(2, keyname); }
void i_KM::KeyPressName(const char* keyname, int min, int max) {
    KeyDownName(keyname);
    DelayRandom(min, max);
    KeyUpName(keyname);
}
void i_KM::KeyDownCode(BYTE keycode) {
    BYTE cmd[2];
    cmd[0] = 1;
    cmd[1] = keycode;
    write_cmd(KEYEVENT, cmd, 2);
    if (wait_respon) {
        do {
            BYTE buf[64];
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_EVENT) {
                    continue;
                }
                break;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
}
void i_KM::KeyUpCode(BYTE keycode) {
    BYTE cmd[2];
    cmd[0] = 2;
    cmd[1] = keycode;
    write_cmd(KEYEVENT, cmd, 2);
    if (wait_respon) {
        do {
            BYTE buf[64];
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_EVENT) {
                    continue;
                }
                break;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
}
void i_KM::KeyPressCode(BYTE keycode, int min, int max) {
    KeyDownCode(keycode);
    DelayRandom(min, max);
    KeyUpCode(keycode);
}
void i_KM::KeyDownVirtualCode(BYTE keycode) {
    BYTE cmd[2];
    cmd[0] = 1;
    cmd[1] = getKeyCodeByVirtualCode(keycode);
    write_cmd(KEYEVENT, cmd, 2);
    if (wait_respon) {
        do {
            BYTE buf[64];
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_EVENT) {
                    continue;
                }
                break;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
}
void i_KM::KeyUpVirtualCode(BYTE keycode) {
    BYTE cmd[2];
    cmd[0] = 2;
    cmd[1] = getKeyCodeByVirtualCode(keycode);
    write_cmd(KEYEVENT, cmd, 2);
    if (wait_respon) {
        do {
            BYTE buf[64];
            int len = read_data_timeout(buf, 100);
            if (len > 0) {
                if (buf[1] != RESPON_EVENT) {
                    continue;
                }
                break;
            } else if (len < 0) {
                break;
            }
        } while (true);
    }
}
void i_KM::KeyPressVirtualCode(BYTE vcode, int min, int max) {
    BYTE keycode = getKeyCodeByVirtualCode(vcode);
    KeyDownCode(keycode);
    DelayRandom(min, max);
    KeyUpCode(keycode);
}
const char k1[49] = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./ ";
const char k2[48] = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
BOOL CanInput(char c) {
    int i;
    for (i = 0; i <= 48; ++i) {
        if (k1[i] == c) {
            return TRUE;
        }
    }
    for (i = 0; i <= 47; ++i) {
        if (k2[i] == c) {
            return TRUE;
        }
    }
    return FALSE;
}
BOOL needShift(char c) {
    for (int i = 0; i <= 47; ++i) {
        if (k2[i] == c) {
            return TRUE;
        }
    }
    return FALSE;
}
char getUnShiftKey(char c) {
    for (int i = 0; i <= 47; ++i) {
        if (k2[i] == c) {
            return k1[i];
        }
    }
    return 0;
}
void i_KM::SayString(const char* s, int min, int max) {
    int len = strlen(s), i;
    BOOL shift = FALSE;
    if (min < 1) min = 1;
    for (i = 0; i < len; ++i) {
        char c = s[i];
        BOOL need_shift = needShift(c);
        std::string keyname;
        keyname = c;
        if (need_shift) {
            keyname = getUnShiftKey(c);
        }
        if (need_shift && !shift) {
            KeyDownCode(KEY_LEFT_SHIFT);
            DelayRandom(min, max);
            shift = TRUE;
        }

        KeyPressName(keyname.c_str(), min, max);
        DelayRandom(min, max);
        if (i == len - 1) {
            need_shift = FALSE;
        } else {
            need_shift = needShift(s[i + 1]);
        }

        if (!need_shift && shift) {
            KeyUpCode(KEY_LEFT_SHIFT);
            DelayRandom(min, max);
            shift = FALSE;
        }
    }
}
void i_KM::SayStringAnsi(const char* s, int min, int max) {
    int len = strlen(s);
    if (min < 1) min = 1;
    for (int i = 0; i < len;) {
        BYTE c = s[i];
        int code;
        if (c < 128) {
            code = c;
            i += 1;
        } else if (i < len - 1) {
            code = c;
            code = code * 256 + (BYTE)s[i + 1];
            i += 2;
        }
        char temp[15];
        int temp_len = sprintf(temp, "%d", code);
        KeyDownCode(KEY_LEFT_ALT);
        DelayRandom(min, max);
        for (int j = 0; j < temp_len; ++j) {
            BYTE c2 = temp[j] - '0';
            if (c2 == 0) c2 = 10;
            BYTE keycode = KEYPAD_1 + c2 - 1;
            KeyDownCode(keycode);
            DelayRandom(min, max);
            KeyUpCode(keycode);
            DelayRandom(min, max);
        }
        KeyUpCode(KEY_LEFT_ALT);
    }
}
int ConvertANSIToUnicode(const char* strANSI, WCHAR** p_utf8) {
    int nLen = ::MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, (LPCTSTR)strANSI, -1, NULL, 0);
    if (nLen > 0) {
        WCHAR* wszUTF_8 = new WCHAR[nLen + 1];
        memset(wszUTF_8, 0, nLen * 2 + 2);
        nLen = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strANSI, -1, wszUTF_8, nLen);
        *p_utf8 = new WCHAR[nLen + 1];
        memcpy(*p_utf8, wszUTF_8, sizeof(WCHAR) * (nLen + 1));
        delete[] wszUTF_8;
        return nLen;
    } else {
        *p_utf8 = NULL;
        return 0;
    }
}
void i_KM::SayStringUnicode(const char* s, int min, int max) {
    WCHAR* uni_str;
    if (min < 1) min = 1;
    int len = ConvertANSIToUnicode(s, &uni_str);
    for (int i = 0; i < len; ++i) {
        int code = (int)uni_str[i];
        char temp[15];
        int temp_len = sprintf(temp, "%d", code);
        KeyDownCode(KEY_LEFT_ALT);
        DelayRandom(min, max);
        for (int j = 0; j < temp_len; ++j) {
            BYTE c2 = temp[j] - '0';
            if (c2 == 0) c2 = 10;
            BYTE keycode = KEYPAD_1 + c2 - 1;
            KeyDownCode(keycode);
            DelayRandom(min, max);
            KeyUpCode(keycode);
            DelayRandom(min, max);
        }
        KeyUpCode(KEY_LEFT_ALT);
    }
}

BYTE i_KM::SetVidPid(USHORT vid, USHORT pid) {
    if (!handle) return 2;
    BYTE cmd[8];
    cmd[0] = 0;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = 4;
    cmd[4] = vid & 0xff;
    cmd[5] = (vid & 0xff00) >> 8;
    cmd[6] = pid & 0xff;
    cmd[7] = (pid & 0xff00) >> 8;
    if (write_cmd(SET_FLASH, cmd, 8) == -1) return 1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_SET_FLASH) {
                continue;
            }
            return 0;
        }
    } while (true);
    return 1;
}

BYTE i_KM::SetConfigData(int index, SHORT data) {
    if (!handle) return 2;
    if (index < 0 || index >= 252) return 2;
    BYTE cmd[6];
    int addr = index * 2 + 8;

    cmd[0] = (addr >> 24) & 0xff;
    cmd[1] = (addr >> 16) & 0xff;
    cmd[2] = (addr >> 8) & 0xff;
    cmd[3] = (addr >> 0) & 0xff;
    cmd[4] = (data & 0xff00) >> 8;
    cmd[5] = data & 0xff;
    if (write_cmd(SET_FLASH, cmd, 6) == -1) return 1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_SET_FLASH) {
                continue;
            }
            return 0;
        }
    } while (true);
    return 1;
}

SHORT i_KM::GetConfigData(int index) {
    if (!handle) return -1;
    if (index < 0 || index >= 252) return -2;
    BYTE cmd[5];
    int addr = index * 2 + 8;

    cmd[0] = (addr >> 24) & 0xff;
    cmd[1] = (addr >> 16) & 0xff;
    cmd[2] = (addr >> 8) & 0xff;
    cmd[3] = (addr >> 0) & 0xff;
    cmd[4] = 2;
    short short_result = 0x7ffd;
    if (write_cmd(GET_FLASH, cmd, 5) == -1) return -1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_GET_FLASH) {
                continue;
            }
            short_result = short(buf[2]) << 8 | buf[3];
            return short_result;
        }
    } while (true);
    return short_result;
}
BYTE i_KM::SetEncrypt(BYTE* dat_param) {
    if (!handle) return -1;
    if (dat_param) {
        if (write_cmd(SET_ENCRYPT, dat_param, 11) == -1) return -1;
        using_encrypt = TRUE;
        memcpy(key, dat_param, 11);
    } else {
        BYTE clear_dat[13];
        memset(clear_dat, 0, 13);
        if (write_cmd(CLEAR_ENCRYPT, clear_dat, 13) == -1) return -1;
        using_encrypt = FALSE;
    }
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            return 1;
        }
    } while (true);
    return 0;
}
BYTE i_KM::SetLed(BOOL on) {
    if (!handle) return -1;
    BYTE dat[1];
    dat[0] = on != 0;
    if (write_cmd(SET_LED, dat, 1) == -1) return -1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            return 1;
        }
    } while (true);
    return 0;
}
BYTE i_KM::Lock_Mouse(BYTE option) {
    if (!handle) return -1;
    BYTE dat[1];
    dat[0] = option;
    if (write_cmd(LOCK_MOUSE, dat, 1) == -1) return -1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            return 1;
        }
    } while (true);
    return 0;
}
BYTE i_KM::Notify_Mouse(BYTE option) {
    if (!handle) return -1;
    BYTE dat[1];
    dat[0] = option;
    if (write_cmd(ENABLE_MOUSE_NOTIFY, dat, 1) == -1) return -1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            return 1;
        }
    } while (true);
    return 0;
}
BYTE i_KM::Lock_KeyBoard(BYTE option) {
    if (!handle) return -1;
    BYTE dat[1];
    dat[0] = option;
    if (write_cmd(LOCK_KEYBOARD, dat, 1) == -1) return -1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            return 1;
        }
    } while (true);
    return 0;
}
BYTE i_KM::Notify_KeyBoard(BYTE option) {
    if (!handle) return -1;
    BYTE dat[1];
    dat[0] = option;
    if (write_cmd(ENABLE_KEYBOARD_NOTIFY, dat, 1) == -1) return -1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            return 1;
        }
    } while (true);
    return 0;
}
int i_KM::Read_Notify(BYTE* out, int timeout) {
    BYTE buf[64];
read_notify:
    int len = read_data_timeout(buf, timeout);
    if (len > 0) {
        if (buf[1] != NOTIFY_MOUSE) {
            goto read_notify;
        }
        memcpy(out, buf + 2, buf[0] - 1);
        return buf[0] - 1;
    }
    return 0;
}
int i_KM::GetKeyState(BYTE index) {
    BYTE buf[64];
    buf[0] = index;
    if (write_cmd(GET_KEY_STATE, buf, 1) == -1) return -1;
read_notify:
    int len = read_data_timeout(buf, -1);
    if (len > 0) {
        if (buf[1] != RESPON_KEYSTATE) {
            goto read_notify;
        }
        if (buf[2] != index) {
            goto read_notify;
        }
        return buf[3];
    }
    return 0;
}

void i_KM::Set_Freq(int time) {
    if (!handle) return;
    BYTE dat[1];
    dat[0] = time;
    if (write_cmd(SET_FREQ, dat, 1) == -1) return;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_OK) {
                continue;
            }
            return;
        }
    } while (true);
    return;
}
int i_KM::Get_Freq() {
    if (!handle) return -1;
    if (write_cmd(GET_FREQ, NULL, 0) == 0) return -1;
    BYTE buf[64];
    do {
        int len = read_data_timeout(buf, 100);
        if (len > 0) {
            if (buf[1] != RESPON_FREQ) {
                continue;
            }
            return buf[2];
        }
    } while (true);
    return -1;
}

void i_KM::SetScreenScale(USHORT width, USHORT height) {
    screenX = width;
    screenY = height;
}
