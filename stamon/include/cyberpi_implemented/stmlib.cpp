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

/*由于代码经常涉及到基类转派生类，所以我编写了这个可以直接转换的宏*/
/*
 * 这个宏的用法是：
 * dst = cast_class(转换后的类型,需要转换的值)
 */
#define cast_class(T, value) (*(T *) (&(value)))
#ifndef NULL
#define NULL 0
#endif

int platform_print_wchart(StamonString s) {
	Bitmap* bm = cyber.create_text((wchar_t*)(s.getstr()), 0xffff, uint8_t(15));
	cyber.set_bitmap(4, 4, bm);
	cyber.render_lcd();
	return 0;
}

int text_position = 0;

int platform_print(StamonString s) {
    wchar_t* print_s = new wchar_t[s.length()+1];
	print_s[s.length()] = L'\0';

    for(int i=0;i<s.length();i++) {
        print_s[i] = s[i];
    }
	Bitmap* bm = cyber.create_text(print_s, 0xffff, uint8_t(20));

	if(text_position==3) {
		//从头到尾循环打印
		text_position = 0;
	}

	if(text_position==0) {
		//重新开始就要清理屏幕
		cyber.clean_lcd();
	}

	cyber.set_bitmap(5, 5+text_position*25, bm);

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