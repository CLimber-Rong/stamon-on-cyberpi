/*
	Name: StringType.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 14/08/23 10:10
	Description: 字符串类型的定义
*/

#pragma once

#include"DataType.cpp"
#include"String.cpp"

namespace stamon::datatype {
	class StringType : public DataType {
			StamonString val;
		public:
			StringType(const StamonString& value) : DataType(StringTypeID) {
				val = value;
			}
			virtual StamonString getVal() const {
				return val;
			}
			virtual ~StringType() = default;
	};
} //namespace stamon::datatype