
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"ctype.h"
#include"stddef.h"
#include"cyberpi.h"

/*
	Name: stack.h
	Copyright: Apache License 2.0
	Author: 瞿相荣
	Date: 23/10/22 21:44
	Description: C语言栈库，版本1.0.0.0
*/


#ifndef STACK_H
#define STACK_H


typedef int (*STACK_VISIT)(void*);					//遍历的函数指针接口

typedef struct _STACK_ELEMENT_TYPE {					//栈的元素，采用双向链表式结构
	void* data;									//数据域
	struct _STACK_ELEMENT_TYPE* next;			//下一个
	struct _STACK_ELEMENT_TYPE* prev;			//上一个
} stack_node;

typedef struct {
	stack_node* top;								//栈顶
	stack_node* base;								//栈底
	int length;										//长度
} STACK;

STACK* InitStack();									//初始化一个栈，成功返回栈指针，否则返回NULL
int ClearStack(STACK* stack);						//清空一个栈，成功返回1，否则返回0
int DestroyStack(STACK* stack);						//销毁一个栈，成功返回1，否则返回0
int StackEmpty(STACK* stack);						//判断栈是否为空，是则返回1，否则返回0，出错返回-1
int StackLength(STACK* stack);						//返回栈的长度，出错返回-1
void* GetTop(STACK* stack);							//返回栈顶元素数据，出错返回NULL
int PushStack(STACK* stack,void* data);				//入栈 ，成功返回1，否则返回0
void* PopStack(STACK* stack);						//出栈并返回出栈的数据，错误返回NULL
int StackTraverse(STACK* stack,STACK_VISIT visit);	//从栈底到栈顶依次调用visit函数
//要求visit函数参数为数据，visit返回1代表处理成功，并开始处理下一个元素
//返回0代表处理失败，StackTraverse则终止处理并退出
//返回成功处理的元素个数

STACK* InitStack() {
	STACK* stack = (STACK*)calloc(1,sizeof(STACK));
	stack->base = (stack_node*)calloc(1,sizeof(stack_node));
	if(stack->base==NULL) {
		return NULL;
	}
	stack->top = stack->base;
	stack->length = 0;
	return stack;
}

int ClearStack(STACK* stack) {
	if(stack==NULL) {
		return 0;
	}
	stack_node* tmp = stack->top->prev;
	while(stack->length!=0) {
		free(tmp->next);
		tmp = tmp->prev;
		stack->length--;
	}
	stack->top = stack->base;
	return 1;
}

int DestroyStack(STACK* stack) {
	if(ClearStack(stack)==0) {
		return 0;
	}
	free(stack);
	return 1;
}

int StackEmpty(STACK* stack) {
	if(stack==NULL) {
		return -1;
	}
	if(stack->length==0) {
		return 1;
	} else {
		return 0;
	}
}

int StackLength(STACK* stack) {
	if(stack==NULL) {
		return -1;
	}
	return stack->length;
}

void* GetTop(STACK* stack) {
	if(stack==NULL) {
		return NULL;
	}
	return stack->top->data;
}

int PushStack(STACK* stack,void* data) {
	if(stack==NULL) {
		return 0;
	}
	stack_node* tmp = (stack_node*)calloc(1,sizeof(stack_node));
	if(tmp==NULL) {
		return 0;
	}
	tmp->data = data;
	tmp->next = NULL;
	tmp->prev = stack->top;
	stack->top->next = tmp;
	stack->top = tmp;
	stack->length++;
	return 1;
}

void* PopStack(STACK* stack) {
	if(stack==NULL) {
		return NULL;
	}
	if(stack->length==0) {
		return NULL;
	}
	void* data = stack->top->data;
	stack_node* tmp = stack->top->prev;
	free(stack->top);
	stack->top = tmp;
	stack->length--;
	return data;
}

int StackTraverse(STACK* stack,STACK_VISIT visit) {
	if(stack==NULL) {
		return 0;
	}
	stack_node* tmp = stack->base->next;
	int i;
	for(i=0; i<(stack->length); i++) {
		if(visit(tmp->data)==0) {
			break;
		}
		tmp = tmp->next;
	}
	return i;
}

#endif

/*
	Name: strie.h
	Copyright: Apache 2.0 License
	Author: 瞿相荣
	Date: 25/10/22 08:22
	Description: C语言字典树库（仅支持普通ASCII码存储），版本1.0.0.0
*/

/*
 * 这个库并不是在编写STVM时诞生的
 * 而是我之前编写过的
 * 这个库的开源地址：github.com/CLimber-Rong/ctrie
 * 注意：这个库经过修改，和原版的并不一样
 */

/*
 * 我改装了strie.h，让它的键不是字符串，而是字节数组
 */


#ifndef STRIE_H
#define STRIE_H


typedef int (*TRIE_VISIT)(void*);							//遍历的函数指针接口

typedef struct _STRIE_NODE_TYPE {
	void* data;				//数据域
	int isexist;			//是否存在，存在为1，不存在为0
	struct _STRIE_NODE_TYPE* child[256];
} STRIE;

STRIE* InitTrie();											//初始化树，成功返回地址，失败返回NULL
int SetTrieKeyVal(STRIE* trie, unsigned char* key, int key_size, void* val);		//设置键-值，成功返回1，失败返回0，可以用此接口创建键-值和更改已有键-值
int DelTrieKeyVal(STRIE* trie, unsigned char* key, int key_size);		//删除键-值，成功返回1，失败返回0
void* GetTrieKeyVal(STRIE* trie, unsigned char* key, int key_size);	//根据键获得获得值，成功返回值，失败返回NULL，
//返回NULL时不排除数据域的指针本来就等于NULL，
//需配合TrieExistKeyVal()接口判断
int TrieExistKeyVal(STRIE* trie, unsigned char* key, int key_size);	//键-值是否存在，存在则返回1，不存在则返回0
int ClearTrie(STRIE* trie);									//清空所有键-值，成功返回1，失败返回0
int DestroyTrie(STRIE* trie);								//销毁树，成功返回1，失败返回0
int TrieEmpty(STRIE* trie);									//树是否为空，是则返回1，否则返回0，错误返回-1
int TrieTraverse(STRIE* trie, TRIE_VISIT visit);				//对树中所有元素依次调用visit函数
//要求visit函数参数为数据，visit返回1代表处理成功，并开始处理下一个元素
//返回0代表处理失败，StackTraverse则终止处理并退出
//返回成功处理的元素个数

STRIE* InitTrie() {
	STRIE* result = (STRIE*)calloc(1, sizeof(STRIE));
//	if(result==NULL){
//		return NULL;										//故意注释给你看，仔细理解一下为什么
//	}
	return result;											//反正如果出错也得返回NULL，干脆直接返回
}

int SetTrieKeyVal(STRIE* trie, unsigned char* key, int key_size, void* val) {
	if(trie==NULL) {
		return 0;
	}
	int i;
	for(i=0; i<key_size; i++) {
		if(trie->child[key[i]]==NULL) {
			trie->child[key[i]] = InitTrie();
			if(trie->child[key[i]]==NULL) {
				//如果内存分配还是有问题
				return 0;
			}
		}
		trie = trie->child[key[i]];
	}
	trie->isexist = 1;
	trie->data = val;
	return 1;
}

int DelTrieKeyVal(STRIE* trie, unsigned char* key, int key_size) {
	int i, j;
	if(trie==NULL) {
		return 0;
	}
	STACK* stack = InitStack();
	if(stack==NULL) {
		return 0;
	}
	for(i=0; i<key_size; i++) {
		if(trie->child[key[i]]==NULL) {
			return 0;
		}
		if(PushStack(stack, trie)==0) {
			return 0;
		}
		trie = trie->child[key[i]];
	}
	if(trie->isexist==0) {
		return 0;
	}

	trie->isexist = 0;

	while(StackEmpty(stack)==0) {
		STRIE* tmp = (STRIE*)GetTop(stack);
		int flag = 0;
		for(i=0; i<256; i++) {
			if((tmp->child[i]!=NULL)||(tmp->isexist==1)) {
				//如果子节点有键-值或本节点已经有存数据的话
				flag = 1;
			}
		}
		if(flag==1) {
			break;
		} else {
			free(tmp);
			PopStack(stack);
		}
	}
	if(DestroyStack(stack)==0) {
		return 0;
	}
	return 1;
}

void* GetTrieKeyVal(STRIE* trie, unsigned char* key, int key_size) {
	int i;
	if(trie==NULL) {
		return 0;
	}
	for(i=0; i<key_size; i++) {
		if(trie->child[key[i]]==NULL) {
			return NULL;
		}
		trie = trie->child[key[i]];
	}
	if(trie->isexist==0) {
		return NULL;
	}
	return trie->data;
}

int TrieExistKeyVal(STRIE* trie, unsigned char* key, int key_size) {
	if(trie==NULL) {
		return 0;
	}
	int i;
	for(i=0; i<key_size; i++) {
		if(trie->child[key[i]]==NULL) {
			return 0;
		}
		trie = trie->child[key[i]];
	}
	return trie->isexist;
}

int ClearTrie(STRIE* trie) {
	STACK* stack = InitStack();
	if(trie==NULL||stack==NULL) {
		return 0;
	}
	PushStack(stack, trie);
	while(StackEmpty(stack)==0) {
		STRIE* tmp = (STRIE*)PopStack(stack);
		STRIE** tmp2 = tmp->child;
		if(tmp!=NULL) {
			for(int i=0; i<256; i++) {
				PushStack(stack, tmp2[i]);
			}
			if(tmp!=trie) 	free(tmp);
		}
	}
	DestroyStack(stack);
	return 1;
}

int DestroyTrie(STRIE* trie) {
	if(trie==NULL) {
		return 0;
	}
	ClearTrie(trie);
	free(trie);
	return 1;
}

int TrieEmpty(STRIE* trie) {
	int i;
	if(trie==NULL) {
		return -1;
	}
	for(i=0; i<256; i++) {
		if(trie->child[i]!=NULL) {
			break;
		}
	}
	if(i!=256) {
		return 0;
	}
	return 1;
}

int TrieTraverse(STRIE* trie, TRIE_VISIT visit) {
	STACK* stack = InitStack();
	if(stack==NULL||trie==NULL) {
		return 0;
	}

	PushStack(stack, trie);
	int i=0;
	while(StackEmpty(stack)==0) {
		trie = (STRIE*)PopStack(stack);
		if(trie!=NULL) {
			int j;
			for(j=0; j<256; j++) {
				PushStack(stack, trie->child[j]);
			}
			if(trie->isexist==1) {
				if(visit(trie->data)==0) {
					break;
				}
			}
		}
		i++;
	}
	DestroyStack(stack);
	return i;
}



#endif
/*
	Name: StamonString.cpp
	Copyright: Apache2.0
	Author: Gusem Fowage, CLimber-Rong
	Date: 29/07/23 12:59
	Description: 跨平台数据类型定义库
*/

// 可以用于去除 符号 限定
// 以及 无符号 限定
// 可以用于转换 类型至相应长度的类型(与 union 配合使用)

template<size_t N, bool sign>
struct bytes_get{
	typedef void type;
};
template<>
struct bytes_get<1, true>{typedef int8_t type;};
template<>
struct bytes_get<2, true>{typedef int16_t type;};
template<>
struct bytes_get<4, true>{typedef int32_t type;};
template<>
struct bytes_get<8, true>{typedef int64_t type;};
// struct bytes_get<16, true>{typedef int8_t type;};
template<>
struct bytes_get<1, false>{typedef uint8_t type;};
template<>
struct bytes_get<2, false>{typedef uint16_t type;};
template<>
struct bytes_get<4, false>{typedef uint32_t type;};
template<>
struct bytes_get<8, false>{typedef uint64_t type;};
// struct bytes_get<16, false>{typedef uint128_t type;};

using len_t = bytes_get<sizeof(size_t), false>::type;
/*
	Name: StamonString.cpp
	Copyright: Apache2.0
	Author: CLimber-Rong, GusemFowage
	Date: 29/07/23 12:59
	Description: 字符串库
*/

/*
 * 一些网络上的大牛，他们告诉我，我必须在一些函数的结尾加上const限定符，才能支持
 const StamonString var_name;
 * 的用法。
 * 我采纳了他们的建议，感谢他们！
*/


typedef char char_type;
typedef size_t size_type;

class StamonString {

		/*
		 * 利用了缓存技术
		 * 对于短字符串（长度小于32字符），采用栈数组存储，减少内存申请次数
		 */

		char_type* str;
		char_type cache[32] = {0};

		void* StrCalloc(int size) {
			if(size*sizeof(char_type)<=32) {
				int i = 0;
				while((cache[i]!=0) && (i<32)) {
					cache[i] = 0;
					i++;
				}
				return cache;
			} else {
				return calloc(size, sizeof(char_type));
			}
		}

		void StrFree() {
			if(str!=cache) {
				free(str);
			}
		}

	public:
		StamonString() {
			str = (char_type*)StrCalloc(1);
		}			   //初始化为空字符串

		StamonString(const char_type *s) {
			str = (char_type*)StrCalloc(strlen(s)+1);
			strcpy(str, s);
		}	   //初始化，将s复制到this

		StamonString(const char_type* s, size_type len) {
			str = (char_type*)StrCalloc(len+1);
			strncpy(str, s, len);
		}

		StamonString(const StamonString& s) {
			str = (char_type*)StrCalloc(s.length()+1);
			strcpy(str, s.getstr());
		}

		StamonString(StamonString&& s) {
			str = (char_type*)StrCalloc(s.length()+1);
			strcpy(str, s.getstr());
		}

		bool equals(const StamonString& s) const {

			if(length()!=s.length()) {
				return false;
			}

			return strcmp(str, s.getstr())==0;

		} //判断this的内容是否与s相等，是则返回true，否则返回false

		bool equals(const char_type* s) const {
			return strcmp(str, s) == 0;
		}

		explicit operator const char_type*() const {
			return str;
		}

		//以下的一系列toString函数会将不同的数据类型转为StamonString后保存到this当中，返回this
		StamonString toString(int value) {
			StrFree();
			str = (char_type*)StrCalloc(256);
			snprintf(str, 256, "%d", value);
			str = (char_type*)realloc(str, strlen(str)+1);
			return StamonString(str);
		}

		StamonString toString(bool value) {
			str = (char_type*)StrCalloc(10);

			if(value==true) {
				str = (char_type*)realloc(str, 5);
				strcpy(str, "true");
			} else {
				str = (char_type*)realloc(str, 6);
				strcpy(str, "false");
			}

			return StamonString(str);
		}

		StamonString toString(float value) {
			StrFree();
			str = (char_type*)StrCalloc(256);
			snprintf(str, 256, "%f", value);
			str = (char_type*)realloc(str, strlen(str)+1);
			return StamonString(str);
		}

		StamonString toString(double value) {
			StrFree();
			str = (char_type*)StrCalloc(256);
			snprintf(str, 256, "%lf", value);
			str = (char_type*)realloc(str, strlen(str)+1);
			return StamonString(str);
		}

		int toInt() const {
			int rst;
			sscanf(str, "%d", &rst);
			return rst;
		}

		int toIntX() const {
			int rst;
			sscanf(str, "%x", &rst);
			return rst;
		}

		float toFloat() const {
			float rst;
			sscanf(str, "%f", &rst);
			return rst;
		}

		double toDouble() const {
			double rst;
			sscanf(str, "%lf", &rst);
			return rst;
		}

		size_type length() const {
			return strlen(str);
		}			   //返回字符串长度

		char_type at(size_type index) const {
			return str[index];
		}		   //返回第index个字符

		char_type* getstr() const {
			return str;
		}	//如果你只需要一个只读用的char_type*字符串，getstr函数足矣

		StamonString substring(size_type start, size_type end) {
			//获取从start到end（不包含end）的子字符串
			return StamonString(str+start, end-start);
		}

		StamonString operator=(const StamonString& right_value) {
			StrFree();

			str = (char_type*)StrCalloc(right_value.length()+1);

			strcpy(str, right_value.getstr());

			return StamonString(str);
		}

		StamonString operator+(const StamonString& right_value) const {
			return StamonString(str).append(right_value);
		}
		
		template<class s_t>
		friend StamonString operator+(const s_t& str1, const s_t& str2) {
			StamonString s(str1);
			s.append(StamonString(str2));
			return s;
		}

		StamonString& operator+=(const StamonString& s) {
			return append(s);
		}

		template<class s_t>
		StamonString& operator+=(const s_t& s) {
			append(StamonString(s));
			return *this;
		}

		StamonString& operator+=(const char_type* s) {
			return append(s);
		}

		StamonString& operator+=(char_type c) {
			push_back(c);
			return *this;
		}

		bool operator==(const StamonString& right_value) const {
			return equals(right_value);
		}

		bool operator==(const char_type* s) const {
			return equals(s);
		}

		bool operator!=(const StamonString& right_value) const {
			return !equals(right_value);
		}

		bool operator!=(const char_type* s) const {
			return !equals(s);
		}

		bool operator<(const StamonString& s) const {
			return strcmp(str, s.getstr())>0;
		}

		bool operator>(const StamonString& s) const {
			return strcmp(str, s.getstr())<0;
		}

		bool operator<=(const StamonString& s) const {
			return !(*this > s);
		}

		bool operator>=(const StamonString& s) const {
			return !(*this < s);
		}

		char_type& operator[](size_type index) {
			return str[index];
		}

		char_type operator[](size_type index) const {
			return str[index];
		}

		bool empty() const {
			return length()==0;
		}

		StamonString& append(const char_type* s) {

			size_type len =  length()+strlen(s);

			if(str==cache && len<=32-1) {
				strcat(str, s);
			} else {
				char_type* temp = (char_type*)calloc(len+1, sizeof(char_type));
				strcpy(temp, str);
				strcat(temp, s);
				StrFree();
				str = temp;
			}

			return *this;
		}

		StamonString& append(const StamonString& s) {
			return append(s.getstr());
		}
		
		void push_back(char_type c) {
			append(StamonString(&c));
			return;
		}

		void pop_back() {
			// 分配长度不变化，直接修改最后一个字符为'\0'
			str[length()-1] = '\0';
		}

		~StamonString() {
			StrFree();
		}
};
/*
	Name: ArrayList
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 28/07/23 17:10
	Description: 动态数组
*/



template<typename T> class ArrayList {
	/*
	 * 采用双倍扩容法
	 * 设立了cache_length和length
	 * cache_length代表缓冲区长度，即当前最多能存储多少元素
	 * length代表当前已经存储了多少元素
	 * 不难发现，剩余的空间可以用(cache_length-length)来表示
	 * 我们规定：当缓冲区已经被占满，将缓冲区翻倍扩容
	 * 我们规定：当实际占用的容量小于缓冲区容量的二分之一时，将缓冲区缩小为原来的一半
	 * 这样，我们将ArrayList的分配次数从线性级将为对数级
	 */

	T *cache = NULL;
	int cache_length = 0;
	int length = 0;

	void realloc_list(int len) {
		if (len < (cache_length / 2)) {
			// 缩小至一半

			T *temp = new T[cache_length / 2];
			for (int i = 0; i < len; i++) {
				temp[i] = cache[i];
				// 尽可能地复制
			}

			this->~ArrayList(); // 销毁原来的cache

			cache = temp;
			cache_length = cache_length / 2;
		} else if (len >= cache_length) {
			// 扩容至两倍

			T *temp = new T[cache_length * 2];
			for (int i = 0; i < length; i++) {
				temp[i] = cache[i];
			}

			this->~ArrayList(); // 销毁原来的cache

			cache = temp;
			cache_length = cache_length * 2;
		}
		// 如果不满足缩小条件，也不满足扩容条件，就什么都不做
	}

public:
	ArrayList() {
		cache = new T[1];
		cache_length = 1;
		length = 0;
	}

	ArrayList(int size) {
		if (size == 0) {
			cache_length = 1;
		} else {
			cache_length = size;
		}
		cache = new T[cache_length];
		length = size;
	}

	ArrayList(const ArrayList<T> &list) {
		if (list.size() == 0) {
			// 缓冲区至少要有一个元素
			cache_length = 1;
		} else {
			cache_length = list.length;
		}

		cache = new T[cache_length];

		for (int i = 0; i < list.size(); i++) {
			cache[i] = list[i];
		}

		length = list.length;
	}

	ArrayList(ArrayList<T> &&list) {
		cache = list.cache;
		cache_length = list.cache_length;
		length = list.length;
		list.cache = NULL;
	}

	ArrayList<T> &operator=(const ArrayList<T> &list) {
		this->~ArrayList();

		if (list.size() == 0) {
			// 缓冲区至少要有一个元素
			cache_length = 1;
		} else {
			cache_length = list.length;
		}

		cache = new T[cache_length];

		for (int i = 0; i < list.size(); i++) {
			cache[i] = list[i];
		}

		length = list.length;

		return *this;
	}

	ArrayList<T> &operator=(ArrayList<T> &&list) {
		this->~ArrayList();

		cache = list.cache;
		cache_length = list.cache_length;
		length = list.length;
		list.cache = NULL;

		return *this;
	}

	void add(const T &value) {
		realloc_list(length + 1); // 重新分配内存
		cache[length] = value;
		length++;
	} // 末尾添加值

	void insert(int index, const T &value) {
		realloc_list(length + 1); // 重新分配内存

		for (int i = length - 1; i >= index; i--) {
			cache[i + 1] = cache[i];
		}

		cache[index] = value;
		length++;
	} // 将value插入到[index]

	void erase(int index) {
		for (int i = index; i < length - 1; i++) {
			cache[i] = cache[i + 1];
		}

		realloc_list(length - 1); // 重新分配内存
		length--;
	} // 删除[index]

	T at(int index) const {
		return cache[index];
	} // 获取[index]

	bool empty() const {
		return (length == 0);
	} // 判断是否为空

	void clear() {
		this->~ArrayList(); // 销毁
		cache = new T[1]; // 新建缓冲区
		cache_length = 1;
		length = 0; // 长度清零
	} // 清空列表

	int size() const {
		return length;
	} // 获得元素个数

	ArrayList<T> operator+(ArrayList<T> src) {
		ArrayList<T> rst = *this;

		for (int i = 0, len = src.size(); i < len; i++) {
			rst.add(src[i]);
		}

		return rst;
	}

	ArrayList<T> operator+=(ArrayList<T> src) {
		return *(this) = *(this) + src;
	}

	T &operator[](int index) {
		return cache[index];
	}

	T operator[](int index) const {
		return cache[index];
	}

	~ArrayList() {
		if (cache != NULL) {
			delete[] cache;
		}
	}
};
/*
	Name: Stack.cpp
	Copyright: Apache 2.0
	Author: 瞿相荣
	Date: 17/01/23 18:06
	Description: 栈库，基于stack.h
*/


template<typename T> class Stack {
	ArrayList<T *> list;

public:
	Stack() {}
	int clear() {
		list.clear();
		return 1;
	} // 清空
	int empty() {
		return list.empty();
	} // 是否为空
	int size() {
		return list.size();
	} // 元素个数
	T *peek() {
		return list[list.size() - 1];
	} // 获取栈顶元素
	int push(T *data) {
		list.add(data);
		return 1;
	} // 入栈
	T *pop() {
		T *rst = list[list.size() - 1];
		list.erase(list.size() - 1);
		return rst;
	} // 出栈
};
/*
	Name: clib.c
	Copyright: Apache 2.0
	Author: CLimber-Rong, GusemFowage
	Date: 12/08/23 23:24
	Description: 一些杂糅的库定义
*/

// 这个库原本叫做clib.c
// 由于需要引入c++代码，所以后缀改.cpp，顺便把文件名改为stmlib


CyberPi cyber;


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
/*
	Name: Exception.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 24/08/23 18:09
	Description: 异常处理的实现
*/

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
/*
	Name: EasySmartPtr.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 30/03/25 12:52
	Description: 一个简单的智能指针，使用了引用计数法
*/


/*
 * 对于一些不会循环引用的数据结构，可以用智能指针来封装指针
 * 利用引用计数法，EasySmartPtr会记录有多少个数据正在使用该指针
 * 当没有任何数据使用该指针时，其可以被视作垃圾内存而删除
 */

template<typename T> class EasySmartPtr;

template<typename T> void __EASYSMARTPTR_DEFAULT_DESTROY_FUNCTION__(EasySmartPtr<T> *p);
// 默认的销毁函数

template<typename T> class EasySmartPtr {
public:
	void (*destroy_fp)(EasySmartPtr<T> *ptr);
	T *ptr;
	int *ref_cnt;

	EasySmartPtr(T *pointer) {
		// 初始化，传入指针
		ref_cnt = new int; // 初始化计数器
		(*ref_cnt) = 1; // 初始化时，只有一个SmartPointer拥有这个指针
		ptr = pointer;
		destroy_fp = __EASYSMARTPTR_DEFAULT_DESTROY_FUNCTION__<T>;
	} // 直接传入指针，默认销毁方式为直接delete

	EasySmartPtr(T *pointer, void (*destroy_funcptr)(EasySmartPtr<T> *ptr)) {
		ref_cnt = new int;
		(*ref_cnt) = 1;
		ptr = pointer;
		destroy_fp = destroy_funcptr;
	} // 传入指针，并指定销毁方式

	EasySmartPtr(const EasySmartPtr &value) {
		ref_cnt = value.ref_cnt; // 复制计数器
		ptr = value.ptr; // 复制指针
		destroy_fp = value.destroy_fp;
		(*ref_cnt)++; // 又多了一个EasySmartPtr指向这个指针
	} // 复制构造函数

	EasySmartPtr &operator=(const EasySmartPtr &value) {
		/*
		 * 将当前EasySmartPtr重新指定一个新的指针
		 * 就意味着需要抛弃当前的指针，指向新的指针
		 * 因此需要先减去当前指针的计数器，再将value的计数器加一
		 */

		if (&value == this) {
			// 自我赋值，直接返回即可
			return *this;
		}

		(*ref_cnt)--; // 减去当前计数器
		if ((*ref_cnt) == 0) {
			delete ref_cnt;
			destroy_fp(this);
			// 如果已经没有任何EasySmartPtr指向该指针
			// 那么此ptr可以被视作垃圾指针，需要被销毁
		}

		ref_cnt = value.ref_cnt; // 复制value
		ptr = value.ptr;
		destroy_fp = value.destroy_fp;

		(*ref_cnt)++; // value的计数器加一

		return *this;
	} // 赋值构造函数

	T *get() {
		return ptr;
	} // 获取指针

	T *operator->() {
		return ptr;
	} // 直接访问指针的成员

	~EasySmartPtr() {
		// 当前计数器需要减一
		(*ref_cnt)--;
		if ((*ref_cnt) == 0) {
			delete ref_cnt;
			destroy_fp(this);
			// 原理同上
		}
	} // 析构函数
};

template<typename T> void __EASYSMARTPTR_DEFAULT_DESTROY_FUNCTION__(EasySmartPtr<T> *p) {
	delete p->ptr;
}
/*
	Name: ByteMap.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 10/02/24 13:38
	Description: 字节串map
*/



void __SMARTSTRIE_DESTROY_FUNCTION__(EasySmartPtr<STRIE>* p) {
	DestroyTrie(p->ptr);
}

class SmartStrie : public EasySmartPtr<STRIE> {
public:
	SmartStrie()
		: EasySmartPtr<STRIE>(InitTrie(), __SMARTSTRIE_DESTROY_FUNCTION__) {
	}
};

