#ifndef I_KM_H__
#define I_KM_H__

#include "cmd.h"

#ifndef LONG
typedef long int LONG;
#endif
#ifndef SHORT
typedef short SHORT;
#endif
#ifndef USHORT
typedef unsigned short USHORT;
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef BOOL
typedef int BOOL;
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

class i_KM
{
public:
	void SetScreenScale(USHORT width,USHORT height);
	i_KM();
	~i_KM();
	BOOL IsOpen(void);//设备是否连接
	BOOL OpenDevice(const char* com_name);//连接默认VID/PID的设备
	void SetWaitRespon(BOOL wait);//该函数决定发送鼠标键盘指令后是否等待，转接器不返回，应该不等待。键盘可选等待或者不等待，不等待时需保证两次调用间隔大于1毫秒

	BYTE GetVersion(void);//获取固件版本
	BYTE GetModel(void);//获取设备型号
	LONG GetChipID(void);//获取芯片序列号
	LONG GetStorageSize(void);//获取存储空间大小
	void Close(void);//关闭设备连接
	void Reboot(void);//重启设备

	void DelayRandom(int min,int max);//随机延时：本地功能
	void EnableAccurateDelay();//提高延时精确度
	
	void RunScript(BYTE mode,BYTE index);//精简版转接器不可用
	//mode=1 启动脚本，index等于脚本序号（0-99为基础宏，100-125是高级宏）
	//mode=2 特殊功能。index=0暂停 index=1继续 index=2重启 index=3停止 index=4暂停继续 index=5弹起并停止
	
	void mouse_event(BYTE e,SHORT x, SHORT y, BYTE extra1=0, BYTE extra2=0);
	void LeftDown();//左键按下
	void LeftUp();//左键弹起
	void LeftClick(int min=0,int max=0);//左键单击。min和max代表随机延时区间
	void LeftDoubleClick(int min=0,int max=0);//左键双击。min和max代表随机延时区间
	void RightDown();//右键按下
	void RightUp();//右键弹起
	void RightClick(int min=0,int max=0);//右键单击。min和max代表随机延时区间
	void MiddleDown();//中键按下
	void MiddleUp();//中键按下
	void MiddleClick(int min=0,int max=0);//中键单击。min和max代表随机延时区间
	void MouseWheel(int delta);//鼠标滚轮。delta需要-128到127之间
	void MouseAllUp();//弹起鼠标所有键
	void MoveTo(USHORT x,USHORT y);//鼠标绝对坐标移动
	void MoveR(short x,short y);//鼠标相对移动
	void MoveD(USHORT x,USHORT y,BYTE delay=8,BYTE delta=10);//鼠标带轨迹平滑移动
	void MoveRD(short dx,short dy,BYTE delay=8,BYTE delta=10);//鼠标带轨迹平滑相对移动

	void key_event(BYTE e,const char* key);
	void KeyDownName(const char* keyname);//以按键名按下键盘按键
	void KeyUpName(const char* keyname);//以按键名弹起键盘按键
	void KeyPressName(const char* keyname,int min=0,int max=0);//以按键名键盘按键
	void KeyDownCode(BYTE keycode);//以扫描码按下键盘按键
	void KeyUpCode(BYTE keycode);//以扫描码弹起键盘按键
	void KeyPressCode(BYTE keycode,int min=0,int max=0);//以扫描码键盘按键
	void KeyDownVirtualCode(BYTE keycode);//以虚拟键码按下键盘按键
	void KeyUpVirtualCode(BYTE keycode);//以虚拟键码弹起键盘按键
	void KeyPressVirtualCode(BYTE keycode,int min=0,int max=0);//以虚拟键码键盘按键
	void SayString(const char* s,int min=0,int max=0);//输入数字、字母、符号
	void SayStringAnsi(const char* s,int min=0,int max=0);//输入gbk编码的字符串
	void SayStringUnicode(const char* s,int min=0,int max=0);//输入unicode编码的字符串

	BYTE SetVidPid(USHORT vid, USHORT pid);//设置VID/PID
	

	//以下是转接器专用函数，键盘勿调用
	BYTE SetConfigData(int index, SHORT data);//设置配置区，序号0-251
	SHORT GetConfigData(int index);//获取配置区数据，序号0-251

	BYTE SetEncrypt(BYTE *dat);//数据必须等于11字节。如果dat等于NULL，清除加密
	BYTE SetLed(BOOL on);//打开、关闭灯

	/*	Lock_Mouse
	 *	锁定插入设备的鼠标的部分功能
	 *	option: LOCK_NONE         0x00
			 	LOCK_LEFT_BUTTON  0x01
				LOCK_RIGHT_BUTTON 0x02
				LOCK_WHEEL_BUTTON 0x04
				LOCK_ALL_BUTTON   0x07
				LOCK_X			  0x08
				LOCK_Y			  0x10
				LOCK_X_Y		  0x18
				LOCK_WHEEL		  0x20
				LOCK_ALL		  0x3F */
	BYTE Lock_Mouse(BYTE option);//转接器专用函数
	BYTE Lock_KeyBoard(BYTE option);//转接器专用函数

	/* Notify_Mouse
	 * 开启鼠标动作通知，鼠标对应动作时反馈给程序
	 * option:	NOTIFY_NONE         0x00
				NOTIFY_LEFT_BUTTON  0x01
				NOTIFY_RIGHT_BUTTON 0x02
				NOTIFY_WHEEL_BUTTON 0x04
				NOTIFY_ALL_BUTTON   0x07
				NOTIFY_X			0x08
				NOTIFY_Y			0x10
				NOTIFY_X_Y		  	0x18
				NOTIFY_WHEEL		0x20
				NOTIFY_ALL		  	0x3F */
	BYTE Notify_Mouse(BYTE option);//转接器专用函数
	BYTE Notify_KeyBoard(BYTE option);//转接器专用函数
	int Read_Notify(BYTE* out,int timeout);//转接器专用函数，读取Notify_Mouse反馈的数据。out数组需保证5字节大小。timeout=-1代表阻塞，其他代表最大等待时间
	int GetKeyState(BYTE index);//转接器专用函数，获取插在上面的按键状态，瞬时读取函数，使用时需要关闭Notify_Mouse和Notify_KeyBoard
	
	void Set_Freq(int percent);//设置dpi百分比
	int Get_Freq();//获取dpi百分比
protected:
	BOOL wait_respon;
	BYTE model;
	BYTE version;
	USHORT screenX,screenY;
private:
	HANDLE handle;
	DWORD MAX_LENGTH;
	BOOL using_encrypt;
	BYTE key[11];
protected:
	void make_encrtpy(BYTE* dat);
	BOOL open_device(const char* com_name);
	int read_data_timeout(BYTE* buf,int timeout);
	int write_data(BYTE *dat, DWORD length);
	int write_cmd(BYTE cmd, BYTE *dat, DWORD length);
};

#endif