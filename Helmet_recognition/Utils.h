#pragma once
#include <string>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// 工具类
class CUtils
{
public:
	// 将宽字节转换成多字节
    static CStringA WChar2MByte(const CString &str);
    // 将多字节转换成宽字节
    static CString MByte2WChar(const CStringA &strA);
	// 将宽字节转换成字符数组
	static void WChar2Char(char *dst, CString &src, int size);

    // 将unicode编码转成utf8
    static CStringA UniToUtf8(const CString &str);
    // 将char数组转换成CString
    static CString Char2Str(char* pstr);
    // 将CString转换成std::string
    static string Str2Stda(CString &strDim);
};