template<typename T> class ByteMap {
	SmartStrie map;

public:
	ByteMap() {
	}

	int put(char *s, int size, T *data) { // 设置键值
		return SetTrieKeyVal(map.get(), (unsigned char *) s, size, (void *) data);
	}
	int del(char *s, int size) { // 删除键值
		return DelTrieKeyVal(map.get(), (unsigned char *) s, size);
	}
	T *get(char *s, int size) { // 获取值
		return (T *) GetTrieKeyVal(map.get(), (unsigned char *) s, size);
	}
	bool containsKey(char *s, int size) { // 是否存在该键
		return TrieExistKeyVal(map.get(), (unsigned char *) s, size);
	}
	int clear() { // 清空
		return ClearTrie(map.get());
	}
	bool empty() { // 是否为空
		return TrieEmpty(map.get());
	}

	STRIE *getStrie() {
		return map.get();
	}

	template<typename list_T> ArrayList<list_T> getValList() {
		ArrayList<list_T> result;

		/*将所有值汇总成一个指定类型的列表*/

		Stack<STRIE> stack;

		stack.push(map.get());

		while (stack.empty()) {
			STRIE *temp = stack.pop();
			if (temp != NULL) {
				for (int i = 0; i < 256; i++) {
					stack.push(temp->child[i]);
				}
				if (map.get()->isexist == 1) {
					result.add(cast_class(list_T, temp->data));
				}
			}
		}

		return result;
	}
};
/*
	Name: NumberMap.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 23/08/23 14:20
	Description: 整数map
*/



template<typename T> class NumberMap {
	ByteMap<T> map;

public:
	NumberMap() {}
	int put(int s, T *data) { // 设置键值
		return map.put((char *) &s, sizeof(int), data);
	}
	int del(int s) { // 删除键值
		return map.del((char *) &s, sizeof(int));
	}
	T *get(int s) { // 获取值
		return map.get((char *) &s, sizeof(int));
	}
	bool containsKey(int s) { // 是否存在该键
		return map.containsKey((char *) &s, sizeof(int));
	}
	int clear() { // 清空
		return map.clear();
	}
	bool empty() { // 是否为空
		return map.empty();
	}

	STRIE *getStrie() {
		return map.getStrie();
	}

	template<typename list_T> ArrayList<list_T> getValList() {
		return map.template getValList<list_T>();
	}
};
/*
	Name: StringMap.cpp
	Copyright: Apache 2.0
	Author: 瞿相荣
	Date: 18/01/23 19:14
	Description: 字符串map
*/



template<typename T> class StringMap {
	ByteMap<T> map;

public:
	StringMap() {} // 构造方法
	int put(const StamonString &s, T *data) {
		return map.put(s.getstr(), s.length(), data);
	} // 设置键值
	int del(const StamonString &s, T *data) {
		return map.del(s.getstr(), s.length());
	} // 删除键值
	T *get(const StamonString &s) {
		return map.get(s.getstr(), s.length());
	} // 获取值
	bool containsKey(const StamonString &s) {
		return map.containsKey(s.getstr(), s.length());
	} // 是否存在该键
	int clear() {
		return map.clear();
	} // 清空
	bool empty() {
		return map.empty();
	} // 是否为空
	STRIE *getStrie() {
		return map.getStrie();
	}

	template<typename list_T> ArrayList<list_T> getValList() {
		return map.template getValList<list_T>();
	}
};


    StringMap<char> stamon_std_lib_code;
    unsigned char stamon_standard_code_data_0[] = {105,109,112,111,114,116,32,115,116,100,100,101,102,59,0};
unsigned char stamon_standard_code_data_1[] = {105,109,112,111,114,116,32,115,116,100,100,101,102,59,13,10,105,109,112,111,114,116,32,110,97,116,105,118,101,59,13,10,13,10,102,117,110,99,32,97,98,115,40,110,41,32,123,13,10,32,32,32,32,105,102,32,110,60,48,58,32,114,101,116,117,114,110,32,45,110,59,13,10,32,32,32,32,114,101,116,117,114,110,32,110,59,13,10,125,13,10,13,10,102,117,110,99,32,97,108,108,40,108,105,115,116,41,32,123,13,10,32,32,32,32,102,111,114,32,105,32,105,110,32,108,105,115,116,32,123,13,10,32,32,32,32,32,32,32,32,105,102,32,33,105,32,123,13,10,32,32,32,32,32,32,32,32,32,32,32,32,114,101,116,117,114,110,32,102,97,108,115,101,59,13,10,32,32,32,32,32,32,32,32,125,13,10,32,32,32,32,125,13,10,32,32,32,32,114,101,116,117,114,110,32,116,114,117,101,59,13,10,125,13,10,13,10,102,117,110,99,32,114,97,110,103,101,40,110,41,32,123,13,10,32,32,32,32,100,101,102,32,108,105,115,116,32,61,32,91,110,93,59,13,10,32,32,32,32,100,101,102,32,105,32,61,32,48,59,13,10,32,32,32,32,119,104,105,108,101,32,105,60,110,32,123,13,10,32,32,32,32,32,32,32,32,108,105,115,116,91,105,93,32,61,32,105,59,13,10,32,32,32,32,32,32,32,32,105,32,43,61,32,49,59,13,10,32,32,32,32,125,13,10,32,32,32,32,114,101,116,117,114,110,32,108,105,115,116,59,13,10,125,13,10,13,10,102,117,110,99,32,114,97,110,103,101,95,97,98,40,97,44,32,98,41,32,123,13,10,32,32,32,32,100,101,102,32,108,105,115,116,32,61,32,91,98,45,97,43,49,93,59,13,10,32,32,32,32,100,101,102,32,105,32,61,32,97,59,13,10,32,32,32,32,119,104,105,108,101,32,105,60,61,98,32,123,13,10,32,32,32,32,32,32,32,32,108,105,115,116,91,105,45,97,93,32,61,32,105,59,13,10,32,32,32,32,32,32,32,32,105,32,43,61,32,49,59,13,10,32,32,32,32,125,13,10,32,32,32,32,114,101,116,117,114,110,32,108,105,115,116,59,13,10,125,13,10,13,10,102,117,110,99,32,105,110,116,40,115,41,32,123,13,10,32,32,32,32,114,101,116,117,114,110,32,110,97,116,105,118,101,40,34,105,110,116,34,44,32,115,41,59,13,10,125,13,10,13,10,102,117,110,99,32,108,101,110,40,110,41,32,123,13,10,32,32,32,32,114,101,116,117,114,110,32,110,97,116,105,118,101,40,34,108,101,110,34,44,32,110,41,59,13,10,125,13,10,13,10,0};
unsigned char stamon_standard_code_data_2[] = {105,109,112,111,114,116,32,115,116,100,100,101,102,59,13,10,13,10,102,117,110,99,32,110,97,116,105,118,101,40,112,111,114,116,44,32,97,114,103,41,32,123,13,10,32,32,32,32,115,102,110,32,112,111,114,116,44,32,97,114,103,59,13,10,32,32,32,32,114,101,116,117,114,110,32,97,114,103,59,13,10,125,0};
unsigned char stamon_standard_code_data_3[] = {47,47,230,160,135,229,135,134,229,186,147,230,177,135,230,128,187,13,10,13,10,105,109,112,111,114,116,32,115,116,100,100,101,102,59,13,10,105,109,112,111,114,116,32,110,97,116,105,118,101,59,13,10,105,109,112,111,114,116,32,109,97,116,104,59,13,10,105,109,112,111,114,116,32,115,116,100,105,111,59,13,10,105,109,112,111,114,116,32,115,121,115,116,101,109,59,13,10,105,109,112,111,114,116,32,115,116,114,105,110,103,59,13,10,105,109,112,111,114,116,32,102,105,108,101,59,0};
unsigned char stamon_standard_code_data_4[] = {47,47,32,228,184,186,228,186,134,233,152,178,230,173,162,228,186,146,231,155,184,229,188,149,231,148,168,231,154,132,230,150,135,228,187,182,229,135,186,231,142,176,226,128,156,230,156,170,229,145,189,229,144,141,229,143,152,233,135,143,226,128,157,231,154,132,230,131,133,229,134,181,13,10,47,47,32,230,136,145,228,187,172,229,128,159,233,137,180,228,186,134,67,43,43,231,154,132,229,137,141,231,189,174,229,174,154,228,185,137,13,10,47,47,32,229,136,169,231,148,168,100,101,102,229,133,136,229,174,154,228,185,137,229,135,189,230,149,176,229,144,141,239,188,140,230,142,165,231,157,128,232,191,155,232,161,140,229,133,183,228,189,147,229,163,176,230,152,142,13,10,13,10,47,47,230,149,176,229,173,166,229,186,147,229,174,154,228,185,137,13,10,100,101,102,32,97,98,115,59,13,10,100,101,102,32,97,108,108,59,13,10,100,101,102,32,114,97,110,103,101,59,13,10,100,101,102,32,114,97,110,103,101,95,97,98,59,13,10,100,101,102,32,105,110,116,59,13,10,100,101,102,32,108,101,110,59,13,10,13,10,47,47,73,79,229,186,147,229,174,154,228,185,137,13,10,100,101,102,32,112,117,116,115,59,13,10,100,101,102,32,112,114,105,110,116,59,13,10,100,101,102,32,112,114,105,110,116,108,110,59,13,10,100,101,102,32,105,110,112,117,116,59,13,10,13,10,47,47,229,185,179,229,143,176,229,186,147,229,174,154,228,185,137,13,10,100,101,102,32,83,116,97,109,111,110,59,13,10,100,101,102,32,83,121,115,116,101,109,59,13,10,13,10,47,47,229,173,151,231,172,166,228,184,178,229,186,147,229,174,154,228,185,137,13,10,100,101,102,32,115,116,114,59,13,10,13,10,47,47,229,164,150,233,131,168,232,176,131,231,148,168,229,186,147,229,174,154,228,185,137,13,10,100,101,102,32,110,97,116,105,118,101,59,13,10,13,10,47,47,230,150,135,228,187,182,229,164,132,231,144,134,229,186,147,229,174,154,228,185,137,13,10,100,101,102,32,70,105,108,101,59,13,10,100,101,102,32,111,112,101,110,59,0};
unsigned char stamon_standard_code_data_5[] = {47,47,230,160,135,229,135,134,232,190,147,229,133,165,232,190,147,229,135,186,229,186,147,13,10,13,10,105,109,112,111,114,116,32,115,116,100,100,101,102,59,13,10,105,109,112,111,114,116,32,109,97,116,104,59,13,10,105,109,112,111,114,116,32,115,116,114,105,110,103,59,13,10,13,10,102,117,110,99,32,112,117,116,115,40,115,41,32,123,13,10,32,32,32,32,110,97,116,105,118,101,40,34,112,114,105,110,116,34,44,32,115,41,59,13,10,125,13,10,13,10,102,117,110,99,32,112,114,105,110,116,40,110,41,32,123,13,10,32,32,32,32,112,117,116,115,40,115,116,114,40,110,41,41,59,13,10,125,13,10,13,10,102,117,110,99,32,112,114,105,110,116,108,110,40,110,41,32,123,13,10,32,32,32,32,112,117,116,115,40,115,116,114,40,110,41,41,59,13,10,32,32,32,32,112,117,116,115,40,34,92,110,34,41,59,13,10,125,13,10,13,10,102,117,110,99,32,105,110,112,117,116,40,41,32,123,13,10,32,32,32,32,114,101,116,117,114,110,32,110,97,116,105,118,101,40,34,105,110,112,117,116,34,44,32,110,117,108,108,41,59,13,10,125,0};
unsigned char stamon_standard_code_data_6[] = {105,109,112,111,114,116,32,115,116,100,100,101,102,59,13,10,13,10,102,117,110,99,32,115,116,114,40,115,41,32,123,13,10,32,32,32,32,114,101,116,117,114,110,32,110,97,116,105,118,101,40,34,115,116,114,34,44,32,115,41,59,13,10,125,0};
unsigned char stamon_standard_code_data_7[] = {47,47,231,179,187,231,187,159,231,177,187,13,10,13,10,105,109,112,111,114,116,32,115,116,100,100,101,102,59,13,10,105,109,112,111,114,116,32,109,97,116,104,59,13,10,13,10,83,116,97,109,111,110,32,61,32,99,108,97,115,115,32,123,13,10,13,10,32,32,32,32,102,117,110,99,32,116,121,112,101,111,102,40,115,101,108,102,44,32,118,41,32,123,13,10,32,32,32,32,32,32,32,32,47,47,231,187,153,229,174,154,229,175,185,232,177,161,239,188,140,232,191,148,229,155,158,229,133,182,231,177,187,229,158,139,13,10,32,32,32,32,32,32,32,32,114,101,116,117,114,110,32,110,97,116,105,118,101,40,34,116,121,112,101,111,102,34,44,32,118,41,59,13,10,32,32,32,32,125,13,10,13,10,32,32,32,32,102,117,110,99,32,118,101,114,115,105,111,110,40,115,101,108,102,41,32,123,13,10,32,32,32,32,32,32,32,32,47,47,232,191,148,229,155,158,231,137,136,230,156,172,229,143,183,13,10,32,32,32,32,32,32,32,32,114,101,116,117,114,110,32,110,97,116,105,118,101,40,34,118,101,114,115,105,111,110,34,44,32,110,117,108,108,41,59,13,10,32,32,32,32,125,13,10,32,32,32,32,13,10,125,46,110,101,119,59,13,10,13,10,83,121,115,116,101,109,32,61,32,99,108,97,115,115,32,123,13,10,13,10,32,32,32,32,102,117,110,99,32,116,104,114,111,119,40,115,101,108,102,44,32,115,41,32,123,13,10,32,32,32,32,32,32,32,32,47,47,230,138,155,229,135,186,229,188,130,229,184,184,239,188,140,229,144,140,230,151,182,232,174,169,232,153,154,230,139,159,230,156,186,233,128,128,229,135,186,13,10,32,32,32,32,32,32,32,32,114,101,116,117,114,110,32,110,97,116,105,118,101,40,34,116,104,114,111,119,34,44,32,115,41,59,13,10,32,32,32,32,125,13,10,13,10,32,32,32,32,102,117,110,99,32,115,121,115,116,101,109,40,115,101,108,102,44,32,115,41,32,123,13,10,32,32,32,32,32,32,32,32,114,101,116,117,114,110,32,110,97,116,105,118,101,40,34,115,121,115,116,101,109,34,44,32,115,41,59,13,10,32,32,32,32,125,13,10,13,10,32,32,32,32,102,117,110,99,32,101,120,105,116,40,115,101,108,102,44,32,110,41,32,123,13,10,32,32,32,32,32,32,32,32,110,97,116,105,118,101,40,34,101,120,105,116,34,44,32,105,110,116,40,110,41,41,59,13,10,32,32,32,32,125,13,10,13,10,125,46,110,101,119,59,0};
void init_stamon_std_lib_code() {
	stamon_std_lib_code.put((char*)"file.st",(char*)stamon_standard_code_data_0);
	stamon_std_lib_code.put((char*)"math.st",(char*)stamon_standard_code_data_1);
	stamon_std_lib_code.put((char*)"native.st",(char*)stamon_standard_code_data_2);
	stamon_std_lib_code.put((char*)"std.st",(char*)stamon_standard_code_data_3);
	stamon_std_lib_code.put((char*)"stddef.st",(char*)stamon_standard_code_data_4);
	stamon_std_lib_code.put((char*)"stdio.st",(char*)stamon_standard_code_data_5);
	stamon_std_lib_code.put((char*)"string.st",(char*)stamon_standard_code_data_6);
	stamon_std_lib_code.put((char*)"system.st",(char*)stamon_standard_code_data_7);
}
/*
	Name: LineReader.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 03/02/24 16:07
	Description: 行阅读器
*/




#define FILE_ERR { THROW("file opening error") return; }
//这个宏用于简写，并且该宏只能在本文件中使用

ArrayList<StamonString> ImportPaths;

StamonString demo_st_code;

class LineReader {
		int size;
		ArrayList<StamonString> split_texts;
		//按行分割后的文本
	public:
		STMException* ex;
		
		LineReader() {}

		LineReader(StamonString filename, STMException* e) {

			ex = e;

			StamonString text;

			if(filename!="demo.st") {
				if(stamon_std_lib_code.containsKey(filename)==false) {
					FILE_ERR;
				}
				text = stamon_std_lib_code.get(filename);
			} else {
				text = demo_st_code;
			}

			//然后开始逐行分割

			int start = 0, end=0;	//设当前分割的文本为text[start...end]

			while(end<text.length()) {
				if(text[end]=='\n') {
					//分割一行文本
					split_texts.add(text.substring(start, end+1));
					start = end;	//更新start
				}
				end++;
			}

			split_texts.add(text.substring(start, end));	//把最后一行文本写入
		}

		StamonString getLine() {
			//读取一行的文本
			StamonString rst = split_texts[0];
			split_texts.erase(0);
			char* a = (char*)calloc(1,1000);
			return rst;
		}

		bool isMore() {
			return !split_texts.empty();
		}

		void close() {
			;
		}
};

#undef FILE_ERR

/*
	Name: BinarySTVC.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 04/04/25 11:05
	Description: 自动生成的二进制文件数组
*/

char binary_buffer_array[] = {171,219,2,74,0,0,0,18,255,0,0,0,8,95,95,105,110,105,116,95,95,255,0,0,0,6,110,97,116,105,118,101,255,0,0,0,4,112,111,114,116,255,0,0,0,3,97,114,103,255,0,0,0,3,115,116,114,255,0,0,0,1,115,5,0,0,0,3,115,116,114,255,0,0,0,7,112,114,105,110,116,108,110,255,0,0,0,4,116,101,120,116,5,0,0,0,5,112,114,105,110,116,255,0,0,0,3,105,110,116,5,0,0,0,3,105,110,116,255,0,0,0,3,108,101,110,255,0,0,0,1,110,5,0,0,0,3,108,101,110,5,0,0,0,6,83,116,97,109,111,110,5,0,0,0,2,111,110,5,0,0,0,8,67,121,98,101,114,80,105,33,0,0,0,1,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,28,0,0,0,1,0,0,0,6,0,0,0,0,0,0,0,28,0,0,0,2,0,0,0,28,0,0,0,3,0,0,0,7,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,28,0,0,0,2,0,0,0,28,0,0,0,3,255,255,255,255,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,3,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,28,0,0,0,4,0,0,0,6,0,0,0,0,0,0,0,28,0,0,0,5,0,0,0,7,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,1,0,0,0,20,0,0,0,2,0,0,0,21,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,6,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,5,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,28,0,0,0,7,0,0,0,6,0,0,0,0,0,0,0,28,0,0,0,8,0,0,0,7,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,1,0,0,0,20,0,0,0,2,0,0,0,21,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,9,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,4,0,0,0,20,0,0,0,2,0,0,0,21,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,8,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,28,0,0,0,10,0,0,0,6,0,0,0,0,0,0,0,28,0,0,0,5,0,0,0,7,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,1,0,0,0,20,0,0,0,2,0,0,0,21,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,11,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,5,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,28,0,0,0,12,0,0,0,6,0,0,0,0,0,0,0,28,0,0,0,13,0,0,0,7,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,1,0,0,0,20,0,0,0,2,0,0,0,21,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,14,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,13,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,7,0,0,0,20,0,0,0,2,0,0,0,21,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,15,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,7,0,0,0,20,0,0,0,2,0,0,0,21,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,16,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,7,0,0,0,20,0,0,0,2,0,0,0,21,0,0,0,0,0,0,0,15,255,255,255,255,0,0,0,18,255,255,255,255,0,0,0,19,255,255,255,255,0,0,0,28,0,0,0,17,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,0};
char* binary_buffer = binary_buffer_array;
int binary_buffer_len = sizeof(binary_buffer_array);
int allocated_buffer_len = binary_buffer_len;
/*
	Name: BinaryWriter.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/


#define FILE_ERR { THROW("file opening error") return; }


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
/*
	Name: BinaryWriter.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/


#define FILE_ERROR { THROW("file opening error") return; }


class BinaryWriter {
    public:
        STMException* ex;

        BinaryWriter() {}
        BinaryWriter(STMException* e, StamonString filename) {
            ex = e;
            binary_buffer = (char*)malloc(1024);
            allocated_buffer_len = 1024;
        }

        void write(char b) {
            if(binary_buffer_len+1>allocated_buffer_len) {
                binary_buffer = (char*)realloc(binary_buffer, allocated_buffer_len+1024);
                allocated_buffer_len += 1024;
            }
            binary_buffer[binary_buffer_len] = b;
            binary_buffer_len++;
        }

        void close() {
            ;
        }
};

#undef FILE_ERROR
/*
	Name: FileMap.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 05/02/24 20:50
	Description: 以文件为键的map
*/



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
/*
	Name: MemoryPool.cpp
	Copyright: Apache2.0
	Author: CLimber-Rong, copi143
	Date: 10/08/24 23:38
	Description: 内存池实现
*/




template<typename T> void *operator new(size_t size, T *ptr) {
	// 重载placement_new
	return (void *) ptr;
}

class MemoryPool {
	// 内存池

	using byte = bytes_get<1, false>::type;
	// 定义字节类型

	NumberMap<ArrayList<byte *>> free_mem_map;
	// 以空闲内存大小为键，空闲内存列表为值
	NumberMap<void> free_mems;
	// 以空闲内存大小为键和值
	// 用于快速得知free_mem_map的值有哪些
	int FreeMemConsumeSize = 0;
	// 目前空闲内存总大小为多少
	int PoolCacheSize;
	// 内存池缓存大小，如果超过限制则把空闲内存全部释放
	STMException *ex;

public:
	MemoryPool() {
	}

	MemoryPool(STMException *e, int mem_limit, int pool_cache_size) {
		ex = e;
		PoolCacheSize = pool_cache_size;
	}

	template<typename T, typename... Types> T *NewObject(Types &&...args) {
		if (PoolCacheSize <= 0) {
			// 无缓存，直接使用new
			return new T(args...);
		} else {
			// 使用内存池
			byte *ptr;

			if (free_mem_map.containsKey(sizeof(T))) {
				ArrayList<byte *> *freelist = free_mem_map.get(sizeof(T));
				if (!freelist->empty()) {
					// 有空闲内存
					ptr = freelist->at(freelist->size() - 1);
					freelist->erase(freelist->size() - 1);
					FreeMemConsumeSize -= sizeof(T);

					new ((T *) ptr) T(args...);
					return (T *) ptr;
				}
			}

			ptr = (byte *) malloc(sizeof(T));

			new ((T *) ptr) T(args...);
			return (T *) ptr;
		}
	}

	void ClearAllFreeMem() {
		ArrayList<void *> frees = free_mems.getValList<void *>();

		for (int i = 0, len = frees.size(); i < len; i++) {
			// 遍历所有空闲内存大小
			ArrayList<byte *> *list = free_mem_map.get((size_t) (frees[i]));
			while (!list->empty()) {
				// 清空列表中所有元素
				free(list->at(list->size() - 1));
				list->erase(list->size() - 1);
				FreeMemConsumeSize -= (size_t) frees[i];
			}
		}

		return;
	}

	template<typename T> void DeleteObject(T *object) {
		if (PoolCacheSize <= 0) {
			// 无缓存，直接使用delete
			delete object;
			return;
		} else {
			object->~T();

			byte *ptr = (byte *) object;

			if (PoolCacheSize == 0) {
				// 直接释放
				free(ptr);
				return;
			}

			if (FreeMemConsumeSize + sizeof(T) > PoolCacheSize) {
				// 释放所有空闲内存
				ClearAllFreeMem();
			}

			if (!free_mem_map.containsKey(sizeof(T))) {
				// 新建ArrayList
				free_mem_map.put(sizeof(T), new ArrayList<byte *>());
			}

			free_mem_map.get(sizeof(T))->add(ptr);
			free_mems.put(sizeof(T), (void *) sizeof(T));

			FreeMemConsumeSize += sizeof(T);

			return;
		}
	}
};
/*
        Name: Ast
        Copyright: Apache 2.0
        Author: CLimber-Rong
        Date: 28/07/23 16:44
        Description:
        * 这里对Ast进行了基本的定义
        * 如果你想要引用所有的Ast源码，只要写入以下代码即可：
        using namespace stamon::ast;
*/



namespace stamon {
	namespace ast {
		// Ast类型定义，详细作用见doc/虚拟机AST设计.md
		enum _AstType {
		    AstNodeType = 0,
		    AstProgramType,
		    AstDefClassType,
			AstDefFuncType,
			AstDefVarType,
		    AstAnonClassType,
		    AstAnonFuncType,
		    AstBlockType,
		    AstBreakType,
			AstContinueType,
		    AstIfStatementType,
		    AstWhileStatementType,
		    AstForStatementType,
		    AstReturnStatementType,
		    AstSFNType,
		    AstExpressionType,
		    AstLeftValueType,
		    AstLeftPostfixType,
		    AstBinaryType,
		    AstUnaryType,
		    AstPostfixType,
			AstArgumentsType,
		    AstIdentifierType,
		    AstNumberType,
		    AstStringType,
		    AstNullType,
		    AstArrayLiteralType,
		    AstListLiteralType,
			AstLeafType,	//见AstIR.cpp
		    AstTypeNum // Ast总数
		};

		// Ast类定义，详细实现见同目录的其他源码
		class AstNode;
		class AstProgram;
		class AstDefClass;
		class AstDefFunc;
		class AstAnonClass;
		class AstAnonFunc;
		class AstBlock;
		class AstBreak;
		class AstContinue;
		class AstIfStatement;
		class AstWhileStatement;
		class AstForStatement;
		class AstReturnStatement;
		class AstSFN;
		class AstExpression;
		class AstLeftValue;
		class AstLeftPostfix;
		class AstBinary;
		class AstUnary;
		class AstPostfix;
		class AstArguments;
		class AstIdentifier;
		class AstNumber;
		class AstString;
		class AstNull;
		class AstArrayLiteral;
		class AstListLiteral;

		class AstNode {
			protected:
				ArrayList<AstNode*>* children;

			public:
				int lineNo;			//语法分析时用于显示行号
				StamonString filename;	//语义分析时用于显示文件名

				AstNode() {
					//构造函数
					children = new ArrayList<AstNode*>();
				}

				AstNode(int line) {
					//构造函数
					children = new ArrayList<AstNode*>();
					lineNo = line;
				}

				virtual int ChildrenNum() {
					//子节点数量
					return children->size();
				}

				virtual ArrayList<AstNode*> *Children() {
					//获得子节点列表
					return children;
				}

				virtual int getType() {
					//获得节点类型
					return AstNodeType;
				}

				virtual ~AstNode() {
					//删除该节点及其所有字节点
					for(int i=0;i<children->size();i++) {
						delete children->at(i);
					}
					delete children;
				}
		};
	}
}

/*
	Name: LeafAst.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 29/07/23 20:14
	Description: 语法树叶子节点的定义
*/



namespace stamon::ast {
	class AstIdentifier : public AstNode {
			unsigned long long id;
		public:
			AstIdentifier(unsigned long long iden_id) : AstNode() {
				id = iden_id;
			}
			virtual int getType() {
				return AstIdentifierType;
			}
			virtual unsigned long long getID() {
				return id;
			}
			virtual ~AstIdentifier() = default;
	};
	/*
	 * 这里需要解释一下
	 * 由于虚拟机的设计，需要定义两种标识符节点
	 * AstIdentifierName存储着标识符的名字（字符串）
	 	* 这个节点并不参与虚拟机的运行，而是参与目标代码的编译
		* 如果你是来阅读虚拟机实现代码的，请不要理会这个节点
	 * AstIdentifier存储着标识符的编号，这个节点用作运行
	 */
	class AstIdentifierName : public AstIdentifier {
			StamonString name;
		public:
			AstIdentifierName(const StamonString& id) : AstIdentifier(-1) {
				name = id;
			}
			virtual int getType() {
				return AstIdentifierType;
			}
			virtual StamonString getName() {
				return name.getstr();
			}
			virtual ~AstIdentifierName() = default;
	};

