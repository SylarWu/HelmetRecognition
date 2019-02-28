// MatWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MatWnd.h"

// CMatWnd

IMPLEMENT_DYNAMIC(CMatWnd, CWnd)

CMatWnd::CMatWnd()
{

}

CMatWnd::~CMatWnd()
{
	m_mat.release();
}


BEGIN_MESSAGE_MAP(CMatWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// 将vc::Mat对象绘制到MFC窗口中
void CMatWnd::DrawMatToHDC( Mat &mat )
{
	CDC *pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CvvImage img;
	IplImage ipm = mat;
	img.CopyOf(&ipm);
	img.DrawToHDC_SCALE(hDC, &m_rtImage);
	ReleaseDC(pDC);
}

// 将IpIImage对象绘制到MFC窗口中
void CMatWnd::DrawPicToHDC( IplImage *img )
{
	CDC *pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &m_rtImage);
	ReleaseDC(pDC);
}

// CMatWnd message handlers

void CMatWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (m_mat.data)
	{
		this->DrawMatToHDC(m_mat);
	}
}

// 释放Mat对象，清空界面
void CMatWnd::ReleaseMat()
{
	if (m_mat.data)
	{
		m_mat.release();
	}

	CDC *pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	pDC->BitBlt(0, 0, m_rtWnd.Width(), m_rtWnd.Height(), NULL, 0, 0, WHITENESS);
	ReleaseDC(pDC);
}

// 更新窗口图像
void CMatWnd::UpdateMat()
{
	if (m_mat.data)
	{
		// 得到窗口的大小
		GetWindowRect(&m_rtWnd);
		// 计算图像在窗口中显示的缩放比
		double dbRate;
		if (m_rtWnd.Width() > m_mat.cols && m_rtWnd.Height() > m_mat.rows)
		{
			dbRate = 1.0;
		}
		else
		{
			dbRate = m_rtWnd.Width() / (double)m_mat.cols;
			if (dbRate * (double)m_mat.rows > m_rtWnd.Height())
			{
				dbRate = m_rtWnd.Height() / (double)m_mat.rows;
			}
		}
		// 计算图片在窗口中的区域
		m_rtImage.left = (long)((m_rtWnd.Width() - m_mat.cols*dbRate) / 2);
		m_rtImage.top = (long)((m_rtWnd.Height() - m_mat.rows*dbRate) / 2);

		m_rtImage.right = (long)(m_rtImage.left + m_mat.cols*dbRate);
		m_rtImage.bottom = (long)(m_rtImage.top + m_mat.rows*dbRate);

		// 绘制图像
		DrawMatToHDC(m_mat);
	}	
}

