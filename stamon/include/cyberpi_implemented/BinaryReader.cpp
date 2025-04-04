/*
	Name: BinaryWriter.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#define FILE_ERR { THROW("file opening error") return; }

#include"Exception.cpp"
#include"String.cpp"
#include"stdio.h"
#include"BinarySTVC.cpp"

class BinaryReader {
	public:
		int size;
		STMException* ex;

		BinaryReader() {}
		BinaryReader(STMException* e, StamonString filename) {
			ex = e;
			size = binary_buffer_len;
		}

		char* read() {
			return binary_buffer_array;
        }

		void close() {
			;
		}
};

#undef FILE_ERROR