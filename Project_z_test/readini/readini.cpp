#include "readini.h"
// 返回配置节的名称
const std::string& section::name() {
	return _sectionName;
}
// 根据键 Key 获取配置节中对应的值
const std::string section::getValue(const std::string& Key) {
	// 如果键不存在，返回空字符串
	if (!_sectionMap.count(Key)) {
		return "";
	}
	// 返回键对应的值
	return _sectionMap[Key].Value;
}
// 设置配置节的名称和行号
void section::setName(const std::string& name, const int& lineNumber) {
	_sectionName = name;		// 设置节名
	_lineNumber = lineNumber;	// 设置行号
}
// 设置配置节中指定键 Key 的值和行号
void section::setValue(const std::string& Key, const std::string& Value, const int line) {
	_sectionMap[Key].Value = Value;		// 设置键 Key 对应的值
	_sectionMap[Key].lineNumber = line;	// 设置键对应的行号
}
// 将另一个配置节 sec 的内容附加到当前配置节的映射表中
void section::append(section& sec) {
	// 插入 sec 的映射表内容到当前映射表
	_sectionMap.insert(sec._sectionMap.begin(), sec._sectionMap.end());
}
// 检查配置节中是否存在指定的键 Key
bool section::isKeyExist(const std::string& Key) {
	// 判断键是否存在，并返回结果
	return !_sectionMap.count(Key) ? false : true;
}
// 获取配置节中最后一个键值对的行号
int section::getEndSection() {
	int line = -1;	// 初始化行号为 -1
	// 如果映射表为空但节名不为空，返回节的行号
	if (_sectionMap.empty() && _sectionName != "") {
		return _lineNumber;
	}
	for (const auto& data : _sectionMap) {
		if (data.second.lineNumber > line) {
			// 更新最大行号
			line = data.second.lineNumber;
		}
	}
	// 返回最大行号
	return line;
}
// 获取配置节中指定键 Key 对应的行号
int section::getLine(const std::string& Key) {
	// 如果键不存在，返回 -1
	if (!_sectionMap.count(Key)) {
		return -1;
	}
	// 返回键对应的行号
	return _sectionMap[Key].lineNumber;
}
// 重载操作符 []，返回配置节中指定键 Key 的值
const std::string section::operator[](const std::string& Key) {
	// 如果键不存在，返回空字符串
	if (!_sectionMap.count(Key)) {
		return "";
	}
	return _sectionMap[Key].Value;
}
// 清除配置节的数据，包括行号、节名和映射表
void section::clear() {
	_lineNumber = -1;		// 重置行号为 -1
	_sectionName.clear();	// 清空节名
	_sectionMap.clear();	// 清空节的映射表
}
// 检查配置节是否为空
bool section::isEmpty() const {
	// 返回节的映射表是否为空
	return _sectionMap.empty();
}
// 将配置节中指定键 Key 的值转换为整数类型
int section::toInt(const std::string& Key) {
	// 如果键不存在，返回默认值 0
	if (!_sectionMap.count(Key)) {
		return 0;
	}
	int result = 0;
	try {
		// 尝试将值转换为 int 类型
		result = std::stoi(_sectionMap[Key].Value);
	}
	catch (const std::invalid_argument& e) {
		// 捕获转换过程中的无效参数异常，记录调试信息
		INI_DEBUG("Invalid argument: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
	}
	catch (const std::out_of_range& e) {
		// 捕获转换过程中的超出范围异常，记录调试信息
		INI_DEBUG("Out of range: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
	}
	// 返回转换后的整数值，如果转换失败，则返回默认值 0
	return result;
}
// 获取配置节中指定键 Key 的值作为字符串
std::string section::toString(const std::string& Key) {
	// 获取配置节中指定键 Key 的值作为字符串
	if (!_sectionMap.count(Key)) {
		return "";// 如果键不存在，返回空字符串
	}
	// 返回键对应的字符串值
	return _sectionMap[Key].Value;
}
// 获取配置节中指定键 Key 的值，并将其转换为宽字符串 (std::wstring)
std::wstring section::toWString(const std::string& Key) {
	// UTF-8 到 UTF-16 转换器
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	// 调用 toString 获取字符串值，然后转换为 std::wstring 并返回
	return converter.from_bytes(toString(Key));
}
// 转成Double
double section::toDouble(const std::string& Key) {
	// 检查是否存在指定的键 Key
	if (!_sectionMap.count(Key)) {
		// 如果不存在，返回默认值 0.0
		return 0.0;
	}
	double result = 0.0;
	try {
		// 尝试将键对应的值转换为 double 类型
		result = std::stod(_sectionMap[Key].Value);
	}
	catch (const std::invalid_argument& e) {
		// 捕获转换过程中的无效参数异常，记录调试信息
		INI_DEBUG("Invalid argument: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
	}
	catch (const std::out_of_range& e) {
		// 捕获转换过程中的超出范围异常，记录调试信息
		INI_DEBUG("Out of range: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
	}
	// 返回转换后的 double 值，如果转换失败，则返回 0.0
	return result;
}

void ini::addSection(section& sec) {
	// 添加一个节到INI配置信息映射 _iniInfoMap 中

	// 检查是否已经存在同名的节
	if (_iniInfoMap.count(sec.name())) {
		// 如果存在同名节，则将当前节的内容附加到已存在的节中
		_iniInfoMap[sec.name()].append(sec);
		return;
	}
	// 如果不存在同名节，则直接将该节添加到 _iniInfoMap 中
	_iniInfoMap.emplace(sec.name(), sec);
	return;
}

void ini::removeSection(const std::string& sectionName) {
	// 从INI配置信息映射 _iniInfoMap 中移除指定名称的节

	// 检查是否存在指定名称的节
	if (!_iniInfoMap.count(sectionName)) {
		return;  // 如果不存在，直接返回
	}
	// 存在则移除该节
	_iniInfoMap.erase(sectionName);
	return;
}

bool ini::isSectionExists(const std::string& sectionName) {
	// 检查是否存在指定名称的节

	// 使用 _iniInfoMap.count(sectionName) 来判断是否存在
	return !_iniInfoMap.count(sectionName) ? false : true;
}

std::list<std::string> ini::getSectionsList() {
	// 获取所有节的名称列表

	std::list<std::string> sectionList;
	for (const auto& data : _iniInfoMap) {
		// 排除空名称和内容为空的节
		if (data.first == "" && data.second.isEmpty()) {
			continue;
		}
		// 将节的名称添加到 sectionList 中
		sectionList.emplace_back(data.first);
	}
	return sectionList;
}

const section& ini::operator[](const std::string& sectionName) {
	// 重载 [] 操作符，用于访问指定名称的节对象

	if (!_iniInfoMap.count(sectionName)) {
		// 如果不存在指定名称的节，则返回一个默认的空节对象
		return _iniInfoMap[""];
	}
	// 存在则返回对应的节对象引用
	return _iniInfoMap[sectionName];
}

const int ini::getSectionSize() {
	// 获取 _iniInfoMap 中节的数量

	return _iniInfoMap.size();
}

std::string ini::getValue(const std::string& sectionName, const std::string& Key) {
	// 获取指定节 sectionName 中指定键 Key 的值

	if (!_iniInfoMap.count(sectionName)) {
		// 检查是否存在指定名称的节
		return "";
	}
	// 返回指定键的值
	return _iniInfoMap[sectionName][Key];
}

int ini::getLine(const std::string& Key) {
	// 获取默认空节中指定键 Key 的行号

	if (!_iniInfoMap.count("")) {
		// 检查默认空节是否存在
		return -1;
	}
	// 返回指定键的行号
	return _iniInfoMap[""].getLine(Key);
}

int ini::getLine(const std::string& sectionName, const std::string& Key) {
	// 获取指定节 sectionName 中指定键 Key 的行号

	if (!_iniInfoMap.count(sectionName)) {
		// 检查是否存在指定名称的节
		return -1;
	}
	// 返回指定键的行号
	return _iniInfoMap[sectionName].getLine(Key);
}

void IniManager::parse() {
	// 打开配置文件
	_iniFile.open(_configFileName, std::ifstream::in | std::ifstream::out | std::fstream::app);
	// 检查文件是否成功打开
	if (!_iniFile.is_open()) {
		INI_DEBUG("Failed to open the input INI file for parsing!");
		return;
	}
	// 清空现有的 INI 数据
	_iniData.clear();
	// 将文件指针定位到文件开始处
	_iniFile.seekg(0, _iniFile.beg);
	// 定义变量以存储每行数据、节名及相关信息
	std::string data, sectionName;
	int sectionLine = -1;
	section sectionRecord;
	// 初始化行数统计
	_SumOfLines = 1;
	// 逐行读取文件内容，直到文件末尾
	do {
		std::getline(_iniFile, data);
		// 过滤无效数据行（未详细说明的函数）
		if (!filterData(data)) {
			++_SumOfLines;
			continue;
		}
		// 处理节名行
		if (data.find('[') == 0) {
			// 如果当前节记录不为空，则将其添加到 _iniData 中
			if (!sectionRecord.isEmpty() || sectionRecord.name() != "") {
				_iniData.addSection(sectionRecord);
			}
			// 查找节名的起始和结束位置
			size_t first = data.find('[');
			size_t last = data.find(']');
			// 如果找不到结束符，则跳过当前行
			if (last == std::string::npos) {
				++_SumOfLines;
				continue;
			}
			// 提取节名，并记录节的起始行号
			sectionName = data.substr(first + 1, last - first - 1);
			sectionLine = _SumOfLines;
			// 清空当前节记录，设置节名和起始行号
			sectionRecord.clear();
			sectionRecord.setName(sectionName, sectionLine);
		}
		// 处理键值对
		size_t pos = data.find('=');
		if (pos != std::string::npos) { // k=v
			std::string key = data.substr(0, pos);
			std::string value = data.substr(pos + 1);
			// 将键值对添加到当前节记录中，并记录行号
			sectionRecord.setValue(key, value, _SumOfLines);
		}
		// 增加行数计数器
		++_SumOfLines;
	} while (!_iniFile.eof());
	// 处理最后一个节记录
	if (!sectionRecord.isEmpty()) {
		sectionRecord.setName(sectionName, -1);
		_iniData.addSection(sectionRecord);
	}
	// 关闭文件流
	if (_iniFile.is_open()) {
		_iniFile.close();
	}
}

bool IniManager::modify(const std::string& Section, const std::string& Key, const std::string& Value, const std::string& comment) {
	// 解析配置文件，确保_iniData与文件同步
	parse();
	// 检查参数是否有效
	if (Key == "" || Value == "") {
		INI_DEBUG("Invalid parameter input: Key[" << Key << "],Value[" << Value << "]");
		return false;
	}
	// 构造要写入文件的键值对数据，考虑注释
	std::string keyValueData = Key + "=" + Value + "\n";
	if (comment.length() > 0) {
		keyValueData = comment + "\n" + keyValueData;
		if (comment[0] != ';') {
			keyValueData = ";" + keyValueData;
		}
	}
	// 临时文件名
	const std::string& tempFile = ".temp.ini";
	// 打开输入文件流和输出临时文件流
	std::fstream input(_configFileName, std::ifstream::in | std::ifstream::out | std::fstream::app);
	std::ofstream output(tempFile);
	// 检查文件流是否成功打开
	if (!input.is_open()) {
		INI_DEBUG("Failed to open the input INI file for modification!");
		return false;
	}
	if (!output.is_open()) {
		INI_DEBUG("Failed to open the temp INI file for modification!");
		return false;
	}
	// 标记要修改的行号
	int line_number_mark = -1;
	bool isInputDataWited = false;
	// 开始处理文件内容
	do {
		// 如果节存在，则查找键在节中的行号
		if (_iniData.isSectionExists(Section)) {
			line_number_mark = (*this)[Section].getLine(Key);
			if (line_number_mark == -1) { // 节存在，键不存在
				line_number_mark = (*this)[Section].getEndSection();
				std::string lineData;
				int input_line_number = 0;
				// 逐行读取输入文件，将新数据插入到正确位置
				while (std::getline(input, lineData)) {
					++input_line_number;

					if (input_line_number == (line_number_mark + 1)) { // 新行，追加新数据
						isInputDataWited = true;
						output << keyValueData;
					}

					output << lineData << "\n";
				}
				// 如果已经到达文件末尾且未追加新数据，则追加新数据
				if (input.eof() && !isInputDataWited) {
					isInputDataWited = true;
					output << keyValueData;
				}
				break;
			}
		}
		// 如果未找到键，则需要根据情况插入新节或在文件末尾添加键值对
		if (line_number_mark <= 0) { // 在配置文件中未找到键
			input.seekg(0, input.beg);
			bool isHoldSection = false;
			std::string newLine = "\n\n";
			// 如果 Section 为非节名格式，则判断是否需要插入新节
			if (Section != "" && Section.find("[") == std::string::npos && Section.find("]") == std::string::npos && Section.find("=") == std::string::npos) {
				if (_iniData.empty() || _iniData.getSectionSize() <= 0) {
					newLine.clear();
				}

				isHoldSection = true;
			}
			// 1. 节存在或者 Section 为空
			if (_iniData.isSectionExists(Section) || Section == "") {
				// 在文件头部写入键值对
				if (isHoldSection) {
					output << newLine << "[" << Section << "]" << "\n";
				}
				output << keyValueData;

				// 写入其余文件内容
				std::string lineData;
				while (std::getline(input, lineData)) {
					output << lineData << "\n";
				}
			}
			// 2. 节不存在
			else {
				// 写入其余文件内容
				std::string lineData;
				while (std::getline(input, lineData)) {
					output << lineData << "\n";
				}
				// 在文件末尾插入新节及键值对
				if (isHoldSection) {
					output << newLine << "[" << Section << "]" << "\n";
				}
				output << keyValueData;
			}
			break;
		}
		else { // 找到要替换的键值对
			std::string lineData;
			int input_line_number = 0;
			// 复制文件内容到临时文件，替换目标行的键值对
			while (std::getline(input, lineData)) {
				++input_line_number;
				// 如果有新的注释要设置，删除旧注释
				if (input_line_number == (line_number_mark - 1) && lineData.length() > 0 && lineData[0] == ';' && comment != "") {
					continue;
				}
				// 替换目标行
				if (input_line_number == line_number_mark) {
					output << keyValueData;
				}
				else {
					output << lineData << "\n";
				}
			}
			break;
		}
		// 如果代码执行到这里，通常表示出现错误或未处理的情况
		INI_DEBUG("Error! Lost process in modify function");
		return false;
	} while (false);
	// 关闭文件流
	input.close();
	output.close();
	// 替换原配置文件为临时文件，并重新加载解析
	std::remove(_configFileName.c_str());
	std::rename(tempFile.c_str(), _configFileName.c_str());
	// 重新解析配置文件，以便更新 _iniData
	parse();
	return true;
}

bool IniManager::modify(const std::string& Section, const std::string& Key, const int& Value, const std::string& comment) {
	// 将整数值转换为字符串
	std::string stringValue = std::to_string(Value);
	// 调用通用的修改函数，传递字符串值
	return modify(Section, Key, stringValue, comment);
}

bool IniManager::modify(const std::string& Section, const std::string& Key, const double& Value, const std::string& comment) {
	// 将双精度浮点数转换为字符串
	std::string stringValue = std::to_string(Value);
	// 调用通用的修改函数，传递字符串值
	return modify(Section, Key, stringValue, comment);
}

bool IniManager::modify(const std::string& Section, const std::string& Key, const std::wstring& Value, const std::string& comment) {
	// 将宽字符串转换为UTF-8编码的字符串
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string stringValue = converter.to_bytes(Value);
	// 调用通用的修改函数，传递字符串值
	return modify(Section, Key, stringValue, comment);
}

bool IniManager::modifyComment(const std::string& Section, const std::string& Key, const std::string& comment) {
	// 获取当前键的当前值，并用它作为新值修改注释
	return modify(Section, Key, (*this)[Section][Key], comment);
}

bool IniManager::isSectionExists(const std::string& sectionName) {
	// 检查给定节是否存在于_iniData中
	return _iniData.isSectionExists(sectionName);
}

bool IniManager::filterData(std::string& data) {
	// 过滤字符串数据：去除空格和制表符，以及以分号或井号开头的行
	data.erase(std::remove_if(data.begin(), data.end(), [](char c) {
		return c == ' ' || c == '\t';
		}), data.end());

	// 检查处理后的数据是否为空或以注释符号开头，返回过滤结果
	if (data.length() == 0) {
		return false;
	}
	if (data[0] == ';') {
		return false;
	}
	if (data[0] == '#') {
		return false;
	}
	return true;
}
