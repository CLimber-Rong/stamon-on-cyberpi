/*
	Name: CompilerExceptionMessage.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 12:46
	Description: 此文件用于存放编译器异常信息的生成函数
*/

#pragma once

#include "String.cpp"

namespace stamon::c::err {
// 这里存放着编译端的报错信息生成函数

/*以下报错信息用于词法分析器*/

// 错误的字符串格式
StamonString WrongStringFormat() {
	return StamonString("the string was entered incorrectly");
}

// 错误的浮点数格式
StamonString WrongFloatFormat() {
	return StamonString("the floating point was entered incorrectly");
}

// 未知单词
StamonString UnknownToken(StamonString text) {
	return StamonString("unknown token: \'") + text + StamonString("\'");
}

/*以下报错信息用于语法分析器*/

// 错误语法
StamonString InvalidSyntax() {
	return StamonString("invalid syntax");
}

// 变量被重复定义
StamonString VariableDeclaredRepeatedly(StamonString iden) {
	return StamonString("variable \"") + iden + StamonString("are declared repeatedly");
}

// 错误的sfn语法
StamonString WrongSfnSyntax() {
	return StamonString("the port of the SFN statement must be an identifier");
}

// continue语句在循环外
StamonString ContinueOutsideLoop() {
	return StamonString("\'continue\' outside loop");
}

// break语句在循环外
StamonString BreakOutsideLoop() {
	return StamonString("\'break\' outside loop");
}

// 变量名不是运算符
StamonString WrongVariableFormat() {
	return StamonString("the name of the variable must be an identifier");
}

// 圆括号未闭合
StamonString ParenthesesNotClosed() {
	return StamonString("the parentheses are not closed");
}

// 类定义中出现异常的成员定义
StamonString WrongClassDefined() {
	return StamonString("only functions, classes, and variables "
				  "can be defined in a class");
}

// 无法引用其他源码
StamonString CannotImport() {
	return StamonString("cannot import");
}

// 需要左值作为赋值的左操作数
StamonString LvalueRequieredLeftOperand() {
	return StamonString("the left operand of an assignment must be an lvalue");
}

// 未定义的变量
StamonString UndefinedVariable(StamonString iden) {
	return StamonString("undefined variable: \"") + iden + StamonString("\"");
}

} // namespace stamon::c::err



namespace stamon::c::warning {
// 这里存放着编译端的警告信息生成函数

/*以下报错信息用于词法分析器*/

// 整数过大导致溢出
StamonString TooLargeInteger() {
	return StamonString("the integer is too large");
}

// 浮点数过大导致溢出
StamonString TooLargeFloat() {
	return StamonString("the floating point is too large");
}

} // namespace stamon::c::warning