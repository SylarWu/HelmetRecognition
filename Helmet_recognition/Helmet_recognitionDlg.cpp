
// Helmet_recognitionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Helmet_recognition.h"
#include "Helmet_recognitionDlg.h"
#include "Utils.h"
#include "SVMTraining.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 视频显示频率(单位：毫秒)
#define WM_TIMER_PLAYER   WM_USER+1000
#define FRAME_INFINITE   -99999


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHelmet_recognitionDlg 对话框




CHelmet_recognitionDlg::CHelmet_recognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHelmet_recognitionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	detector.Load();
}

void CHelmet_recognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAT, m_wndMat);
	DDX_Control(pDX, IDC_LIST_INFO, m_lstInfo);
}

BEGIN_MESSAGE_MAP(CHelmet_recognitionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_WM_TIMER()
	ON_COMMAND(IDM_OPEN_FILE, &CHelmet_recognitionDlg::OnOpenFile)
	ON_COMMAND(IDM_CLOSE_FILE, &CHelmet_recognitionDlg::OnCloseFile)

	ON_COMMAND(IDM_OPEN_CAMERA, &CHelmet_recognitionDlg::OnOpenCamera)
	ON_COMMAND(IDM_CLOSE_CAMERA, &CHelmet_recognitionDlg::OnCloseCamera)

	ON_BN_CLICKED(IDC_BUTTONOPEN, &CHelmet_recognitionDlg::OnOpenFile)
END_MESSAGE_MAP()


// CHelmet_recognitionDlg 消息处理程序

BOOL CHelmet_recognitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化列表
	LONG lStyle = ::GetWindowLong(m_lstInfo.m_hWnd, GWL_STYLE); //获取当前窗口style
	lStyle |= LVS_REPORT;   //记录样式
	lStyle |= LVS_SINGLESEL; //单项选择，默认时可以多项选择
	lStyle |= LVS_SHOWSELALWAYS;
	::SetWindowLong(m_lstInfo.m_hWnd, GWL_STYLE, lStyle);//设置style

	//设置listCtrl的扩展样式
	DWORD dwStyle = m_lstInfo.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT; //选中某行使整行高亮
	m_lstInfo.SetExtendedStyle(dwStyle); 

	m_lstInfo.InsertColumn(0, _T("时间"), LVCFMT_LEFT, 130);
	m_lstInfo.InsertColumn(1, _T("信息"), LVCFMT_LEFT, 150);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHelmet_recognitionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHelmet_recognitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHelmet_recognitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 打开视频文件
void CHelmet_recognitionDlg::OnOpenFile()
{
	// 打开文件对话框的标题名
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		NULL, NULL);
	dlg.m_ofn.lpstrTitle = _T("打开视频文件");

	// 判断是否获得图片
	if( dlg.DoModal() != IDOK )                    
		return;

	// 获取图片路径
	CString path = dlg.GetPathName();
	if (path.GetLength() > 0)
	{
		OnCloseFile();
		// 打开视频文件
		if(!m_vcVideo.open(CUtils::Str2Stda(path)))
		{
			MessageBox(_T("视频文件打开失败"));
		}
		else
		{
			// 读取视频频率,计算每帧之间的间隔毫秒数
			int nFps = (int)m_vcVideo.get(CV_CAP_PROP_FPS);
			int nElapse = (int)(1000.0 / nFps);
			// 读取视频频数
			m_nFrameCount = (int)m_vcVideo.get(CV_CAP_PROP_FRAME_COUNT);
			
			this->AddInfo(dlg.GetFileName());

			// 启动定时器，逐帧播放
			SetTimer(WM_TIMER_PLAYER, nElapse, NULL);		
		}
	}
}

// 关闭视频文件
void CHelmet_recognitionDlg::OnCloseFile()
{
	KillTimer(WM_TIMER_PLAYER);
	if (m_vcVideo.isOpened())
	{
		m_vcVideo.release();
		m_wndMat.ReleaseMat();
	}	
}

// 打开摄像头
void CHelmet_recognitionDlg::OnOpenCamera()
{
	// 打开摄像机，默认当前设备为0
	if(!m_vcVideo.open(0))
	{
		MessageBox(_T("打开摄像头失败"));
	}
	else
	{
		// 表示无限帧数
		m_nFrameCount = FRAME_INFINITE;
		// 启动定时器
		SetTimer(WM_TIMER_PLAYER, 40, NULL);
	}
}

// 关闭摄像头
void CHelmet_recognitionDlg::OnCloseCamera()
{
	KillTimer(WM_TIMER_PLAYER);
	if (m_vcVideo.isOpened())
	{
		m_vcVideo.release();
		m_wndMat.ReleaseMat();
	}
}

// 定时器响应函数
void CHelmet_recognitionDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == WM_TIMER_PLAYER)
	{
		// 若不限制视频帧数或则帧数大于0
		if (m_nFrameCount == FRAME_INFINITE || --m_nFrameCount > 0)
		{
			m_vcVideo >> m_wndMat.m_mat;
		
			if (!m_wndMat.m_mat.empty())
			{
				detector.HOGScan(m_wndMat.m_mat);
				m_wndMat.UpdateMat();
			}			
		}
		else
		{
			// 循环播放，则移到视频第一帧
			if (m_bLoop == true)
			{
				m_vcVideo.set(CV_CAP_PROP_POS_AVI_RATIO, 0.0);
				m_nFrameCount = (int)m_vcVideo.get(CV_CAP_PROP_FRAME_COUNT);
			}
			else // 不循环播放，则结束视频，并关闭定时器
			{
				KillTimer(WM_TIMER_PLAYER);
				OnCloseFile();
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CHelmet_recognitionDlg::OnStnClickedStaticMat()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CHelmet_recognitionDlg::AddInfo( CString &szInfo )
{
	CTime t = CTime::GetCurrentTime();	
	m_lstInfo.InsertItem(0, t.Format(_T("%Y-%m-%d %H:%M:%S")));
	m_lstInfo.SetItemText(0, 1, szInfo);
}
