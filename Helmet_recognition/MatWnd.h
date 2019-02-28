#pragma once

#include "CvvImage.h"

// CMatWnd

class CMatWnd : public CWnd
{
	DECLARE_DYNAMIC(CMatWnd)

public:
	CMatWnd();
	virtual ~CMatWnd();

public:
	void UpdateMat();
	void ReleaseMat();

public:
	Mat m_mat;

protected:
	void DrawMatToHDC(Mat &mat);
	void DrawPicToHDC(IplImage *img);

	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	// 记录视频图像区域
	CRect m_rtImage;
	// 记录当前窗口的区域
	CRect m_rtWnd;
};
