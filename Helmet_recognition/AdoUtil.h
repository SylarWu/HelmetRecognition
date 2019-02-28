#pragma once

class CAdoUtil
{
public:
	CAdoUtil(void);
	~CAdoUtil(void);

private:
	_ConnectionPtr m_pConnection;
public:
	_ConnectionPtr GetConnectionPtr(void);
	BOOL ConnectDB(CString & szServer, CString & szName, CString & szPassword);
	bool Open(CString szDbName, CString szUid, CString szPwd);
	bool Open(CString szConnect);
	bool Close(void);
	bool Execute(const CString & strSql);
	_RecordsetPtr Select(const CString & strSql);
};

