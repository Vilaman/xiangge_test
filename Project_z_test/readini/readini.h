#pragma once
#include <algorithm>// �㷨��
#include <fstream>	// �ļ������������
#include <string>	// �ַ�����
#include <list>		// list����
#include <map>		// map����

// for std::string <==> std::wstring convert
#include <codecvt>	// ��װ�ַ�����ת��
#include <locale>	// ��׼��
// ���Կ���
#ifndef INICPP_DEBUG 
#define INICPP_DEBUG 
#include <array>	// array����
#include <ctime>	// ʱ���
#include <iostream>	// ���������

class TimeFormatter {
public:
	// ��ȡʱ��
	static std::string format(const std::string& format = "%Y-%m-%d %H:%M:%S") {
		std::time_t t = std::time(nullptr);	// ��ȡ��ǰʱ���
		std::tm tm = *std::localtime(&t);	// ��ʱ���ת��Ϊ����ʱ��ṹ��
		std::array<char, 100> buffer;		// ����һ����СΪ100���ַ�������Ϊ��ʽ������Ļ�����
		// ʹ�� strftime ����������ʱ���ʽ��Ϊָ�����ַ�����ʽ
		std::strftime(buffer.data(), buffer.size(), format.c_str(), &tm);
		return buffer.data();				// ���ַ�����ת��Ϊ std::string
	}
};
// ������Ϣ
#define CODE_INFO std::string(" | Code:\'file:") + std::string(__FILE__) + ",function:" + std::string(__FUNCTION__) + ",line:" + std::to_string(__LINE__) + '\''
// ini�ļ��ĵ�����Ϣ
#define INI_DEBUG(x) std::cout << "INICPP " << TimeFormatter::format() << " : " << x << CODE_INFO << std::endl

#else  
#define INI_DEBUG(x)
#endif 
// ��-ֵ �ڵ� �ṹ��
typedef struct KeyValueNode {
	std::string Value = "";	// ��ֵ
	int lineNumber = -1;	// �ı�����1��ͷ
};

/*********************************************
*				[�� Section]					 *
*				��(Key) = ֵ(value)			 *
*********************************************/

class section {
public:
	section() :_sectionName() {}						// Ĭ�Ϲ��캯������ʼ�� _sectionName Ϊ���ַ���
	explicit section(const std::string& sectionName) : _sectionName(sectionName) {}		// ��ʽ���캯�������� _sectionName
	const std::string& name();							// ���ؽڵ�����
	const std::string getValue(const std::string& Key);	// ��ȡ��� Key �������ֵ
	void setName(const std::string& name, const int& lineNumber);						// ���ýڵ����ƺ��к�
	void setValue(const std::string& Key, const std::string& Value, const int line);	// ���ü� Key ��Ӧ��ֵ���к�
	void append(section& sec);							// ����һ���� sec ��ӳ������ӵ���ǰ�ڵ�ӳ�����
	bool isKeyExist(const std::string& Key);			// ���ڵ�ӳ������Ƿ����ָ���ļ� Key
	int getEndSection();								// ��ȡ�ڵ�ӳ��������һ����ֵ�Ե��к�
	int getLine(const std::string& Key);				// ��ȡ��� Key ��������к�
	const std::string operator[](const std::string& Key);	// ���ز����� []��������� Key �������ֵ
	void clear();										// ��սڣ���������ֵ
	bool isEmpty() const;								// �����Ƿ�Ϊ��
	int toInt(const std::string& Key);					// ����� Key �������ֵת��Ϊ����
	std::string toString(const std::string& Key);		// ������� Key �������ֵ��Ϊ�ַ���
	std::wstring toWString(const std::string& Key);		// ������� Key �������ֵ��Ϊ���ַ��� (wstring)
	double toDouble(const std::string& Key);			// ����� Key �������ֵת��Ϊ˫���ȸ�����
private:
	std::string _sectionName;							// �ڵ�����
	std::map<std::string, KeyValueNode> _sectionMap;	// �洢��ֵ�Ե�ӳ���
	int _lineNumber = -1;								// �ڵ��к�
};

class ini {
public:
	void addSection(section& sec);						// ����һ���ڶ���_iniInfoMap��
	void removeSection(const std::string& sectionName);	// �Ƴ�ָ�����ƵĽ�
	bool isSectionExists(const std::string& sectionName);	// ����Ƿ����ָ�����ƵĽ�
	std::list<std::string> getSectionsList();			// ��ȡ���нڵ������б�
	const section& operator[](const std::string& sectionName);	// ���ز����� []������ָ�����ƵĽڶ�������
	const int getSectionSize();							// ��ȡ_iniInfoMap�нڵ�����
	std::string getValue(const std::string& sectionName, const std::string& Key);	// ��ȡָ���ںͼ���ֵ
	int getLine(const std::string& Key);				// ��ȡ����ָ�������к�
	int getLine(const std::string& sectionName, const std::string& Key);	// ��ȡָ�����а���ָ�������к�
	inline void clear() { _iniInfoMap.clear(); }		// ���_iniInfoMap���Ƴ����н�
	inline bool empty() { return _iniInfoMap.empty(); }	// ���_iniInfoMap�Ƿ�Ϊ��
protected:
	std::map<std::string/*Section Name*/, section> _iniInfoMap;
};

class IniManager
{
public:
	// ���캯����ʹ�������ļ�����ʼ�� IniManager�������������ļ�
	explicit IniManager(const std::string& configFileName) : _configFileName(configFileName) { parse(); }
	// �����������ر� Ini �ļ���
	~IniManager() { _iniFile.close(); };
	// ���ز�������ͨ���������ʽڶ���
	section operator[](const std::string& sectionName) { return _iniData[sectionName]; }
	// ���������ļ������ _iniData
	void parse();
	// �޸Ľ��м����ַ���ֵ
	bool modify(const std::string& Section, const std::string& Key, const std::string& Value, const std::string& comment = "");
	// �޸Ľ��м�������ֵ
	bool modify(const std::string& Section, const std::string& Key, const int& Value, const std::string& comment = "");
	// �޸Ľ��м���˫���ȸ�����ֵ
	bool modify(const std::string& Section, const std::string& Key, const double& Value, const std::string& comment = "");
	// �޸Ľ��м��Ŀ��ַ���ֵ
	bool modify(const std::string& Section, const std::string& Key, const std::wstring& Value, const std::string& comment = "");
	// �޸ļ���ע��
	bool modifyComment(const std::string& Section, const std::string& Key, const std::string& comment);
	// ���ָ�����Ƿ����
	bool isSectionExists(const std::string& sectionName);
	// ��ȡ���н������б�
	inline std::list<std::string> getSectionsList() { return _iniData.getSectionsList(); }
private:
	bool filterData(std::string& data);

private:
	ini _iniData;				// ���� INI ���ݵ� ini ��ʵ��
	int _SumOfLines;			// INI �ļ���������
	std::fstream _iniFile;		// INI �ļ����ļ���
	std::string _configFileName;// �����ļ���
};