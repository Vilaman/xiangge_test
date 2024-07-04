#pragma once
#include <algorithm>// 算法库
#include <fstream>	// 文件输入输出操作
#include <string>	// 字符串库
#include <list>		// list容器
#include <map>		// map容器

// for std::string <==> std::wstring convert
#include <codecvt>	// 封装字符串的转换
#include <locale>	// 标准库
// 调试开关
#ifndef INICPP_DEBUG 
#define INICPP_DEBUG 
#include <array>	// array容器
#include <ctime>	// 时间库
#include <iostream>	// 输入输出流

class TimeFormatter {
public:
	// 获取时间
	static std::string format(const std::string& format = "%Y-%m-%d %H:%M:%S") {
		std::time_t t = std::time(nullptr);	// 获取当前时间戳
		std::tm tm = *std::localtime(&t);	// 将时间戳转换为本地时间结构体
		std::array<char, 100> buffer;		// 创建一个大小为100的字符数组作为格式化输出的缓冲区
		// 使用 strftime 函数将本地时间格式化为指定的字符串格式
		std::strftime(buffer.data(), buffer.size(), format.c_str(), &tm);
		return buffer.data();				// 将字符数组转换为 std::string
	}
};
// 代码信息
#define CODE_INFO std::string(" | Code:\'file:") + std::string(__FILE__) + ",function:" + std::string(__FUNCTION__) + ",line:" + std::to_string(__LINE__) + '\''
// ini文件的调试信息
#define INI_DEBUG(x) std::cout << "INICPP " << TimeFormatter::format() << " : " << x << CODE_INFO << std::endl

#else  
#define INI_DEBUG(x)
#endif 
// 键-值 节点 结构体
typedef struct KeyValueNode {
	std::string Value = "";	// 数值
	int lineNumber = -1;	// 文本行以1开头
};

/*********************************************
*				[节 Section]					 *
*				键(Key) = 值(value)			 *
*********************************************/

class section {
public:
	// 默认构造函数，初始化 _sectionName 为空字符串
	section() :_sectionName() {}
	// 显式构造函数，设置 _sectionName
	explicit section(const std::string& sectionName) : _sectionName(sectionName) {}
	// 返回节的名称
	const std::string& name();
	// 获取与键 Key 相关联的值
	const std::string getValue(const std::string& Key);
	// 设置节的名称和行号
	void setName(const std::string& name, const int& lineNumber);
	// 设置键 Key 对应的值和行号
	void setValue(const std::string& Key, const std::string& Value, const int line);
	// 将另一个节 sec 的映射表附加到当前节的映射表中
	void append(section& sec);
	// 检查节的映射表中是否存在指定的键 Key
	bool isKeyExist(const std::string& Key);
	// 获取节的映射表中最后一个键值对的行号
	int getEndSection();
	// 获取与键 Key 相关联的行号
	int getLine(const std::string& Key);
	// 重载操作符 []，返回与键 Key 相关联的值
	const std::string operator[](const std::string& Key);
	// 清空节，重置所有值
	void clear();
	// 检查节是否为空
	bool isEmpty() const;
	// 将与键 Key 相关联的值转换为整数
	int toInt(const std::string& Key);
	// 返回与键 Key 相关联的值作为字符串
	std::string toString(const std::string& Key);
	// 返回与键 Key 相关联的值作为宽字符串 (wstring)
	std::wstring toWString(const std::string& Key);
	// 将与键 Key 相关联的值转换为双精度浮点数
	double toDouble(const std::string& Key);
private:
	std::string _sectionName;							// 节的名称
	std::map<std::string, KeyValueNode> _sectionMap;	// 存储键值对的映射表
	int _lineNumber = -1;								// 节的行号
};

class ini {
public:
	// 添加一个节对象到_iniInfoMap中
	void addSection(section& sec);
	// 移除指定名称的节
	void removeSection(const std::string& sectionName);
	// 检查是否存在指定名称的节
	bool isSectionExists(const std::string& sectionName);
	// 获取所有节的名称列表
	std::list<std::string> getSectionsList();
	// 重载操作符 []，返回指定名称的节对象引用
	const section& operator[](const std::string& sectionName);
	// 获取_iniInfoMap中节的数量
	const int getSectionSize();
	// 获取指定节和键的值
	std::string getValue(const std::string& sectionName, const std::string& Key);
	// 获取包含指定键的行号
	int getLine(const std::string& Key);
	// 获取指定节中包含指定键的行号
	int getLine(const std::string& sectionName, const std::string& Key);
	// 清空_iniInfoMap，移除所有节
	inline void clear() { _iniInfoMap.clear(); }
	// 检查_iniInfoMap是否为空
	inline bool empty() { return _iniInfoMap.empty(); }
protected:
	std::map<std::string/*Section Name*/, section> _iniInfoMap;
};

class IniManager
{
public:
	// 构造函数：使用配置文件名初始化 IniManager，并解析配置文件
	explicit IniManager(const std::string& configFileName) : _configFileName(configFileName) { parse(); }
	// 析构函数：关闭 Ini 文件流
	~IniManager() { _iniFile.close(); };
	// 重载操作符：通过节名访问节对象
	section operator[](const std::string& sectionName) { return _iniData[sectionName]; }
	// 解析配置文件，填充 _iniData
	void parse();
	// 修改节中键的字符串值
	bool modify(const std::string& Section, const std::string& Key, const std::string& Value, const std::string& comment = "");
	// 修改节中键的整数值
	bool modify(const std::string& Section, const std::string& Key, const int& Value, const std::string& comment = "");
	// 修改节中键的双精度浮点数值
	bool modify(const std::string& Section, const std::string& Key, const double& Value, const std::string& comment = "");
	// 修改节中键的宽字符串值
	bool modify(const std::string& Section, const std::string& Key, const std::wstring& Value, const std::string& comment = "");
	// 修改键的注释
	bool modifyComment(const std::string& Section, const std::string& Key, const std::string& comment);
	// 检查指定节是否存在
	bool isSectionExists(const std::string& sectionName);
	// 获取所有节名的列表
	inline std::list<std::string> getSectionsList() { return _iniData.getSectionsList(); }
private:
	bool filterData(std::string& data);

private:
	ini _iniData;				// 管理 INI 数据的 ini 类实例
	int _SumOfLines;			// INI 文件的总行数
	std::fstream _iniFile;		// INI 文件的文件流
	std::string _configFileName;// 配置文件名
};