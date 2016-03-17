
// RESTClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RESTClient.h"
#include "RESTClientDlg.h"
#include "afxdialogex.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

#include<iostream>

using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

using namespace web::json;

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRESTClientDlg dialog



CRESTClientDlg::CRESTClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRESTClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRESTClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRESTClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRESTClientDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRESTClientDlg message handlers

BOOL CRESTClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CEdit *myEdit = (CEdit *)GetDlgItem(IDC_HOST);
	myEdit->SetWindowTextW(L"http://localhost:9000/");

	myEdit = (CEdit *)GetDlgItem(IDC_PATH);
	myEdit->SetWindowTextW(L"/JobData");


	myEdit = (CEdit *)GetDlgItem(IDC_BODY);
	myEdit->SetWindowTextW(L"{\"jobId\":\"JOB158645996312\",\"pageCount\":123,\"printerName\":\"KYO85696932\"}");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRESTClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRESTClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void AppendTextToEditCtrl(CEdit& edit, LPCTSTR pszText)
{
	// get the initial text length
	int nLength = edit.GetWindowTextLength();
	// put the selection at the end of text
	edit.SetSel(nLength, nLength);
	// replace the selection
	edit.ReplaceSel(pszText);

}

void AppendLineToMultilineEditCtrl(CEdit& edit, LPCTSTR pszText)
{
	CString strLine;
	// add CR/LF to text
	strLine.Format(_T("%s\r\n"), pszText);
	AppendTextToEditCtrl(edit, strLine);
}

void CRESTClientDlg::OnBnClickedButton1()
{
	//In the OnInitDialog function
	CEdit *myEdit = (CEdit *)GetDlgItem(IDC_CONSOLE);
	AppendLineToMultilineEditCtrl(*myEdit, L"Try to send a request\n\r");

	CString csText;
	GetDlgItemText(IDC_HOST, csText);
	http_client client(csText.GetString());

	GetDlgItemText(IDC_PATH, csText);
	uri_builder builder(csText.GetString());

	GetDlgItemText(IDC_BODY, csText);

	try {
		client.request(methods::POST, builder.to_string(), (const WCHAR *)csText, L"application/json").then([this](http_response response)
		{
			try
			{
				CString format;
				format.Format(_T("Received response status code: %u\n\r"), response.status_code());
				CEdit *myEdit = (CEdit *)GetDlgItem(IDC_CONSOLE);
				AppendTextToEditCtrl(*myEdit, format);
			}
			catch (const exception& e)
			{
				CString format;
				format.Format(_T("HTTP Exception caught: %s\n\r"), e.what());
				CEdit *myEdit = (CEdit *)GetDlgItem(IDC_CONSOLE);
				AppendTextToEditCtrl(*myEdit, format);
			}
		});
	}
	catch (const http_exception& e)
	{
		CString format;
		format.Format(_T("HTTP Exception caught: %u; %s\n\r"), e.error_code(), e.what());
		CEdit *myEdit = (CEdit *)GetDlgItem(IDC_CONSOLE);
		AppendTextToEditCtrl(*myEdit, format);
	}
}
