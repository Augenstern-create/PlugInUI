#pragma once
#include <iostream>
#include <fstream>
#include "iconv.h"

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

void InitLoadingFile() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string CurrentDirectory = currentPath.string();
    std::string filename = CurrentDirectory + "\\offset\\28.1.2.3.txt";

    ExtractedContent(filename);

    // std::ifstream file(filename, std::ios::binary);
    // if (!file.is_open()) {
    //     std::cerr << "Error opening file: " << filename << std::endl;
    //     return;
    // }

    // std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // std::cout << content << std::endl;
    //     std::string encoding = isUTF8(filename);
    //     std::cout << "File encoding format is " << encoding << std::endl;
    //     readFile(filename, encoding);
}
