#include "conversion.h"

Text::Text(std::string path) : m_index(0) {
    std::filebuf *pbuf;
    std::ifstream filestr;
    filestr.open(path.c_str(), std::ios::binary);
    if (!filestr) {
        cerr << path << " Load text error." << endl;
        return;
    }
    pbuf = filestr.rdbuf();
    m_length = (int)pbuf->pubseekoff(0, std::ios::end, std::ios::in);
    pbuf->pubseekpos(0, std::ios::in);
    m_binaryStr = new char[m_length + 1];
    pbuf->sgetn(m_binaryStr, m_length);
    filestr.close();
}

void Text::SetIndex(size_t index) { m_index = index; }

size_t Text::Size() { return m_length; }

Text::~Text() { delete[] m_binaryStr; }

GbkText::GbkText(string path) : Text(path) {}
GbkText::~GbkText(void) {}
bool GbkText::ReadOneChar(string &oneChar) {
    // return true 表示读取成功，
    // return false 表示已经读取到流末尾
    if (m_length == m_index) return false;
    if ((unsigned char)m_binaryStr[m_index] < 0x81) {
        oneChar = m_binaryStr[m_index];
        m_index++;
    } else {
        oneChar = string(m_binaryStr, 2);
        m_index += 2;
    }
    return true;
}

UtfText::UtfText(string path) : Text(path) {}
UtfText::~UtfText(void) {}
bool UtfText::ReadOneChar(string &oneChar) {
    // return true 表示读取成功，
    // return false 表示已经读取到流末尾
    if (m_length == m_index) return false;
    size_t utf8_char_len = get_utf8_char_len(m_binaryStr[m_index]);
    if (0 == utf8_char_len) {
        oneChar = "";
        m_index++;
        return true;
    }
    size_t next_idx = m_index + utf8_char_len;
    if (m_length < next_idx) {
        // cerr << "Get utf8 first byte out of input src string." << endl;
        next_idx = m_length;
    }
    // 输出UTF-8的一个字符
    oneChar = string(m_binaryStr + m_index, next_idx - m_index);
    // 重置偏移量
    m_index = next_idx;
    return true;
}

size_t UtfText::get_utf8_char_len(const char &byte) {
    // return 0 表示错误
    // return 1-6 表示正确值
    // 不会 return 其他值

    // UTF8 编码格式：
    //      U-00000000 - U-0000007F: 0xxxxxxx
    //      U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
    //      U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
    //      U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    //      U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    //      U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

    size_t len = 0;
    unsigned char mask = 0x80;
    while (byte & mask) {
        len++;
        if (len > 6) {
            // cerr << "The mask get len is over 6." << endl;
            return 0;
        }
        mask >>= 1;
    }
    if (0 == len) {
        return 1;
    }
    return len;
}

Text *TextFactory::CreateText(string textCode, string path) {
    if ((textCode == "utf-8") || (textCode == "UTF-8") || (textCode == "ISO-8859-2") || (textCode == "ascii") || (textCode == "ASCII") ||
        (textCode == "TIS-620") || (textCode == "ISO-8859-5") || (textCode == "ISO-8859-7")) {
        return new UtfText(path);
    } else if ((textCode == "windows-1252") || (textCode == "Big5") || (textCode == "EUC-KR") || (textCode == "GB2312") || (textCode == "ISO-2022-CN") ||
               (textCode == "HZ-GB-2312") || (textCode == "GB18030")) {
        return new GbkText(path);
    }
    return NULL;
}
