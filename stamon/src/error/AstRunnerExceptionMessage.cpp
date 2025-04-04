/*
	Name: AstRunnerExceptionMessage.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 13:53
	Description: 此文件用于存放AstRunner的异常信息生成函数
*/

#pragma once

#include "AstRunner.cpp"
#include "Exception.cpp"
#include "AstIR.cpp"
#include "DataType.cpp"

namespace stamon::vm {

// 获取执行中的位置信息
inline StamonString stamon::vm::AstRunner::getExcutePosition() {
	if (RunningFileName.equals("") && RunningLineNo == -1) {
		// 字节码中被没有调试信息，即被字节码被strip过
		return StamonString("");
	} else {
		return StamonString("at \"") + RunningFileName + StamonString("\": ")
			 + toString(RunningLineNo) + StamonString(": ");
	}
}

// 类型错误
inline void AstRunner::ThrowTypeError(int type) {
	THROW_S(StamonString("Type Error: ") + getExcutePosition()
			+ StamonString("an error of data type \'") + getDataTypeName(type)
			+ StamonString("\' occurred in the calculation"))
}

// 未知的后缀
inline void AstRunner::ThrowPostfixError() {
	THROW_S(StamonString("ast::Postfix Error: ") + getExcutePosition()
			+ StamonString("unknown type of postfix"))
}

// 下标越界
inline void AstRunner::ThrowIndexError() {
	THROW_S(StamonString("Index Error: ") + getExcutePosition()
			+ StamonString("list index out of range"))
}

// 常量表下标错误
inline void AstRunner::ThrowConstantsError() {
	THROW_S(StamonString("Constants Error: ") + getExcutePosition()
			+ StamonString("wrong index of constants"))
}

// 除数为0
inline void AstRunner::ThrowDivZeroError() {
	THROW_S(StamonString("Zero Division Error: ") + getExcutePosition()
			+ StamonString("division by zero"))
}

// break语句在循环外
inline void AstRunner::ThrowBreakError() {
	THROW_S(StamonString("Break Error: ") + getExcutePosition()
			+ StamonString("\'break\' outside loop"))
}

// continue语句在循环外
inline void AstRunner::ThrowContinueError() {
	THROW_S(StamonString("Continue Error: ") + getExcutePosition()
			+ StamonString("\'continue\' outside loop"))
}

// 参数传递错误
inline void AstRunner::ThrowArgumentsError(int form_args, int actual_args) {
	THROW_S(StamonString("Arguments Error: ") + getExcutePosition() + StamonString("takes ")
			+ toString(form_args) + StamonString(" form arguments but ") + toString(actual_args)
			+ StamonString(" was given"))
}

// 返回语句在函数外
inline void AstRunner::ThrowReturnError() {
	THROW_S(StamonString("Return Error: ") + getExcutePosition()
			+ StamonString("\'return\' outside function"))
}

// 未知运算符
inline void AstRunner::ThrowUnknownOperatorError() {
	THROW_S(StamonString("Operator Error: ") + getExcutePosition() + StamonString("unknown operator"))
}

// 未知成员
inline void AstRunner::ThrowUnknownMemberError(int id) {
	StamonString iden = ((ir::IdenConstType *) tabconst[id])->getVal();
	THROW_S(StamonString("Unknown Member Error: ") + getExcutePosition()
			+ StamonString("object has no member \'") + iden + StamonString("\'"))
}

// 长度不能为负数
inline void AstRunner::ThrowLengthError() {
	THROW_S(StamonString("Length Error: ") + getExcutePosition()
			+ StamonString("the length must be non-negative"))
}

// 位移运算操作数不能为负数
inline void AstRunner::ThrowNegativeShiftError() {
	THROW_S(StamonString("Negative Shift Error: ") + getExcutePosition()
			+ StamonString("negative shift count"))
}

} // namespace stamon::vm