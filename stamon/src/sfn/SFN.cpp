/*
	Name: SFN.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 20/02/24 15:39
	Description: Stamon For Native，即本地库的实现
*/

#pragma once

#include "ObjectManager.cpp"
#include "Exception.cpp"
#include "DataType.cpp"
#include "String.cpp"
#include "stmlib.cpp"
#include "StringMap.cpp"
#include "StamonConfig.cpp"

#define SFN_PARA_LIST \
	stamon::sfn::SFN &sfn, stamon::datatype::Variable *arg, \
			stamon::vm::ObjectManager *manager

// 用这个宏（SFN Parameter List）可以快速声明SFN函数的参数列表

namespace stamon::sfn {
constexpr int STAMON_SFN_FUNCTIONS_MAX = 65536;
// SFN的库函数个数最大值
class SFN;
} // namespace stamon::sfn

StamonString DataType2String(STMException *ex, stamon::datatype::DataType *dt);

void sfn_print(SFN_PARA_LIST);

void sfn_int(SFN_PARA_LIST);
void sfn_str(SFN_PARA_LIST);
void sfn_len(SFN_PARA_LIST);

void sfn_typeof(SFN_PARA_LIST);
void sfn_throw(SFN_PARA_LIST);
void sfn_version(SFN_PARA_LIST);

namespace stamon::sfn {
class SFN {
	StringMap<void(SFN_PARA_LIST)> sfn_functions;
	// 定义一个函数指针map
public:
	STMException *ex;

	vm::ObjectManager *manager;

	SFN() {
	}

	SFN(STMException *e, vm::ObjectManager *objman) {
		ex = e;

		manager = objman;

		// 在这里将库函数按接口号填入
		sfn_functions.put(StamonString("print"), sfn_print);
		sfn_functions.put(StamonString("int"), sfn_int);
		sfn_functions.put(StamonString("str"), sfn_str);
		sfn_functions.put(StamonString("len"), sfn_len);
		sfn_functions.put(StamonString("typeof"), sfn_typeof);
		sfn_functions.put(StamonString("throw"), sfn_throw);
		sfn_functions.put(StamonString("version"), sfn_version);
	}
	void call(StamonString port, datatype::Variable *arg) {
		if (sfn_functions.containsKey(port) == 0) {
			THROW("undefined sfn port")
		} else {
			sfn_functions.get(port)(*this, arg, manager);
		}
		CATCH {
			THROW_S(StamonString((char *) "SFN Error: ") + ex->getError())
		}
	}
};
} // namespace stamon::sfn

StamonString DataType2String(STMException *ex, stamon::datatype::DataType *dt) {
	switch (dt->getType()) {
	case stamon::datatype::IntegerTypeID:
		return toString(((stamon::datatype::IntegerType *) dt)->getVal());

	case stamon::datatype::FloatTypeID:
		return toString(((stamon::datatype::FloatType *) dt)->getVal());

	case stamon::datatype::DoubleTypeID:
		return toString(((stamon::datatype::DoubleType *) dt)->getVal());

	case stamon::datatype::StringTypeID:
		return StamonString((char *) "\"") + ((stamon::datatype::StringType *) dt)->getVal()
			 + StamonString((char *) "\"");

	case stamon::datatype::NullTypeID:
		return StamonString((char *) "null");

	case stamon::datatype::SequenceTypeID: {
		StamonString rst((char *) "{ ");

		ArrayList<stamon::datatype::Variable *> list;

		list = ((stamon::datatype::SequenceType *) dt)->getVal();

		for (int i = 0, len = list.size(); i < len; i++) {
			rst = rst + DataType2String(ex, list[i]->data);

			if (i != len - 1) {
				// 如果不是最后一个元素，那么就在元素末尾加逗号
				rst = rst + StamonString((char *) ", ");
			}
		}

		rst = rst + StamonString((char *) " }");

		return rst;
	}

	case stamon::datatype::ClassTypeID:
		return StamonString((char *) "<class>");

	case stamon::datatype::MethodTypeID:
		return StamonString((char *) "<function>");

	case stamon::datatype::ObjectTypeID:
		return StamonString((char *) "<object>");

	default: {
		THROW("unknown type")
		return StamonString((char *) "");
	}
	}
}