	/*
	 * 在 虚拟机AST设计.md 里，你会看到一个名为“NUMBER”的节点
	 * 由于数字字面量有很多种（例如整数、浮点数）
	 * 所以我先编写了AstNumber作为数字字面量的基类，所有的数字字面量节点都继承这个基类
	 */

	enum _NumberType {
	    IntNumberType,
	    FloatNumberType,
	    DoubleNumberType
	};

	class AstNumber : public AstNode {
		protected:
			int number_type;
		public:
			virtual int getType() {
				return AstNumberType;
			}
			virtual int getNumberType() {
				return number_type;
			}
			virtual ~AstNumber() = default;
	};

	class AstIntNumber : public AstNumber {
			int value;
		public:
			AstIntNumber(int val) : AstNumber() {
				number_type = IntNumberType;
				value = val;
			}
			virtual int getVal() {
				return value;
			}
			virtual ~AstIntNumber() = default;
	};

	class AstFloatNumber : public AstNumber {
			float value;
		public:
			AstFloatNumber(float val) : AstNumber() {
				number_type = FloatNumberType;
				value = val;
			}
			virtual float getVal() {
				return value;
			}
			virtual ~AstFloatNumber() = default;
	};

	class AstDoubleNumber : public AstNumber {
			double value;
		public:
			AstDoubleNumber(double val) : AstNumber() {
				number_type = DoubleNumberType;
				value = val;
			}
			virtual double getVal() {
				return value;
			}
			virtual ~AstDoubleNumber() = default;
	};


	class AstString : public AstNode {
			StamonString val;
		public:
			AstString(const StamonString& str) : AstNode() {
				val = str;
			}
			virtual int getType() {
				return AstStringType;
			}
			virtual StamonString getVal() {
				return val;
			}
			virtual ~AstString() = default;
	};

	class AstNull : public AstNode {
		public:
			virtual int getType() {
				return AstNullType;
			}
	};

	class AstBreak : public AstNode {
		public:
			AstBreak() : AstNode() {};
			virtual int getType() {
				return AstBreakType;
			}
	};

	class AstContinue : public AstNode {
		public:
			AstContinue() : AstNode() {};
			virtual int getType() {
				return AstContinueType;
			}
	};
}
/*
    Name: CodeLogicAst.cpp
    Copyright: Apache 2.0
    Author: CLimber-Rong
    Date: 28/07/23 15:45
    Description: 在这里写代码逻辑（如函数、流程控制等）节点的定义
*/



namespace stamon::ast {
	class AstProgram : public AstNode {

		public:
			AstProgram() : AstNode() {}

			AstProgram(ArrayList<AstNode*>* statements) : AstNode() {
				children = statements;
			}
			virtual int getType() {
				return AstProgramType;
			}
	};
	class AstDefClass : public AstNode {

		public:
			AstDefClass() : AstNode() {}

			AstDefClass(AstIdentifier* iden, AstAnonClass* object_class) : AstNode() {
				children->add((AstNode*)iden);
				children->add((AstNode*)object_class);
			}
			virtual int getType() {
				return AstDefClassType;
			}
	};
	class AstDefFunc : public AstNode {

		public:

			AstDefFunc() : AstNode() {}

			AstDefFunc(AstIdentifier* iden, AstAnonFunc* func) : AstNode() {
				children->add((AstNode*)iden);
				children->add((AstNode*)func);
			}

			virtual int getType() {
				return AstDefFuncType;
			}
	};
	class AstDefVar : public AstNode {

		public:

			AstDefVar() : AstNode() {}

			AstDefVar(AstIdentifier* iden, AstExpression* expr) : AstNode() {
				children->add((AstNode*)iden);
				children->add((AstNode*)expr);
			}

			virtual int getType() {
				return AstDefVarType;
			}
	};
	class AstAnonClass : public AstNode {

		public:
			bool isHaveFather = false;

			AstAnonClass() : AstNode() {}

			AstAnonClass(AstIdentifier* father, ArrayList<AstNode*>* expr) : AstNode() {
				children = expr;
				if(father!=NULL) {
					isHaveFather = true;
					children->insert(0, (AstNode*)father);
				}
			}
			virtual int getType() {
				return AstAnonClassType;
			}

			virtual ~AstAnonClass() = default;
	};
	class AstAnonFunc : public AstNode {

		public:

			AstAnonFunc() : AstNode() {}

			AstAnonFunc(ArrayList<AstNode*>* args, AstBlock* block) : AstNode() {
				children = args;
				children->add((AstNode*)block);
			}
			virtual int getType() {
				return AstAnonFuncType;
			}
	};
	class AstBlock : public AstNode {

		public:

			AstBlock() : AstNode() {}

			AstBlock(ArrayList<AstNode*>* statements) : AstNode() {
				children = statements;
			}
			virtual int getType() {
				return AstBlockType;
			}
	};
	class AstIfStatement : public AstNode {

		public:

			AstIfStatement() : AstNode() {}

			AstIfStatement(AstExpression* expr, AstBlock* block_if) : AstNode() {
				children->add((AstNode*)expr);
				children->add((AstNode*)block_if);
			}
			AstIfStatement(AstExpression* expr, AstBlock* block_if, AstBlock* block_else) : AstNode() {
				children->add((AstNode*)expr);
				children->add((AstNode*)block_if);
				children->add((AstNode*)block_else);
			}
			virtual int getType() {
				return AstIfStatementType;
			}
	};
	class AstWhileStatement : public AstNode {

		public:

			AstWhileStatement() : AstNode() {}

			AstWhileStatement(AstExpression* expr, AstBlock* block_while) : AstNode() {
				children->add((AstNode*)expr);
				children->add((AstNode*)block_while);
			}
			virtual int getType() {
				return AstWhileStatementType;
			}
	};
	class AstForStatement : public AstNode {

		public:

			AstForStatement() : AstNode() {}

			AstForStatement(AstIdentifier* iden, AstExpression* expr, AstBlock* block_for) : AstNode() {
				children->add((AstNode*)iden);
				children->add((AstNode*)expr);
				children->add((AstNode*)block_for);
			}
			virtual int getType() {
				return AstForStatementType;
			}
	};
	class AstReturnStatement : public AstNode {

		public:

			AstReturnStatement() : AstNode() {}

			AstReturnStatement(AstExpression* expr) : AstNode() {
				children->add((AstNode*)expr);
			}
			virtual int getType() {
				return AstReturnStatementType;
			}
	};
}
/*
	Name: ExprAst.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 28/07/23 15:48
	Description: 在这里定义与表达式有关的Ast节点
*/



namespace stamon::ast {
	enum _PostfixType {
	    //后缀类型定义
	    PostfixMemberType = 0, //成员后缀
	    PostfixElementType,	   //下标后缀
	    PostfixCallType,	   //调用函数后缀
	    PostfixNewType		   //新建对象后缀
	};
	enum _BinaryOperatorType {
	    //双目运算符类型定义
	    BinaryLogicORType = 0,	   //逻辑或
	    BinaryLogicANDType,		   //逻辑与
	    BinaryBitORType,		   //按位或
	    BinaryBitXORType,		   //按位异或
	    BinaryBitANDType,		   //按位与
	    BinaryEqualityType,		   //是否相等
	    BinaryInequalityType,	   //是否不相等
	    BinaryBigThanType,		   //大于
	    BinaryLessThanType,		   //小于
	    BinaryBigThanOrEqualType,  //大等于
	    BinaryLessThanOrEqualType, //小等于
	    BinaryLeftShiftType,	   //左移位
	    BinaryRightShiftType,	   //右移位
	    BinaryAddType,			   //加法
	    BinarySubType,			   //减法
	    BinaryMultType,			   //乘法
	    BinaryDiviType,			   //除法
	    BinaryModType,			   //取余
		BinaryTypeCount			   //类型总数
	};
	enum _UnaryOperatorType {
	    //单目运算符类型定义
	    UnaryPositiveType = 0, //正
	    UnaryNegativeType,		   //负
	    UnaryNotType,		   //非
	    UnaryInverseType,	   //按位反
		UnaryTypeCount		   //类型总数
	};

	class AstExpression : public AstNode {

		public:
			int ass_type;

			AstExpression() : AstNode() {}

			AstExpression(AstLeftValue* LeftValue, int AssTok, AstExpression* expr) : AstNode() {
				children->add((AstNode*)LeftValue);
				children->add((AstNode*)expr);
				ass_type = AssTok;
			}
			AstExpression(AstBinary* value) : AstNode() {
				children->add((AstNode*)value);
				ass_type = -1;
			}
			virtual int getType() {
				return AstExpressionType;
			}
			virtual ~AstExpression() = default;

	};
	class AstLeftValue : public AstNode {

		public:

			AstLeftValue() : AstNode() {}

			AstLeftValue(AstIdentifier* iden, ArrayList<AstNode*>* postfix) : AstNode() {
				children = postfix;
				children->insert(0,(AstNode*)iden);
			}
			virtual int getType() {
				return AstLeftValueType;
			}
	};
	class AstLeftPostfix : public AstNode {
		public:
			int postfix_type;
			virtual int getPostfixType() {
				return postfix_type;
			}

			AstLeftPostfix() : AstNode() {}

			AstLeftPostfix(int PostfixType, AstNode* value) : AstNode() {
				postfix_type = PostfixType;
				children->add((AstNode*)value);
			}
			virtual int getType() {
				return AstLeftPostfixType;
			}
			virtual ~AstLeftPostfix() = default;
	};
	class AstBinary : public AstNode {
		public:
			int operator_type;
			virtual int getOperatorType() {
				return operator_type;
			}

			AstBinary() : AstNode() {}

			AstBinary(int OperatorType, AstNode* left, AstNode* right) : AstNode() {
				operator_type = OperatorType;
				children->add((AstNode*)left);
				children->add((AstNode*)right);
			}
			AstBinary(AstNode* left) : AstNode() {
				operator_type = -1;
				children->add((AstNode*)left);
			}
			virtual int getType() {
				return AstBinaryType;
			}
			virtual ~AstBinary() = default;
	};
	class AstUnary : public AstNode {
		public:
			int operator_type;
			virtual int getOperatorType() {
				return operator_type;
			}
			AstUnary() : AstNode() {}
			AstUnary(int OperatorType, AstNode* value) : AstNode() {
				operator_type = OperatorType;
				children->add((AstNode*)value);
			}
			AstUnary(AstNode* value, ArrayList<AstNode*>* postfix) : AstNode() {
				operator_type = -1;
				children = postfix;
				children->insert(0, (AstNode*)value);
			}
			virtual int getType() {
				return AstUnaryType;
			}
			virtual ~AstUnary() = default;
	};
	class AstPostfix : public AstNode {

		public:
			int postfix_type;
			virtual int getPostfixType() {
				return postfix_type;
			}
			AstPostfix() : AstNode() {}
			AstPostfix(int PostfixType, AstNode* value) : AstNode() {
				postfix_type = PostfixType;
				children->add((AstNode*)value);
			}
			virtual int getType() {
				return AstPostfixType;
			}
			virtual ~AstPostfix() = default;
	};
	class AstArguments : public AstNode {

		public:

			AstArguments() : AstNode() {}

			AstArguments(ArrayList<AstNode*>* exprs) : AstNode() {
				children = exprs;
			}
			virtual int getType() {
				return AstArgumentsType;
			}
	};
	class AstArrayLiteral : public AstNode {

		public:

			AstArrayLiteral() : AstNode() {}

			AstArrayLiteral(AstExpression* expr) : AstNode() {
				children->add((AstNode*)expr);
			}
			virtual int getType() {
				return AstArrayLiteralType;
			}
	};
	class AstListLiteral : public AstNode {

		public:

			AstListLiteral() : AstNode() {}

			AstListLiteral(ArrayList<AstNode*>* exprs) : AstNode() {
				children = exprs;
			}
			virtual int getType() {
				return AstListLiteralType;
			}
	};

}
/*
	Name: SfnAst.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 01/08/23 19:55
	Description: SFN节点的基本定义
*/



namespace stamon::ast {
	class AstSFN : public AstNode {
		public:
			AstSFN() : AstNode() {}
			AstSFN(AstIdentifier* port, AstIdentifier* result) : AstNode() {
				children->add((AstNode*)port);
				children->add((AstNode*)result);
			}
			virtual int getType() {
				return AstSFNType;
			}
	};
}
/*
	Name: StamonConfig.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 18:21
	Description: 用于存储stamon的公共配置
*/


// stamon的版本号
namespace stamon {
constexpr int STAMON_VER_X = 2;
constexpr int STAMON_VER_Y = 4;
constexpr int STAMON_VER_Z = 43;
} // namespace stamon

namespace stamon::config {

enum STAMON_WARNING_SAFE_LEVEL {
	// 警告等级
	StamonWarningSafeLevel_IgnoreWarning = 0, // 忽略警告
	StamonWarningSafeLevel_JustWarn, // 只警告，程序继续运行
	StamonWarningSafeLevel_FatalWarning // 将警告视为致命错误
};

}

/*
	Name: CompilerConfig.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 18:19
	Description: 此文件用于存储编译器的相关配置信息
*/



namespace stamon::c::config {

// 警告的重要程度
int WarningLevel = stamon::config::StamonWarningSafeLevel_JustWarn;
// 默认的目标文件名
StamonString DefaultObjectFileName = StamonString("a.stvc");
// 是否支持import
bool isSupportImport = true;
// 是否剥削调试信息
bool isStrip = false;

} // namespace stamon::c::config
/*
	Name: VmConfig.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 18:35
	Description: 此文件用于存储虚拟机的相关配置信息
*/



namespace stamon::vm::config {

// 警告的重要程度
int WarningLevel = stamon::config::StamonWarningSafeLevel_JustWarn;
// 是否启用垃圾回收
bool isGC = true;
// 虚拟机对象占用的内存限制
int MemLimit = 16 * 1024 * 1024;
// 内存池的缓存大小
int PoolCacheSize = 16 * 1024 * 1024;

} // namespace stamon::vm::config
/*
	Name: DataType.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 14/08/23 10:08
	Description:
        * 这里对数据类型进行了基本的定义
        * 如果你想要引用所有的数据类型源码，只要写入以下代码即可：
        using namespace stamon::datatype;
*/


namespace stamon::datatype {
	enum _DataTypeID {
	    DataTypeID = 0,
	    NullTypeID,
	    IntegerTypeID,
	    FloatTypeID,
	    DoubleTypeID,
	    StringTypeID,
	    SequenceTypeID,
	    ClassTypeID,
	    MethodTypeID,
	    ObjectTypeID,
		DataTypeCount
	};

	class DataType;
	class Variable;
	class NullType;
	class IntegerType;
	class FloatType;
	class DoubleType;
	class StringType;
	class SequenceType;
	class ClassType;
	class MethodType;
	class ObjectType;

	class DataType {
			int type;
		public:
			bool gc_flag = false;	//用于在GC时标记这个值是否被遍历到

			DataType(int type_id) {
				type = type_id;
			}
			virtual int getType() const {
				return type;
			}
			virtual ~DataType() = default;	//等价于~DataType() {}
	};
} //namespace stamon::datatype

/*
	Name: Variable.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 01/12/23 19:52
	Description: 变量类，Variable类存储着一个DataType*数据（也就是值）
            * 当需要给每个变量（包括数组元素等）赋值时，只需改变DataType*数据即可
*/



namespace stamon::datatype {
	class Variable {
		public:
			DataType* data;
			Variable() {}
			Variable(const Variable& right) {
				data = right.data;
			}
			Variable(DataType* dt) {
				data = dt;
			}
			virtual ~Variable() = default;
	};
} //namespace stamon::datatype
/*
	Name: ClassType.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 14/08/23 11:54
	Description:
    * 在STVM中，类本身也是一个值，它对应的数据类型就是ClassType
    * 这里就写上了ClassType的定义
*/



namespace stamon::datatype {
	class ClassType : public DataType {
			ast::AstAnonClass* val;
		public:
			ClassType(ast::AstAnonClass* value) : DataType(ClassTypeID) {
				val = value;
			}
			virtual ast::AstAnonClass* getVal() const {
				return val;
			}
			virtual ~ClassType() = default;
	};
} //namespace stamon::datatype
/*
	Name: MethodType.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 14/08/23 11:57
	Description:
    * 在STVM中，方法本身也是一个值，它对应的数据类型就是MethodType
    * 这里就写上了MethodType的定义
*/



namespace stamon::datatype {
	class MethodType : public DataType {
			ast::AstAnonFunc* val;
		public:
			ObjectType* container; //容器
			int id;	//函数的名字，如果是匿名函数，则该值为-1
			MethodType(
			    int iden, ast::AstAnonFunc* value,
			    ObjectType* father
			) : DataType(MethodTypeID) {
				id = iden;
				val = value;
				container = father;
			}
			virtual ast::AstAnonFunc* getVal() const {
				return val;
			}
			virtual ObjectType* getContainer() const {
				return container;
			}
			virtual ~MethodType() = default;
	};
} //namespace stamon::datatype
/*
	Name:
	Copyright:
	Author:
	Date: 14/08/23 10:10
	Description: 空值数据类型定义
*/



namespace stamon::datatype {
	class NullType : public DataType {
		public:
			NullType() : DataType(NullTypeID) {}
			virtual ~NullType() = default;
	};
} //namespace stamon::datatype
/*
	Name: NumberType.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 14/08/23 10:09
	Description: 数字类型的定义，包括整数和浮点数
*/



namespace stamon::datatype {
	class IntegerType : public DataType {
			int val;
		public:
			IntegerType(int value) : DataType(IntegerTypeID) {
				val = value;
			}
			virtual int getVal() const {
				return val;
			}
			virtual ~IntegerType() = default;
	};

	class FloatType : public DataType {
			float val;
		public:
			FloatType(float value) : DataType(FloatTypeID) {
				val = value;
			}
			virtual float getVal() {
				return val;
			}
			virtual FloatType toThisType(DataType src) {
				//这个函数用于把低等数据类型转为高等数据类型
				//由于比float低等的只有int，所以直接把int转float就可以了
				IntegerType src_int = cast_class(IntegerType, src);
				int src_val = src_int.getVal();
				FloatType result((float)src_val);
				return result;
			}
			virtual ~FloatType() = default;
	};

	class DoubleType : public DataType {
			double val;
		public:
			DoubleType(double value) : DataType(DoubleTypeID) {
				val = value;
			}
			virtual double getVal() {
				return val;
			}
			virtual DoubleType toThisType(DataType src) {
				if(src.getType()==IntegerTypeID) {
					//整数转双精度浮点数
					IntegerType src_int = cast_class(IntegerType, src);
					int src_val = src_int.getVal();
					DoubleType result((double)src_val);
					return result;
				} else {
					//单精度浮点数转双精度浮点数
					FloatType src_float = cast_class(FloatType, src);
					int src_val = src_float.getVal();
					DoubleType result((double)src_val);
					return result;
				}
			}
			virtual ~DoubleType() = default;
	};
} //namespace stamon::datatype
/*
	Name: ObjectType.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 17/08/23 11:49
	Description:
    * 在STVM中，新建的类对象也是一个值，它对应的数据类型就是ObjectType
    * 这里就写上了ObjectType的定义
*/



namespace stamon::datatype {
	class ObjectType : public DataType {
			NumberMap<Variable> vals;  
			//由于STVM内部以数字来代替标识符，所以采用NumberMap
		public:
			ObjectType(NumberMap<Variable> value)
				: DataType(ObjectTypeID), vals(NumberMap<Variable>()) {
				vals = value;
			}
			virtual NumberMap<Variable> getVal() const {
				return vals;
			}
			virtual ~ObjectType() {
				ArrayList<Variable*> vars = vals.getValList<Variable*>();
				for(int i=0;i<vars.size();i++) {
					delete vars[i];
				}
			}
	};

} //namespace stamon::datatype
/*
	Name: SequenceType.cpp
	Copyright: Apache 2.0
	Author: Climber-Rong
	Date: 14/08/23 10:19
	Description: 数列数据类型的定义
    * 也许你会对这种数据类型的名字感到困惑
    * 数列其实是数组和列表的结合
    * 数列可以在创建的时候就初始化长度，这点与类似Python的列表不同
    * 数列也可以动态的改变长度，这点与类似C++的数组不同
    * 数列这种数据结构很大程度上使得这个虚拟机可以兼容更多的语言，而且它的实现也并非难事
*/



namespace stamon::datatype {
	class SequenceType : public DataType {
		public:
			ArrayList<Variable*> sequence;
			SequenceType(int length) 
			: DataType(SequenceTypeID), sequence(ArrayList<Variable*>(length)) {
			}
			SequenceType(ArrayList<Variable*> value) : DataType(SequenceTypeID) {
				sequence = value;
			}
			virtual ArrayList<Variable*> getVal() const {
				return sequence;
			}
			virtual ~SequenceType() {
				for(int i=0;i<sequence.size();i++) {
					delete sequence[i];
				}
			}
	};
} //namespace stamon::datatype
/*
	Name: StringType.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 14/08/23 10:10
	Description: 字符串类型的定义
*/



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
/*
	Name: ASTBytecode.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 09/02/24 08:48
	Description: ast::Ast-IR生成器
*/



//为了方便，我定义了宏
//这些宏只能在本文件中使用

#define CHECK_SPECIAL_AST(type, ast_data) \
	if(top->getType()==type##Type) {\
		rst.data = ((type*)top)->ast_data;\
	}

#define CHECK_IR(ast_type) \
	if(ir[i].type==ast_type##Type) {\
		n = new ast_type();\
		n->lineNo = ir[i].lineNo;\
		n->filename = ir[i].filename;\
	}

#define CHECK_SPECIAL_IR(ast_type, ast_data) \
	if(ir[i].type==ast_type##Type) {\
		n = (ast::AstNode*)new ast_type();\
		n->lineNo = ir[i].lineNo;\
		n->filename = ir[i].filename;\
		((ast_type*)n)->ast_data = ir[i].data;\
	}

namespace stamon::ir {

	class AstIR {
		public:
			int type;
			/*
			 * 一些特别的规定
			 * 当type为-1，代表这是一个结束符，即</>
			 */
			int data;
			//如果这个IR是字面量或标识符，则data存储着其在常量表中的下标
			//否则存储这个IR的具体信息（例如运算符类型）
			StamonString filename;	//IR所在的文件名
			int lineNo;	//IR所在的行号
	};

	enum {
	    IdenConstTypeID = -1
	};

	class IdenConstType : public datatype::DataType {
			//我需要在生成常量表时将标识符和数据类型统一
			//所以我建立了IdenConstType
			StamonString iden;
		public:
			IdenConstType(const StamonString& value) : datatype::DataType(-1) {
				iden = value;
			}
			virtual StamonString getVal() const {
				return iden;
			}
	};

	class AstLeaf : public ast::AstNode {
			int index;	//常量表下标
		public:
			AstLeaf(int data) : AstNode() {
				index = data;
			}
			virtual int getVal() const {
				return index;
			}
			virtual int getType() {
				return ast::AstLeafType;
			}
			virtual ~AstLeaf() = default;
	};

	class AstIRConverter {
		public:

			ArrayList<datatype::DataType*> tableConst;
			ByteMap<void> tableConstFloat;
			ByteMap<void> tableConstDouble;
			NumberMap<void> tableConstInt;
			StringMap<void> tableConstString;
			StringMap<void> tableConstIden;
			/*
			 * 这里需要解释一下
			 * 以上是不同类型的常量表
			 * 无论是ByteMap，抑或是NumberMap、StringMap
			 * 如果设置模板类型为void，即代表该map的值的类型为void*（详见库的定义）
			 * 这些void*里存的是整数，即键在tableConst当中的下标
			 * 举个例子：
				 * 遇到整数常量1145
				 * 以1145为键，在tableConstInt中查找值
				 * 假设查到值为14
				 * 则代表tableConst[14]是一个IntegerType，且值为1145
			 */

			int createConst(datatype::DataType* dt) {
				/*
				 * 新建一个常量，常量的值为dt
				 * 在调用这个函数前，请确保dt不在常量表中
				 * 返回新建后，该常量在常量表中的下标
				 */
				tableConst.add(dt);

				int index = tableConst.size()-1;

				if(dt->getType()==datatype::IntegerTypeID) {
					datatype::IntegerType* dtInt = (datatype::IntegerType*)dt;
					tableConstInt.put(dtInt->getVal(), *(void**)&index);
					//*(void**)&index等价于(void*)index
				}

				if(dt->getType()==datatype::FloatTypeID) {
					datatype::FloatType* dtFlt = (datatype::FloatType*)dt;

					float val = dtFlt->getVal();
					char* key = (char*)&val;

					tableConstFloat.put(
					    key, sizeof(float), *(void**)&index
					);
				}

				if(dt->getType()==datatype::DoubleTypeID) {
					datatype::DoubleType* dtDb = (datatype::DoubleType*)dt;

					double val = dtDb->getVal();
					char* key = (char*)&val;

					tableConstFloat.put(
					    key, sizeof(double), *(void**)&index
					);
				}

				if(dt->getType()==datatype::StringTypeID) {
					datatype::StringType* dtStr = (datatype::StringType*)dt;

					StamonString key = dtStr->getVal();

					tableConstString.put(key, *(void**)&index);
				}

				if(dt->getType()==-1) {
					//新建的是标识符
					IdenConstType* dtIden = (IdenConstType*)dt;

					StamonString key = dtIden->getVal();

					tableConstIden.put(
					    key, *(void**)&index
					);
				}

				return index;
			}

			int searchConst(datatype::DataType* dt) {
				//查找dt在常量表当中的下标
				//如果dt不在常量表中，则返回-1

				if(dt->getType()==datatype::IntegerTypeID) {
					datatype::IntegerType* dtInt = (datatype::IntegerType*)dt;
					if(!tableConstInt.containsKey(dtInt->getVal())) {
						//不在常量表中
						return -1;
					}
					return (long long)tableConstInt.get(dtInt->getVal());
				}

				if(dt->getType()==datatype::FloatTypeID) {
					datatype::FloatType* dtFlt = (datatype::FloatType*)dt;

					float val = dtFlt->getVal();
					char* key = (char*)&val;

					if(!tableConstFloat.containsKey(key, sizeof(float))) {
						return -1;
					}
					return (long long)tableConstFloat.get(
					           key, sizeof(float)
					       );
				}

				if(dt->getType()==datatype::DoubleTypeID) {
					datatype::DoubleType* dtDb = (datatype::DoubleType*)dt;

					double val = dtDb->getVal();
					char* key = (char*)&val;

					if(!tableConstDouble.containsKey(key, sizeof(double))) {
						return -1;
					}
					return (long long)tableConstDouble.get(
					           key, sizeof(double)
					       );
				}

				if(dt->getType()==datatype::StringTypeID) {
					datatype::StringType* dtStr = (datatype::StringType*)dt;

					StamonString key = dtStr->getVal();

					if(!tableConstString.containsKey(key)) {
						return -1;
					}
					return (long long)tableConstString.get(key);
				}

				if(dt->getType()==-1) {
					//查找的是标识符
					IdenConstType* dtIden = (IdenConstType*)dt;

					StamonString key = dtIden->getVal();

					if(!tableConstIden.containsKey(key)) {
						return -1;
					}
					return (long long)tableConstIden.get(key);
				}

				return -1;
			}

