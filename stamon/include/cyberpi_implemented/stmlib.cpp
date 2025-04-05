/*
	Name: clib.c
	Copyright: Apache 2.0
	Author: CLimber-Rong, GusemFowage
	Date: 12/08/23 23:24
	Description: 一些杂糅的库定义
*/

// 这个库原本叫做clib.c
// 由于需要引入c++代码，所以后缀改.cpp，顺便把文件名改为stmlib

#pragma once

#include"cyberpi.h"
CyberPi cyber;

#include "String.cpp"
#include "stdlib.h"
#include "ctype.h"
#include "time.h"

/*由于代码经常涉及到基类转派生类，所以我编写了这个可以直接转换的宏*/
/*
 * 这个宏的用法是：
 * dst = cast_class(转换后的类型,需要转换的值)
 */
#define cast_class(T, value) (*(T *) (&(value)))
#ifndef NULL
#define NULL 0
#endif

void wait_a_b_button() {
	//等待用户按下AB按钮
	while(
		(cyber.get_button_a()||cyber.get_button_b())
		==false
	);
}

int get_random_num() {
	//生成0到65535的随机数，利用时钟
	clock_t tm = clock();
	int rst = tm+876345;	//随便加一个大数
	if(rst<0) {
		rst = -rst;
	}
	return rst%65536;
}

u_int8_t get_utf8_bytes(u_int8_t utf8)
{
	for(u_int8_t i = 0;i < 6;i++) {
		if((utf8 & (0x80>>i)) == 0) {
			return i == 0? 1:i;
		}
	}
 
	return 1;
}
 
u_int16_t utf8_to_utf16(u_int8_t* in, u_int16_t inLen, u_int16_t* out, u_int16_t outLen)
{
	u_int16_t length = 0;
	u_int8_t bytes = 0;
 
	for (u_int16_t i=0;i<inLen && length<outLen;i++,length++) {
		bytes = get_utf8_bytes(in[i]);
 
		if (bytes > 1) {
			out[length] = in[i] & (0xFF>>(bytes + 1));
			for (u_int8_t j = 1;j<bytes;j++) {
				i++;
				out[length] <<= 6;
				out[length] += in[i] & 0x3F;
			}
		} else {
			out[length] = in[i];
		}
	}
 
	out[length] = 0;
	return length;
}

wchar_t* String2Wchart(StamonString s) {
	//需要将char*的utf8格式转成wchar_t*的utf16be格式
	wchar_t* utf16 = (wchar_t*)calloc(sizeof(wchar_t), 1024);
	utf8_to_utf16((u_int8_t*)s.getstr(), s.length(), (u_int16_t*)utf16, 1024);
	return utf16;
}

int text_position = 0;

int platform_print(StamonString s) {

	wchar_t* print_s = String2Wchart(s);

	Bitmap* bm = cyber.create_text(print_s, 0xffff, uint8_t(25));

	if(text_position==2) {
		//从头到尾循环打印
		text_position = 0;
	}

	if(text_position==0) {
		//重新开始就要清理屏幕
		cyber.clean_lcd();
	}

	cyber.set_bitmap(5, 5+text_position*30, bm);

	text_position++;

	delete print_s;
	
	cyber.render_lcd();
	return 0;
}

template<class T>
StamonString toString(T&& t){
	return StamonString().toString(t);
}

#define MACRO_START do {

#define MACRO_END \
	} \
	while (0)

#define _PLATFORM_INPUT_BUFFER_SIZE 1024