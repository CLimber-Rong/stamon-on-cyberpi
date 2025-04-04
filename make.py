import re
import os

os.system("cd program&&stamon build main.st program.stvc --strip=true&&cd ..")

def getSource(filename):
    f = open(filename, 'r', encoding='utf8')
    lines = f.read().splitlines()
    src = ''
    for i in lines:
        if re.match('\\s*#include', i) or re.match('\\s*#pragma', i):
            src += ''
        else:
            src+=(i+'\n')
    f.close()
    return src

# 先将二进制文件写入BinarySTVC

def GenFileData(code: bytes) -> bytes:
        data = code
        rst = '{'
        for i in data:
            rst += str(int(i)) + ','
        return rst[0:-1] + '}'

fin = open('program/program.stvc', 'rb')

fout = open('stamon/include/cyberpi_implemented/BinarySTVC.cpp', 'w', encoding='utf8')
fout.write('''
/*
	Name: BinarySTVC.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 04/04/25 11:05
	Description: 自动生成的二进制文件数组
*/

#pragma once
'''+
"char binary_buffer_array[] = "+GenFileData(fin.read())+";\n"+
"char* binary_buffer = binary_buffer_array;\n"+
"int binary_buffer_len = sizeof(binary_buffer_array);\n"+
"int allocated_buffer_len = binary_buffer_len;\n"
)

fout.close()
fin.close()

src = '''
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"ctype.h"
#include"stddef.h"
#include"cyberpi.h"\n
'''

# 依赖库
src += getSource('stamon/include/cyberpi_implemented/stack.c')
src += getSource('stamon/include/cyberpi_implemented/strie.c')
src += getSource('stamon/include/cyberpi_implemented/TypeDef.cpp')
src += getSource('stamon/include/cyberpi_implemented/String.cpp')
src += getSource('stamon/include/cyberpi_implemented/ArrayList.cpp')
src += getSource('stamon/include/cyberpi_implemented/Stack.cpp')
src += getSource('stamon/include/cyberpi_implemented/stmlib.cpp')
src += getSource('stamon/include/cyberpi_implemented/Exception.cpp')
src += getSource('stamon/include/cyberpi_implemented/EasySmartPtr.cpp')
src += getSource('stamon/include/cyberpi_implemented/ByteMap.cpp')
src += getSource('stamon/include/cyberpi_implemented/NumberMap.cpp')
src += getSource('stamon/include/cyberpi_implemented/StringMap.cpp')
src += getSource('stamon/include/cyberpi_implemented/StamonStandardLib.cpp')
src += getSource('stamon/include/cyberpi_implemented/LineReader.cpp')
src += getSource('stamon/include/cyberpi_implemented/BinarySTVC.cpp')
src += getSource('stamon/include/cyberpi_implemented/BinaryReader.cpp')
src += getSource('stamon/include/cyberpi_implemented/BinaryWriter.cpp')
src += getSource('stamon/include/cyberpi_implemented/FileMap.cpp')
src += getSource('stamon/include/cyberpi_implemented/MemoryPool.cpp')

# 抽象语法树
src += getSource('stamon/src/ast/Ast.cpp')
src += getSource('stamon/src/ast/LeafAst.cpp')
src += getSource('stamon/src/ast/CodeLogicAst.cpp')
src += getSource('stamon/src/ast/ExprAst.cpp')
src += getSource('stamon/src/ast/SfnAst.cpp')

# 配置
src += getSource("stamon/src/config/StamonConfig.cpp")
src += getSource("stamon/src/config/CompilerConfig.cpp")
src += getSource("stamon/src/config/VmConfig.cpp")

# 数据类型
src += getSource("stamon/src/data_type/DataType.cpp")
src += getSource("stamon/src/data_type/Variable.cpp")
src += getSource("stamon/src/data_type/ClassType.cpp")
src += getSource("stamon/src/data_type/MethodType.cpp")
src += getSource("stamon/src/data_type/NullType.cpp")
src += getSource("stamon/src/data_type/NumberType.cpp")
src += getSource("stamon/src/data_type/ObjectType.cpp")
src += getSource("stamon/src/data_type/SequenceType.cpp")
src += getSource("stamon/src/data_type/StringType.cpp")

# Ast与AstIr互转
src += getSource("stamon/src/ir/AstIR.cpp")

# 编码文件读写
src += getSource('stamon/src/action/AstIrReader.cpp')
src += getSource('stamon/src/action/AstIrWriter.cpp')

# 编译端
src += getSource("stamon/src/error/CompilerExceptionMessage.cpp")
src += getSource("stamon/src/compiler/Lexer.cpp")
src += getSource("stamon/src/compiler/Parser.cpp")
src += getSource("stamon/src/compiler/Compiler.cpp")

# 运行端
src += getSource("stamon/src/vm/ObjectManager.cpp")
src += getSource("stamon/src/sfn/SFN.cpp")
src += getSource("stamon/src/vm/TypeCalculator.cpp")
src += getSource("stamon/src/vm/AstRunner.cpp")
src += getSource("stamon/src/error/AstRunnerExceptionMessage.cpp")

# 主文件
src += getSource("stamon/src/Stamon.cpp")
src += getSource("stamon/src/Main.cpp")

fout = open("src/main.ino", 'w', encoding='utf8')
fout.write(src)
fout.close()

os.system("C:/Users/Administrator/.platformio/penv/Scripts/platformio.exe run --target upload")