void sfn_print(SFN_PARA_LIST) {
	STMException *ex = sfn.ex;
	stamon::datatype::DataType *val = arg->data;
	if (val->getType() != stamon::datatype::StringTypeID) {
		THROW("bad type in print")
		return;
	}
	platform_print(((stamon::datatype::StringType *) (val))->getVal());
	return;
}

void sfn_int(SFN_PARA_LIST) {
	STMException *ex = sfn.ex;
	stamon::datatype::DataType *val = arg->data;

	if (val->getType() == stamon::datatype::IntegerTypeID) {
		return;

	} else if (val->getType() == stamon::datatype::FloatTypeID) {
		arg->data = manager->MallocObject<stamon::datatype::IntegerType>(
				(int) (((stamon::datatype::FloatType *) val)->getVal()));

	} else if (val->getType() == stamon::datatype::DoubleTypeID) {
		arg->data = manager->MallocObject<stamon::datatype::IntegerType>(
				(int) (((stamon::datatype::DoubleType *) val)->getVal()));

	} else if (val->getType() == stamon::datatype::StringTypeID) {
		StamonString src = ((stamon::datatype::StringType *) val)->getVal();
		int dst = 0;

		for (int i = 0; i < src.length(); i++) {
			if ('0' <= src[i] && src[i] <= '9') {
				dst *= 10;
				dst += src[i] - '0';
			} else {
				THROW("bad integer format");
				return;
			}
		}

		arg->data = manager->MallocObject<stamon::datatype::IntegerType>(dst);

	} else {
		THROW("bad type in\'int\'")
	}

	return;
}

void sfn_str(SFN_PARA_LIST) {
	STMException *ex = sfn.ex;
	stamon::datatype::DataType *val = arg->data;

	if (arg->data->getType() == stamon::datatype::StringTypeID) {
		return;
	}

	arg->data = manager->MallocObject<stamon::datatype::StringType>(
			DataType2String(ex, val)
	);

	return;
}

void sfn_len(SFN_PARA_LIST) {
	STMException *ex = sfn.ex;
	stamon::datatype::DataType *val = arg->data;

	if (val->getType() == stamon::datatype::SequenceTypeID) {
		arg->data = manager->MallocObject<stamon::datatype::IntegerType>(
				((stamon::datatype::SequenceType *) val)->getVal().size());

	} else if (val->getType() == stamon::datatype::StringTypeID) {
		arg->data = manager->MallocObject<stamon::datatype::IntegerType>(
				((stamon::datatype::StringType *) val)->getVal().length());

	} else {
		THROW("bad type in\'len\'")
	}

	return;
}

#define CHECK_DATA_TYPE_ID(type) \
	if (val->getType() == stamon::datatype::type##ID) { \
		rst = StamonString((char *) #type); \
	}

void sfn_typeof(SFN_PARA_LIST) {
	STMException *ex = sfn.ex;
	stamon::datatype::DataType *val = arg->data;
	StamonString rst;

	CHECK_DATA_TYPE_ID(NullType)
	CHECK_DATA_TYPE_ID(IntegerType)
	CHECK_DATA_TYPE_ID(FloatType)
	CHECK_DATA_TYPE_ID(DoubleType)
	CHECK_DATA_TYPE_ID(StringType)
	CHECK_DATA_TYPE_ID(SequenceType)
	CHECK_DATA_TYPE_ID(ClassType)
	CHECK_DATA_TYPE_ID(MethodType)
	CHECK_DATA_TYPE_ID(ObjectType)

	arg->data = manager->MallocObject<stamon::datatype::StringType>(rst);

	return;
}

void sfn_throw(SFN_PARA_LIST) {
	STMException *ex = sfn.ex;
	THROW_S(((stamon::datatype::StringType *) arg->data)->getVal())
	return;
}

void sfn_version(SFN_PARA_LIST) {
	arg->data = manager->MallocObject<stamon::datatype::StringType>(
			toString(stamon::STAMON_VER_X) + StamonString((char *) ".")
			+ toString(stamon::STAMON_VER_Y) + StamonString((char *) ".")
			+ toString(stamon::STAMON_VER_Z));
	return;
}