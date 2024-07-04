#include "readini.h"
// �������ýڵ�����
const std::string& section::name() {
	return _sectionName;
}
// ���ݼ� Key ��ȡ���ý��ж�Ӧ��ֵ
const std::string section::getValue(const std::string& Key) {
	// ����������ڣ����ؿ��ַ���
	if (!_sectionMap.count(Key)) {
		return "";
	}
	// ���ؼ���Ӧ��ֵ
	return _sectionMap[Key].Value;
}
// �������ýڵ����ƺ��к�
void section::setName(const std::string& name, const int& lineNumber) {
	_sectionName = name;		// ���ý���
	_lineNumber = lineNumber;	// �����к�
}
// �������ý���ָ���� Key ��ֵ���к�
void section::setValue(const std::string& Key, const std::string& Value, const int line) {
	_sectionMap[Key].Value = Value;		// ���ü� Key ��Ӧ��ֵ
	_sectionMap[Key].lineNumber = line;	// ���ü���Ӧ���к�
}
// ����һ�����ý� sec �����ݸ��ӵ���ǰ���ýڵ�ӳ�����
void section::append(section& sec) {
	// ���� sec ��ӳ������ݵ���ǰӳ���
	_sectionMap.insert(sec._sectionMap.begin(), sec._sectionMap.end());
}
// ������ý����Ƿ����ָ���ļ� Key
bool section::isKeyExist(const std::string& Key) {
	// �жϼ��Ƿ���ڣ������ؽ��
	return !_sectionMap.count(Key) ? false : true;
}
// ��ȡ���ý������һ����ֵ�Ե��к�
int section::getEndSection() {
	int line = -1;	// ��ʼ���к�Ϊ -1
	// ���ӳ���Ϊ�յ�������Ϊ�գ����ؽڵ��к�
	if (_sectionMap.empty() && _sectionName != "") {
		return _lineNumber;
	}
	for (const auto& data : _sectionMap) {
		if (data.second.lineNumber > line) {
			// ��������к�
			line = data.second.lineNumber;
		}
	}
	// ��������к�
	return line;
}
// ��ȡ���ý���ָ���� Key ��Ӧ���к�
int section::getLine(const std::string& Key) {
	// ����������ڣ����� -1
	if (!_sectionMap.count(Key)) {
		return -1;
	}
	// ���ؼ���Ӧ���к�
	return _sectionMap[Key].lineNumber;
}
// ���ز����� []���������ý���ָ���� Key ��ֵ
const std::string section::operator[](const std::string& Key) {
	// ����������ڣ����ؿ��ַ���
	if (!_sectionMap.count(Key)) {
		return "";
	}
	return _sectionMap[Key].Value;
}
// ������ýڵ����ݣ������кš�������ӳ���
void section::clear() {
	_lineNumber = -1;		// �����к�Ϊ -1
	_sectionName.clear();	// ��ս���
	_sectionMap.clear();	// ��սڵ�ӳ���
}
// ������ý��Ƿ�Ϊ��
bool section::isEmpty() const {
	// ���ؽڵ�ӳ����Ƿ�Ϊ��
	return _sectionMap.empty();
}
// �����ý���ָ���� Key ��ֵת��Ϊ��������
int section::toInt(const std::string& Key) {
	// ����������ڣ�����Ĭ��ֵ 0
	if (!_sectionMap.count(Key)) {
		return 0;
	}
	int result = 0;
	try {
		// ���Խ�ֵת��Ϊ int ����
		result = std::stoi(_sectionMap[Key].Value);
	}
	catch (const std::invalid_argument& e) {
		// ����ת�������е���Ч�����쳣����¼������Ϣ
		INI_DEBUG("Invalid argument: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
	}
	catch (const std::out_of_range& e) {
		// ����ת�������еĳ�����Χ�쳣����¼������Ϣ
		INI_DEBUG("Out of range: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
	}
	// ����ת���������ֵ�����ת��ʧ�ܣ��򷵻�Ĭ��ֵ 0
	return result;
}
// ��ȡ���ý���ָ���� Key ��ֵ��Ϊ�ַ���
std::string section::toString(const std::string& Key) {
	// ��ȡ���ý���ָ���� Key ��ֵ��Ϊ�ַ���
	if (!_sectionMap.count(Key)) {
		return "";// ����������ڣ����ؿ��ַ���
	}
	// ���ؼ���Ӧ���ַ���ֵ
	return _sectionMap[Key].Value;
}
// ��ȡ���ý���ָ���� Key ��ֵ��������ת��Ϊ���ַ��� (std::wstring)
std::wstring section::toWString(const std::string& Key) {
	// UTF-8 �� UTF-16 ת����
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	// ���� toString ��ȡ�ַ���ֵ��Ȼ��ת��Ϊ std::wstring ������
	return converter.from_bytes(toString(Key));
}
// ת��Double
double section::toDouble(const std::string& Key) {
	// ����Ƿ����ָ���ļ� Key
	if (!_sectionMap.count(Key)) {
		// ��������ڣ�����Ĭ��ֵ 0.0
		return 0.0;
	}
	double result = 0.0;
	try {
		// ���Խ�����Ӧ��ֵת��Ϊ double ����
		result = std::stod(_sectionMap[Key].Value);
	}
	catch (const std::invalid_argument& e) {
		// ����ת�������е���Ч�����쳣����¼������Ϣ
		INI_DEBUG("Invalid argument: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
	}
	catch (const std::out_of_range& e) {
		// ����ת�������еĳ�����Χ�쳣����¼������Ϣ
		INI_DEBUG("Out of range: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
	}
	// ����ת����� double ֵ�����ת��ʧ�ܣ��򷵻� 0.0
	return result;
}

void ini::addSection(section& sec) {
	// ���һ���ڵ�INI������Ϣӳ�� _iniInfoMap ��

	// ����Ƿ��Ѿ�����ͬ���Ľ�
	if (_iniInfoMap.count(sec.name())) {
		// �������ͬ���ڣ��򽫵�ǰ�ڵ����ݸ��ӵ��Ѵ��ڵĽ���
		_iniInfoMap[sec.name()].append(sec);
		return;
	}
	// ���������ͬ���ڣ���ֱ�ӽ��ý���ӵ� _iniInfoMap ��
	_iniInfoMap.emplace(sec.name(), sec);
	return;
}

void ini::removeSection(const std::string& sectionName) {
	// ��INI������Ϣӳ�� _iniInfoMap ���Ƴ�ָ�����ƵĽ�

	// ����Ƿ����ָ�����ƵĽ�
	if (!_iniInfoMap.count(sectionName)) {
		return;  // ��������ڣ�ֱ�ӷ���
	}
	// �������Ƴ��ý�
	_iniInfoMap.erase(sectionName);
	return;
}

bool ini::isSectionExists(const std::string& sectionName) {
	// ����Ƿ����ָ�����ƵĽ�

	// ʹ�� _iniInfoMap.count(sectionName) ���ж��Ƿ����
	return !_iniInfoMap.count(sectionName) ? false : true;
}

std::list<std::string> ini::getSectionsList() {
	// ��ȡ���нڵ������б�

	std::list<std::string> sectionList;
	for (const auto& data : _iniInfoMap) {
		// �ų������ƺ�����Ϊ�յĽ�
		if (data.first == "" && data.second.isEmpty()) {
			continue;
		}
		// ���ڵ�������ӵ� sectionList ��
		sectionList.emplace_back(data.first);
	}
	return sectionList;
}

const section& ini::operator[](const std::string& sectionName) {
	// ���� [] �����������ڷ���ָ�����ƵĽڶ���

	if (!_iniInfoMap.count(sectionName)) {
		// ���������ָ�����ƵĽڣ��򷵻�һ��Ĭ�ϵĿսڶ���
		return _iniInfoMap[""];
	}
	// �����򷵻ض�Ӧ�Ľڶ�������
	return _iniInfoMap[sectionName];
}

const int ini::getSectionSize() {
	// ��ȡ _iniInfoMap �нڵ�����

	return _iniInfoMap.size();
}

std::string ini::getValue(const std::string& sectionName, const std::string& Key) {
	// ��ȡָ���� sectionName ��ָ���� Key ��ֵ

	if (!_iniInfoMap.count(sectionName)) {
		// ����Ƿ����ָ�����ƵĽ�
		return "";
	}
	// ����ָ������ֵ
	return _iniInfoMap[sectionName][Key];
}

int ini::getLine(const std::string& Key) {
	// ��ȡĬ�Ͽս���ָ���� Key ���к�

	if (!_iniInfoMap.count("")) {
		// ���Ĭ�Ͽս��Ƿ����
		return -1;
	}
	// ����ָ�������к�
	return _iniInfoMap[""].getLine(Key);
}

int ini::getLine(const std::string& sectionName, const std::string& Key) {
	// ��ȡָ���� sectionName ��ָ���� Key ���к�

	if (!_iniInfoMap.count(sectionName)) {
		// ����Ƿ����ָ�����ƵĽ�
		return -1;
	}
	// ����ָ�������к�
	return _iniInfoMap[sectionName].getLine(Key);
}

void IniManager::parse() {
	// �������ļ�
	_iniFile.open(_configFileName, std::ifstream::in | std::ifstream::out | std::fstream::app);
	// ����ļ��Ƿ�ɹ���
	if (!_iniFile.is_open()) {
		INI_DEBUG("Failed to open the input INI file for parsing!");
		return;
	}
	// ������е� INI ����
	_iniData.clear();
	// ���ļ�ָ�붨λ���ļ���ʼ��
	_iniFile.seekg(0, _iniFile.beg);
	// ��������Դ洢ÿ�����ݡ������������Ϣ
	std::string data, sectionName;
	int sectionLine = -1;
	section sectionRecord;
	// ��ʼ������ͳ��
	_SumOfLines = 1;
	// ���ж�ȡ�ļ����ݣ�ֱ���ļ�ĩβ
	do {
		std::getline(_iniFile, data);
		// ������Ч�����У�δ��ϸ˵���ĺ�����
		if (!filterData(data)) {
			++_SumOfLines;
			continue;
		}
		// ���������
		if (data.find('[') == 0) {
			// �����ǰ�ڼ�¼��Ϊ�գ�������ӵ� _iniData ��
			if (!sectionRecord.isEmpty() || sectionRecord.name() != "") {
				_iniData.addSection(sectionRecord);
			}
			// ���ҽ�������ʼ�ͽ���λ��
			size_t first = data.find('[');
			size_t last = data.find(']');
			// ����Ҳ�������������������ǰ��
			if (last == std::string::npos) {
				++_SumOfLines;
				continue;
			}
			// ��ȡ����������¼�ڵ���ʼ�к�
			sectionName = data.substr(first + 1, last - first - 1);
			sectionLine = _SumOfLines;
			// ��յ�ǰ�ڼ�¼�����ý�������ʼ�к�
			sectionRecord.clear();
			sectionRecord.setName(sectionName, sectionLine);
		}
		// �����ֵ��
		size_t pos = data.find('=');
		if (pos != std::string::npos) { // k=v
			std::string key = data.substr(0, pos);
			std::string value = data.substr(pos + 1);
			// ����ֵ����ӵ���ǰ�ڼ�¼�У�����¼�к�
			sectionRecord.setValue(key, value, _SumOfLines);
		}
		// ��������������
		++_SumOfLines;
	} while (!_iniFile.eof());
	// �������һ���ڼ�¼
	if (!sectionRecord.isEmpty()) {
		sectionRecord.setName(sectionName, -1);
		_iniData.addSection(sectionRecord);
	}
	// �ر��ļ���
	if (_iniFile.is_open()) {
		_iniFile.close();
	}
}

bool IniManager::modify(const std::string& Section, const std::string& Key, const std::string& Value, const std::string& comment) {
	// ���������ļ���ȷ��_iniData���ļ�ͬ��
	parse();
	// �������Ƿ���Ч
	if (Key == "" || Value == "") {
		INI_DEBUG("Invalid parameter input: Key[" << Key << "],Value[" << Value << "]");
		return false;
	}
	// ����Ҫд���ļ��ļ�ֵ�����ݣ�����ע��
	std::string keyValueData = Key + "=" + Value + "\n";
	if (comment.length() > 0) {
		keyValueData = comment + "\n" + keyValueData;
		if (comment[0] != ';') {
			keyValueData = ";" + keyValueData;
		}
	}
	// ��ʱ�ļ���
	const std::string& tempFile = ".temp.ini";
	// �������ļ����������ʱ�ļ���
	std::fstream input(_configFileName, std::ifstream::in | std::ifstream::out | std::fstream::app);
	std::ofstream output(tempFile);
	// ����ļ����Ƿ�ɹ���
	if (!input.is_open()) {
		INI_DEBUG("Failed to open the input INI file for modification!");
		return false;
	}
	if (!output.is_open()) {
		INI_DEBUG("Failed to open the temp INI file for modification!");
		return false;
	}
	// ���Ҫ�޸ĵ��к�
	int line_number_mark = -1;
	bool isInputDataWited = false;
	// ��ʼ�����ļ�����
	do {
		// ����ڴ��ڣ�����Ҽ��ڽ��е��к�
		if (_iniData.isSectionExists(Section)) {
			line_number_mark = (*this)[Section].getLine(Key);
			if (line_number_mark == -1) { // �ڴ��ڣ���������
				line_number_mark = (*this)[Section].getEndSection();
				std::string lineData;
				int input_line_number = 0;
				// ���ж�ȡ�����ļ����������ݲ��뵽��ȷλ��
				while (std::getline(input, lineData)) {
					++input_line_number;

					if (input_line_number == (line_number_mark + 1)) { // ���У�׷��������
						isInputDataWited = true;
						output << keyValueData;
					}

					output << lineData << "\n";
				}
				// ����Ѿ������ļ�ĩβ��δ׷�������ݣ���׷��������
				if (input.eof() && !isInputDataWited) {
					isInputDataWited = true;
					output << keyValueData;
				}
				break;
			}
		}
		// ���δ�ҵ���������Ҫ������������½ڻ����ļ�ĩβ��Ӽ�ֵ��
		if (line_number_mark <= 0) { // �������ļ���δ�ҵ���
			input.seekg(0, input.beg);
			bool isHoldSection = false;
			std::string newLine = "\n\n";
			// ��� Section Ϊ�ǽ�����ʽ�����ж��Ƿ���Ҫ�����½�
			if (Section != "" && Section.find("[") == std::string::npos && Section.find("]") == std::string::npos && Section.find("=") == std::string::npos) {
				if (_iniData.empty() || _iniData.getSectionSize() <= 0) {
					newLine.clear();
				}

				isHoldSection = true;
			}
			// 1. �ڴ��ڻ��� Section Ϊ��
			if (_iniData.isSectionExists(Section) || Section == "") {
				// ���ļ�ͷ��д���ֵ��
				if (isHoldSection) {
					output << newLine << "[" << Section << "]" << "\n";
				}
				output << keyValueData;

				// д�������ļ�����
				std::string lineData;
				while (std::getline(input, lineData)) {
					output << lineData << "\n";
				}
			}
			// 2. �ڲ�����
			else {
				// д�������ļ�����
				std::string lineData;
				while (std::getline(input, lineData)) {
					output << lineData << "\n";
				}
				// ���ļ�ĩβ�����½ڼ���ֵ��
				if (isHoldSection) {
					output << newLine << "[" << Section << "]" << "\n";
				}
				output << keyValueData;
			}
			break;
		}
		else { // �ҵ�Ҫ�滻�ļ�ֵ��
			std::string lineData;
			int input_line_number = 0;
			// �����ļ����ݵ���ʱ�ļ����滻Ŀ���еļ�ֵ��
			while (std::getline(input, lineData)) {
				++input_line_number;
				// ������µ�ע��Ҫ���ã�ɾ����ע��
				if (input_line_number == (line_number_mark - 1) && lineData.length() > 0 && lineData[0] == ';' && comment != "") {
					continue;
				}
				// �滻Ŀ����
				if (input_line_number == line_number_mark) {
					output << keyValueData;
				}
				else {
					output << lineData << "\n";
				}
			}
			break;
		}
		// �������ִ�е����ͨ����ʾ���ִ����δ��������
		INI_DEBUG("Error! Lost process in modify function");
		return false;
	} while (false);
	// �ر��ļ���
	input.close();
	output.close();
	// �滻ԭ�����ļ�Ϊ��ʱ�ļ��������¼��ؽ���
	std::remove(_configFileName.c_str());
	std::rename(tempFile.c_str(), _configFileName.c_str());
	// ���½��������ļ����Ա���� _iniData
	parse();
	return true;
}

bool IniManager::modify(const std::string& Section, const std::string& Key, const int& Value, const std::string& comment) {
	// ������ֵת��Ϊ�ַ���
	std::string stringValue = std::to_string(Value);
	// ����ͨ�õ��޸ĺ����������ַ���ֵ
	return modify(Section, Key, stringValue, comment);
}

bool IniManager::modify(const std::string& Section, const std::string& Key, const double& Value, const std::string& comment) {
	// ��˫���ȸ�����ת��Ϊ�ַ���
	std::string stringValue = std::to_string(Value);
	// ����ͨ�õ��޸ĺ����������ַ���ֵ
	return modify(Section, Key, stringValue, comment);
}

bool IniManager::modify(const std::string& Section, const std::string& Key, const std::wstring& Value, const std::string& comment) {
	// �����ַ���ת��ΪUTF-8������ַ���
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string stringValue = converter.to_bytes(Value);
	// ����ͨ�õ��޸ĺ����������ַ���ֵ
	return modify(Section, Key, stringValue, comment);
}

bool IniManager::modifyComment(const std::string& Section, const std::string& Key, const std::string& comment) {
	// ��ȡ��ǰ���ĵ�ǰֵ����������Ϊ��ֵ�޸�ע��
	return modify(Section, Key, (*this)[Section][Key], comment);
}

bool IniManager::isSectionExists(const std::string& sectionName) {
	// ���������Ƿ������_iniData��
	return _iniData.isSectionExists(sectionName);
}

bool IniManager::filterData(std::string& data) {
	// �����ַ������ݣ�ȥ���ո���Ʊ�����Լ��ԷֺŻ򾮺ſ�ͷ����
	data.erase(std::remove_if(data.begin(), data.end(), [](char c) {
		return c == ' ' || c == '\t';
		}), data.end());

	// ��鴦���������Ƿ�Ϊ�ջ���ע�ͷ��ſ�ͷ�����ع��˽��
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
