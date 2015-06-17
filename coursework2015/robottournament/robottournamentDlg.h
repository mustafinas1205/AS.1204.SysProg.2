
// robottournamentDlg.h : ���� ���������
//

#pragma once
//#include "Parameters.h"
#include "CfieldDlg.h"
#include "afxwin.h"

using namespace std;

// ���������� ���� CrobottournamentDlg
class CrobottournamentDlg : public CDialogEx
{
// ��������
public:
	CrobottournamentDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_ROBOTTOURNAMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CfieldDlg Field;
	fieldData Data;
	afx_msg void OnBnClickedOk();
	int fieldHeight;
	int fieldWidth;
	CString PathToDllList;
	int robotsNumber;
	void CountRobots();
	void LoadRobots();
	afx_msg void OnBnClickedButtonLoaddll();
	int N;
	int T;
	int Emax;
	int Lmax;
	int Vmax;
	int Rmax;
	int dL;
	int dEs;
	int dEv;
	int dEa;
	int dEp;
	int dE;
	int Ne;
	int Nl;
	double rndmin;
	double rndmax;
	int K;
	CButton m_DeadBonus;
};


