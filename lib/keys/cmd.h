#ifndef CMD_H__
#define CMD_H__

#define WRITE_REPORT_ID 32
#define READ_REPORT_ID 31
/* 以下为主机发给设备的命令 */
#define GET_VERSION 1
#define GET_STORAGE 2
#define RUN_SCRIPT 5     //RUN_SCRIPT cmd(2)
#define GET_FLASH 6      //GET_FLASH 起始地址(4) 长度(1)
#define SET_FLASH 7      //SET_FLASH 起始地址(4) 数据...
#define GET_ID 12        //GET_ID
#define MOVEEVENT        16//MOVEEVENT type(1) data(4)
                           //type:1=leftdown 2=leftup 3=rightdown 4=rightup 5=middledown 6=middleup 7=mouseallup 8=moveto 9=mover 10=wheel
#define KEYEVENT         17//KEYEVENT type(1) data(1)
                           //type:1=keydown 2=keyup 3=keyallup
#define REBOOT            20//REBOOT
#define SET_ENCRYPT       22//SET_ENCRYPT dat(11)
#define CLEAR_ENCRYPT	  23//CLEAR_ENCRYPT
#define SET_LED     	  24//SET_LED on(1)
#define LOCK_MOUSE		  25//LOCK_MOUSE option(1):0=none 1=Left_button 2=right_button 4=wheel_button 8=x 16=y 32=wheel all=63
	#define LOCK_NONE         0x00
	#define LOCK_LEFT_BUTTON  0x01
	#define LOCK_RIGHT_BUTTON 0x02
	#define LOCK_WHEEL_BUTTON 0x04
	#define LOCK_ALL_BUTTON   0x07
	#define LOCK_X			  0x08
	#define LOCK_Y			  0x10
	#define LOCK_X_Y		  0x18
	#define LOCK_WHEEL		  0x20
	#define LOCK_ALL		  0x3F
#define ENABLE_MOUSE_NOTIFY	  26//ENABLE_MOUSE_NOTIFY option(1):0=none 1=Left_button 2=right_button 4=wheel_button 8=x 16=y 32=wheel all=63
	#define NOTIFY_NONE         0x00
	#define NOTIFY_LEFT_BUTTON  0x01
	#define NOTIFY_RIGHT_BUTTON 0x02
	#define NOTIFY_WHEEL_BUTTON 0x04
	#define NOTIFY_ALL_BUTTON   0x07
	#define NOTIFY_X			0x08
	#define NOTIFY_Y			0x10
	#define NOTIFY_X_Y		  	0x18
	#define NOTIFY_WHEEL		0x20
	#define NOTIFY_ALL		  	0x3F
#define SET_FREQ		     28//SET_FREQ time(1)
#define GET_FREQ		     29//GET_FREQ
#define LOCK_KEYBOARD   	 31//LOCK_KEYBOARD option(1)
#define ENABLE_KEYBOARD_NOTIFY  32//ENABLE_KEYBOARD_NOTIFY option(1)
#define GET_KEY_STATE		33//GET_KEY_STATE keycode(1)
#define SET_SYNCMODE		 34//SET_SYNCMODE on(1)
	
/* 以下为设备发给主机的命令 */
#define RESPON_VERSION 1    //RESPON_VERSION MODEL(1) 版本(1)
#define RESPON_STORAGE 2    //RESPON_STORAGE 容量(4)
#define RESPON_GET_FLASH  6 //RESPON_GET_FLASH 数据...
#define RESPON_SET_FLASH  7 //RESPON_SET_FLASH 结果(1) 1=失败 0=成功
#define RESPON_GETID      9 //RESPON_GETID id(4)
#define RESPON_EVENT      20//RESPON_EVENT
#define RESPON_OK           39//RESPON_OK
#define NOTIFY_MOUSE		43//NOTIFY_MOUSE type(1) date(4) type与ENABLE_MOUSE_NOTIFY参数相同
#define RESPON_FREQ		  44//RESPON_FREQ time(1)
#define RESPON_KEYSTATE   49//RESPON_KEYSTATE keycode(1) state(1)

#endif
