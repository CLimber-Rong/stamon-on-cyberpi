/*
	Name: Main.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 24/02/24 11:59
	Description:
    * 命令行工具
    * 该文件并不属于编译器或虚拟机的范畴，所以使用了C标准库
    * 开发者可以自行更改或者建立属于自己的命令行工具
	* 在使用stamon之前，你需要配置环境变量
	* 即：将可执行文件所在目录设为STAMON
*/

#include"Stamon.cpp"

using namespace stamon;
using namespace stamon::config;	//由于命名空间前缀过于繁琐，因此引用此命名空间

#include"locale.h"


StamonString getNoEndingSeparatorPath(StamonString path);	//获取末尾没有分隔符的路径
int StamonBuildCommand(ArrayList<StamonString> args);
int StamonRunCommand(ArrayList<StamonString> args);

int StamonMain(int argc, char* argv[]) {

	//参数表
	ArrayList<StamonString> args;

	//获取可执行文件路径
	StamonString s(argv[0]);

	for(int i=1; i<argc; i++) {
		args.add(StamonString(argv[i]));
	}

	if(args.empty()) {
		//没有传入任何参数
		platform_print(
		    "stamon: fatal error: too few arguments\n"
		    "please enter \'stamon help\' to get more information.\n"
		);
		return -1;
	}

	if(
	    args[0].equals(StamonString((char*)"build"))
	    ||args[0].equals(StamonString((char*)"-b"))
	) {
		return StamonBuildCommand(args);

	} else if(
	    args[0].equals(StamonString((char*)"run"))
	    ||args[0].equals(StamonString((char*)"-r"))
	) {
		return StamonRunCommand(args);

	}

	return 0;

}

StamonString getNoEndingSeparatorPath(StamonString path) {
	//去除末尾分隔符
	if(path[path.length()-1]=='\\' || path[path.length()-1]=='/') {
		return path.substring(0, path.length()-1);
	}
	return path;
}

int StamonBuildCommand(ArrayList<StamonString> args) {

	StamonString program_path("");

	StamonString src;

	//先设置为配置文件的默认值
	StamonString dst = stamon::c::config::DefaultObjectFileName;
	int warning_level = stamon::c::config::WarningLevel;
	bool isSupportImport = stamon::c::config::isSupportImport;
	bool isStrip = stamon::c::config::isStrip;

	//解析编译的文件名

	if(args.size()>=2) {
		src = args[1];

		//目标文件名是可选的
		
		if(args.size()>=3) {

			dst = args[2];

			for(int i=3; i<args.size(); i++) {

				if(args[i].equals(StamonString((char*)"--import=false"))) {

					isSupportImport = false;

				} else if(args[i].equals(StamonString((char*)"--import=true"))) {

					isSupportImport = true;

				} else if(args[i].equals(StamonString((char*)"--strip=false"))) {

					isStrip = false;

				} else if(args[i].equals(StamonString((char*)"--strip=true"))) {

					isStrip = true;
				
				} else if(args[i].equals(StamonString((char*)"--IgnoreWarning"))) {

					warning_level = StamonWarningSafeLevel_IgnoreWarning;
				
				} else if(args[i].equals(StamonString((char*)"--JustWarn"))) {

					warning_level = StamonWarningSafeLevel_JustWarn;
				
				} else if(args[i].equals(StamonString((char*)"--FatalWarning"))) {

					warning_level = StamonWarningSafeLevel_FatalWarning;
				
				} else if(
					args[i].length()>=3
					&&args[i].substring(0, 2).equals((char*)"-I")) {

					//添加引用路径
					ImportPaths.add(
						getNoEndingSeparatorPath(
							args[i].substring(2, args[i].length())
						)
					);

				} else if(
					args[i].length()>=10
					&&args[i].substring(0, 9).equals((char*)"--locale=")) {

					//设置语言环境
					setlocale(LC_ALL, args[i].substring(9,args[i].length()).getstr());

				} else {

					//错误参数
					platform_print(
						"stamon: compile: bad command\n"
						"please enter \'stamon help\' "
						"to get more information.\n"
					);

					return -1;
				}

			}
		}
	} else {
		platform_print("stamon: build: too few arguments\n"
			   "please enter \'stamon help\' "
			   "to get more information.\n");
		return -1;
	}

	if(isSupportImport) {
		ImportPaths.insert(
			0,
			getNoEndingSeparatorPath(program_path)
			+ StamonString((char*)"/include/")
		);
		//加入标准库路径
	}

	Stamon stamon;

	stamon.Init();

	stamon.compile(src, dst, isSupportImport, isStrip);

	if(
		stamon.WarningMsg.empty()==false
		&& warning_level != StamonWarningSafeLevel_IgnoreWarning
	) {
		if(warning_level==StamonWarningSafeLevel_JustWarn) {
			platform_print("stamon: compile: warning:\n");
		} else if(warning_level==stamon::config::StamonWarningSafeLevel_FatalWarning) {
			platform_print("stamon: compile: fatal error:\n");
		}

		for(int i=0,len=stamon.WarningMsg.size(); i<len; i++) {
			platform_print(stamon.WarningMsg.at(i).getstr());
		}
		if(warning_level==StamonWarningSafeLevel_FatalWarning) {
			return -1;
		}
	}

	if(stamon.ErrorMsg.empty()==false) {
		platform_print("stamon: compile: fatal error:\n");
		for(int i=0,len=stamon.ErrorMsg.size(); i<len; i++) {
			platform_print(stamon.ErrorMsg.at(i).getstr());
		}
		return -1;
	}

	return 0;
}

