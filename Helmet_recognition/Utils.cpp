#include "StdAfx.h"
#include "Utils.h"

// 类型转换使用
#include <atlconv.h>

// 将宽字节转换成多字节
CStringA CUtils::WChar2MByte(const CString &str)   
{ 
    // 以下nCharLen和nByteLen的值大小不同, nCharLen是按字符计算的，nByteLen是按字节计算的
    int nCharLen = str.GetLength();
    // 获取宽字节字符的大小，大小是按字节计算的
    int nByteLen = WideCharToMultiByte(CP_UTF8, 0, str, nCharLen, NULL, 0, NULL, NULL);
    
    //为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
    char* pStr = (char*)malloc(sizeof(char)*(nByteLen+1)); // 以字节为单位

    //宽字节编码转换成多字节编码
    WideCharToMultiByte(CP_UTF8, 0, str, nCharLen, pStr, nByteLen, NULL, NULL);
    pStr[nByteLen] = '\0';   //多字节字符以'\0'结束
    
    // 转换成CStringA变量
    CStringA strA(pStr);

    // 删除临时数组
    free(pStr);

    return strA;
}

void CUtils::WChar2Char( char *dst, CString &src, int size)
{
	// 以下nCharLen和nByteLen的值大小不同, nCharLen是按字符计算的，nByteLen是按字节计算的
	int nCharLen = src.GetLength();
	// 获取宽字节字符的大小，大小是按字节计算的
	int nByteLen = WideCharToMultiByte(CP_UTF8, 0, src, nCharLen, NULL, 0, NULL, NULL);

	if(nByteLen > size)
	{
		return;
	}

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_UTF8, 0, src, nCharLen, dst, nByteLen, NULL, NULL);
	dst[nByteLen] = '\0';   //多字节字符以'\0'结束
}

// 将多字节转换成宽字节
CString CUtils::MByte2WChar(const CStringA &strA)
{
    // 计算字节数，以字节为单位，一个汉字占两个字节
    int charLen = strA.GetLength();

    // 计算多字节字符的大小，按字符计算。
    int nByteLen = MultiByteToWideChar(CP_UTF8, 0, strA, charLen, NULL, 0);
    wchar_t* pStr = (wchar_t*)malloc(sizeof(wchar_t)*(nByteLen+1));
    MultiByteToWideChar(CP_UTF8, 0, strA, charLen, pStr, nByteLen);
    pStr[nByteLen] = '\0';
    CString str(pStr);
    free(pStr);

    return str;
}

CString CUtils::Char2Str(char* pstr)
{
    // 计算字节数，以字节为单位，一个汉字占两个字节
    int charLen = strlen(pstr);
    // 计算多字节字符的大小，按字符计算。
    int nByteLen = MultiByteToWideChar(CP_UTF8, 0, pstr, charLen, NULL, 0);
    wchar_t* pszWchar = (wchar_t*)malloc(sizeof(wchar_t)*(nByteLen+1));
    MultiByteToWideChar(CP_UTF8, 0, pstr, charLen, pszWchar, nByteLen);
    pszWchar[nByteLen] = '\0';
    CString str(pszWchar);
    free(pszWchar);

    return str;
}

string CUtils::Str2Stda(CString &strDim)
{
    CStringA stra(strDim.GetBuffer(0));
    strDim.ReleaseBuffer();

    string str;
    str = stra.GetBuffer(0);
    stra.ReleaseBuffer();

    return str;
}

// 将unicode编码转成utf8
CStringA CUtils::UniToUtf8(const CString &str)
{
    int len;
    len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    char *pszUtf8 = (char*)malloc(len + 1);
    memset(pszUtf8, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, str, -1, pszUtf8, len, NULL, NULL);

    // 转换成CStringA变量
    CStringA strA(pszUtf8);
    free(pszUtf8);
    return strA;
}