			ArrayList<AstIR> ast2ir(ast::AstNode* program) {

				Stack<ast::AstNode> stack;
				ArrayList<AstIR> list;

				if(program==NULL) {
					return list;
				}

				//先把__init__放入常量表
				//这也就意味着__init__始终在常量表的第一位
				createConst(new IdenConstType(StamonString((char*)"__init__")));

				stack.push(program);

				//迭代遍历语法树，编译成AstIR

				while(stack.empty()==false) {
					bool isLeafNode = false;
					AstIR rst;
					ast::AstNode* top = stack.pop();	//弹出栈顶

					rst.type = top->getType();
					rst.data = 0;	//默认为0
					rst.lineNo = top->lineNo;
					rst.filename = top->filename;


					//先特判一些节点
					CHECK_SPECIAL_AST(ast::AstAnonClass, isHaveFather)
					CHECK_SPECIAL_AST(ast::AstExpression, ass_type)
					CHECK_SPECIAL_AST(ast::AstLeftPostfix, getPostfixType())
					CHECK_SPECIAL_AST(ast::AstBinary, getOperatorType())
					CHECK_SPECIAL_AST(ast::AstUnary, getOperatorType())
					CHECK_SPECIAL_AST(ast::AstPostfix, getPostfixType())
					//特判结束符
					if(top->getType()==ast::AstNodeType) {
						isLeafNode = true;
						rst.type = -1;
						rst.lineNo = -1;
						rst.filename = StamonString((char*)"");
					}
					//特判叶子节点

					if(
					    top->getType() == ast::AstNumberType
					    ||top->getType()==ast::AstStringType
					    ||top->getType()==ast::AstIdentifierType
					) {
						isLeafNode = true;
						rst.type = ast::AstLeafType;	//统一代表叶子节点
					}

					if(top->getType()==ast::AstIdentifierType) {

						StamonString iden = ((ast::AstIdentifierName*)top)->getName();
						IdenConstType* dtIden = new IdenConstType(iden);

						int index;	//常量表下标

						index = searchConst(dtIden);

						if(index==-1) {
							index = createConst(dtIden);
						}

						rst.data = index;
					}

					if(top->getType()==ast::AstStringType) {

						StamonString s = ((ast::AstString*)top)->getVal();
						datatype::StringType* dt = new datatype::StringType(s);

						int index;	//常量表下标

						index = searchConst(dt);

						if(index==-1) {
							index = createConst(dt);
						}

						rst.data = index;
					}

					if(top->getType()==ast::AstNumberType) {

						int num_type = ((ast::AstNumber*)top)->getNumberType();
						if(num_type==ast::IntNumberType) {
							int n = ((ast::AstIntNumber*)top)->getVal();
							datatype::IntegerType* dt
							    = new datatype::IntegerType(n);

							int index;	//常量表下标

							index = searchConst(dt);

							if(index==-1) {
								index = createConst(dt);
							}

							rst.data = index;
						}
						if(num_type==ast::FloatNumberType) {
							float n = ((ast::AstFloatNumber*)top)->getVal();
							datatype::FloatType* dt
							    = new datatype::FloatType(n);

							int index;	//常量表下标

							index = searchConst(dt);

							if(index==-1) {
								index = createConst(dt);
							}

							rst.data = index;
						}
						if(num_type==ast::DoubleNumberType) {
							double n = ((ast::AstDoubleNumber*)top)->getVal();
							datatype::DoubleType* dt
							    = new datatype::DoubleType(n);

							int index;	//常量表下标

							index = searchConst(dt);

							if(index==-1) {
								index = createConst(dt);
							}

							rst.data = index;
						}
					}

					//将AST IR存入列表

					list.add(rst);

					if(isLeafNode==false) {
						//压入终结符
						stack.push(new ast::AstNode(rst.lineNo));

						//接着遍历子节点
						for(int i=top->Children()->size()-1; i>=0; i--) {
							stack.push(top->Children()->at(i));
						}

					}

				}

				return list;
			}

			ast::AstNode* ir2ast(ArrayList<AstIR> ir) {
				/*
				 * AstIR转Ast
				 * 如果ir不正确，程序会运行时错误
				 * 所以请在运行该函数前检查ir
				 */

				Stack<ast::AstNode> stack;

				ast::AstNode* rst;

				for(int i=0,len=ir.size(); i<len; i++) {
					if(ir[i].type==-1) {
						//结束符
						stack.pop();
						continue;
					}
					if(ir[i].type==ast::AstLeafType) {
						//叶子节点
						AstLeaf* leaf = new AstLeaf(ir[i].data);
						leaf->lineNo = ir[i].lineNo;
						leaf->filename = ir[i].filename;

						stack.peek()->Children()->add(
						    leaf
						);
						continue;
					}
					ast::AstNode* n = NULL;
					CHECK_IR(ast::AstProgram)
					CHECK_IR(ast::AstDefClass)
					CHECK_IR(ast::AstDefFunc)
					CHECK_IR(ast::AstDefVar)
					CHECK_IR(ast::AstAnonFunc)
					CHECK_IR(ast::AstBlock)
					CHECK_IR(ast::AstBreak)
					CHECK_IR(ast::AstContinue)
					CHECK_IR(ast::AstIfStatement)
					CHECK_IR(ast::AstWhileStatement)
					CHECK_IR(ast::AstForStatement)
					CHECK_IR(ast::AstReturnStatement)
					CHECK_IR(ast::AstSFN)
					CHECK_IR(ast::AstLeftValue)
					CHECK_IR(ast::AstArguments)
					CHECK_IR(ast::AstNull)
					CHECK_IR(ast::AstArrayLiteral)
					CHECK_IR(ast::AstListLiteral)

					CHECK_SPECIAL_IR(ast::AstAnonClass, isHaveFather)
					CHECK_SPECIAL_IR(ast::AstExpression, ass_type)
					CHECK_SPECIAL_IR(ast::AstLeftPostfix, postfix_type)
					CHECK_SPECIAL_IR(ast::AstBinary, operator_type)
					CHECK_SPECIAL_IR(ast::AstUnary, operator_type)
					CHECK_SPECIAL_IR(ast::AstPostfix, postfix_type)

					if(stack.empty()==false) {
						//说明有父节点，那么绑定它
						stack.peek()->Children()->add(n);
					} else {
						rst = n;
						//说明这个节点是总节点
					}
					//压入
					stack.push(n);
				}

				return rst;
			}

			void destroyConst(ArrayList<datatype::DataType*> tabconst) {
				//销毁指定的常量表
				for(int i=0;i<tabconst.size();i++) {
					delete tabconst[i];
				}
			}
	};
} //namespace stamon::ir


#undef CHECK_SPECIAL_AST
/*
	Name: AstIrReader.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 09/12/23 17:34
	Description: IR读取器
*/



namespace stamon::ir {
	class AstIrReader {
			char* stvc;		//代码
			int stvc_size;	//代码大小
			int pos;		//设当前需要读取的字节为stvc[pos]
		public:
			int VerX, VerY, VerZ;   //这三个变量代表版本为X.Y.Z
			ArrayList<datatype::DataType*> tableConst;   //常量表

			STMException* ex;

			AstIrReader(char* vmcode, int code_size, STMException* e) {
				ex = e;
				//传入字节码和字节码大小
				stvc_size = code_size;
				stvc = vmcode;
			}
			bool ReadHeader() {
				//读取字节码头，读取失败则抛出异常并返回false，否则返回true

				if(stvc_size<8) {
					//字节码太少了
					THROW("The code size is too small")
					return false;
				}

				pos = 0;    //初始化

				if(stvc[pos]==(char)0xAB&&stvc[pos+1]==(char)0xDB) {
					//通过魔数查看该字节码是否为STVC
					NextPos(2);
				} else {
					THROW("Not STVC")
					return false;
				}
				//版本计算方法：X、Y、Z共占两个字节
				//其中X占一个字节，Y占一个字节的前四位，Z占一个字节的后四位

				VerX = stvc[pos];
				VerY = stvc[pos+1]>>4;
				VerZ = stvc[pos+1]&0x0f;

				NextPos(2);
				//获取常量表
				int tableConstNumber = ReadInt();
				//常量表项数
				NextPos(4);

				CATCH {
					//如果上述代码当中出现了异常
					/*
					 * 值得注意的是，由于我在执行了好几步可能报错的代码后才判错
					 * 所以上述当中的有些值可能是错乱的
					 * 不过反正也要return false，所以这些错乱的值无伤大雅
					*/
					return false;
				}

				if(ReadTableConst(tableConstNumber)==false) {
					return false;
				}

				return true;
			}

			bool ReadTableConst(int size) {
				//读取常量表，size为常量表项数，pos为下一个读取的字节的下标
				while(size!=0) {
					char type = stvc[pos];

					NextPos(1);

					if(type==datatype::IntegerTypeID) {
						int value = ReadInt();
						NextPos(4);
						tableConst.add(new datatype::IntegerType(value));
					} else if(type==datatype::FloatTypeID) {
						float value;
						char* tmp = (char*)&value;
						tmp[0] = stvc[pos]<<24;
						tmp[1] = stvc[pos+1]<<16;
						tmp[2] = stvc[pos+2]<<8;
						tmp[3] = stvc[pos+3];
						//通过写入字节的方式给value赋值
						NextPos(4);
						tableConst.add(new datatype::FloatType(value));
					} else if(type==datatype::DoubleTypeID) {
						double value;
						char* tmp = (char*)&value;
						for(int i=0; i<8; i++) {
							tmp[i] = stvc[pos+i];
							/*
							 * 比如i=0时：
							 * tmp[0] = stvc[pos]<<56
							 * i=1时
							 * tmp[1] = stvc[pos+1]<<48
							 * 以此类推，这样可以通过写入字节的方式给value赋值
							*/
						}
						NextPos(8);
						tableConst.add(new datatype::DoubleType(value));
					} else if(type==datatype::NullTypeID) {
						tableConst.add(new datatype::NullType());
					} else if(type==datatype::StringTypeID) {
						int slen = ReadInt();
						NextPos(4);

						char* c_arr = new char[slen+1];
						c_arr[slen] = '\0';

						for(int i=0; i<slen; i++) {
							c_arr[i] = stvc[pos+i];
						}
						NextPos(slen);
						StamonString s(c_arr);

						delete[] c_arr;

						tableConst.add(new datatype::StringType(s));
					} else if(type==0xff) {
						int slen = ReadInt();
						NextPos(4);

						char* c_arr = new char[slen+1];
						c_arr[slen] = '\0';

						for(int i=0; i<slen; i++) {
							c_arr[i] = stvc[pos+i];
						}

						NextPos(slen);

						tableConst.add(
						    new ir::IdenConstType(StamonString(c_arr))
						);

						delete[] c_arr;

					} else {
						THROW("Unknown constants")
					}

					CATCH {
						return false;
					}

					size--;
				}
				return true;
			}

			ArrayList<AstIR> ReadIR() {

				ArrayList<AstIR> ir;

				int lineNo = -1;

				StamonString filename = StamonString((char*)"");

				while(pos!=stvc_size) {
					int type = ReadInt();

					NextPos(4);

					if(type==-2) {

						//更新行号
						lineNo = ReadInt();

						NextPos(4);

					} else if(type==-3) {

						//更新文件名
						int slen = ReadInt();

						NextPos(4);

						char* c_arr = new char[slen];

						for(int i=0; i<slen; i++) {
							c_arr[i] = stvc[pos+i];
						}

						NextPos(slen);

						filename = StamonString(c_arr);

						delete[] c_arr;

					} else {
						//正常的IR
						AstIR rst;

						rst.data = ReadInt();

						NextPos(4);

						rst.lineNo = lineNo;
						rst.filename = filename;
						rst.type = type;

						ir.add(rst);
					}

					CATCH {
						return ir;
					}
				}

				return ir;
			}

			void NextPos(int x) {
				//这个函数把pos向后移动x位，如果pos超出了stvc_size，那么抛出异常
				if(pos >= stvc_size) {
					/*
					 * 为了保护程序不会因为越界而退出
					 * 我在pos越界后，抛出异常并把pos调整至非越界范围
					 */
					pos = 0;	//调整至非越界范围
					THROW("The code size is too small")
				} else {
					pos += x;
				}
				return;
			}

			int ReadInt() {
				//将stvc[pos]到stvc[pos+3]转换成int

				char arr[4] = {
					stvc[pos+3], stvc[pos+2], stvc[pos+1], stvc[pos]
				};

				int rst = *((int*)arr);

				return rst;
			}
	};
} //namespace stamon::ir
/*
	Name: AstIrWriter.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 09/12/23 17:34
	Description: IR写入器
*/



#define WRITE(b) \
	writer.write(b);\
	CATCH {\
		return;\
	}

#define WRITE_I(n) \
	if(true) {\
		int tmp = n;\
		WRITE(tmp>>24)\
		WRITE( (tmp>>16) & 0xff)\
		WRITE( (tmp>>8) & 0xff)\
		WRITE(tmp & 0xff)\
		CATCH {\
			return;\
		}\
	}

namespace stamon::ir {
	class AstIrWriter {

		public:

			STMException* ex;

			AstIrWriter() {}
			AstIrWriter(STMException* e) {
				ex = e;
			}

			void write(
				ArrayList<ir::AstIR> ir_list,
				ArrayList<datatype::DataType*> ir_tableconst,
				StamonString dst, bool isStrip,
				int VerX, int VerY, int VerZ
			) {

				BinaryWriter writer(ex, dst);

				CATCH {
					return;
				}

				//写入魔数

				WRITE(0xAB)
				WRITE(0xDB)

				//写入版本

				WRITE(VerX)
				WRITE((VerY<<4) + (VerZ & 0x0f))

				//写入常量表

				WRITE_I(ir_tableconst.size())

				for(int i=0,len=ir_tableconst.size(); i<len; i++) {

					WRITE((char)ir_tableconst[i]->getType())

					if(ir_tableconst[i]->getType()==datatype::IntegerTypeID) {
						int val = ((datatype::IntegerType*)ir_tableconst[i])
									->getVal();

						WRITE_I(val)
					}

					if(ir_tableconst[i]->getType()==datatype::FloatTypeID) {
						float val = ((datatype::FloatType*)ir_tableconst[i])
									->getVal();
						//逐个字节写入
						char* val_ptr = (char*)&val;
						WRITE(val_ptr[0])
						WRITE(val_ptr[1])
						WRITE(val_ptr[2])
						WRITE(val_ptr[3])
					}

					if(ir_tableconst[i]->getType()==datatype::DoubleTypeID) {
						double val = ((datatype::DoubleType*)ir_tableconst[i])
									->getVal();
						//逐个字节写入
						char* val_ptr = (char*)&val;
						WRITE(val_ptr[0])
						WRITE(val_ptr[1])
						WRITE(val_ptr[2])
						WRITE(val_ptr[3])
						WRITE(val_ptr[4])
						WRITE(val_ptr[5])
						WRITE(val_ptr[6])
						WRITE(val_ptr[7])
					}

					if(ir_tableconst[i]->getType()==datatype::StringTypeID) {
						StamonString s = ((datatype::StringType*)ir_tableconst[i])
									->getVal();

						WRITE_I(s.length());

						for(int i=0,len=s.length(); i<len; i++) {
							WRITE(s[i])
						}
					}

					if(ir_tableconst[i]->getType()==ir::IdenConstTypeID) {
						StamonString s = ((ir::IdenConstType*)ir_tableconst[i])
									->getVal();

						WRITE_I(s.length());

						for(int i=0,len=s.length(); i<len; i++) {
							WRITE(s[i])
						}
					}

				}

				//最后写入IR

				int lineNo = -1;			//当前正在写入的行号
				StamonString filename((char*)"");	//当前正在写入的文件名

				for(int i=0,len=ir_list.size(); i<len; i++) {

					if(isStrip==false) {
						//要求附加调试信息

						if(lineNo!=ir_list[i].lineNo
								&&ir_list[i].type!=-1) {

							//注意：结束符不用在意行号和文件名
							//这里要特判结束符（即ir_list[i].type!=-1）

							//如果行号没更新，更新行号并且将当前行号写入
							lineNo = ir_list[i].lineNo;

							WRITE_I(-2)		//-2代表更新行号
							WRITE_I(lineNo)
						}

						if(filename.equals(ir_list[i].filename) == false
								&&ir_list[i].type!=-1) {
							//检查文件名，原理同上
							filename = ir_list[i].filename;

							WRITE_I(-3)		//-2代表更新文件
							WRITE_I(filename.length())

							for(int i=0,len=filename.length(); i<len; i++) {
								WRITE(filename[i])
							}
						}
					}

					WRITE_I(ir_list[i].type)
					WRITE_I(ir_list[i].data)

				}

				writer.close();
			}

	};
} // namespace stamon::ir

#undef WRITE
#undef WRITE_I
/*
	Name: CompilerExceptionMessage.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 12:46
	Description: 此文件用于存放编译器异常信息的生成函数
*/



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
/*
	Name: Lexer.cpp
	Copyright: Apache2.0
	Author: CLimber-Rong
	Date: 24/12/23 12:00
	Description: 词法分析器
*/



#define CHECK_KEYWORD(keyword, TokType) \
	if(iden==StamonString((char*)keyword)) {\
		st = ed;\
		Token* rst = new Token(line, TokType);\
		tokens.add(rst);\
		continue;\
	}
//为了方便判断关键字，我编写了这个宏

#define STR_EX(len) \
	if(text_len-ed<(len-1)) {\
		THROW_S(err::WrongStringFormat());\
		return st;\
	}
//为了方便判断字符串token是否错误，我编写了这个宏（全名应该叫做CHECK_STRING_EXCEPTION）
//这个宏用于检查文本后还有是否还有len个字符（不包括当前正在扫描的字符）

#define CHECK_OPERATOR(op, TokType) \
	if(text[ed]==op) {\
		ed++;\
		st = ed;\
		Token* rst = new Token(line, TokType);\
		tokens.add(rst);\
		continue;\
	}
//这个宏用于匹配单个字符的运算符，其中op应是char类型

#define CHECK_LONG_OPERATOR(op, TokType) \
	if(text_len-ed>1) {\
		if(text[ed]==op[0]&&text[ed+1]==op[1]) {\
			ed += 2;\
			st = ed;\
			Token* rst = new Token(line, TokType);\
			tokens.add(rst);\
			continue;\
		}\
	}

//这个宏用于匹配两个字符的运算符，其中op应是char*类型

#define CHECK_LONG_LONG_OPERATOR(op, TokType) \
	if(text_len-ed>2) {\
		if(text[ed]==op[0]&&text[ed+1]==op[1]&&text[ed+2]==op[2]) {\
			ed += 3;\
			st = ed;\
			Token* rst = new Token(line, TokType);\
			tokens.add(rst);\
			continue;\
		}\
	}
//这个宏用于匹配三个字符的运算符，其中op应是char*类型

#define CHECK_ESCAPE_CHAR(charactor, str) \
	if(tmp[i]==charactor) {\
		val += StamonString((char*)str);\
		i++;\
	} else

//这个宏用于StringToken当中，用于简便的判断转义字符

//以上定义的宏，都只能在该文件中使用


namespace stamon::c {   //编译器命名空间

	enum TOKEN_TYPE {
	    TokenClass = 0,
	    TokenDef,
	    TokenExtends,
	    TokenFunc,
	    TokenBreak,
	    TokenContinue,
	    TokenIf,
	    TokenElse,
	    TokenWhile,
	    TokenFor,
	    TokenIn,
	    TokenReturn,
	    TokenSFN,
	    TokenNew,
	    TokenNull,      //空值
	    TokenImport,	//导入
	    KEYWORDS_MAX,   //关键词个数
	    TokenAssign,//赋值
	    TokenSemi,  //分号
	    TokenLBC,   //左花括号
	    TokenRBC,   //右花括号
	    TokenLBR,   //左括号
	    TokenRBR,   //右括号
	    TokenLSB,   //左方括号
	    TokenRSB,   //右方括号
	    TokenCmm,   //逗号
	    TokenColon,	//冒号
	    TokenMember,//小数点
	    TokenAddAss,    //加等于
	    TokenSubAss,
	    TokenMulAss,
	    TokenDivAss,
	    TokenModAss,
	    TokenAndAss,
	    TokenXOrAss,
	    TokenOrAss,
	    TokenLSHAss,
	    TokenRSHAss,
	    TokenLogOR, //逻辑运算符
	    TokenLogAND,
	    TokenBitOR, //位运算符
	    TokenBitXOR,
	    TokenBitAND,
	    TokenEqu,
	    TokenNotEqu,
	    TokenBig,
	    TokenLess,
	    TokenBigEqu,
	    TokenLessEqu,
	    TokenLSH,   //左移
	    TokenRSH,   //右移
	    TokenAdd,
	    TokenSub,
	    TokenMul,
	    TokenDiv,
	    TokenMod,
	    TokenLogNot,    //逻辑非
	    TokenBitNot,    //按位取反
	    TokenIden,      //标识符
	    TokenInt,       //整数
	    TokenDouble,    //浮点数
	    TokenString,    //字符串
	    TokenTrue,		//布尔真值
	    TokenFalse,		//布尔假值
	    TokenEOF
	};

	class Token {
		public:
			Token(int line, int tok_type) {
				lineNo = line;
				type = tok_type;
			}
			int type;
			int lineNo;
	};

	class StringToken : public Token {
		public:
			StamonString content;
			//这个字符串存储着token文本
			//（token文本并没有去掉字符串前后的引号，以及转义字符）
			StamonString val;
			//这个字符串由content处理而来
			StringToken(int line, const StamonString& s)
				: Token(line, TokenString) {

				content = s;

				//开始处理content，并存储到s
				StamonString tmp = content.substring(1,content.length()-1);
				//去除前后的引号
				int i=0;
				while(i<tmp.length()) {
					if(tmp[i]=='\\') {
						//碰到转义字符
						i++;
						CHECK_ESCAPE_CHAR('\"', "\"")
						CHECK_ESCAPE_CHAR('\\', "\\")
						CHECK_ESCAPE_CHAR('0', "\0")
						CHECK_ESCAPE_CHAR('n', "\n")
						CHECK_ESCAPE_CHAR('t', "\t")
						if(tmp[i]=='x') {
							i++;
							StamonString data = tmp.substring(i, i+2);
							char* c = new char[1];
							*c = data.toIntX();
							val += StamonString(c);
							delete[] c;
							i+=2;
						}
					} else {
						val += tmp.substring(i, i+1);	//把tmp[i]拼到val里
						i++;
					}
				}
			}
	};

	class IdenToken : public Token {
		public:
			StamonString iden;
			IdenToken(int line, const StamonString& name)
				: Token(line, TokenIden) {
				iden = name;
			}
	};

	class IntToken : public Token {
		public:
			int val;
			IntToken(int line, int value) : Token(line, TokenInt) {
				val = value;
			}
	};

	class DoubleToken : public Token {
		public:
			double val;
			DoubleToken(int line, double value) : Token(line, TokenDouble) {
				val = value;
			}
	};

	Token TokEOF(-1, TokenEOF);

	class Lexer {
			/*
			 * 词法分析器有三个最主要的函数：getTok，peek和getLineTok
			 * 首先，你需要逐行的读取文本，并调用getLineTok(行号, 文本)
			 	* 这个函数可以将文本分析成一系列token，并把这些token存入缓冲区
			 * 在所有的文本被逐行读取并分析成token之后，就可以正常使用getTok和peek了
			 * getTok()会从缓冲区里读取一个token，并把这个token从缓冲区里移除
				*（可以简单理解为弹出缓冲区的第一个元素）
			 * peek(index)会获取第index个token，但是不会从缓冲区里移除它
			 	*（可以简单理解为读取操作）
			*/

			ArrayList<Token*> tokens;	//缓存token用

		public:

			STMException* ex;

			Lexer() {}

			Lexer(STMException* e) {
				ex = e;
			}

			int StrToInt(int line, StamonString s) {
				//词法分析器需要特判整数过大引起溢出的情况，所以不能直接使用String的toInt
				bool warnflag = false;
				int rst = 0;
				for(int i=0,len=s.length();i<len;i++) {
					rst *= 10;
					rst += s[i] - '0';
					if(rst<0 && warnflag==false) {
						WARN_S(warning::TooLargeInteger());
						warnflag = true;
					}
				}
				return rst;
			}

			double StrToDouble(int line, StamonString s) {
				//词法分析器需要特判小数过大引起溢出的情况
				bool warnflag = false;
				int integer = 0;
				double decimal = 0.0;
				int i = 0;

				while(s[i]!='.') {
					//分析整数部分
					integer *= 10;
					integer += s[i] - '0';
					if(integer<0 && warnflag==false) {
						WARN_S(warning::TooLargeFloat());
						warnflag = true;
					}
					i++;
				}

				i++;	//去除小数点

				while(i<s.length()) {
					//分析小数部分
					decimal /= 10;
					decimal += (double)(s[i]-'0');
					if(decimal<0 && warnflag==false) {
						WARN_S(warning::TooLargeFloat());
						warnflag = true;
					}
					i++;
				}

				return (double)integer + decimal;
			}

			Token* getTok() {
				//读取一个Token
				if(tokens.empty()==true) {
					return &TokEOF;
				}

				Token* rst = tokens.at(0);
				tokens.erase(0);

				return rst;
			}

			Token* peek(int index) {
				//查看第index个Token
				if(tokens.size()<=index) {
					return &TokEOF;
					//没有这么多Token了
				}

				Token* rst = tokens[index];

				return rst;
			}

