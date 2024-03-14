#include "stdafx.h"
#include "key.h"

#pragma warning(disable : 4786)

#include <algorithm>    // transform
#include <map>
#include <string>

std::map<std::string,BYTE> keymap;
BYTE getkeycode(std::string keyname)
{
	if(keymap.size()==0){
		keymap["enter"]=KEY_ENTER;
		keymap["\n"]=KEY_ENTER;
		keymap["\r"]=KEY_ENTER;
		keymap["esc"]=KEY_ESC;
		keymap["`"]=KEY_TILDE;
		keymap["backspace"]=KEY_BACKSPACE;
		keymap["back space"]=KEY_BACKSPACE;
		keymap["tab"]=KEY_TAB;
		keymap["\t"]=KEY_TAB;
		keymap[" "]=KEY_SPACE;
		keymap["space"]=KEY_SPACE;
		keymap["-"]=KEY_MINUS;
		keymap["="]=KEY_EQUAL;
		keymap["["]=KEY_LEFT_BRACE;
		keymap["]"]=KEY_RIGHT_BRACE;
		keymap["\\"]=KEY_BACKSLASH;
		keymap[";"]=KEY_SEMICOLON;
		keymap["'"]=KEY_QUOTE;
		keymap[","]=KEY_COMMA;
		keymap["."]=KEY_PERIOD;
		keymap["/"]=KEY_SLASH;
		keymap["capslock"]=KEY_CAPS_LOCK;
		keymap["caps lock"]=KEY_CAPS_LOCK;
		keymap["print screen"]=KEY_PRINTSCREEN;
		keymap["scrolllock"]=KEY_SCROLL_LOCK;
		keymap["scroll lock"]=KEY_SCROLL_LOCK;
		keymap["scroll"]=KEY_SCROLL_LOCK;
		keymap["pause"]=KEY_PAUSE;
		keymap["break"]=KEY_PAUSE;
		keymap["pause/break"]=KEY_PAUSE;
		keymap["insert"]=KEY_INSERT;
		keymap["home"]=KEY_HOME;
		keymap["pageup"]=KEY_PAGE_UP;
		keymap["page up"]=KEY_PAGE_UP;
		keymap["delete"]=KEY_DELETE;
		keymap["del"]=KEY_DELETE;
		keymap["end"]=KEY_END;
		keymap["pagedown"]=KEY_PAGE_DOWN;
		keymap["page down"]=KEY_PAGE_DOWN;
		keymap["right"]=KEY_RIGHT_ARROW;
		keymap["left"]=KEY_LEFT_ARROW;
		keymap["down"]=KEY_DOWN_ARROW;
		keymap["up"]=KEY_UP_ARROW;
		keymap["numlock"]=KEY_NUM_LOCK;
		keymap["num lock"]=KEY_NUM_LOCK;
		keymap["小键盘/"]=KEYPAD_DIVIDE;
		keymap["num/"]=KEYPAD_DIVIDE;
		keymap["num /"]=KEYPAD_DIVIDE;
		keymap["小键盘*"]=KEYPAD_MULTIPLY;
		keymap["num*"]=KEYPAD_MULTIPLY;
		keymap["num *"]=KEYPAD_MULTIPLY;
		keymap["小键盘-"]=KEYPAD_SUBTRACT;
		keymap["num-"]=KEYPAD_SUBTRACT;
		keymap["num -"]=KEYPAD_SUBTRACT;
		keymap["小键盘+"]=KEYPAD_ADD;
		keymap["num+"]=KEYPAD_ADD;
		keymap["num +"]=KEYPAD_ADD;
		keymap["小键盘enter"]=KEYPAD_ENTER;
		keymap["numenter"]=KEYPAD_ENTER;
		keymap["num enter"]=KEYPAD_ENTER;
		keymap["小键盘1"]=KEYPAD_1;
		keymap["num1"]=KEYPAD_1;
		keymap["num 1"]=KEYPAD_1;
		keymap["小键盘2"]=KEYPAD_2;
		keymap["num2"]=KEYPAD_2;
		keymap["num 2"]=KEYPAD_2;
		keymap["小键盘3"]=KEYPAD_3;
		keymap["num3"]=KEYPAD_3;
		keymap["num 3"]=KEYPAD_3;
		keymap["小键盘4"]=KEYPAD_4;
		keymap["num4"]=KEYPAD_4;
		keymap["num 4"]=KEYPAD_4;
		keymap["小键盘5"]=KEYPAD_5;
		keymap["num5"]=KEYPAD_5;
		keymap["num 5"]=KEYPAD_5;
		keymap["小键盘6"]=KEYPAD_6;
		keymap["num6"]=KEYPAD_6;
		keymap["num 6"]=KEYPAD_6;
		keymap["小键盘7"]=KEYPAD_7;
		keymap["num7"]=KEYPAD_7;
		keymap["num 7"]=KEYPAD_7;
		keymap["小键盘8"]=KEYPAD_8;
		keymap["num8"]=KEYPAD_8;
		keymap["num 8"]=KEYPAD_8;
		keymap["小键盘9"]=KEYPAD_9;
		keymap["num9"]=KEYPAD_9;
		keymap["num 9"]=KEYPAD_9;
		keymap["小键盘0"]=KEYPAD_0;
		keymap["num0"]=KEYPAD_0;
		keymap["num 0"]=KEYPAD_0;
		keymap["小键盘."]=KEYPAD_DOT;
		keymap["num."]=KEYPAD_DOT;
		keymap["num ."]=KEYPAD_DOT;
		keymap["小键盘="]=KEY_PAD_EQUALS;
		keymap["num="]=KEY_PAD_EQUALS;
		keymap["num ="]=KEY_PAD_EQUALS;
		keymap["poweroff"]=KEY_POWER;
		keymap["power"]=KEY_POWER;
		keymap["静音"]=KEY_MUTE;
		keymap["音量加"]=KEY_VOLUME_UP;
		keymap["volume+"]=KEY_VOLUME_UP;
		keymap["音量减"]=KEY_VOLUME_DOWN;
		keymap["volume-"]=KEY_VOLUME_DOWN;
		keymap["lctrl"]=KEY_LEFT_CTRL;
		keymap["lshift"]=KEY_LEFT_SHIFT;
		keymap["lalt"]=KEY_LEFT_ALT;
		keymap["lwin"]=KEY_LEFT_WINDOWS;
		keymap["rctrl"]=KEY_RIGHT_CTRL;
		keymap["rshift"]=KEY_RIGHT_SHIFT;
		keymap["ralt"]=KEY_RIGHT_ALT;
		keymap["rwin"]=KEY_RIGHT_WINDOWS;
		keymap["ctrl"]=KEY_LEFT_CTRL;
		keymap["shift"]=KEY_LEFT_SHIFT;
		keymap["alt"]=KEY_LEFT_ALT;
		keymap["win"]=KEY_LEFT_WINDOWS;
		keymap["menu"]=KEY_APPLICATION;
	}
	std::transform(keyname.begin(), keyname.end(), keyname.begin(), ::tolower);

	std::map<std::string,BYTE>::iterator iter=keymap.find(keyname);
	if(iter!=keymap.end()){
		return iter->second;
	}
	if(keyname.length()==1){
		char k=keyname[0];
		if(k>='a'&&k<='z'){
			return k-'a'+KEY_A;
		}
		if(k>='1'&&k<='9'){
			return k-'1'+KEY_1;
		}
		if(k=='0') return KEY_0;
	}else if(keyname.length()>=2&&keyname.length()<=3&&keyname[0]=='f'){
		return atoi(keyname.substr(1).c_str())-1+KEY_F1;
	}
	return 0;
}
struct KEYMAP{
	int keycode;
	char keyname[32];
}keys[]={
	{VK_BACK,"Backspace"},
	{VK_TAB,"Tab"},
	{VK_CLEAR,"Clear"},
	{VK_RETURN,"Enter"},
	{VK_SHIFT,"Shift"},
	{VK_LSHIFT,"LShift"},
	{VK_RSHIFT,"RShift"},
	{VK_CONTROL,"Ctrl"},
	{VK_LCONTROL,"LCtrl"},
	{VK_RCONTROL,"RCtrl"},
	{VK_MENU,"Alt"},
	{VK_LMENU,"LAlt"},
	{VK_RMENU,"RAlt"},
	{VK_PAUSE,"Pause"},
	{VK_CAPITAL,"CapsLock"},
	{VK_ESCAPE,"Esc"},
	{VK_SPACE,"Space"},
	{VK_PRIOR,"PageUp"},
	{VK_NEXT,"PageDown"},
	{VK_END,"End"},
	{VK_HOME,"Home"},
	{VK_LEFT,"Left"},
	{VK_UP,"Up"},
	{VK_RIGHT,"Right"},
	{VK_DOWN,"Down"},
	{VK_SELECT,"Select"},
	{VK_PRINT,"Print"},
	{VK_EXECUTE,"Execute"},
	{VK_SNAPSHOT,"PrintScreen"},
	{VK_INSERT,"Insert"},
	{VK_DELETE,"Delete"},
	{VK_HELP,"Help"},
	{VK_LWIN,"LWin"},
	{VK_RWIN,"RWin"},
	{'0',"0"},
	{'1',"1"},
	{'2',"2"},
	{'3',"3"},
	{'4',"4"},
	{'5',"5"},
	{'6',"6"},
	{'7',"7"},
	{'8',"8"},
	{'9',"9"},
	{'A',"A"},
	{'B',"B"},
	{'C',"C"},
	{'D',"D"},
	{'E',"E"},
	{'F',"F"},
	{'G',"G"},
	{'H',"H"},
	{'I',"I"},
	{'J',"J"},
	{'K',"K"},
	{'L',"L"},
	{'M',"M"},
	{'N',"N"},
	{'O',"O"},
	{'P',"P"},
	{'Q',"Q"},
	{'R',"R"},
	{'S',"S"},
	{'T',"T"},
	{'U',"U"},
	{'V',"V"},
	{'W',"W"},
	{'X',"X"},
	{'Y',"Y"},
	{'Z',"Z"},
	{VK_APPS,"Menu"},
	{VK_NUMPAD0,"小键盘0"},
	{VK_NUMPAD1,"小键盘1"},
	{VK_NUMPAD2,"小键盘2"},
	{VK_NUMPAD3,"小键盘3"},
	{VK_NUMPAD4,"小键盘4"},
	{VK_NUMPAD5,"小键盘5"},
	{VK_NUMPAD6,"小键盘6"},
	{VK_NUMPAD7,"小键盘7"},
	{VK_NUMPAD8,"小键盘8"},
	{VK_NUMPAD9,"小键盘9"},
	{VK_MULTIPLY,"小键盘*"},
	{VK_ADD,"小键盘+"},
	{VK_SEPARATOR,"小键盘Enter"},
	{VK_SUBTRACT,"小键盘-"},
	{VK_DECIMAL,"小键盘."},
	{VK_DIVIDE,"小键盘/"},
	{VK_F1,"F1"},
	{VK_F2,"F2"},
	{VK_F3,"F3"},
	{VK_F4,"F4"},
	{VK_F5,"F5"},
	{VK_F6,"F6"},
	{VK_F7,"F7"},
	{VK_F8,"F8"},
	{VK_F9,"F9"},
	{VK_F10,"F10"},
	{VK_F11,"F11"},
	{VK_F12,"F12"},
	{VK_F13,"F13"},
	{VK_F14,"F14"},
	{VK_F15,"F15"},
	{VK_F16,"F16"},
	{VK_F17,"F17"},
	{VK_F18,"F18"},
	{VK_F19,"F19"},
	{VK_F20,"F20"},
	{VK_F21,"F21"},
	{VK_F22,"F22"},
	{VK_F23,"F23"},
	{VK_F24,"F24"},
	{VK_NUMLOCK,"NumLock"},
	{VK_SCROLL,"Scroll"},
	{186,";"},
	{187,"="},
	{188,","},
	{189,"-"},
	{190,"."},
	{191,"/"},
	{192,"`"},
	{219,"["},
	{220,"\\"},
	{221,"]"},
	{222,"'"}
};
BYTE getKeyCodeByVirtualCode(int vitual_keycode)
{
	static int MAX_SIZE=0;
	if(MAX_SIZE==0) {
		MAX_SIZE=sizeof(keys)/sizeof(KEYMAP);
	}
	for(int i=0;i<MAX_SIZE;++i){
		if(vitual_keycode==keys[i].keycode){
			return getkeycode(keys[i].keyname);
		}
	}
	return 0;
}
