
// Buffer CacheDlg.h: 헤더 파일
//

#pragma once
#include <vector>

struct Block { // double linked list로 구현된 buffer block
	int bufferNumber;
	CString status;
	Block* next;
	Block* previous;
};


// CBufferCacheDlg 대화 상자
class CBufferCacheDlg : public CDialogEx
{
// 생성입니다.
public:
	CBufferCacheDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUFFERCACHE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CRect mCurWindowSize; // 현재 윈도우 창 사이즈
public:
	CStatic mStatic_ModNumber;// Mod Number Static Text
	CEdit mModNumber; // 사용자가 입력한 mod number
	CButton mButton_BuffersInit; // 사용자가 입력한 Buffer 설정
	afx_msg void OnBnClickedButtoninit();

	std::vector<std::vector<CStatic*>> mBlockText; // 각 block 프린트를 위한 static* vector 형 vector
	std::vector<Block*> mBlockHeaders; // block 관리를 위한 Struct 포인트 형 vector
	std::vector<int> mCountBlock; // mod 하나당 block이 몇 개 있는지 저장하기 위한 vector
	int mModnum; // 사용자가 입력한 modnum 저장
	
	CStatic mStatic_GetBlkNumber; // GetBlk number Static Text
	CEdit mGetBlkNumber; // 사용자가 입력한 GetBlk number
	CButton mButton_GetBlk; // 사용자가 입력한 GetBlk 설정
	afx_msg void OnBnClickedButtongetblk();

	void fnUpdateAllState(); // block 전체 다시 그리는 함수
};