			int getLineTok(int line, StamonString text) {
				//分析一行的token，line是行号，text是文本（不包含换行符）
				//分析后的token添加在tokens的末尾
				//返回解析到的位置，如果返回值是text.length()就说明解析到末尾，即解析成功
				//否则说明解析失败
				int text_len = text.length();

				int st = 0;
				while(st<text_len) {

					int ed = st;

					//先过滤空格
					while(
					    ed<text_len
					    &&(text[ed]==' '||text[ed]=='\t'
					       ||text[ed]=='\n'||text[ed]=='\r')
					) {
						ed++;
					}

					st = ed;

					if(!(st<text_len)) {
						//如果读完空格后就已经整行读完了
						return text_len; //直接正常退出即可
					}

					if(
					    text_len-st-1>=2
					    &&text[ed]=='/'
					    &&text[ed+1]=='/'
					) {
						//碰到注释，返回
						ed += 2;
						return text_len;	//直接正常退出即可
					}

					if('0'<=text[ed]&&text[ed]<='9') {
						bool isInt = true;  //该token是否是整数
						ed++;
						while(ed<text_len
						        &&'0'<=text[ed]
						        &&text[ed]<='9') {
							ed++;
						} //分析整数

						if(text[ed]=='.') {
							isInt = false;  //小数token
							ed++;
							bool is_has_decimal_part = false;
							while(ed<text_len
							        &&'0'<=text[ed]
							        &&text[ed]<='9') {
								ed++;
								is_has_decimal_part = true;
							}
							
							if(is_has_decimal_part==false) {
								THROW_S(err::WrongFloatFormat());
								return st;
							}
						} //分析小数
						if(isInt==true) {
							int value = StrToInt(line, text.substring(st, ed));
							IntToken* rst = new IntToken(line, value);
							tokens.add((Token*)rst);
						} else {
							double value = StrToDouble(line 
												,text.substring(st, ed));
							DoubleToken* rst = new DoubleToken(line, value);
							tokens.add((Token*)rst);
						}
						st = ed;
						continue;
					} else if(
					    ('A'<=text[ed]&&text[ed]<='Z')
					    ||('a'<=text[ed]&&text[ed]<='z')
					    ||(text[ed]=='_')
					) {     //标识符
						ed++;
						while(
						    ed<text_len
						    &&(
						        ('A'<=text[ed]&&text[ed]<='Z')
						        ||('a'<=text[ed]&&text[ed]<='z')
						        ||('0'<=text[ed]&&text[ed]<='9')
						        ||(text[ed]=='_')
						    )
						) {
							ed++;
						}
						StamonString iden = text.substring(st, ed);

						//判断关键字
						CHECK_KEYWORD("class", TokenClass)
						CHECK_KEYWORD("def", TokenDef)
						CHECK_KEYWORD("extends", TokenExtends)
						CHECK_KEYWORD("func", TokenFunc)
						CHECK_KEYWORD("break", TokenBreak)
						CHECK_KEYWORD("continue", TokenContinue);
						CHECK_KEYWORD("if", TokenIf)
						CHECK_KEYWORD("else", TokenElse)
						CHECK_KEYWORD("while", TokenWhile)
						CHECK_KEYWORD("for", TokenFor)
						CHECK_KEYWORD("in", TokenIn)
						CHECK_KEYWORD("return", TokenReturn)
						CHECK_KEYWORD("sfn", TokenSFN)
						CHECK_KEYWORD("new", TokenNew)
						CHECK_KEYWORD("null", TokenNull)
						CHECK_KEYWORD("import", TokenImport)
						CHECK_KEYWORD("true", TokenTrue)
						CHECK_KEYWORD("false", TokenFalse)
						//都不是关键字的话，那就是正常的标识符

						st = ed;
						IdenToken* rst = new IdenToken(line, iden);
						tokens.add((Token*)rst);
						continue;
					} else if(text[ed]=='\"') {
						//读取字符串
						bool is_str_closed = false;
						//用于标记字符串的是否有用'\"'结尾
						//因为循环的退出可能并不是因为读到'\"'，而是文本读完了
						ed++;
						while(ed<text_len&&is_str_closed==false) {
							if(text[ed]=='\"') {
								//字符串结束
								ed++;
								is_str_closed = true;

								StamonString s = text.substring(st,ed);
								StringToken* rst = new StringToken(line, s);
								tokens.add((Token*)rst);
								st = ed;

							} else if(text[ed]=='\\') {
								//碰到转义字符
								STR_EX(2)	//文本至少还得剩两个字符
								ed++;
								if(
								    text[ed]=='\"'
								    ||text[ed]=='\\'
								    ||text[ed]=='0'
								    ||text[ed]=='n'
								    ||text[ed]=='t'
								) {
									ed++;
								} else if(text[ed]=='x') {
									STR_EX(3)	//文本至少还得剩三个字符

									if(
									    (
									        ('0'<=text[ed+1]
									         &&text[ed+1]<='9')
									        ||('a'<=text[ed+1]
									           &&text[ed+1]<='f')
									        ||('A'<=text[ed+1]
									           &&text[ed+1]<='F')
									    )
									    &&(
									        ('0'<=text[ed+2]
									         &&text[ed+1]<='9')
									        ||('a'<=text[ed+2]
									           &&text[ed+1]<='f')
									        ||('A'<=text[ed+2]
									           &&text[ed+1]<='F')
									    )
									) {
										//用于判断\xDD转义字符中，DD是否符合标准
										ed += 2;
									} else {
										STR_EX(text_len)
										//由于字符串永远无法剩下text_len个字符
										//所以异常必然会被触发
										//所以STR_EX(text_len)的目的，
										//就是直接抛出异常
										//这里运用了取巧的设计
									}

								}
							} else {
								//正常字符
								ed++;
							}
						}

						if(is_str_closed==false) {
							//如果字符串未关闭
							STR_EX(text_len);
						}

					} else {
						CHECK_LONG_LONG_OPERATOR("<<=", TokenLSHAss)
						CHECK_LONG_LONG_OPERATOR(">>=", TokenRSHAss)

						CHECK_LONG_OPERATOR("+=", TokenAddAss)
						CHECK_LONG_OPERATOR("-=", TokenSubAss)
						CHECK_LONG_OPERATOR("*=", TokenMulAss)
						CHECK_LONG_OPERATOR("/=", TokenDivAss)
						CHECK_LONG_OPERATOR("%=", TokenModAss)
						CHECK_LONG_OPERATOR("&=", TokenAndAss)
						CHECK_LONG_OPERATOR("^=", TokenXOrAss)
						CHECK_LONG_OPERATOR("|=", TokenOrAss)
						CHECK_LONG_OPERATOR("||", TokenLogOR)
						CHECK_LONG_OPERATOR("&&", TokenLogAND)
						CHECK_LONG_OPERATOR("==", TokenEqu)
						CHECK_LONG_OPERATOR("!=", TokenNotEqu)
						CHECK_LONG_OPERATOR(">=", TokenBigEqu)
						CHECK_LONG_OPERATOR("<=", TokenLessEqu)
						CHECK_LONG_OPERATOR("<<", TokenLSH)
						CHECK_LONG_OPERATOR(">>", TokenRSH)

						CHECK_OPERATOR('=', TokenAssign)
						CHECK_OPERATOR(';', TokenSemi)
						CHECK_OPERATOR('{', TokenLBC)
						CHECK_OPERATOR('}', TokenRBC)
						CHECK_OPERATOR('(', TokenLBR)
						CHECK_OPERATOR(')', TokenRBR)
						CHECK_OPERATOR('[', TokenLSB)
						CHECK_OPERATOR(']', TokenRSB)
						CHECK_OPERATOR(',', TokenCmm)
						CHECK_OPERATOR(':', TokenColon)
						CHECK_OPERATOR('.', TokenMember)
						CHECK_OPERATOR('|', TokenBitOR)
						CHECK_OPERATOR('^', TokenBitXOR)
						CHECK_OPERATOR('&', TokenBitAND)
						CHECK_OPERATOR('>', TokenBig)
						CHECK_OPERATOR('<', TokenLess)
						CHECK_OPERATOR('+', TokenAdd)
						CHECK_OPERATOR('-', TokenSub)
						CHECK_OPERATOR('*', TokenMul)
						CHECK_OPERATOR('/', TokenDiv)
						CHECK_OPERATOR('%', TokenMod)
						CHECK_OPERATOR('!', TokenLogNot)
						CHECK_OPERATOR('~', TokenBitNot)
						//执行到这里，说明碰到了未知字符
						THROW_S(err::UnknownToken(text.substring(ed, ed+1)));
						return st;
					}
				}
				return st;	//到这里说明一切正常
			}


	};
}

#undef CHECK_KEYWORD
#undef STR_EX
#undef CHECK_OPERATOR
#undef CHECK_LONG_OPERATOR
#undef CHECK_LONG_LONG_OPERATOR
/*
	Name: Parser.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 05/01/24 21:33
	Description: 语法分析器
*/




#define check(type) (matcher.Check(type))
#define _pop			(matcher.Pop())
#define CE			CATCH { return NULL; }	//如果执行代码中出现异常，直接返回
#define CTH(message)	CATCH { THROW(message) } //如果执行代码中出现异常，抛出异常
#define GETLN(type) \
	int lineNo;\
	if(true) {\
		Token* tok = match(type);\
		CE\
		lineNo = tok->lineNo;\
	}

/*
 * GETLN的全写是：Get LineNo
 * 这个宏会匹配一个类型为type的token，并且把行号存入lineNo
 * 套一层if(true)的作用是：防止后续的代码也定义了tok导致编译器报错
 	* 套一层if(true)之后，tok这个变量会随着if(true)这个作用域的消失而消失
*/

#define pushscope(wall) \
	if(true) {\
		SyntaxScope scope(ex);\
		scope.isWall = wall;\
		scopes.add(scope);\
	}
//这里if(true)的作用同GETLN
#define popscope (scopes.erase(scopes.size()-1))


#define unary_check(tok_type, unary_type) \
	if(check(tok_type)) {\
		int line = _pop->lineNo;\
		return Ast<ast::AstUnary>(\
		                          line,\
		                          ast::unary_type,\
		                          unary_operator()\
		                         );\
	}

//为了方便编写语法分析的代码，我设计了这些宏
//这些宏只能在这个文件中被使用



namespace stamon::c {

	class Matcher {
			//用于在语法分析中查看并匹配token
			Lexer lexer;

		public:

			STMException* ex;

			Matcher() {}

			Matcher(Lexer lex, STMException* e) {
				lexer = lex;
				ex = e;
			}

			bool Check(int type) {
				//用于检查token的类型
				if(lexer.peek(0)->type==type) {
					return true;
				}
				return false;
			}

			Token* Match(int type) {
				//匹配一个类型为type的token，如果不匹配，就直接报错
				if(Check(type)) {
					return lexer.getTok();
				} else {
					THROW_S(err::InvalidSyntax());
					return NULL;
				}
			}

			Token* Peek(int index) {
				//返回第index后个token
				return lexer.peek(index);
			}

			Token* Pop() {
				//直接弹出一个token
				return lexer.getTok();
			}
	};

	class SyntaxScope {	//用于记录每个作用域的变量，防止变量重定义、变量未定义就使用
		public:
			//这个类要区分与vm::ObjectScope
			StringMap<void> scope;
			int isWall = 0;
			/*
			 * 这里需要详细介绍一下isWall的用法
			 * 在用户编程的过程中，可能会出现如下代码：
			 func f1 {
				 def a = 0;
				 func f2 {
					 a += 1;
				 }
			 }
			 * 容易看出，由于f1和f2是两个不同的函数，所以两个函数的作用域不能互相使用
			 * 换句话说：在f2中使用f1中的a是非法行为
			 * 但是如果采取常规的作用域分析——因为f2在f1内定义，所以f2可以使用变量a
			 * 这样就导致了分析的出错
			 * 为此，我设立了一个标志——isWall，所有函数、类的作用域的isWall都为1
			 * 其余的作用域（例如if,while等）为0
			 * 在搜寻某个标识符是否存在时，应当从作用域栈的栈顶开始从上到下搜索
			 * 直到碰到一个isWall=1的作用域为止
				 * （由于这种标志很想一堵墙，阻止了搜索，所以我取名为“墙”）
			 * 这样就防止一个函数使用另外一个函数的变量，但是并未报错
			*/

			STMException* ex;

			SyntaxScope() {}

			SyntaxScope(const SyntaxScope& s) {
				ex = s.ex;
				scope = s.scope;
			}

			SyntaxScope(STMException* e) {
				ex = e;
			}

			bool exist(Token* iden) {
				return scope.containsKey(((IdenToken*)iden)->iden);
			}

			void mark(Token* iden) {	//声明一个变量
				if(scope.containsKey(((IdenToken*)iden)->iden)) {
					THROW_S(
						err::VariableDeclaredRepeatedly(
									((IdenToken*)iden)->iden
							 )
					);
					return;
				}
				scope.put(((IdenToken*)iden)->iden, NULL);
			}

			void force_mark(Token* iden) {
				//无论该标识符是否被定义过，强制定义一遍
				//该函数用于弱定义
				scope.put(((IdenToken*)iden)->iden, NULL);
			}
	};

	class SourceSyntax {	//用来存储一个文件的文件名和程序
		public:
			ast::AstNode* program;
			StamonString filename;
	};

	class Parser {
			Matcher matcher;
			//定义双目运算符的优先级表
			int bin_prio[19] = {
				1, 2, 3, 4, 5,
				6, 6,
				7, 7, 7, 7,
				8, 8,
				9, 9,
				10, 10, 10,
				-1
				//这个-1用于内部的迭代语法分析，我们称为结束符
			};
			//bin_prio[i]代表_BinaryOperatorType中
			//第i个枚举的优先级数值，优先级数值越大，则越为优先

			//定义每个优先级的运算符有哪些
			int bin_layer[10][2] {
				{TokenLogOR, TokenLogOR},
				{TokenLogAND, TokenLogAND},
				{TokenBitOR, TokenBitOR},
				{TokenBitXOR, TokenBitXOR},
				{TokenBitAND, TokenBitAND},
				{TokenEqu, TokenNotEqu},
				{TokenBig, TokenLessEqu},
				{TokenLSH, TokenRSH},
				{TokenAdd, TokenSub},
				{TokenMul, TokenMod}
			};
			/*
			 * 第i层运算符在bin_layer[i-1][0]~bin_layer[i-1][1]的范围内
			 * 例如TokenBigEqu在第7层，
			 	* 则bin_layer[6][0]<=TokenBigEqu<=bin_layer[6][1]
			 * 会发现：我并没有把每层的所有运算符都罗列出来，而是只表示了范围
			*/

			ArrayList<SyntaxScope> scopes;

		public:

			int ParsingLineNo = 1;	//当前正在分析的行号
			bool ImportFlag = false;	//表示是否支持引用代码
			STMException* ex = NULL;
			StamonString ParsingFileName;

			FileMap filemap;
			ArrayList<SourceSyntax>* src_project;
			ArrayList<StamonString>* ErrorMsg;

			ArrayList<int> loop_levels;
			/*
			 * 通过维护该变量，我们可以判断break和continue是否处于循环当中
			 * 在全局初始化时，loop_levels默认只有一个值为0的元素
			 * 每进入一个新的函数，loop_levels加入一个值为0的元素，退出时弹出这个元素
			 * 每嵌套一个新的循环，loop_levels的结尾元素自增1，退出时结尾元素自减1
			 * 不难发现，如果loop_levels的结尾元素大于0，则当前处于循环当中
			 */

			Parser(Matcher matcher, STMException* e) {
				this->matcher = matcher;
				ex = e;
				SyntaxScope global_scope(ex);
				//压入一个空的全局作用域
				scopes.add(global_scope);
				ImportFlag = false;
			}	//这个构造函数用于兼容之前的测试样例

			Parser(
			    Matcher matcher, STMException* e,
			    SyntaxScope global_scope, StamonString filename,
			    ArrayList<SourceSyntax>* src, FileMap map,
			    ArrayList<StamonString>* msg, bool isSupportImport
			) {
				this->matcher = matcher;
				ex = e;
				//压入全局作用域
				scopes.add(global_scope);
				ImportFlag = isSupportImport;
				ParsingFileName = filename;
				filemap = map;
				src_project = src;
				ErrorMsg = msg;
				loop_levels.add(0);
			}

			template<class T, typename...Types>
			T* Ast(int line, Types&& ...args) {
				//这个代码比较难懂，涉及到形参模板和右值引用
				T* rst = new T(args...);
				rst->lineNo = line;
				rst->filename = ParsingFileName;
				ParsingLineNo = line;
				return rst;
			}

			Token* match(int TokType) {
				Token* rst = matcher.Match(TokType);

				CATCH {
					if(matcher.Peek(0)->type!=TokenEOF) {
						ParsingLineNo = matcher.Peek(0)->lineNo;
					}
					return NULL;
				}

				ParsingLineNo = rst->lineNo;
				return rst;
			}


			ast::AstNode* Parse() {
				return program();
			}

			ast::AstProgram* program() {
				ArrayList<ast::AstNode*>* stm
				    = new ArrayList<ast::AstNode*>();

				while(matcher.Peek(0)->type!=TokenEOF) {
					statement(stm);
					CE
				}

				return Ast<ast::AstProgram>(
				           1,	//整个程序的行号默认为1
				           stm
				       );
			}

			ast::AstSFN* sfn() {
				match(TokenSFN);
				CE

				ast::AstIdentifierName *port, *arg;

				if(!check(TokenIden)) {
					THROW_S(err::WrongSfnSyntax());
					CE;
				}
				port = IDEN();
				CE

				match(TokenCmm);

				if(!check(TokenIden)) {
					THROW_S(err::WrongSfnSyntax());
					CE;
				}

				arg = IDEN();
				CE

				match(TokenSemi);
				CE

				return Ast<ast::AstSFN>(port->lineNo ,port, arg);
			}

			ast::AstBlock* block() {
				//代码块

				ArrayList<ast::AstNode*>* statements
				    = new ArrayList<ast::AstNode*>();

				//特判是否为单个语句的代码块
				//（即simple_block）
				if(check(TokenColon)) {
					int lineNo = _pop->lineNo;	//弹出冒号
					statement(statements);
					CE
					return Ast<ast::AstBlock>(lineNo, statements);
				}

				//如果不是simple_block，那就是常规的代码块

				GETLN(TokenLBC);	//匹配左花括号

				while(check(TokenRBC)==false) {
					statement(statements);
					CE
				}

				_pop;	//弹出右花括号

				return Ast<ast::AstBlock>(lineNo, statements);

			}

			void* statement(ArrayList<ast::AstNode*>* stm) {
				//读取一条语句，并将解析后的ast加入stm当中
				//这里的返回值类型为void*，纯粹为了方便CE时return NULL;
				if(check(TokenDef)) {

					def_var(stm);

				} else if(check(TokenFunc)
				          &&matcher.Peek(1)->type==TokenIden) {

					//定义函数
					stm->add(def_func());

				} else if(check(TokenClass)
				          ||matcher.Peek(1)->type==TokenClass) {

					stm->add(def_class());

				} else if(check(TokenIf)) {

					stm->add(statement_if());

				} else if(check(TokenWhile)) {

					stm->add(statement_while());
					CE

				} else if(check(TokenFor)) {

					stm->add(statement_for());

				}  else if(check(TokenReturn)) {
					
					stm->add(statement_return());

				} else if(check(TokenContinue)) {

					if(loop_levels[loop_levels.size()-1]==0) {
						THROW_S(err::ContinueOutsideLoop());
						return NULL;
					}
					stm->add(
					    Ast<ast::AstContinue>(
					        _pop->lineNo
					    )
					);
					match(TokenSemi);

				} else if(check(TokenBreak)) {

					if(loop_levels[loop_levels.size()-1]==0) {
						THROW_S(err::BreakOutsideLoop());
						return NULL;
					}
					stm->add(
					    Ast<ast::AstBreak>(
					        _pop->lineNo
					    )
					);
					match(TokenSemi);

				} else if(check(TokenSemi)) {	//空语句
					_pop;	//弹出分号
				} else if(check(TokenSFN)) {

					stm->add(sfn());

				} else if(check(TokenImport)) {

					statement_import();

				} else {

					//如果以上情况都不是，那就只有可能是表达式了
					stm->add(expression());
					CE
					match(TokenSemi);

				}
				CE

				return NULL;
			}

			ast::AstDefVar* MakeNullDef(Token* iden) {
				//用于生成一个def_var，将iden赋为空值
				return Ast<ast::AstDefVar>(
				           iden->lineNo,
				           Ast<ast::AstIdentifierName>(
				               iden->lineNo,
				               ((IdenToken*)iden)->iden
				           ),
				           Ast<ast::AstExpression>(	//右值（即null）
				               iden->lineNo,
				               Ast<ast::AstBinary>(
				                   iden->lineNo,
				                   Ast<ast::AstUnary>(
				                       iden->lineNo,
				                       Ast<ast::AstNull>(
				                           iden->lineNo
				                       ),
				                       new ArrayList<ast::AstNode*>()//null无后缀
				                   )
				               )
				           )

				       );
			}

			ast::AstDefVar* assign_new_var() {

				Token* iden = matcher.Peek(0);

				if(iden->type!=TokenIden) {
					//变量名必须为标识符
					THROW_S(err::WrongVariableFormat());
					return NULL;
				}

				scopes[scopes.size()-1].mark(iden);	//登记该变量
				_pop;		//弹出iden
				CE

				if(matcher.Peek(0)->type==TokenAssign) {
					//初始化赋值
					_pop;	//弹出等号

					ast::AstExpression* expr = expression();
					CE

					ast::AstDefVar* rst = Ast<ast::AstDefVar>(
					                          iden->lineNo,
					                          Ast<ast::AstIdentifierName>(
					                              iden->lineNo,
					                              ((IdenToken*)iden)->iden
					                          ),
					                          expr
					                      );

					return rst;
				}

				//没有初始化赋值，则赋空值

				return MakeNullDef(iden);

			}

			void* def_var(ArrayList<ast::AstNode*>* stm) {
				match(TokenDef);	//弹出def
				CE

				stm->add(assign_new_var());
				CE

				while(check(TokenCmm)) {
					//如果变量的声明不止一个
					_pop;	//弹出逗号
					stm->add(assign_new_var());
					CE
				}

				match(TokenSemi);	//匹配分号
				CE

				return NULL;
			}


			ast::AstDefFunc* def_func() {

				GETLN(TokenFunc)

				Token* iden_tok = match(TokenIden);

				CE

				ast::AstIdentifierName* iden = Ast<ast::AstIdentifierName>(
				                                   iden_tok->lineNo,
				                                   ((IdenToken*)iden_tok)->iden
				                               );

				scopes[scopes.size()-1].force_mark(iden_tok);	//弱定义

				//新建作用域
				pushscope(1)

				loop_levels.add(0);

				ArrayList<ast::AstNode*>* args = new ArrayList<ast::AstNode*>();

				if(check(TokenLBR)) {
					//有参数列表
					_pop;	//弹出左括号

					if(check(TokenIden)) {
						IdenToken* iden = (IdenToken*)_pop;
						args->add(
						    Ast<ast::AstIdentifierName>(
						        iden->lineNo,
						        iden->iden
						    )
						);
						//在新建作用域中登记参数
						scopes[scopes.size()-1].mark(iden);
						CE
					}

					while(check(TokenCmm)) {
						//只要后面还有参数
						_pop;	//弹出逗号
						IdenToken* iden = (IdenToken*)_pop;
						args->add(
						    Ast<ast::AstIdentifierName>(
						        iden->lineNo,
						        iden->iden
						    )
						);
						//在新建作用域中登记参数
						scopes[scopes.size()-1].mark(iden);
						CE
					}

					if(match(TokenRBR)==NULL) {
						THROW_S(err::ParenthesesNotClosed());
					}
				}

				CE

				ast::AstBlock* blk = block();
				CE

				loop_levels.erase(loop_levels.size()-1);

				popscope;

				return Ast<ast::AstDefFunc>(
				           lineNo,
				           iden,
				           Ast<ast::AstAnonFunc>(
				               lineNo, args, blk
				           )
				       );

			}

			ast::AstAnonFunc* anon_func() {

				GETLN(TokenFunc)

				//新建作用域
				pushscope(1)

				loop_levels.add(0);

				ArrayList<ast::AstNode*>* args = new ArrayList<ast::AstNode*>();

				if(check(TokenLBR)) {
					//有参数列表
					_pop;	//弹出左括号

					if(check(TokenIden)) {
						IdenToken* iden = (IdenToken*)_pop;
						args->add(
						    Ast<ast::AstIdentifierName>(
						        iden->lineNo,
						        iden->iden
						    )
						);
						//在新建作用域中登记参数
						scopes[scopes.size()-1].mark(iden);
						CE
					}

					while(check(TokenCmm)) {
						//只要后面还有参数
						_pop;	//弹出逗号
						IdenToken* iden = (IdenToken*)_pop;
						args->add(
						    Ast<ast::AstIdentifierName>(
						        iden->lineNo,
						        iden->iden
						    )
						);
						//在新建作用域中登记参数
						scopes[scopes.size()-1].mark(iden);
						CE
					}

					if(match(TokenRBR)==NULL) {
						THROW_S(err::ParenthesesNotClosed())
					}
				}

				CE

				ast::AstBlock* blk = block();
				CE

				loop_levels.erase(loop_levels.size()-1);

				popscope;

				return Ast<ast::AstAnonFunc>(
				           lineNo, args, blk
				       );

			}

			ast::AstDefClass* def_class() {

				ast::AstIdentifierName* iden;	//类的标识符
				ast::AstIdentifierName* father = NULL;
				//父类标识符，默认没有父类

				if(matcher.Peek(0)->type==TokenIden) {
					//检查是否有父类
					father = IDEN();
					CE
				}

				GETLN(TokenClass)	//匹配class

				Token* iden_tok = match(TokenIden);
				CE
				iden = Ast<ast::AstIdentifierName>(
				           iden_tok->lineNo,
				           ((IdenToken*)iden_tok)->iden
				       );

				scopes[scopes.size()-1].force_mark(iden_tok);	//弱定义

				match(TokenLBC);	//匹配左花括号
				CE

				//新建作用域
				pushscope(1)

				ArrayList<ast::AstNode*>* stm = new ArrayList<ast::AstNode*>();
				//类里的语句

				while(check(TokenRBC)==false) {
					if(check(TokenFunc)) {
						stm->add(def_func());
					} else if(check(TokenDef)) {
						def_var(stm);
					} else if(check(TokenClass)
					          ||matcher.Peek(1)->type==TokenClass) {
						stm->add(def_class());
					} else {
						THROW_S(err::WrongClassDefined());
						return NULL;
					}

					CE
				}

				_pop;	//弹出右花括号

				popscope;

				return Ast<ast::AstDefClass>(
				           lineNo,
				           iden,
				           Ast<ast::AstAnonClass>(
				               lineNo,
				               father,
				               stm
				           )
				       );

			}

			ast::AstAnonClass* anon_class() {

				ast::AstIdentifierName* father = NULL;
				//父类标识符，默认没有父类

				if(matcher.Peek(0)->type==TokenIden) {
					//检查是否有父类
					father = IDEN();
					CE
				}

				GETLN(TokenClass)	//匹配class

				CE

				match(TokenLBC);	//匹配左花括号
				CE

				//新建作用域
				pushscope(1)

				ArrayList<ast::AstNode*>* stm = new ArrayList<ast::AstNode*>();
				//类里的语句

				while(check(TokenRBC)==false) {
					if(check(TokenFunc)) {
						stm->add(def_func());
					} else if(check(TokenDef)) {
						def_var(stm);
					} else if(check(TokenClass)
					          ||matcher.Peek(1)->type==TokenClass) {
						stm->add(def_class());
					} else {
						THROW_S(err::WrongClassDefined());
						return NULL;
					}

					CE
				}

				_pop;	//弹出右花括号

				popscope;

				return Ast<ast::AstAnonClass>(
				           lineNo,
				           father,
				           stm
				       );

			}

			ast::AstIfStatement* statement_if() {
				GETLN(TokenIf);

				ast::AstExpression* condition = expression();
				CE

				pushscope(0)

				ast::AstBlock* block_if = block();
				CE

				if(check(TokenElse)) {
					_pop;		//弹出else
					ast::AstBlock* block_else = block();
					CE
					popscope;
					return Ast<ast::AstIfStatement>(
					           lineNo,
					           condition,
					           block_if,
					           block_else
					       );
				}

				popscope;

				return Ast<ast::AstIfStatement>(
				           lineNo,
				           condition,
				           block_if
				       );

			}

			ast::AstWhileStatement* statement_while() {
				GETLN(TokenWhile)

				ast::AstExpression* condition = expression();
				CE

				pushscope(0)

				loop_levels[loop_levels.size()-1]++;

				ast::AstBlock* blk = block();
				CE

				loop_levels[loop_levels.size()-1]--;

				popscope;

				return Ast<ast::AstWhileStatement>(
				           lineNo,
				           condition,
				           blk
				       );
			}

			ast::AstForStatement* statement_for() {
				GETLN(TokenFor)	//匹配for

				Token* iden_tok = match(TokenIden);
				CE
				ast::AstIdentifierName* iden = Ast<ast::AstIdentifierName>(
				                                   iden_tok->lineNo,
				                                   ((IdenToken*)iden_tok)->iden
				                               );
				//获取循环用的变量

				match(TokenIn);	//匹配in
				CE

				ast::AstExpression* expr = expression();		//循环所需的表达式
				CE

				pushscope(0)	//新建作用域
				scopes[scopes.size()-1].mark(iden_tok);	//登记变量

				loop_levels[loop_levels.size()-1]++;

				ast::AstBlock* blk = block();	//代码块
				CE

				loop_levels[loop_levels.size()-1]--;

				popscope;

				return Ast<ast::AstForStatement>(
				           lineNo,
				           iden,
				           expr,
				           blk
				       );
			}

			ast::AstReturnStatement* statement_return() {
				GETLN(TokenReturn);

				ast::AstExpression* expr = expression();
				CE

				match(TokenSemi);
				CE

				return Ast<ast::AstReturnStatement>(
				           lineNo,
				           expr
				       );
			}

