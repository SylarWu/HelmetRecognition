
// Helmet_recognitionDlg.h : 头文件
//

#pragma once

#include "MatWnd.h"
#include "SVMTraining.h"
#include "afxcmn.h"
#include "Detector.h"

// CHelmet_recognitionDlg 对话框
class CHelmet_recognitionDlg : public CDialogEx
{
// 构造
public:
	CHelmet_recognitionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HELMET_RECOGNITION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnOpenFile();
	afx_msg void OnCloseFile();
	afx_msg void OnOpenCamera();
	afx_msg void OnCloseCamera();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStaticMat();
	DECLARE_MESSAGE_MAP()

	void AddInfo(CString &szInfo);

private:
	// 视频对象
	VideoCapture m_vcVideo;
	// Mat显示窗口
	CMatWnd m_wndMat;
	// 视频文件是否循环播放
	bool m_bLoop;
	// 视频文件剩余频数
	int m_nFrameCount;

	// 创建监测对象
	SVMTraining svm_detector_obj;
	// 检测器
	Detector detector;
	// 列表对象
	CListCtrl m_lstInfo;
};
