/*
	Name: FileMap.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 05/02/24 20:50
	Description: 以文件为键的map
*/

#pragma once

#include"stdlib.h"
#include"Exception.cpp"
#include"StringMap.cpp"
#include"LineReader.cpp"
#include"String.cpp"

class FileMap {
		StringMap<void> map;
	public:
		STMException* ex;

		FileMap() {}

		FileMap(STMException* e) {
			ex = e;
		}

		LineReader mark(const StamonString& filename) {
			/*
			 * 将该文件标记，并且返回打开该文件后的LineReader
			 * 如果该文件不存在或出错，抛出异常
			*/
			LineReader reader(filename, ex);

			map.put(filename, NULL);

			return reader;
		}

		bool exist(const StamonString& filename) {
			return map.containsKey(filename);
		}
};