			void* statement_import() {
				GETLN(TokenImport)
				ParsingLineNo = lineNo;

				if(ImportFlag==false) {
					THROW_S(err::CannotImport());
					return NULL;
				}

				StamonString import_path;	//导入路径

				Token* iden = match(TokenIden);
				CE

				import_path += ((IdenToken*)iden)->iden;

				while(check(TokenMember)) {
					import_path += StamonString((char*)"/");	//还有一层路径
					iden = match(TokenIden);
					CE
					import_path += ((IdenToken*)iden)->iden;
				}

				import_path += StamonString((char*)".st");

				match(TokenSemi);	//匹配分号
				CE

				/*开始分析*/
				if(filemap.exist(import_path)==true) {
					return NULL;	//已经分析过了
				}

				LineReader reader = filemap.mark(import_path);
				CE

				//进行词法分析
				lineNo = 1;
				Lexer lexer(ex);

				while(reader.isMore()) {
					StamonString text = reader.getLine();
					CE

					int index = lexer.getLineTok(
					                lineNo, text
					            );

					CATCH {
						THROW_S(
						    StamonString((char*)"Error: at \"")
						    + import_path
						    + StamonString((char*)"\": ")
						    + toString(lineNo)
						    + StamonString((char*)":")
						    + toString(index)
						    + StamonString((char*)" : ")
						    + ex->getError()
						)
						ErrorMsg->add(ex->getError());

						ex->isError = false;
					}

					lineNo++;
				}

				Matcher matcher(lexer, ex);
				Parser* parser = new Parser(
				    matcher, ex, scopes[0],
				    import_path, src_project, filemap,
				    ErrorMsg, ImportFlag
				);

				ast::AstNode* node = parser->Parse();	//语法分析

				CATCH {
					THROW_S(
					    StamonString((char*)"Syntax Error: at \"")
					    + import_path
					    + StamonString((char*)"\": ")
					    + toString(parser->ParsingLineNo)
					    + StamonString((char*)": ")
					    + ex->getError()
					)
					ErrorMsg->add(ex->getError());

					ex->isError = false;
				}

				SourceSyntax syntax;
				syntax.program = node;
				syntax.filename = import_path;

				src_project->add(syntax);

				reader.close();

				return NULL;

			}

			ast::AstExpression* expression() {
				ast::AstExpression* rst;
				ast::AstBinary* val = binary_operator();
				CE
				if(
				    check(TokenAssign)
				    ||check(TokenAddAss)||check(TokenSubAss)
				    ||check(TokenMulAss)||check(TokenDivAss)
				    ||check(TokenModAss)
				    ||check(TokenAndAss)||check(TokenOrAss)
				    ||check(TokenXOrAss)
				    ||check(TokenLSHAss)||check(TokenRSHAss)
				) {
					//看到赋值的token
					ast::AstLeftValue* left = left_value(val);	//解析成左值
					CE
					int ass_type = _pop->type;	//弹出赋值的token
					ast::AstExpression* right = expression();
					CE
					return Ast<ast::AstExpression>(
					           left->lineNo, left, ass_type, right
					       );
				} else {
					//单纯的右值表达式
					return Ast<ast::AstExpression>(val->lineNo, val);
				}
			}

			ast::AstLeftValue* left_value(ast::AstBinary* val) {
				//用于将右值表达式转换为左值表达式
				//（即ast::AstBinary* -> ast::AstLeftValue*）
				//如果无法转换，则报错

				/*
				 * 一个左值表达式的ast格式应该是
				 * binary_operator: -1
				 	* unary_operator: -1
					 	* quark: IDEN
						* postfix: 下标后缀或成员后缀
						* postfix: 下标后缀或成员后缀
						...
				*/

				ast::AstNode* quark;
				ArrayList<ast::AstNode*>* postfixs
				    = new ArrayList<ast::AstNode*>();

				if(val->getOperatorType()!=-1) {	//binary_operator: -1
					THROW_S(err::LvalueRequieredLeftOperand())
				}

				ast::AstUnary* unary = (ast::AstUnary*)val->Children()->at(0);

				if(unary->getOperatorType()!=-1) {	//unary_operator: -1
					THROW_S(err::LvalueRequieredLeftOperand())
				}

				ArrayList<ast::AstNode*>* children = unary->Children();

				quark = children->at(0);

				//开始分析左值表达式

				if(quark->getType()!=ast::AstIdentifierType) {
					//quark: IDEN
					THROW_S(err::LvalueRequieredLeftOperand())
				}

				CE

				for(int i=1,len=children->size(); i<len; i++) {
					//postfix: 下标后缀或成员后缀

					ast::AstPostfix* p;		//转换成右值后缀
					p = (ast::AstPostfix*)children->at(i);

					if(
					    p->getPostfixType()!=ast::PostfixMemberType
					    &&p->getPostfixType()!=ast::PostfixElementType
					) {	//如果不满足左值后缀条件
						THROW_S(err::LvalueRequieredLeftOperand())
					}


					ast::AstLeftPostfix* tmp = Ast<ast::AstLeftPostfix>(
					                               p->lineNo,
					                               p->getPostfixType(),
					                               p->Children()->at(0)
					                           );

					postfixs->add(tmp);


				}

				CE

				return Ast<ast::AstLeftValue>(
				           quark->lineNo,
				           (ast::AstIdentifierName*)quark,
				           postfixs
				       );

			}

			ast::AstBinary* binary_operator() {
				ast::AstNode* value = _bin_operator(1);
				CE
				return Ast<ast::AstBinary>(value->lineNo, value);
			}

			ast::AstNode* _bin_operator(int layer) {
				//双目运算符的主要分析算法在这里实现
				//这个函数是一个递归函数

				if(layer==11) {
					//递归边界
					return unary_operator();
				}

				ast::AstNode* rst = _bin_operator(layer+1);
				CE

				Token* op = matcher.Peek(0);

				while (bin_layer[layer-1][0]<=op->type
				        &&op->type<=bin_layer[layer-1][1]) {
					//只有还有运算符
					_pop;	//弹出运算符
					ast::AstNode* right = _bin_operator(layer+1);
					CE
					rst = Ast<ast::AstBinary>(
					          op->lineNo,
					          op->type-38,
					          rst,
					          right
					      );
					//其中，op->type-38其实是将token里的运算符映射到ast中的运算符
					//例如TokenBitOR-38 = 2 = BinaryBitORType

					op = matcher.Peek(0);
				}

				return rst;
			}

			//双目运算符的迭代分析将会在后续实现

			ast::AstUnary* unary_operator() {
				//判断是否还有前缀的单目运算符
				unary_check(TokenAdd, UnaryPositiveType)
				unary_check(TokenSub, UnaryNegativeType)
				unary_check(TokenBitNot, UnaryInverseType)
				unary_check(TokenLogNot, UnaryNotType)
				//如果没有，则直接返回quark { postfix }
				ast::AstNode* q = quark();
				CE
				ArrayList<ast::AstNode*>* postfixs
				    = new ArrayList<ast::AstNode*>();
				while(
				    check(TokenLBR)
				    ||check(TokenLSB)
				    ||check(TokenMember)
				) {
					//只要还有后缀运算符，就匹配
					postfixs->add(postfix());
					CE
				}
				return Ast<ast::AstUnary>(q->lineNo, q, postfixs);
			}

			ast::AstPostfix* postfix() {
				if(check(TokenLBR)) {
					//调用函数后缀
					ast::AstArguments* rst = arguments();
					CE
					return Ast<ast::AstPostfix>(
					           rst->lineNo , ast::PostfixCallType, rst
					       );
				}
				if(check(TokenLSB)) {
					//下标后缀
					int line = _pop->lineNo;
					ParsingLineNo = line;
					ast::AstExpression* expr = expression();
					CE
					match(TokenRSB);
					return Ast<ast::AstPostfix>(
					           line,
					           ast::PostfixElementType,
					           (ast::AstNode*)expr
					       );
				}
				if(check(TokenMember)) {

					int line = _pop->lineNo;

					ParsingLineNo = line;

					//还要特判是不是.new
					if(check(TokenNew)) {
						match(TokenNew);
						//特性支持：如果新建对象的构造函数无参数，可以省略括号
						if(check(TokenLBR)==false) {
							return Ast<ast::AstPostfix> (
							           line,
							           ast::PostfixNewType,
							           (ast::AstNode*) Ast<ast::AstArguments>(
							               line,
							               new ArrayList<ast::AstNode*>()
							           )
							           //空的参数列表
							       );
						} else {
							//正常的.new(...)
							ast::AstArguments* rst = arguments();
							CE
							return Ast<ast::AstPostfix> (
							           rst->lineNo ,
							           ast::PostfixNewType, (ast::AstNode*)rst
							       );
						}
					}

					//正常的访问成员
					IdenToken* iden_tok = (IdenToken*)match(TokenIden);
					CTH("the member name must be an identifier");
					CE;
					ast::AstIdentifierName* iden = Ast<ast::AstIdentifierName>(
					                                   iden_tok->lineNo,
					                                   iden_tok->iden
					                               );
					return Ast<ast::AstPostfix>(
					           line, ast::PostfixMemberType, iden
					       );
				}
				THROW_S(err::InvalidSyntax())
				return NULL;
			}

			ast::AstArguments* arguments() {
				ArrayList<ast::AstNode*>* exprs
				    = new ArrayList<ast::AstNode*>();
				int line = match(TokenLBR)->lineNo;
				CE
				if(check(TokenRBR)==false) {
					//左括号后没有紧跟着右括号，代表有传参数
					exprs->add(expression());
					CE
				}
				while(check(TokenRBR)==false) {
					//只要没有匹配到右括号
					match(TokenCmm);
					CE
					exprs->add(expression());
					CE
				}
				_pop;	//弹出右括号
				return Ast<ast::AstArguments>(line, exprs);
			}

			ast::AstNode* quark() {
				if(check(TokenIden)) {
					return IDEN();
				}
				if(
				    check(TokenInt)||check(TokenDouble)
				    ||check(TokenTrue)||check(TokenFalse)
				) {	//分析一个数字

					return NUM();
				}
				if(check(TokenString)) {
					return STR();
				}
				if(check(TokenNull)) {
					return _psr_NULL();
				}
				if(check(TokenLBC)) {
					return list_literal();
				}
				if(check(TokenLSB)) {
					return array_literal();
				}
				if(check(TokenLBR)) {
					_pop;
					ast::AstExpression* expr = expression();
					CE
					match(TokenRBR);
					CTH("the parentheses are not closed")
					return expr;
				}
				if(check(TokenFunc)) {
					return anon_func();
				}
				if(check(TokenClass)
				        ||matcher.Peek(0)->type==TokenClass) {
					return anon_class();
				}
				CE
				THROW_S(err::InvalidSyntax());
				return NULL;
			}

			//注意：这里的IDEN()只有在匹配变量名的时候能使用
			//如果是匹配类成员名等，请自行建立ast::AstIdentifierName节点
			ast::AstIdentifierName* IDEN() {
				//分析一个标识符
				IdenToken* tok = (IdenToken*)match(TokenIden);

				//分析作用域
				bool isIdenExist = false;	//标识符是否被定义过

				for(int i=scopes.size()-1; i>=0; i--) {
					if(scopes[i].exist(tok)==true) {
						isIdenExist = true;
						break;
					}
					if(scopes[i].isWall==1) {
						break;	//碰到“墙”，终止搜索
					}
				}

				if(scopes[0].exist(tok)==true) {
					isIdenExist = true;	//全局作用域也要检查
				}

				if(isIdenExist==false) {
					//未声明的标识符
					THROW_S(err::UndefinedVariable(tok->iden))

					return NULL;
				}

				return Ast<ast::AstIdentifierName>(tok->lineNo, tok->iden);
			}

			ast::AstNumber* NUM() {
				if(check(TokenInt)) {
					IntToken* tok = (IntToken*)_pop;
					ParsingLineNo = tok->lineNo;
					return Ast<ast::AstIntNumber>(tok->lineNo, tok->val);
				} else if(check(TokenDouble)) {
					DoubleToken* tok = (DoubleToken*)_pop;
					ParsingLineNo = tok->lineNo;
					return Ast<ast::AstDoubleNumber>(tok->lineNo, tok->val);
				} else if(check(TokenTrue)) { //布尔值在语法分析时会被转换成1和0
					ParsingLineNo = matcher.Peek(0)->lineNo;
					return Ast<ast::AstIntNumber>(_pop->lineNo, 1);
				} else if(check(TokenFalse)) {
					ParsingLineNo = matcher.Peek(0)->lineNo;
					return Ast<ast::AstIntNumber>(_pop->lineNo, 0);
				} else {
					THROW_S(err::InvalidSyntax());
				}
				return NULL;
			}

			ast::AstString* STR() {
				StringToken* tok = (StringToken*)match(TokenString);
				return Ast<ast::AstString>(tok->lineNo, tok->val);
			}

			ast::AstNull* _psr_NULL() {
				Token* t = match(TokenNull);
				CE
				return Ast<ast::AstNull>(
				           t->lineNo
				       );
			}

			ast::AstArrayLiteral* array_literal() {
				Token* t = match(TokenLSB);
				ast::AstArrayLiteral* rst = Ast<ast::AstArrayLiteral>(
				                                t->lineNo,
				                                expression()
				                            );
				match(TokenRSB);
				CTH("the square parentheses are not closed")
				return rst;
			}

			ast::AstListLiteral* list_literal() {
				Token* t = match(TokenLBC);
				ArrayList<ast::AstNode*>* exprs
				    = new ArrayList<ast::AstNode*>();

				if(check(TokenRBC)) {
					//如果列表为空
					_pop;
					return Ast<ast::AstListLiteral>(t->lineNo, exprs);
				}

				exprs->add((ast::AstNode*)expression());
				CE
				while(check(TokenCmm)) {
					_pop;
					exprs->add((ast::AstNode*)expression());
					CE
				}

				ast::AstListLiteral* rst
				    = Ast<ast::AstListLiteral>(t->lineNo, exprs);

				match(TokenRBC);
				CTH("the parentheses are not closed")
				return rst;
			}
	};
}

#undef check
#undef pop
#undef CE

#undef unary_check
#undef GETLN
#undef pushscope
/*
	Name: Compiler.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 12:31
	Description: 编译器头文件
*/



namespace stamon::c {
class Compiler {
public:
	FileMap filemap;
	SyntaxScope global_scope;
	ArrayList<SourceSyntax> *src;
	STMException *ex;
	ArrayList<StamonString> *ErrorMsg;
	ArrayList<StamonString> *WarningMsg;
	bool ImportFlag = false;

	Compiler() {
	}

	Compiler(Compiler &right) {
		filemap = right.filemap;
		global_scope = right.global_scope;
		src = right.src;
		ex = right.ex;
		ErrorMsg = right.ErrorMsg;
		ImportFlag = right.ImportFlag;
	}

	Compiler(STMException *e, ArrayList<StamonString> *error_msg, ArrayList<StamonString> *warning_msg)
		: filemap(e)
		, global_scope(e) {

		src = new ArrayList<SourceSyntax>();
		ex = e;
		ErrorMsg = error_msg;
		WarningMsg = warning_msg;
		
	}

	void compile(StamonString filename, bool isSupportImport) {
		ImportFlag = isSupportImport;

		LineReader reader = filemap.mark(filename);

		CATCH {
			return;
		}

		int lineNo = 1;
		Lexer lexer(ex);

		while (reader.isMore()) {
			StamonString text = reader.getLine();

			CATCH {
				return;
			}

			int index = lexer.getLineTok(lineNo, text);

			CATCH {
				THROW_S(
					StamonString((char *) "Error: at \"") 
					+ filename
					+ StamonString((char *) "\": ") 
					+ toString(lineNo) + StamonString((char *) ":")
					+ toString(index) + StamonString((char *) " : ") 
					+ ex->getError()
				)
				ErrorMsg->add(ex->getError());

				ex->isError = false;
			}

			CATCH_WARNING {
				for (int i = 0, len = WARNING.size(); i < len; i++) {
					StamonString warning_msg = WARNING[i];
					WarningMsg->add(
						StamonString((char *) "Warning: at \"") 
						+ filename
						+ StamonString((char *) "\": ") 
						+ toString(lineNo)
						+ StamonString((char *) ": ") + warning_msg
					);
				}
				ex->isWarning = false;
			}

			lineNo++;
		}

		Matcher matcher(lexer, ex);
		Parser *parser = new Parser(
				matcher, ex, global_scope, filename, src, filemap, ErrorMsg, ImportFlag);

		ast::AstNode *node = parser->Parse(); // 语法分析

		CATCH {
			THROW_S(
				StamonString((char *) "Syntax Error: at \"") 
				+ filename
				+ StamonString((char *) "\": ") 
				+ toString(parser->ParsingLineNo)
				+ StamonString((char *) ": ") 
				+ ex->getError()
			)
			ErrorMsg->add(ex->getError());

			ex->isError = false;
		}

		SourceSyntax syntax;
		syntax.program = node;
		syntax.filename = filename;

		src->add(syntax);

		reader.close();
	}
};
} // namespace stamon::c
/*
	Name: ObjectManager.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 29/09/23 16:56
	Description: 对象管理器
*/


//备注：这个对象管理器的GC还需要再更改（需要把操作栈当中的对象也作为GCRoots）

#define FREE_OBJECT(obj, type, typeID)\
	if(obj->getType()==typeID) {\
		type* p = cast_class(type*, obj);\
		Pool.DeleteObject(p);\
		MemConsumeSize -= sizeof(type);\
	} else
//这个宏用来简便FreeScopeTrash方法的开发
//这个宏只能在ObjectManager.cpp当中使用


namespace stamon::vm {
	typedef datatype::Variable Variable;

	class ObjectScope {		//单个对象作用域
			NumberMap<Variable> scope; //实际上存储的是DataType指针
		public:

			ObjectScope() {}
			ObjectScope(NumberMap<Variable> s) {
				scope = s;
			}

			bool exist(int key) {	//是否存在该标识符
				return scope.containsKey(key);
			}

			Variable* get(int key) {
				//除非你能保证要获取的变量一定存在，否则你最好先用exist函数检查
				return scope.get(key);
			}

			void put(int key, Variable* object) {	//存储一个标识符
				scope.put(key, object);
				return;
			}

			NumberMap<Variable> getScope() {
				return scope;
			}

			ArrayList<datatype::DataType*> getObjects() {
				ArrayList<Variable*> var;
				ArrayList<datatype::DataType*> obj;

				var = getScope().getValList<Variable*>();

				for(int i=0,len=var.size(); i<len; i++) {
					obj.add(var.at(i)->data);
				}

				return obj;
			}
			
			void destroyScope() {
				ArrayList<Variable*> var;

				var = getScope().getValList<Variable*>();

				for(int i=0;i<var.size();i++) {
					delete var[i];
				}
			}
	};

	class ObjectManager {
			unsigned long long MemConsumeSize;     //占用内存大小，按字节计
			unsigned long long MemLimit;	//对象占用内存的最大限制，按字节计
			ArrayList<datatype::DataType*> Objects; //用一个列表维护所有对象
			ArrayList<ObjectScope> Scopes;		//当前的作用域用一个列表来表示
			MemoryPool Pool;				//对象内存池
			//最新的作用域在列表末尾，全局作用域在列表开头

			datatype::NullType NullConst;
			//在新建左值变量的时候，会给变量赋null
			//此时的null来自于这里
			//这个null值不参与gc

		public:

			ArrayList<datatype::DataType*> OPND; //正在计算中的数据，也是GcRoot
			STMException* ex;
			bool is_gc;

			ObjectManager() {}

			ObjectManager(
			    bool isGC, unsigned long long mem_limit,
				int pool_cache_size, STMException* e
			) : Pool(e, mem_limit, pool_cache_size) {
				//构造函数，mem_limit表示最大内存限制，按字节计
				MemConsumeSize = 0;
				MemLimit = mem_limit;
				NullConst.gc_flag = true;	//这个值不参与gc
				is_gc = isGC;
				ex = e;
			}

			datatype::DataType* getNullConst() {
				return (datatype::DataType*)(&NullConst);
			}

			template<class T>
			bool GCConditions(T* object) {
				/*
				* 用于判断此时是否要进行GC
				* 开发者也可以选择重写这个函数
				* 有些开发者编写的GC条件取决于object
				* 所以我编写了这个参数，但是我不会用到
				* 这里默认当对象占用内存以及GC预留内存大于内存限制时触发GC
				*/
				if(is_gc==false) {
					return false;
				}
				int TotalConsumeSize =
				    MemConsumeSize
				    + sizeof(datatype::DataType*) * Objects.size();
				//总占用内存=对象占用内存+GC预留内存
				if(TotalConsumeSize>=MemLimit) {
					return true;
				}
				return false;
			}

			/*
			 * 开发者应该用MallocObject函数创建对象
			 * 这个函数的用法是MallocObject<对象类型>()
			 *
			 * 举例：
			 * 已知引用了stamon::datatype和stamon::vm
			 * 已知有名为manager的ObjectManager对象、名为s的String对象
			 * 创建一个内容为s的字符串对象，应该这么做
			 * StringType* str;
			 * str = manager.MallocObject<StringType>(s);
			*/

			template<class T, typename...Types>
			T* MallocObject(Types&& ...args) {
				//这个代码比较难懂，涉及到形参模板和右值引用
				T* result;      //要返回的对象

				result = Pool.NewObject<T>(args...);		//从内存池新建对象

				CATCH {		//如果GC报错就退出函数
					return NULL;
				}

				MemConsumeSize += sizeof(T);   //更新内存占用数

				if(GCConditions<T>(result)) {      //如果满足GC条件
					GC();
					CATCH {		//如果GC报错就退出函数
						return NULL;
					}
				}

				if(MemConsumeSize>MemLimit) {
					//如果GC后内存还是不够，就报错
					THROW("out of memory")
					return NULL;
				}

				if(result==NULL) {
					//如果物理内存不足，就报错
					THROW("out of physical memory")
					return NULL;
				}

				/*
				 * 注意：一定要在GC后才能添加到列表，否则刚申请的对象可能会被GC掉
				*/


				Objects.add(result);    //添加对象到列表

				return result;  //返回对象
			}

			Variable* NewVariable(int id) {
				Variable* result = new Variable();
				result->data = &NullConst;
				Scopes.at(Scopes.size()-1).put(id, result);
				return result;
			}

			Variable* NewVariable(int id, datatype::DataType* val) {
				Variable* result = new Variable();
				result->data = val;
				Scopes.at(Scopes.size()-1).put(id, result);
				return result;
			}

			Variable* GetVariable(int id) {
				//从最新的作用域到全局作用域逐个查找
				for(int i=Scopes.size()-1; i>=0; i--) {
					if(Scopes.at(i).exist(id) == true) {
						return Scopes.at(i).get(id);
					}
				}
				THROW("undefined identifier")	//未定义标识符
				return NULL;
			}

			void PushScope() {
				ObjectScope scope;
				Scopes.add(scope);
			}

			void PushScope(ObjectScope s) {
				Scopes.add(s);
			}

			void PopScope() {
				Scopes[Scopes.size()-1].destroyScope();
				Scopes.erase(Scopes.size()-1);
			}

			ObjectScope getTopScope() {
				return Scopes[Scopes.size()-1];
			}

			void GC() {
				//垃圾回收函数，是整个项目最难的部分之一
				//采用标准的标记清除算法

				//标记非垃圾对象

				//先把操作数标记为非垃圾对象
				ArrayList<datatype::DataType*> opnd_unscanned = OPND;
				for(int i=0,len=OPND.size(); i<len; i++) {
					OPND[i]->gc_flag = true;
				}
				MarkScopeObject(opnd_unscanned);

				//再根据GC Root标记非垃圾对象
				for(int i=0; i<Scopes.size(); i++) {
					//遍历作用域
					ObjectScope scope = Scopes.at(i);
					//获取当前作用域的变量表
					ArrayList<datatype::DataType*> unscanned;
					//未扫描的对象列表
					InitUnscannedScope(scope, unscanned);
					//把作用域里的变量（也就是GCRoots）加载到unscanned里
					MarkScopeObject(unscanned);
					//遍历该作用域的变量涉及到的全部对象，并且标记他们
				}

				//清除垃圾对象
				CleanScopeTrash();
			}

			void InitUnscannedScope(
			    ObjectScope &scope,
			    ArrayList<datatype::DataType*> &unscanned
			) {
				unscanned = scope.getObjects();
				//把scope的GCRoots全部存到unscanned中
				for(int i=0,len=unscanned.size(); i<len; i++) {
					unscanned.at(i)->gc_flag = true; //把GCRoots标记为已搜索
				}
				return;
			}

			void MarkScopeObject(ArrayList<datatype::DataType*>& unscanned) {
				//遍历该作用域的变量涉及到的全部对象，并且标记他们
				while(unscanned.empty()==false) {
					int len = unscanned.size();
					/*从unscanned列表中删除某个对象o*/
					datatype::DataType* o = unscanned.at(len-1);
					unscanned.erase(len-1);
					/*for(在o中引用的每个对象p)*/

					if(o->getType()==datatype::SequenceTypeID) {
						//扫描列表当中的对象
						datatype::SequenceType* list;
						list = cast_class(datatype::SequenceType*, o);
						//把Variable里的DataType*提取出来
						ArrayList<Variable*> referVariables;
						referVariables = list->getVal();
						ArrayList<datatype::DataType*> referObjects;
						for(int i=0,len=referVariables.size(); i<len; i++) {
							referObjects.add(referVariables.at(i)->data);
						}

						for(int i=0,len=referObjects.size(); i<len; i++) {
							datatype::DataType* p = referObjects.at(i);
							if(p->gc_flag==false) {
								/*
								 * 如果这个对象还没有被扫描过
								 * 那么标记，并放到unscanned中
								*/
								p->gc_flag = true;
								unscanned.add(p);
							}
						}

					}

					if(o->getType()==datatype::ObjectTypeID) {
						//扫描类对象引用的对象
						datatype::ObjectType* obj
						    = cast_class(datatype::ObjectType*, o);
						NumberMap<Variable> map = obj->getVal();
						//获得对象表

						//把Variable里的DataType*提取出来
						ArrayList<Variable*> referVaiables;
						ArrayList<datatype::DataType*> referObjects;
						//引用的对象的列表

						referVaiables = map.getValList<Variable*>();
						for(int i=0,len=referVaiables.size(); i<len; i++) {
							referObjects.add(referVaiables.at(i)->data);
						}

						for(int i=0,len=referObjects.size(); i<len; i++) {
							datatype::DataType* p = referObjects.at(i);
							if(p->gc_flag==false) {
								/*如果这个对象还没有被扫描过，
								那么标记，并放到unscanned中*/
								p->gc_flag = true;
								unscanned.add(p);
							}
						}

					}

					if(o->getType()==datatype::MethodTypeID) {
						datatype::MethodType* func
						    = cast_class(datatype::MethodType*, o);
						datatype::ObjectType* obj = func->getContainer();
						if(obj!=NULL) {
							//有容器，容器也要扫描
							if(obj->gc_flag==false) {
								obj->gc_flag = true;
								unscanned.add(obj);
							}
						}
					}
				}
			}

			void CleanScopeTrash() {
				ArrayList<datatype::DataType*> NewObjects;
				//把清理后有用的对象存储在这个列表里

				for(int i=0,len=Objects.size();i<len;i++) {
					if(Objects.at(i)->gc_flag==false) {
						//垃圾对象
						FreeObject(Objects.at(i));	//释放对象
					} else {
						//非垃圾对象
						Objects.at(i)->gc_flag = false;	//把gc_flag设为false
						NewObjects.add(Objects.at(i));
					}
				}

				Objects = NewObjects;
				//更新对象列表
			}

			void FreeObject(datatype::DataType* o) {
				//释放对象
				delete o;
			}

			~ObjectManager() {
				//释放所有对象
				for(int i=0,len=Objects.size(); i<len; i++) {
					FreeObject(Objects[i]);
				}
			}
	};
} //namespace stamon::vm

