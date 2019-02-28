#include "StdAfx.h"
#include "AdoUtil.h"


CAdoUtil::CAdoUtil(void)
{
	m_pConnection = NULL;
}


CAdoUtil::~CAdoUtil(void)
{
}

_ConnectionPtr CAdoUtil::GetConnectionPtr(void)
{
	return m_pConnection;
}




bool CAdoUtil::Open(CString szDbName, CString szUid, CString szPwd)
{
	try
	{
		if (m_pConnection) this->Close();

		HRESULT hr = m_pConnection.CreateInstance(__uuidof(Connection));
		if (FAILED(hr))
		{
			m_pConnection = NULL;
			TRACE("Fail to CreateInstance!\n ");
			return false;
		}

		CString szConnect;
		szConnect.Format(_T("Provider=MSDASQL.1;Password=%s;Persist Security Info=True;User ID=%s;DataBase = %s;"), szPwd, szUid,szDbName);
		hr = m_pConnection->Open(_bstr_t(szConnect.GetBuffer(0)), _T(""), _T(""), adModeUnknown);
		if (FAILED(hr))
		{
			m_pConnection = NULL;
			TRACE("Fail to Open database!\n ");
			return false;
		}

		// 切换到当前要操作的数据库
		CString szSQL;
		szSQL.Format(_T("use %s;"), szDbName);
		Execute(szSQL);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		//SetError(e.ErrorMessage());
		return false;
	}
	return false;
}




bool CAdoUtil::Open(CString szConnect)
{
	try
	{
		if (m_pConnection) this->Close();

		HRESULT hr = m_pConnection.CreateInstance(__uuidof(Connection));
		if (FAILED(hr))
		{
			m_pConnection = NULL;
			TRACE("Fail to CreateInstance!\n ");
			return false;
		}

		hr = m_pConnection->Open(_bstr_t(szConnect.GetBuffer(0)), _T(""), _T(""), adModeUnknown);
		if (FAILED(hr))
		{
			m_pConnection = NULL;
			TRACE("Fail to Open database!\n ");
			return false;
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		//SetError(e.ErrorMessage());
		return false;
	}
	return false;
}


bool CAdoUtil::Close(void)
{
	if (m_pConnection != NULL)
	{
		if (m_pConnection->State != adStateClosed)
		{
			m_pConnection->Close();
		}
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	return false;
}




bool CAdoUtil::Execute(const CString & strSql)
{
	try
	{
		m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	}
	catch (_com_error e)
	{
		//SetError(e.ErrorMessage());
		return false;
	}    
	return false;
}


_RecordsetPtr CAdoUtil::Select(const CString & strSql)
{
	_RecordsetPtr pRecordset = NULL;
	try
	{
		pRecordset = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	}
	catch (_com_error e)
	{
		//SetError(e.ErrorMessage());
		return NULL;
	}
	return pRecordset;
	return _RecordsetPtr();
}
