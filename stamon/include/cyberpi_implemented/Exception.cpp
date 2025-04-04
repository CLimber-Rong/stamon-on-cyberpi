/*
	Name: Exception.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 24/08/23 18:09
	Description: 异常处理的实现
*/
#pragma once

#include"stmlib.cpp"
#include"String.cpp"
#include"ArrayList.cpp"
//该库不依赖平台，无需移植

char* ExceptionMessage;

/*
	CATCH的使用方法：
	CATCH {
		//在这里写上捕捉异常的代码
	}
	THROW(message)用于抛出异常，异常信息为message
	THROW_S的作用与THROW相近，通常用于抛出“运行时才能决定异常信息”的异常
	ERROR是异常信息
*/ 

/*
 * 这里了经过改装
 * 开发者只需要在当前代码所在的作用域中定义STMException* ex
 * 即可使用以下的宏
 * 之前的异常实现是全局性的
 * 但是考虑到今后要尝试多线程运行，所以我将异常实现变成面向对象的
*/

#define THROW(message) ex->Throw(StamonString((char*)message));
//THROW当中的message是char*
#define THROW_S(message_s) ex->Throw(message_s);
//THROW_S当中的message_s是String
#define CATCH if(ex->isError)
#define ERROR (ex->getError())

#define WARN(message) ex->Warn(StamonString(message));
#define WARN_S(message_s) ex->Warn(message_s);
#define CATCH_WARNING if(ex->isWarning)
#define IS_WARNING (ex->getError())
#define WARNING (ex->getWarning())

class STMException {
		StamonString ExceptionMessage;
		ArrayList<StamonString> WarningMessages;
	public:
		bool isError = false;
		bool isWarning = false;

		void Throw(StamonString msg) {
			ExceptionMessage = msg;
			isError = true;
		}

		void Warn(StamonString msg) {
			WarningMessages.add(msg);
			isWarning = true;
		}

		StamonString getError() {
			return ExceptionMessage;
		}

		ArrayList<StamonString> getWarning() {
			return WarningMessages;
		}

};