#undef FREE_OBJECT
/*
	Name: SFN.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 20/02/24 15:39
	Description: Stamon For Native，即本地库的实现
*/



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
/*
	Name: TypeCalculator.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 04/01/25 16:46
	Description: 类型计算器
*/




//为了方便开发，我编写了一系列简写的宏
#define DTT datatype::DataType
#define INT datatype::IntegerType 
#define FLT datatype::FloatType
#define DBL datatype::DoubleType
#define STR datatype::StringType
#define NUL datatype::NullType
#define SEQ datatype::SequenceType

#define IDINT datatype::IntegerTypeID
#define IDFLT datatype::FloatTypeID
#define IDDBL datatype::DoubleTypeID
#define IDSTR datatype::StringTypeID
#define IDNUL datatype::NullTypeID
#define IDSEQ datatype::SequenceTypeID

#define ISNUM(dt) (\
                    (dt->getType()==IDINT)\
                    ||(dt->getType()==IDFLT)\
                    ||(dt->getType()==IDDBL)\
                  )

#define TYPEERR(dt) THROW_S(getDataTypeName(dt->getType()))

#define NEWDT(type) manager->MallocObject<type>

#define BIND_UN(type, name) UnCalcFunc[type] = &TypeCalculator::name;
#define BIND_BIN(type, name) BinCalcFunc[type] = &TypeCalculator::name;

/*
 * 类型计算是个比较复杂的部分，我很难找到一个优雅的算法来统一所有运算符
 * 我将会建立一个由类方法指针所组成的数组，通过查找数组下标，可以快速找到相关的运算函数
 */

namespace stamon::vm {
    class TypeCalculator {

        public:

            datatype::DataType*
            (TypeCalculator::*UnCalcFunc[ast::UnaryTypeCount])
			(datatype::DataType* a);

            datatype::DataType*
            (TypeCalculator::*BinCalcFunc[ast::BinaryTypeCount])
			(datatype::DataType* a, datatype::DataType* b);
            //由类方法所组成的数组

            STMException* ex;
            ObjectManager* manager;

            TypeCalculator() {}

            TypeCalculator(STMException* e, ObjectManager* man) {
                ex = e;
                manager = man;

                //初始化单目运算函数
                BIND_UN(ast::UnaryPositiveType, UnaryPositive);
                BIND_UN(ast::UnaryNegativeType, UnaryNegative);
                BIND_UN(ast::UnaryNotType     , UnaryNot);
                BIND_UN(ast::UnaryInverseType , UnaryInverse);
                
                //初始化双目运算函数
                BIND_BIN(ast::BinaryLogicORType        , BinaryLogicOR);
                BIND_BIN(ast::BinaryLogicANDType       , BinaryLogicAND);
                BIND_BIN(ast::BinaryBitORType          , BinaryBitOR);
                BIND_BIN(ast::BinaryBitXORType         , BinaryBitXOR);
                BIND_BIN(ast::BinaryBitANDType         , BinaryBitAND);
                BIND_BIN(ast::BinaryEqualityType       , BinaryEquality);
                BIND_BIN(ast::BinaryInequalityType     , BinaryInequality);
                BIND_BIN(ast::BinaryBigThanType        , BinaryBigThan);
                BIND_BIN(ast::BinaryLessThanType       , BinaryLessThan);
                BIND_BIN(ast::BinaryBigThanOrEqualType , BinaryBigThanOrEqual);
                BIND_BIN(ast::BinaryLessThanOrEqualType, BinaryLessThanOrEqual);
                BIND_BIN(ast::BinaryLeftShiftType      , BinaryLeftShift);
                BIND_BIN(ast::BinaryRightShiftType     , BinaryRightShift);
                BIND_BIN(ast::BinaryAddType            , BinaryAdd);
                BIND_BIN(ast::BinarySubType            , BinarySub);
                BIND_BIN(ast::BinaryMultType           , BinaryMult);
                BIND_BIN(ast::BinaryDiviType           , BinaryDivi);
                BIND_BIN(ast::BinaryModType            , BinaryMod);

            }

            inline int AssignOperatorToBinaryOperator(int op) {
                //将运算赋值（例如S）
                switch (op)
                {
                case c::TokenAddAss: return ast::BinaryAddType;
				case c::TokenSubAss: return ast::BinarySubType;
				case c::TokenMulAss: return ast::BinaryMultType;
				case c::TokenDivAss: return ast::BinaryDiviType;
				case c::TokenModAss: return ast::BinaryModType;
				case c::TokenAndAss: return ast::BinaryBitANDType;
				case c::TokenXOrAss: return ast::BinaryBitXORType;
				case c::TokenOrAss:  return ast::BinaryBitORType;
				case c::TokenLSHAss: return ast::BinaryLeftShiftType;
				case c::TokenRSHAss: return ast::BinaryRightShiftType;
                
                default: return -1;
                }
            }

            inline bool CheckUnaryOperator(int optype) {
                /*
                 * 检查该运算符是否合理，是则返回true，否则返回false
                 */
                return ast::UnaryPositiveType<=optype
                    && optype<=ast::UnaryTypeCount;
            }

            bool CheckUnaryOperandType(
                datatype::DataType* a, int optype
            ) {
                /*
                 * 检查运算数的类型是否合理
                 * 合理则返回true，否则返回false
                 * 在调用此函数前，请先调用CheckUnaryOperator来确保运算符是合理的
                 */
                switch (optype)
                {
                case ast::UnaryPositiveType:
                case ast::UnaryNegativeType:
                    //正负运算的类型判断相同，因此case被整合到一起
                    return ISNUM(a);
                case ast::UnaryNotType:
                    return true;
                case ast::UnaryInverseType:
                    return a->getType()==IDINT;
                default:
                    return false;
                }
            }

            datatype::DataType* UnaryOperate(
                datatype::DataType* a, int optype
            ) {
                /*
                 * 单目运算，给定需要运算的a，以及运算类型optype，计算后返回结果
                 * optype以ExprAst.cpp的_UnaryOperatorType为准
                 * 调用该函数之前请先分别调用：
                 * CheckUnaryOperator和CheckUnaryOperandType
                 * 来确保运算符的合理性和运算数类型的合理性
                 */
                return (this->*(UnCalcFunc[optype]))(a);
            }

            inline bool CheckBinaryOperator(int optype) {
                /*
                 * 检查该运算符是否合理，是则返回true，否则返回false
                 */
                return ast::BinaryLogicORType<=optype
                    && optype<=ast::BinaryTypeCount;
            }

            bool CheckBinaryOperandType(
                datatype::DataType* a, int optype, datatype::DataType* b
            ) {
                /*
                 * 检查运算数的类型是否合理
                 * 合理则返回true，否则返回false
                 * 在调用此函数前，请先调用CheckBinaryOperator来确保运算符是合理的
                 */

                //部分运算的类型规则是相同的，因此我把case整合到一起，即跳转到同一代码
                switch(optype) {
                case ast::BinaryLogicORType:
                case ast::BinaryLogicANDType:
                    //任何类型都能参与逻辑运算
                    return true;

                case ast::BinaryBitORType:
                case ast::BinaryBitXORType:
                case ast::BinaryBitANDType:
                case ast::BinaryLeftShiftType:
                case ast::BinaryRightShiftType:
                    //只有整数能参与位运算
                    return a->getType()==IDINT && b->getType()==IDINT;
                
                case ast::BinaryEqualityType:
                case ast::BinaryInequalityType: 
                {   //注意：如果case代码块中要创建变量，则需要用花括号包围代码块
                    //如果同为数字，则可以判等
                    if(ISNUM(a) && ISNUM(b)) {
                        return true;
                    }
                    //如果是非数字且两者非同类，必然无法判等
                    if(a->getType()!=b->getType()) {
                        return false;
                    }
                    //在保障两者同类的情况下，字符串、空值、类、函数都可以判等
                    /*
                     * 我想要解释一下接下来的做法
                     * 显然，用if-else或者switch去逐个判断，是个效率低且冗杂的方法
                     * 因此，我利用了位运算的特性，编写了效率更快且更简洁的代码
                     * 我设立了一个整数变量range：
                     * range的第StringTypeID位、第NullTypeID位等等都被设置为1
                     * 其余位数被设置为0
                     * 如此一来，如果range的第a->getType()位是1
                     * 则a的类型是上述的类型中的一种，即a的类型是允许判等的
                     */
                    constexpr int range = (1<<datatype::StringTypeID)
                                        | (1<<datatype::NullTypeID)
                                        | (1<<datatype::ClassTypeID)
                                        | (1<<datatype::MethodTypeID);
                    return ((1<<a->getType()) & range) != 0;
                }

                case ast::BinaryBigThanType:
                case ast::BinaryLessThanType:
                case ast::BinaryBigThanOrEqualType:
                case ast::BinaryLessThanOrEqualType:
                    //如果是都是数字或者都是字符串，则可以判断
                    return (ISNUM(a) && ISNUM(b))
                         ||(a->getType()==b->getType() && a->getType()==IDSTR);
                
                case ast::BinaryAddType:
                {
                    //数字、字符串和数列都能参与加法运算
                    //为了使代码更简洁，我采用位运算来判断类型，原理同上
                    if(ISNUM(a) && ISNUM(b)) {
                        return true;
                    }
                    if(a->getType()!=b->getType()) {
                        return false;
                    }
                    if(a->getType()==IDSTR || a->getType()==IDSEQ) {
                        return true;
                    } else {
                        return false;
                    }
                }

                case ast::BinarySubType:
                case ast::BinaryMultType:
                case ast::BinaryDiviType:
                case ast::BinaryModType:
                    return ISNUM(a) && ISNUM(b);

                default:
                    return false;
                }
            }

            datatype::DataType* BinaryOperate(
                datatype::DataType* a, int optype, datatype::DataType* b
            ) {
                /*
                 * 双目运算，给定需要运算的a和b，以及运算类型optype，计算后返回结果
                 * optype以ExprAst.cpp的_BinaryOperatorType为准
                 * 调用该函数之前请先分别调用：
                 * CheckBinaryOperator和CheckBinaryOperandType
                 * 来确保运算符的合理性和运算数类型的合理性
                 */
                return (this->*(BinCalcFunc[optype]))(a, b);
            }

            //以下是一些用于数据类型转换的函数

            inline bool toBool(DTT* dt) {
                switch (dt->getType())
                {
                case IDINT:
                    return ((INT*)dt)->getVal()!=0;
                
                case IDFLT:
                    return ((FLT*)dt)->getVal()!=0.0f;
                
                case IDDBL:
                    return ((DBL*)dt)->getVal()!=0.0;

                case IDNUL:
                    return false;
                
                default:
                    return true;
                }
            }

            /*
             * 接下来我将编写toInt、toFloat和toDouble
             * 三个函数用于互相精度转换
             */

            inline int toInt(DTT* dt) {
                //调用此函数前请先确保dt是数字
                switch (dt->getType())
                {
                case IDINT:
                    return (int) ((INT*)dt)->getVal();
                case IDFLT:
                    return (int) ((FLT*)dt)->getVal();
                case IDDBL:
                    return (int) ((DBL*)dt)->getVal();
                default:
                    return 0;
                }
            }

            inline float toFloat(DTT* dt) {
                //调用此函数前请先确保dt是数字
                switch (dt->getType())
                {
                case IDINT:
                    return (float) ((INT*)dt)->getVal();
                case IDFLT:
                    return (float) ((FLT*)dt)->getVal();
                case IDDBL:
                    return (float) ((DBL*)dt)->getVal();
                default:
                   return 0;
                }
            }

            inline double toDouble(DTT* dt) {
                //调用此函数前请先确保dt是数字
                switch (dt->getType())
                {
                case IDINT:
                    return (double) ((INT*)dt)->getVal();
                case IDFLT:
                    return (double) ((FLT*)dt)->getVal();
                case IDDBL:
                    return (double) ((DBL*)dt)->getVal();
                default:
                    return 0;
                }
            }

        private:

            //以下是用于计算数据的函数

            DTT* UnaryPositive(DTT* a) {
                return a;
            }

            DTT* UnaryNegative(DTT* a) {
                switch (a->getType())
                {
                case IDINT:
                    return NEWDT(INT)(
                        -((INT*)a)->getVal()
                    );
                case IDFLT:
                    return NEWDT(FLT)(
                        -((FLT*)a)->getVal()
                    );
                case IDDBL:
                    return NEWDT(DBL)(
                        -((DBL*)a)->getVal()
                    );
                default:
                    return NULL;
                }
            }

            DTT* UnaryNot(DTT* a) {
                return NEWDT(INT)(!toBool(a));
            }
            
            DTT* UnaryInverse(DTT* a) {
                return NEWDT(INT)(
                    ~( ((INT*)a)->getVal() )
                );
            }

            DTT* BinaryLogicOR(DTT* a, DTT* b) {
                return NEWDT(INT)(toBool(a) || toBool(b));
            }

            DTT* BinaryLogicAND(DTT* a, DTT* b) {
                return NEWDT(INT)(toBool(a) && toBool(b));
            }

            //利用宏来简写运算的实现，下同
            #define DEFINE_BIT_OPERATE(name, op) \
            DTT* name(DTT* a, DTT* b) { \
                return NEWDT(INT)( \
                    (((INT*)a)->getVal()) \
                 op (((INT*)b)->getVal()) \
                ); \
            }

            DEFINE_BIT_OPERATE(BinaryBitOR, |);
            DEFINE_BIT_OPERATE(BinaryBitXOR, ^);
            DEFINE_BIT_OPERATE(BinaryBitAND, &);
            DEFINE_BIT_OPERATE(BinaryLeftShift, <<);
            DEFINE_BIT_OPERATE(BinaryRightShift, >>);
            //请在调用前确保位移数是非负数

            #undef DEFINE_BIT_OPERATE

            //将判断数字精度并计算的代码封装到一个宏
            #define MATH_CALCULATION(op) \
                /*从精度最高的DoubleType开始向下判断*/ \
                if(a->getType()==IDDBL || b->getType()==IDDBL) { \
                    return NEWDT(DBL)(toDouble(a) op toDouble(b)); \
                } \
                if(a->getType()==IDFLT || b->getType()==IDFLT) { \
                    return NEWDT(FLT)(toFloat(a) op toFloat(b)); \
                } \
                if(a->getType()==IDINT || b->getType()==IDINT) { \
                    return NEWDT(INT)(toInt(a) op toInt(b)); \
                }

            //封装判等和判不等的运算
            #define DEFINE_EQU_OPERATE(name, op) \
                DTT* name(DTT* a, DTT* b) { \
                    switch (a->getType()) \
                    { \
                    /*特判类和函数*/ \
                    \
                    case datatype::ClassTypeID: \
                        return NEWDT(INT)( \
                                    ((datatype::ClassType*)a)->getVal() \
                                    op ((datatype::ClassType*)b)->getVal() \
                        ); \
                    \
                    case datatype::MethodTypeID: \
                        return NEWDT(INT)( \
                                    ((datatype::MethodType*)a)->getVal() \
                                    op ((datatype::MethodType*)b)->getVal() \
                        ); \
                    \
                    default: \
                        break; \
                    } \
                    \
                    MATH_CALCULATION(op); \
                    \
                    return NULL; \
                }
            
            DEFINE_EQU_OPERATE(BinaryEquality, ==);
            DEFINE_EQU_OPERATE(BinaryInequality, !=);

            #undef DEFINE_EQU_OPERATE

            //封装大小比较的运算
            #define DEFINE_COMPARE_OPERATE(name, op) \
                DTT* name(DTT* a, DTT* b) { \
                    if(a->getType()==IDSTR) { \
                        return NEWDT(INT)( \
                            ((STR*)a)->getVal() \
                            op ((STR*)b)->getVal() \
                        ); \
                    } \
                    \
                    MATH_CALCULATION(op); \
                    \
                    return NULL; \
                    \
                }

            DEFINE_COMPARE_OPERATE(BinaryBigThan, >);
            DEFINE_COMPARE_OPERATE(BinaryLessThan, <);
            DEFINE_COMPARE_OPERATE(BinaryBigThanOrEqual, >=);
            DEFINE_COMPARE_OPERATE(BinaryLessThanOrEqual, <=);
            
            #undef DEFINE_COMPARE_OPERATE

            DTT* BinaryAdd(DTT* a, DTT* b) {

                //需要特判字符串和数列的加法
                if(a->getType()==IDSTR) {
                    return NEWDT(STR)(
                        ((STR*)a)->getVal()
                        + ((STR*)b)->getVal()
                    );
                }

                if(a->getType()==IDSEQ) {
                    return NEWDT(SEQ)(
                        ((SEQ*)a)->getVal()
                        + ((SEQ*)b)->getVal()
                    );
                }

                MATH_CALCULATION(+);

                return NULL;
            }

            DTT* BinaryMod(DTT* a, DTT* b) {
                //请在调用BinaryMod之前确保除数不为0

                //由于浮点取模较为复杂，无法用MATH_CALCULATION简写
                //因此我自己动手实现取模运算

                if(a->getType()==IDDBL || b->getType()==IDDBL) {
                    double x = toDouble(a);
                    double y = toDouble(b);
                    return NEWDT(DBL)(x - (long)(x / y) * y);
                }

                if(a->getType()==IDFLT || b->getType()==IDFLT) {
                    float x = toFloat(a);
                    float y = toFloat(b);
                    return NEWDT(FLT)(x - (long)(x / y) * y);
                }

                if(a->getType()==IDINT || b->getType()==IDINT) {
                    return NEWDT(INT)(toInt(a) % toInt(b)); 
                }

                return NULL;
            }

            #define DEFINE_MATH_OPERATE(name, op) \
                DTT* name(DTT* a, DTT* b) { \
                    MATH_CALCULATION(op); \
                    return NULL; \
                }

            DEFINE_MATH_OPERATE(BinarySub, -);
            DEFINE_MATH_OPERATE(BinaryMult, *);
            DEFINE_MATH_OPERATE(BinaryDivi, /);
            //请在调用BinaryDiv之前确保除数不为0

            #undef DEFINE_MATH_OPERATE


            #undef MATH_CALCULATION

    };
}

#undef INT
#undef FLT
#undef DBL
#undef STR
#undef NUL
#undef SEQ

#undef IDINT
#undef IDFLT
#undef IDDBL
#undef IDSTR
#undef IDNUL
#undef IDSEQ

#undef ISNUM
#undef TYPEERR
#undef NEWDT
/*
	Name: ast::AstRunner.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 11/02/24 14:16
	Description: 语法树的运行器
*/



#define CDT(dt, type) \
	if(dt->getType()!=type##ID) {\
		ThrowTypeError(dt->getType());\
	}\
	CE

//强制检查数据类型（Check Data Type）

#define OPND_PUSH(data)	manager->OPND.add(data);
#define OPND_POP manager->OPND.erase(manager->OPND.size()-1);
#define RUN(ast) runAst(ast); CE
#define GETDT(var, code) \
	if(true) {\
		RetStatus tmp = (code);\
		CE\
		var = tmp.retval->data;\
	}
//Get datatype::DataType，想要安全的获取datatype::DataType，应该使用这个宏

#define CE			CATCH { return RetStatus(RetStatusErr, NullVariablePtr()); }
//如果执行代码中出现异常，直接返回
#define CTH(message)	CATCH { THROW(message) }
//如果执行代码中出现异常，抛出异常
#define CTH_S(message)	CATCH { THROW_S(message) }
//用于绑定函数指针
#define BIND(name) RunAstFunc[ast::Ast##name##Type] = &AstRunner::run##name;

namespace stamon::vm {

	enum RET_STATUS_CODE {	//返回的状态码集合
	    RetStatusErr = -1,	//错误退出（Error）
	    RetStatusNor,		//正常退出（Normal）
	    RetStatusCon,		//继续循环（Continue）
	    RetStatusBrk,		//退出循环（Break）
	    RetStatusRet		//函数返回（Return）
	};

	/*
	 * 由于在运行过程中，传递RetStatus就会传递Variable*
	 * 这些Variable*中：
	 	* 有些是为了传递而生成的（即临时Variable*）
		* 有些是归ObjectManager管理的（即左值Variable*）
		* 有些是不用传递的空指针（即空Variable*）
	 * 为了保障将临时Variable*在合适的时间删除的同时
	 * 不让空Variable*和左值Variable*被删除
	 * 我使用了智能指针
	 * 具体方式就是定义三个继承自智能指针类的子类：
	 	* 左值Variable*（即LeftVariablePtr）
		* 临时Variable*（即RightVariablePtr）
		* 空Variable*（即NullVariablePtr）
	 * 这三个子类会在初始化时指定销毁方式，只有RightVariablePtr会真正销毁指针
	 * 利用这种方式，就能让AstRunner在运行过程中及时清理掉垃圾内存，减小内存泄漏
	 */

	using VariablePtr = EasySmartPtr<Variable>;
	//为了简写，使用using关键字定义（效果类似typedef）

	void __LEFT_VARIABLE_PTR_DESTROY_FUNCTION__(VariablePtr* p) {
		return;
		//左值变量指针不需要被销毁
	}

	void __RIGHT_VARIABLE_PTR_DESTROY_FUNCTION__(VariablePtr* p) {
		delete p->ptr;
		//右值变量指针需要被销毁
	}

	void __NULL_VARIABLE_PTR_DESTROY_FUNCTION__(VariablePtr* p) {
		return;
		//空变量指针不需要被销毁
	}

	class LeftVariablePtr : public VariablePtr {
		public:
		LeftVariablePtr(Variable* ptr)
		: VariablePtr(ptr, __LEFT_VARIABLE_PTR_DESTROY_FUNCTION__)
		{}
	};

	class RightVariablePtr : public VariablePtr {
		public:
		RightVariablePtr(datatype::DataType* ptr)
		: VariablePtr(
			new Variable(ptr), __RIGHT_VARIABLE_PTR_DESTROY_FUNCTION__
		) {}
	};

	class NullVariablePtr : public VariablePtr {
		public:
		NullVariablePtr()
		: VariablePtr(NULL, __NULL_VARIABLE_PTR_DESTROY_FUNCTION__)
		{}
	};

	class RetStatus {	//返回的状态（Return Status）
			//这个类用于运行时
		public:
			int status;	//状态码
			VariablePtr retval;	//返回值（Return-Value），无返回值时为NULL
			RetStatus() : retval(NullVariablePtr()) {}
			RetStatus(const RetStatus& right) : retval(right.retval) {
				status = right.status;
			}
			RetStatus(int status_code, VariablePtr retvalue) 
			: retval(retvalue) {
				status = status_code;
			}
	};

	class AstRunner {

		public:
			ObjectManager* manager;
			RetStatus(AstRunner::*RunAstFunc[ast::AstTypeNum])
			(ast::AstNode* node);
			//由类成员函数指针组成的数组
			ast::AstNode* program;
			bool is_gc;	//是否允许gc
			int gc_mem_limit;	//对象内存最大限制
			ArrayList<datatype::DataType*> tabconst;	//常量表
			ArrayList<StamonString> vm_args;	//虚拟机参数
			STMException* ex;	//异常
			sfn::SFN sfn;
			TypeCalculator typecalculator;

			int RunningLineNo;
			StamonString RunningFileName;

			AstRunner() {
				BIND(Program)
				BIND(DefClass)
				BIND(DefFunc)
				BIND(DefVar)
				BIND(AnonClass)
				BIND(AnonFunc)
				BIND(Block)
				BIND(Break)
				BIND(Continue)
				BIND(IfStatement)
				BIND(WhileStatement)
				BIND(ForStatement)
				BIND(ReturnStatement)
				BIND(SFN)
				BIND(Expression)
				BIND(LeftValue)
				BIND(Binary)
				BIND(Unary)
				BIND(Leaf)
				BIND(Null)
				BIND(ArrayLiteral)
				BIND(ListLiteral)
			}

			StamonString getDataTypeName(int type);
			StamonString getExcutePosition();
			void ThrowTypeError(int type);
			void ThrowPostfixError();
			void ThrowIndexError();
			void ThrowConstantsError();
			void ThrowDivZeroError();
			void ThrowBreakError();
			void ThrowContinueError();
			void ThrowArgumentsError(int form_args, int actual_args);
			void ThrowReturnError();
			void ThrowUnknownOperatorError();
			void ThrowUnknownMemberError(int id);
			void ThrowLengthError();
			void ThrowNegativeShiftError();

			/**
			 * \brief 执行程序
			 *
			 * \param main_node 虚拟机的入口ast节点，即ast::AstProgram
			 * \param isGC 是否允许gc
			 * \param vm_mem_limit 虚拟机内存的最大限制
			 * \param tableConst 常量表
			 * \param args 虚拟机的命令行参数
			 * \param pool_cache_size 内存池缓冲区大小
			 * \param e 异常对象，虚拟机发生异常时会将异常信息存入
			 *
			 * \return 程序的执行状态
			 */

			RetStatus excute(
			    ast::AstNode* main_node, bool isGC, int vm_mem_limit,
			    ArrayList<datatype::DataType*> tableConst,
			    ArrayList<StamonString> args, int pool_cache_size, STMException* e
			) {

				//初始化参数
				program = main_node;
				is_gc = isGC;
				gc_mem_limit = vm_mem_limit;
				tabconst = tableConst;
				vm_args = args;

				ex = e;
				//初始化对象管理器
				manager = new ObjectManager(is_gc, vm_mem_limit,
											pool_cache_size, ex);

				sfn = sfn::SFN(e, manager);

				typecalculator = TypeCalculator(ex, manager);

				//执行程序
				auto st = runAst(program);
				//释放内存

				delete manager;

				return st;
			}

			RetStatus runAst(ast::AstNode* node) {
				RunningLineNo = node->lineNo;
				RunningFileName = node->filename;

				if(node->getType()==-1) {
					//叶子节点
					return runLeaf(node);
				} else {
					return (this->*(RunAstFunc[node->getType()]))(node);				
				}
			}

