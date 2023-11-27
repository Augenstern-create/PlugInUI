
#include <iostream>
#include <fstream>
#include "curl/curl.h"
#include "version.h"
#include <string>
#include <filesystem>
#define _CRT_SECURE_NO_WARNINGS  // To disable deprecation warning for fopen_s
#include <cstdio>                // Include for fopen_s

// #include <zip.h>
// #include <unzip.h>

// const std::string serverURL = "http://103.214.174.42:8999/tiamo97/";

const std::string URL = "ftp://windows:GZXDaGPMPctaKKeZ@103.214.174.42:21";
const std::string usre_name = "windows";
const std::string usre_password = "GZXDaGPMPctaKKeZ";
const std::string updateFileName = "PlugInUI.exe";        // 替换为你的实际更新文件名
const std::string version_file_name = "remote_file.txt";  // 替换为你的实际更新文件名
std::filesystem::path currentPath;
std::string run_path;

bool UpdateProgram();
bool unzipFile(const std::string& zipFilePath, const std::string& destinationPath);

int main(int, char**) {
    if (UpdateProgram()) {
        return 0;
    }
    std::string newProgramPath = ".\\PlugInUI.exe";
    std::system(newProgramPath.c_str());
    return 0;
}

// 回调函数用于写入下载的数据
size_t writeCallback(void* contents, size_t size, size_t nmemb, FILE* file) {
    return fwrite(contents, size, nmemb, file);
}

// 函数：FTP下载
bool downloadFile(std::string ftp_url, std::string download_file) {
    CURL* curl;
    FILE* downloadFile;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // 设置下载文件的URL
        std::string downloadURL = ftp_url + "/" + download_file;
        curl_easy_setopt(curl, CURLOPT_URL, downloadURL.c_str());

        // 打开文件
        if (fopen_s(&downloadFile, download_file.c_str(), "wb+") != 0) {
            fprintf(stderr, "Failed to open download file for writing.\n");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return false;
        }

        // 设置回调函数，用于写入下载的数据到文件
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, downloadFile);
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // 执行FTP下载
        res = curl_easy_perform(curl);
        // 关闭文件
        fclose(downloadFile);

        // 检查是否有错误
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return false;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    // 下载成功
    return true;
}

// 函数：检查是否有更新
bool checkForUpdate() {
    // 在服务器上存放一个文本文件，记录最新的版本号
    std::string local_version = std::to_string(PROJECT_VERSION_MAJOR) + std::to_string(PROJECT_VERSION_MINOR) +
                                std::to_string(PROJECT_VERSION_PATCH);
    // 从服务器获取版本号
    std::string remoteVersionData;
    if (downloadFile(URL, version_file_name)) {
        std::ifstream remoteVersionFile(version_file_name);
        if (remoteVersionFile.is_open()) {
            remoteVersionFile >> remoteVersionData;
            remoteVersionFile.close();
        }
    }
    if (!remoteVersionData.empty() && !local_version.empty()) {
        int remoteVersion = std::stoi(remoteVersionData);
        int local_version_value = std::stoi(local_version);
        std::cout << "New edition :" << remoteVersion << " Old version :" << local_version_value << std::endl;
        // 检查是否有新版本
        return remoteVersion > local_version_value;
    }

    return false;
}

bool UpdateProgram() {
    currentPath = std::filesystem::current_path();
    run_path = currentPath.string();
    if (checkForUpdate()) {
        if (downloadFile(URL, updateFileName)) {
            // 退出当前版本的程序
            return true;
        }
    }
    return false;
}