int StamonRunCommand(ArrayList<StamonString> args) {

	StamonString src;

	//先设置为配置文件的默认值
	int warning_level = stamon::vm::config::WarningLevel;
	bool isGC = stamon::vm::config::isGC;
	int MemLimit = stamon::vm::config::MemLimit;
	int PoolCacheSize = stamon::vm::config::PoolCacheSize;

	if(args.size()<2) {
		platform_print("stamon: run: too few arguments\n"
			   "please enter \'stamon help\' "
			   "to get more information.\n");
		return -1;
	} else {
		src = args[1];

		for(int i=2,len=args.size(); i<len; i++) {
			if(args[i].equals(StamonString((char*)"--GC=true"))) {
				isGC = true;
			} else if(args[i].equals(StamonString((char*)"--GC=false"))) {
				isGC = false;
			} else if(
				args[i].length()>11
				&&args[i].substring(0, 11).equals(
					StamonString((char*)"--MemLimit=")
				)
			) {
				MemLimit = args[i]
						   .substring(11, args[i].length())
						   .toInt();
			} else if(
				args[i].length()>15
				&&args[i].substring(0, 15).equals(
					StamonString((char*)"--MemPoolCache=")
				)
			) {
				PoolCacheSize = args[i]
								.substring(15, args[i].length())
								.toInt();
			} else if(args[i].equals(StamonString((char*)"--IgnoreWarning"))) {

				warning_level = StamonWarningSafeLevel_IgnoreWarning;
			
			} else if(args[i].equals(StamonString((char*)"--JustWarn"))) {

				warning_level = StamonWarningSafeLevel_JustWarn;
			
			} else if(args[i].equals(StamonString((char*)"--FatalWarning"))) {

				warning_level = StamonWarningSafeLevel_FatalWarning;
			
			} else if(
				args[i].length()>=10
				&&args[i].substring(0, 9).equals((char*)"--locale=")) {

				//设置语言环境
				setlocale(LC_ALL, args[i].substring(9,args[i].length()).getstr());

			} else {
				platform_print(
					"stamon: run: bad command\n"
					"please enter \'stamon help\' "
					"to get more information.\n"
				);
				return -1;
			}
		}
	}

	Stamon stamon;

	stamon.Init();

	stamon.run(src, isGC, MemLimit, PoolCacheSize);

	if(
		stamon.WarningMsg.empty()==false
		&& warning_level != StamonWarningSafeLevel_IgnoreWarning
	) {
		if(warning_level==StamonWarningSafeLevel_JustWarn) {
			platform_print("stamon: run: warning:\n");
		} else if(warning_level==StamonWarningSafeLevel_FatalWarning) {
			platform_print("stamon: run: fatal error:\n");
		}
		for(int i=0,len=stamon.WarningMsg.size(); i<len; i++) {
			platform_print(stamon.WarningMsg.at(i).getstr());
		}
		if(warning_level==StamonWarningSafeLevel_FatalWarning) {
			return -1;
		}
	}

	if(stamon.ErrorMsg.empty()==false) {
		platform_print("stamon: run: fatal error:\n");
		for(int i=0,len=stamon.ErrorMsg.size(); i<len; i++) {
			platform_print(stamon.ErrorMsg.at(i).getstr());
		}
		return -1;
	}

	return 0;
}

void close_binary_buffer() {
	free(binary_buffer);
	binary_buffer_len = 0;
	allocated_buffer_len = 0;
}

void RunStamon(void* ptr) {

    char* argv[] = {
        (char*)"stamon",
        (char*)"run",
        (char*)"demo.stvc",
		(char*)"--MemLimit=4096",
		(char*)"--MemPoolCache=1024"
    };

	StamonMain(4, argv);
}

void setup() {
	Serial.begin(112500);
    delay(1000);
    cyber.begin();
    cyber.set_bitmap(4,4,cyber.create_text(L"Stamon启动中",0xffff,(uint8_t)15));
	cyber.render_lcd();
	RunStamon(NULL);
}

void loop() {
	if(cyber.get_button_a() || cyber.get_button_b()) {
		RunStamon(NULL);
	}
}