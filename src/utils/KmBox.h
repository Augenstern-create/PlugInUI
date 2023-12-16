#include <iostream>
#include <windows.h>
#include <string>

class SerialPort {
public:
    SerialPort(int port, int baudRate) : _port(port), _baudRate(baudRate), _handle(INVALID_HANDLE_VALUE) {
        init();
    }

    ~SerialPort() {
        close();
    }

    bool open() {
        if (isOpened()) {
            close();
        }

        setComPortName();

        _handle = CreateFile(_comPortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (_handle == INVALID_HANDLE_VALUE) {
            return false;
        }

        if (!setupPort()) {
            close();
            return false;
        }

        if (!setBaudRate()) {
            close();
            return false;
        }

        return true;
    }

    bool isOpened() const {
        return _handle != INVALID_HANDLE_VALUE;
    }

    void close() {
        if (isOpened()) {
            CloseHandle(_handle);
            _handle = INVALID_HANDLE_VALUE;
        }
    }

    bool write(const std::string& data) {
        if (!isOpened() || data.empty()) {
            return false;
        }

        DWORD bytesWritten;
        return WriteFile(_handle, data.c_str(), static_cast<DWORD>(data.size()), &bytesWritten, NULL) != 0;
    }

    bool read(std::string& buffer, int bufferSize) {
        if (!isOpened() || bufferSize <= 0) {
            return false;
        }

        buffer.resize(bufferSize);
        DWORD bytesRead;
        if (ReadFile(_handle, &buffer[0], static_cast<DWORD>(bufferSize), &bytesRead, NULL)) {
            buffer.resize(bytesRead);
            return true;
        }

        return false;
    }

private:
    int _port;
    int _baudRate;
    std::string _comPortName;
    HANDLE _handle;

    void init() {
        _handle = INVALID_HANDLE_VALUE;
    }

    void setComPortName() {
        char portStr[12];
        _comPortName = "\\\\.\\COM";
        _itoa_s(_port, portStr, 10);
        _comPortName += portStr;
    }

    bool setupPort() {
        if (!isOpened()) {
            return false;
        }

        DCB dcbSerialParams = { 0 };
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        if (!GetCommState(_handle, &dcbSerialParams)) {
            return false;
        }

        dcbSerialParams.BaudRate = _baudRate;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.Parity = NOPARITY;
        dcbSerialParams.StopBits = ONESTOPBIT;

        return SetCommState(_handle, &dcbSerialParams) == TRUE;
    }

    bool setBaudRate() {
        if (!isOpened()) {
            return false;
        }

        COMMTIMEOUTS timeouts = { 0 };
        timeouts.ReadIntervalTimeout = MAXDWORD;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.ReadTotalTimeoutConstant = 0;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 2000;

        return SetCommTimeouts(_handle, &timeouts) == TRUE;
    }
};

class KmBox
{
public:
    static BOOL Init();
    static BOOL Move(float x, float y);
    static void Close();
};