#include "KmBox.h"
#include <iostream>
#include <string>
#include <algorithm>

SerialPort* serial = nullptr;

BOOL Write(std::string sendData)
{
    return serial->write(sendData + "\r\n");
}

void forceCloseSerialPort(const char* portName) {
    HANDLE hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        std::cout << "Force closed serial port: " << portName << std::endl;
    }
}

std::string Read()
{
    std::string receivedData;
    serial->read(receivedData, 100);
    return receivedData;
}

BOOL KmBox::Init()
{
    /*const char* serialPortName = "COM3";
    forceCloseSerialPort(serialPortName);*/

    serial = new SerialPort(3, 115200);
    return serial->open();
}

void KmBox::Close()
{
    serial->close();
}

BOOL KmBox::Move(float x, float y)
{
    int screenWidth = 1728;
    int screenHeight = 1080;

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    if (serial->open()) {
        int deltaX = (int)(x - centerX);
        int deltaY = (int)(y - centerY);
        int stepSize = 100;   // 每次移动的步长
        int delayMs = 1;   // 每步之间的延迟（毫秒）

        int remainingX = deltaX;
        int remainingY = deltaY;

        while (remainingX != 0 || remainingY != 0) {
            int moveX = (remainingX > 0) ? ((remainingX < stepSize) ? remainingX : stepSize)
                : ((remainingX > -stepSize) ? remainingX : -stepSize);
            int moveY = (remainingY > 0) ? ((remainingY < stepSize) ? remainingY : stepSize)
                : ((remainingY > -stepSize) ? remainingY : -stepSize);

            //int moveX = (remainingX > 0) ? std::min(stepSize, remainingX) : std::max(-stepSize, remainingX);
            //int moveY = (remainingY > 0) ? std::min(stepSize, remainingY) : std::max(-stepSize, remainingY);

            // 使用 km.move() 函数模拟鼠标移动，将 moveX 和 moveY 传递给函数
            Write("km.move(" + std::to_string((int)moveX) + ", " + std::to_string((int)moveY) + ");");

            remainingX -= moveX;
            remainingY -= moveY;

            //// 添加延迟
            Sleep(delayMs);
        }


        /*std::string receivedData;
        Sleep(5);
        if (serial->read(receivedData, 100)) {
            std::cout << "kmbox -> PC: " << receivedData;
        }*/

        //Close();
        
    }
    else {
        std::cerr << "Failed to open serial port." << std::endl;
    }
    return true;
}