			RetStatus runProgram(ast::AstNode* node) {
				manager->PushScope();	//全局作用域

				for(int i=0,len=node->Children()->size(); i<len; i++) {
					auto st = RUN(node->Children()->at(i));
					if(st.status!=RetStatusNor) {
						if(st.status==RetStatusRet) {
							ThrowReturnError();
						} else if(st.status==RetStatusBrk) {
							ThrowBreakError();
						} else if(st.status==RetStatusCon) {
							ThrowContinueError();
						}
						CE
					}

				}

				manager->PopScope();

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			datatype::ObjectType* initObject(datatype::ClassType* cls) {
				datatype::ObjectType* rst;
				NumberMap<Variable> membertab;	//成员表
				auto node = cls->getVal();

				/*处理父类*/
				if(node->isHaveFather == true) {
					//有父类，先初始化父类
					auto father_class = manager->GetVariable(
					                        (
					                            (ast::AstIdentifier*)
					                            node
					                            ->Children()
					                            ->at(0)
					                        )
					                        ->getID()
					                    );
					CATCH {
						return NULL;
					}
					if(father_class->data->getType()!=datatype::ClassTypeID) {
						ThrowTypeError(father_class->data->getType());
						return NULL;
					}

					rst = initObject((datatype::ClassType*)(father_class->data));
					CATCH {
						return NULL;
					}

					membertab = rst->getVal();

				} else {
					rst = manager
					      ->MallocObject<datatype::ObjectType>(membertab);

					CATCH {
						return NULL;
					}
				}

				OPND_PUSH(rst);

				/*接着继续初始化*/
				manager->PushScope(ObjectScope(membertab));
				//将membertab注入到新的作用域当中
				//这样所有的操作都会直接通过membertab反馈到rst

				for(int i=0,len=node->Children()->size(); i<len; i++) {
					if(i==0 && node->isHaveFather) {
						//如果有父类，则要从第二个节点开始
						continue;
					}

					auto n = node->Children()->at(i); //用n代替这个复杂的表达式

					if(n->getType()==ast::AstDefClassType) {
						runDefClass(n);
					} else if(n->getType()==ast::AstDefFuncType) {
						runDefMethod(rst, n);
					} else if(n->getType()==ast::AstDefVarType) {
						runDefVar(n);
					}

					CATCH {
						return NULL;
					}

				}

				/*收尾*/
				OPND_POP	//弹出rst
				manager->PopScope();

				return rst;
			}

			RetStatus runMethod(
			    datatype::MethodType* method, ArrayList<ast::AstNode*>* args
			) {
				OPND_PUSH(method);

				/*先获取容器*/
				auto container = method->getContainer();

				/*再获得形参*/
				auto FormArg = *(method->getVal()->Children());
				FormArg.erase(FormArg.size()-1);	//删除ast::AstBlock，只保留参数

				/*接着计算实际参数的表达式*/
				ArrayList<datatype::DataType*> args_val;

				if(container!=NULL) {
					//说明该函数有容器，即该函数是方法
					//规定方法的第一个参数为容器
					args_val.add(container);
					OPND_PUSH(container)
				}

				for(int i=0,len=args->size(); i<len; i++) {

					auto st = RUN(args->at(i));	//计算参数

					args_val.add(st.retval->data);	//加入参数表

					OPND_PUSH(args_val[args_val.size()-1]);	//压入参数
				}

				for(int i=0,len=args_val.size(); i<len; i++) {
					OPND_POP	//弹出参数
				}

				/*然后对比形参和实参的个数*/
				if(FormArg.size()!=args_val.size()) {
					//形式参数个数要等于实际参数个数
					ThrowArgumentsError(FormArg.size(), args_val.size());
					CE
				}

				/*新建作用域*/
				manager->PushScope();

				/*如果有函数名，就存入它*/
				if(method->id!=-1) {
					manager->NewVariable(method->id, method);
				}

				/*存入参数*/
				for(int i=0,len=FormArg.size(); i<len; i++) {
					manager->NewVariable(
					    (
					        (ast::AstIdentifier*)FormArg.at(i)
					    )
					    ->getID(),
					    args_val[i]
					);
				}

				/*执行函数体*/
				auto st = RUN(
				              method
				              ->getVal()
				              ->Children()
				              ->at(FormArg.size())
				          );
				/*
				 * 这里用到了一些简便思路
				 * 我想要运行函数体（即ast::AstBlock）
				 * 函数体位于Children()的最后一个元素
				 * 如果直接暴力的访问最后一个元素，那么代码要写为
				 * method->getVal()->Children()->at(
				 * 		method->getVal()->Children->size()-1
				 * )
				 * 这样太复杂了
				 * 我注意到：
				 * 		FormArg.size()
				 * 		= method->getVal()->Children->size()-1
				 * 所以以FormArg.size()访问函数体
				 */

				/*弹出作用域*/
				manager->PopScope();
				OPND_POP	//弹出method

				/*返回*/
				if(st.status==RetStatusRet) {
					//有返回值
					return RetStatus(RetStatusNor, st.retval);
				}
				//无返回值，返回rst（即null）
				return RetStatus(RetStatusNor, RightVariablePtr(
												manager->getNullConst()
											   )
								);
			}

			RetStatus runBlock(ast::AstNode* node) {
				auto blk_node = (ast::AstBlock*)node;
				ArrayList<ast::AstNode*>* codes = node->Children();

				for(int i=0,len=codes->size(); i<len; i++) {
					auto st = RUN(codes->at(i));
					if(
					    st.status==RetStatusBrk
					    || st.status==RetStatusCon
					    || st.status==RetStatusRet
					) {
						return st;
					}
				}

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runDefVar(ast::AstNode* node) {
				auto val = RUN(node->Children()->at(1));
				manager->NewVariable(
				    (
				        (ast::AstIdentifier*)node->Children()->at(0)
				    )
				    ->getID(),
				    val.retval->data
				);
				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			datatype::MethodType* runDefMethod(
			    datatype::ObjectType* container, ast::AstNode* node) {

				auto iden = (
				                (ast::AstIdentifier*)node->Children()->at(0)
				            )
				            ->getID();

				RetStatus st = runAst(node->Children()->at(1));

				CATCH {
					return NULL;
				}

				auto func = (datatype::MethodType*)st.retval->data;

				func->id = iden;
				func->container = container;

				manager->NewVariable(iden, func);

				return func;
			}

			RetStatus runDefFunc(ast::AstNode* node) {
				auto iden = (
				                (ast::AstIdentifier*)node->Children()->at(0)
				            )
				            ->getID();

				auto st = RUN(node->Children()->at(1));

				auto func = (datatype::MethodType*)st.retval->data;

				func->id = iden;

				manager->NewVariable(iden, func);

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runDefClass(ast::AstNode* node) {
				auto iden = (
				                (ast::AstIdentifier*)node->Children()->at(0)
				            )
				            ->getID();
				auto st = RUN(node->Children()->at(1));

				auto cls = (datatype::ClassType*)st.retval->data;

				manager->NewVariable(iden, cls);

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runAnonClass(ast::AstNode* node) {
				auto ancl_node = (ast::AstAnonClass*)node;
				return RetStatus(
				           RetStatusNor,
				           RightVariablePtr(
				               manager
				               ->MallocObject<datatype::ClassType>(
				                   ancl_node
				               )
				           )
				       );
			}

			RetStatus runAnonFunc(ast::AstNode* node) {
				auto anfc_node = (ast::AstAnonFunc*)node;
				return RetStatus(
				           RetStatusNor,
				           RightVariablePtr(
				               manager
				               ->MallocObject<datatype::MethodType>(
				                   -1, anfc_node, (datatype::ObjectType*)NULL
				               )
				           )

				       );
			}

			RetStatus runForStatement(ast::AstNode* node) {
				auto stm_node = (ast::AstForStatement*)node;

				int iden = (
				               (ast::AstIdentifier*)
				               node->Children()->at(0)
				           )
				           ->getID();	//遍历的标识符

				RetStatus list_st = RUN(node->Children()->at(1));

				datatype::DataType* list_dt = list_st.retval->data;

				OPND_PUSH(list_dt)

				CDT(list_dt, datatype::SequenceType)

				auto list = ((datatype::SequenceType*)list_dt)->getVal();

				for(int i=0; i<list.size(); i++) {

					manager->PushScope();

					//把迭代变量放到作用域当中
					manager->NewVariable(
						iden, list[i]->data
					);

					//每次都新建一个标识符

					auto st = RUN(node->Children()->at(2));

					if(st.status==RetStatusBrk) {
						return RetStatus(RetStatusNor, NullVariablePtr());
					}

					if(st.status==RetStatusRet) {
						return st;
					}

					manager->PopScope();

				}

				OPND_POP	//弹出list_dt

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runWhileStatement(ast::AstNode* node) {
				auto stm_node = (ast::AstWhileStatement*)node;

				RetStatus cond_st = RUN(node->Children()->at(0));

				datatype::DataType* cond = cond_st.retval->data;

				OPND_PUSH(cond)

				manager->PushScope();

				while(typecalculator.toBool(cond)==true) {
					RetStatus st = RUN(node->Children()->at(1));

					if(st.status==RetStatusBrk) {
						OPND_POP;	//弹出cond
						return RetStatus(RetStatusNor, NullVariablePtr());
					}
					if(st.status==RetStatusRet) {
						OPND_POP;	//弹出cond
						return st;
					}

					OPND_POP	//弹出cond
					manager->PopScope();

					RetStatus cond_st = RUN(node->Children()->at(0));
					cond = cond_st.retval->data;

					OPND_PUSH(cond)
					manager->PushScope();
				}

				OPND_POP	//弹出cond
				manager->PopScope();

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runIfStatement(ast::AstNode* node) {
				auto stm_node = (ast::AstIfStatement*)node;

				RetStatus cond_st = RUN(node->Children()->at(0));

				datatype::DataType* cond = cond_st.retval->data;

				OPND_PUSH(cond)

				RetStatus st;

				if(typecalculator.toBool(cond)==true) {

					manager->PushScope();

					st = RUN(node->Children()->at(1));

					manager->PopScope();

				} else if(node->Children()->size()==3) {

					//有三个子节点，证明有else代码块

					manager->PushScope();

					st =RUN(node->Children()->at(2));

					manager->PopScope();

				} else {
					//直接略过本代码块
					st = RetStatus(RetStatusNor, NullVariablePtr());
				}

				if(st.status==RetStatusBrk) {
					ThrowBreakError();
					CE
				}

				if(st.status==RetStatusCon) {
					ThrowContinueError();
					CE
				}

				if(st.status==RetStatusRet) {
					return st;
				}

				OPND_POP	//弹出cond

				return RetStatus(RetStatusNor, NullVariablePtr());
			}

			RetStatus runReturnStatement(ast::AstNode* node) {
				auto st = RUN(node->Children()->at(0));
				return RetStatus(RetStatusRet, st.retval);
			}

			RetStatus runSFN(ast::AstNode* node) {
				int port = ((ast::AstIdentifier*)node->Children()->at(0))
				           ->getID();
				int arg = ((ast::AstIdentifier*)node->Children()->at(1))
				          ->getID();

				Variable* port_var = manager->GetVariable(port);
				CE
				Variable* arg_var = manager->GetVariable(arg);
				CE

				CDT(port_var->data, datatype::StringType)

				OPND_PUSH(port_var->data);
				OPND_PUSH(arg_var->data);

				sfn.call(
				    (
				        (datatype::StringType*)port_var->data
				    )
				    ->getVal(),
				    arg_var
				);
				CE

				OPND_POP;
				OPND_POP;

				return RetStatus(RetStatusNor, NullVariablePtr());
			}


			RetStatus runExpression(ast::AstNode* node) {
				auto expr_node = (ast::AstExpression*)node;
				if(expr_node->ass_type==-1) {
					return runAst(node->Children()->at(0));
				}

				//如果是赋值表达式

				//先得出左右两式
				VariablePtr left_value = runAst(node->Children()->at(0))
				                       .retval;
				CE

				VariablePtr right_value = runAst(node->Children()->at(1))
				                        .retval;
				CE

				if(expr_node->ass_type==c::TokenAssign) {
					//直接赋值
					left_value->data = right_value->data;
					return RetStatus(RetStatusNor, left_value);
				}

				OPND_PUSH(left_value->data);
				OPND_PUSH(right_value->data);

				//特判错误情况

				if(
					(
						expr_node->ass_type==c::TokenDivAss
						|| expr_node->ass_type==c::TokenModAss
					)
					&& typecalculator.toBool(right_value->data)==false
				) {
					//除数为0
					ThrowDivZeroError();
					CE;
				}

				int optype = typecalculator.AssignOperatorToBinaryOperator(
												expr_node->ass_type
											);
				
				if(optype==-1) {
					//符号有误
					ThrowUnknownOperatorError();
					CE;
				}

				if(typecalculator.CheckBinaryOperandType(
									left_value->data, optype, right_value->data
								  )
					==false
				) {
					//运算类型有误
					ThrowTypeError(left_value->data->getType());
					CE;
				}

				//开始运算

				left_value->data = typecalculator.BinaryOperate(
											left_value->data,
											optype,
											right_value->data
								   );
				
				OPND_POP;
				OPND_POP;

				return RetStatus(RetStatusNor, left_value);

			}

			RetStatus runLeftValue(ast::AstNode* node) {
				auto lv_node = (ast::AstLeftValue*)node;
				//获取标识符
				VariablePtr lvalue = LeftVariablePtr(
											manager->GetVariable(
											(
											(ast::AstIdentifier*)
											node->Children()->at(0)
											)
											->getID()
				                   		)
									);
				CE;
				for(int i=1,len=node->Children()->size(); i<len; i++) {
					//分析后缀
					lvalue = runPostfix(
					             node->Children()->at(i), lvalue->data
					         )
					         .retval;
					CE
				}
				return RetStatus(RetStatusNor, lvalue);
			}

			RetStatus runBreak(ast::AstNode* node) {
				return RetStatus(RetStatusBrk, NullVariablePtr());
			}

			RetStatus runContinue(ast::AstNode* node) {
				return RetStatus(RetStatusCon, NullVariablePtr());
			}

			RetStatus runBinary(ast::AstNode* node) {
				ast::AstBinary* bin_node = (ast::AstBinary*)node;
				int optype = bin_node->getOperatorType();

				if(optype==-1) {
					RetStatus st = RUN(bin_node->Children()->at(0));
					return st;
				}

				//获取运算数

				RetStatus left_st = RUN(bin_node->Children()->at(0));
				datatype::DataType* left = left_st.retval->data;
				OPND_PUSH(left);

				RetStatus right_st = RUN(bin_node->Children()->at(1));
				datatype::DataType* right = right_st.retval->data;
				OPND_PUSH(right);

				//先特判运算符和运算类型是否合法
				if(typecalculator.CheckBinaryOperator(optype)==false) {
					ThrowUnknownOperatorError();
					CE;
				}
				
				if(
					typecalculator.CheckBinaryOperandType(
									left, optype, right
								  )
					== false
				) {
					ThrowTypeError(left->getType());
					CE;
				}

				//再判断位移、除法的数值是否合法

				if(
					(
						optype==ast::BinaryLeftShiftType
						||optype==ast::BinaryRightShiftType
					)
					&& typecalculator.toInt(right) < 0
				) {
					//位移数不能为负
					ThrowNegativeShiftError();
					CE;
				}

				if(
					(
						optype==ast::BinaryDiviType
						||optype==ast::BinaryModType
					)
					&& typecalculator.toBool(right)==false
				) {
					//除数不能为0
					//将b转为布尔值，如果b是0，则一定为false
					ThrowDivZeroError();
					CE;
				}
				
				datatype::DataType* rst = typecalculator.BinaryOperate(
															left, optype, right
														 );

				OPND_POP;
				OPND_POP;

				return RetStatus(RetStatusNor, RightVariablePtr(rst));

			}

			RetStatus runUnary(ast::AstNode* node) {
				ast::AstUnary* unary_node = (ast::AstUnary*)node;

				if(unary_node->getOperatorType()==-1) {
					//先分析quark
					datatype::DataType* quark;
					GETDT(quark, runAst(node->Children()->at(0)))
					OPND_PUSH(quark);

					//接着逐个分析后缀
					for(int i=1,len=node->Children()->size(); i<len; i++) {
						GETDT(
						    quark,
						    runPostfix(node->Children()->at(i), quark)
						)
						OPND_POP;
						OPND_PUSH(quark);
					}

					OPND_POP;

					return RetStatus(RetStatusNor, RightVariablePtr(quark));
				}

				//如果是单目运算符

				datatype::DataType* src;
				int optype = unary_node->getOperatorType();

				GETDT(src, runAst(node->Children()->at(0)));

				OPND_PUSH(src);

				//特判运算符和运算类型是否合法
				if(typecalculator.CheckUnaryOperator(optype)==false) {
					ThrowUnknownOperatorError();
					CE;
				}
				
				if(
					typecalculator.CheckUnaryOperandType(src, optype) == false
				) {
					ThrowTypeError(src->getType());
					CE;
				}

				datatype::DataType* rst = typecalculator
										  .UnaryOperate(src, optype);

				OPND_POP;

				return RetStatus(RetStatusNor, RightVariablePtr(rst));

			}

			RetStatus runPostfix(ast::AstNode* node, datatype::DataType* src) {
				ast::AstPostfix* postfix_node = (ast::AstPostfix*)node;
				int postfix_type = postfix_node->getPostfixType();

				if(postfix_type==ast::PostfixMemberType) {
					//访问成员
					CDT(src, datatype::ObjectType)
					ast::AstIdentifier* member = (ast::AstIdentifier*)
					                             postfix_node
					                             ->Children()
					                             ->at(0);

					int member_id = member->getID();

					if(
					    ((datatype::ObjectType*)src)
					    ->getVal()
					    .containsKey(member_id)==false
					) {
						//未知成员
						ThrowUnknownMemberError(member_id);
						CE
					}

					return RetStatus(
					           RetStatusNor, LeftVariablePtr(
									((datatype::ObjectType*)src)	//类对象
									->getVal()	//获取成员表
									.get(member_id)	//获取成员
							   )
					       );
				}

				if(postfix_type==ast::PostfixElementType) {
					//取下标
					CDT(src, datatype::SequenceType)
					ast::AstExpression* expr = (ast::AstExpression*)
					                           postfix_node
					                           ->Children()
					                           ->at(0);
					RetStatus st = RUN(expr);

					VariablePtr index_var = st.retval;

					datatype::DataType* index_dt = index_var->data;

					CDT(index_dt, datatype::IntegerType)

					ArrayList<Variable*> list = ((datatype::SequenceType*)src)
					                            ->getVal();

					int index = ((datatype::IntegerType*)index_dt)->getVal();

					if(index<0 ||index>=list.size()) {
						ThrowIndexError();
						return RetStatus(RetStatusErr, NullVariablePtr());
					}

					return RetStatus(RetStatusNor, LeftVariablePtr(
														list[index]
												   )
									);

				}

				if(postfix_type==ast::PostfixNewType) {
					CDT(src, datatype::ClassType)

					ast::AstArguments* arg_ast = (ast::AstArguments*)
					                             postfix_node
					                             ->Children()
					                             ->at(0);
					ArrayList<ast::AstNode*>* arg = arg_ast->Children();

					OPND_PUSH(src)

					datatype::ObjectType* obj_dt = initObject(
					                                   (datatype::ClassType*)src
					                               );
					CE

					OPND_POP

					OPND_PUSH(obj_dt)

					if(obj_dt->getVal().containsKey(0)==true) {
						//有构造函数
						datatype::DataType* init_func_dt = obj_dt
						                                   ->getVal()
						                                   .get(0)
						                                   ->data;
						CDT(init_func_dt, datatype::MethodType)
						runMethod((datatype::MethodType*)init_func_dt, arg);
						CE
					}

					OPND_POP

					return RetStatus(RetStatusNor, RightVariablePtr(obj_dt));

				}

				if(postfix_type==ast::PostfixCallType) {
					//调用函数
					CDT(src, datatype::MethodType);
					ast::AstArguments* arg_ast = (ast::AstArguments*)
					                             postfix_node
					                             ->Children()
					                             ->at(0);
					ArrayList<ast::AstNode*>* arg = arg_ast->Children();

					OPND_PUSH(src)

					auto st = runMethod((datatype::MethodType*)src, arg);
					CE

					OPND_POP

					return st;
				}

				ThrowPostfixError();
				return RetStatus(RetStatusErr, NullVariablePtr());
			}

			RetStatus runArrayLiteral(ast::AstNode* node) {
				ast::AstArrayLiteral* literal = (ast::AstArrayLiteral*)node;

				//获得长度
				RetStatus st = RUN(literal->Children()->at(0));

				datatype::DataType* length = st.retval->data;
				CDT(length, datatype::IntegerType)

				if(((datatype::IntegerType*)length)->getVal()<0) {
					//错误的数列长度
					ThrowLengthError();
					CE;
				}

				OPND_PUSH(length)

				VariablePtr rst_var = RightVariablePtr(
				    manager->MallocObject<datatype::SequenceType>(
				        ((datatype::IntegerType*)length)->getVal()
				    )
				);

				CE

				for(
				    int i=0,len=((datatype::IntegerType*)length)->getVal();
				    i<len;
				    i++
				) {
					((datatype::SequenceType*)rst_var->data)->sequence[i]
					    = new Variable(
					    manager->getNullConst()
					);
				}

				CE

				OPND_POP

				return RetStatus(RetStatusNor, rst_var);
			}

			RetStatus runListLiteral(ast::AstNode* node) {

				ast::AstListLiteral* literal = (ast::AstListLiteral*)node;
				ArrayList<Variable*> content;

				for (
				    int i=0,len=literal->Children()->size();
				    i<len;
				    i++
				) {
					RetStatus st = RUN(literal->Children()->at(i));

					datatype::DataType* item = st.retval->data;
					OPND_PUSH(item)
					content.add(new Variable(item));
				}


				VariablePtr rst_var = RightVariablePtr(
				    manager->MallocObject<datatype::SequenceType>(
				        content
				    )
				);

				CE

				for (
				    int i=0,len=literal->Children()->size();
				    i<len;
				    i++
				) {
					OPND_POP
				}	//弹出所有计算中的数据


				return RetStatus(RetStatusNor, rst_var);
			}

			RetStatus runIden(ast::AstNode* node) {
				int index = ((ir::AstLeaf*)node)->getVal();

				if(index>=tabconst.size()) {
					ThrowConstantsError();
					return RetStatus(RetStatusErr, NullVariablePtr());
				}

				datatype::DataType* rst = tabconst[index];

				CDT(rst, ir::IdenConstType)

				return RetStatus(RetStatusNor, RightVariablePtr(rst));
			}

			RetStatus runLeaf(ast::AstNode* node) {
				int index = ((ir::AstLeaf*)node)->getVal();
				if(index>=tabconst.size()) {
					ThrowConstantsError();
					return RetStatus(RetStatusErr, NullVariablePtr());
				}

				datatype::DataType* rst = tabconst[index];

				if(rst->getType()==-1) {
					//标识符

					return RetStatus(
					           RetStatusNor, LeftVariablePtr(
												manager->GetVariable(index)
											 )
					       );
				}

				return RetStatus(
				           RetStatusNor, RightVariablePtr(rst)
				       );
			}

			RetStatus runNull(ast::AstNode* node) {
				return RetStatus(
				           RetStatusNor,
				           RightVariablePtr(
				               manager->getNullConst()
				           )
				       );
			}

	};
} //namespace stamon::vm

//一些冗余的函数放到后面

inline StamonString stamon::vm::AstRunner::getDataTypeName(int type) {
	switch (type)
	{
	case stamon::datatype::ClassTypeID:		return StamonString("class");
	case stamon::datatype::MethodTypeID:	return StamonString("function");
	case stamon::datatype::IntegerTypeID:	return StamonString("int");
	case stamon::datatype::FloatTypeID:		return StamonString("float");
	case stamon::datatype::DoubleTypeID:	return StamonString("double");
	case stamon::datatype::ObjectTypeID:	return StamonString("object");
	case stamon::datatype::SequenceTypeID:	return StamonString("sequence");
	case stamon::datatype::StringTypeID:	return StamonString("string");
	case -1:								return StamonString("identifier");
	default:								return StamonString("unknown-type");
	}
}

#undef CDT
#undef OPND_PUSH
#undef OPND_POP
#undef RUN
#undef GETDT
#undef CTH
#undef CTH_S
#undef OPERATE_BINARY
#undef ASMD_OPERATE
#undef DIV_ERRCHECK
#undef MATH_OPERATE
#undef BIND
#undef CHECK_ASS
#undef CHECK_INT_ASS
/*
	Name: AstRunnerExceptionMessage.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 08/03/25 13:53
	Description: 此文件用于存放AstRunner的异常信息生成函数
*/



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
/*
	Name: Stamon.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 24/12/23 11:23
	Description: Stamon头文件
*/



namespace stamon {
	// using namespace stamon::ir;
	// using namespace stamon::datatype;
	// using namespace stamon::c;
	// using namespace stamon::vm;
	// using namespace stamon::sfn;

	class Stamon {
		public:
			STMException *ex;
			ArrayList<StamonString> ErrorMsg;
			ArrayList<StamonString> WarningMsg;

			int VerX, VerY, VerZ;   //这三个变量代表版本为X.Y.Z

			Stamon() {}

			void Init() {
				ex = new STMException();
				VerX = STAMON_VER_X;
				VerY = STAMON_VER_Y;
				VerZ = STAMON_VER_Z;
			}

			void compile(
			    StamonString src, StamonString dst, bool isSupportImport, bool isStrip
			) {
				c::Compiler compiler(ex, &ErrorMsg, &WarningMsg);

				compiler.compile(src, isSupportImport);
				//开始编译

				if(ErrorMsg.empty() == false) {
					//出现报错
					return;
				}

				//将编译结果整合成一个AST

				ArrayList<ast::AstNode*>* program 
										  = new ArrayList<ast::AstNode*>();

				for(int i=0,len=compiler.src->size(); i<len; i++) {
					//获取该文件的AST根节点
					ast::AstNode* node = compiler.src->at(i).program;

					//放入总节点中
					(*program) += *(node->Children());

					//node只是暂存了ast，需要被删除
					//为了防止删除时将子节点也删除，因此需要先清空字节点列表
					node->Children()->clear();
					delete node;
				}

				ast::AstNode* node = new ast::AstProgram(program);

				//初始化node
				node->filename = StamonString();
				node->lineNo = -1;

				//编译为IR

				ir::AstIRConverter converter;

				ArrayList<ir::AstIR> ir_list = converter.ast2ir(node);

				delete node;	//删除AST

				//开始写入

				ArrayList<datatype::DataType*>
				ir_tableconst = converter.tableConst;

				ir::AstIrWriter writer(ex);

				writer.write(
					ir_list, ir_tableconst, dst, isStrip,
					VerX, VerY, VerZ
				);

				converter.destroyConst(converter.tableConst);

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				return;
			}

			void run(StamonString src, bool isGC, int MemLimit, int PoolCacheSize) {

				//实现流程：文件读取器->字节码读取器->IR读取器->虚拟机

				ArrayList<ir::AstIR> ir_list;

				BinaryReader reader(ex, src);	//打开文件

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				ir::AstIrReader ir_reader(reader.read(), reader.size, ex);
				//初始化字节码读取器

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				if(ir_reader.ReadHeader()==false) {
					//读取文件头
					ErrorMsg.add(ex->getError());
					return;
				}

				ir_list = ir_reader.ReadIR();

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				reader.close();	//关闭文件

				//复制版本号
				VerX = ir_reader.VerX;
				VerY = ir_reader.VerY;
				VerZ = ir_reader.VerZ;

				ir::AstIRConverter converter;	//初始化转换器

				converter.tableConst = ir_reader.tableConst;
				//复制常量表到转换器

				ast::AstNode* running_node = converter.ir2ast(ir_list);

				vm::AstRunner runner;

				runner.excute(
				    running_node, isGC, MemLimit, converter.tableConst,
				    ArrayList<StamonString>(), PoolCacheSize ,ex
				);

				delete running_node;	//删除AST

				converter.destroyConst(converter.tableConst);

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				WarningMsg = runner.ex->getWarning();

				return;
			}

			void strip(StamonString src) {
				//剥夺调试信息
				//这些代码直接改编自run方法和compile方法

				ArrayList<ir::AstIR> ir_list;

				BinaryReader reader(ex, src);	//打开文件

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				ir::AstIrReader ir_reader(reader.read(), reader.size, ex);
				//初始化字节码读取器

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				if(ir_reader.ReadHeader()==false) {
					//读取文件头
					ErrorMsg.add(ex->getError());
					return;
				}

				ir_list = ir_reader.ReadIR();

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				reader.close();	//关闭文件

				ArrayList<datatype::DataType*>
				ir_tableconst = ir_reader.tableConst;

				//写入魔数
				
				ir::AstIrWriter writer(ex);

				writer.write(
					ir_list, ir_tableconst, src, true,
					VerX, VerY, VerZ
				);

				ir::AstIRConverter converter;
				//利用转换器来销毁常量表
				converter.destroyConst(ir_tableconst);

				CATCH {
					ErrorMsg.add(ex->getError());
					return;
				}

				WarningMsg = ex->getWarning();

				return;
			}

			ArrayList<StamonString> getErrorMessages() {
				return ErrorMsg;
			}

			ArrayList<StamonString> getWarningMessages() {
				return WarningMsg;
			}

			~Stamon() {
				delete ex;
			}
	};
} //namespace stamon
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


using namespace stamon;
using namespace stamon::config;	//由于命名空间前缀过于繁琐，因此引用此命名空间



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
