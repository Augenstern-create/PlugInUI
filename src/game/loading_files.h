#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <stdexcept>
#include "iconv.h"
#include "utils/conversion.h"
#include "Data.h"
#include <sstream>
#include <iomanip>

std::string GB18030ToUTF8(std::string txt) {
    // const char *from_charset = "GB18030";
    const char *from_charset = "GB2312";
    const char *to_charset = "UTF-8";

    const char *inbuf = txt.c_str();
    size_t inbytesleft = strlen(inbuf);
    size_t outbytesleft = 2 * inbytesleft;
    char *outbuf = new char[outbytesleft];

    iconv_t cd = iconv_open(to_charset, from_charset);
    if (cd == (iconv_t)-1) {
        perror("iconv_open");
        delete[] outbuf;  // 在错误发生时释放内存
        return "";
    }

    char *inptr = (char *)txt.c_str();  // 直接使用 c_str() 获取字符串指针
    char *outptr = outbuf;

    if (iconv(cd, &inptr, &inbytesleft, &outptr, &outbytesleft) == (size_t)-1) {
        perror("iconv");
        delete[] outbuf;  // 在错误发生时释放内存
        return "";
    }

    if (iconv_close(cd) == -1) {
        perror("iconv_close");
        delete[] outbuf;  // 在错误发生时释放内存
        return "";
    }

    // 将 char* 转换为 std::string
    std::string converted_str(outbuf, outptr - outbuf);

    return converted_str;
}

bool isString(std::string txt1, std::string txt2) {
    auto size1 = txt1.size();
    auto size2 = txt2.size();
    if (size1 != size2) return false;
    std::stringstream hexStream1;
    std::stringstream hexStream2;
    for (size_t i = 0; i < size1; ++i) {
        hexStream1 << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(txt1[i]);
        hexStream2 << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(txt2[i]);
    }
    std::string hexString1 = hexStream1.str();
    std::string hexString2 = hexStream2.str();
    return hexString1 == hexString2;
}

void ExtractedContent(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::vector<std::pair<std::string, std::string>> constants;
    // std::string line((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::string line;
    std::string PracticalContent = std::string(".常量");
    std::cout << std::endl;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::string utf8Line = GB18030ToUTF8(line);
        if (isString(utf8Line.substr(0, 5), PracticalContent.substr(0, 5))) {
            size_t comma_pos = line.find(',');
            if (comma_pos != std::string::npos) {
                std::string field_name = line.substr(6, comma_pos - 6);
                if (isString(field_name, "")) continue;
                size_t last_comma_pos = line.find_last_of(',');
                std::string value = line.substr(last_comma_pos + 1);
                constants.push_back(std::make_pair(field_name, value));
            }
        }
    }
    // 打印提取的字段名和数值
    for (const auto &constant : constants) {
        std::cout << "Field Name: " << constant.first << ", Value: " << constant.second << std::endl;
    }
}

std::string CodeConversion(std::string path, std::string code) {
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        return "";
    }

    // 读取文件内容
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // 关闭文件
    file.close();
    iconv_t cd = iconv_open("UTF-8", "GB18030");
    if (cd == (iconv_t)(-1)) {
        return "";
    }

    char *inbuf = new char[content.size()];
    char *outbuf = new char[content.size() * 2];  // UTF-8最多为GB18030的2倍长度
    char *in = &content[0];
    char *out = outbuf;
    size_t inbytesleft = content.size();
    size_t outbytesleft = content.size() * 2;

    if (iconv(cd, &in, &inbytesleft, &out, &outbytesleft) == (size_t)(-1)) {
        iconv_close(cd);
        return "";
    }

    iconv_close(cd);

    std::string utf8_content(outbuf, out - outbuf);

    delete[] inbuf;
    delete[] outbuf;
    return utf8_content;
}

bool hexStringToUint32(const std::string &hexString, uint32_t *value) {
    std::string cleanString = hexString.substr(0, 2) == "0x" ? hexString.substr(2) : hexString;
    for (char c : cleanString) {
        if (!std::isxdigit(c)) {
            return false;
        }
    }
    unsigned long offsetLong;
    try {
        offsetLong = std::stoul(cleanString, nullptr, 16);
    } catch (const std::invalid_argument &e) {
        return false;
    } catch (const std::out_of_range &e) {
        return false;
    }

    const unsigned long maxUint32 = static_cast<unsigned long>(0xFFFFFFFF);  // uint32_t 的最大值
    if (offsetLong > maxUint32) {
        return false;
    }
    uint32_t offset = static_cast<uint32_t>(offsetLong);
    *value = offset;
    return true;
}
bool stringToHexUint32(const std::string &str, uint32_t *value) {
    size_t start = (str.size() >= 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) ? 2 : 0;
    *value = 0;
    for (size_t i = start; i < str.size(); ++i) {
        char c = std::tolower(str[i]);
        if (c == '\r') {
            break;
        }
        if (!std::isxdigit(c)) {
            return false;
        }
        *value = (*value << 4) + std::stoi(std::string(1, c), nullptr, 16);
    }
}

bool InitLoadingFile() {
    SetConsoleOutputCP(CP_UTF8);
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string CurrentDirectory = currentPath.string();
    std::string filename = CurrentDirectory + "\\offset\\28.2.1.91.txt";
    std::string code = "GB2312";
    std::string utf8_content = CodeConversion(filename, code);
    if (utf8_content == "") return false;

    std::string line;
    std::string PracticalContent = std::string(".常量");
    std::istringstream iss(utf8_content);
    std::vector<std::pair<std::string, uint32_t>> constants;
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        if (line.substr(0, 5) == PracticalContent.substr(0, 5)) {
            size_t comma_pos = line.find(',');
            if (comma_pos != std::string::npos) {
                std::string field_name = line.substr(8, comma_pos - 8);
                if (isString(field_name, "")) continue;
                size_t last_comma_pos = line.find_last_of(',');
                std::string strValue = "0x" + line.substr(last_comma_pos + 1);
                uint32_t offset = 0x0;
                if (stringToHexUint32(strValue, &offset)) {
                    constants.push_back(std::make_pair(field_name, offset));
                }
            }
        }
    }
    // 打印提取的字段名和数值
    for (const auto &constant : constants) {
        if (constant.first == "Shield指针") {
            gameData.Offset["Decrypt"] = constant.second;
        } else if (constant.first == "被瞄偏移") {
            gameData.Offset["AimOffsets"] = constant.second;
        } else {
            gameData.Offset[constant.first] = constant.second;
        }
    }
    // for (auto it : gameData.Offset) {  // 使用it作为迭代器的名字
    //     std::cout << "Name: " << it.first << " value: 0x" << std::hex << it.second << std::endl;
    // }
    return true;
}
