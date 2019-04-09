 
// Buffer CacheDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Buffer Cache.h"
#include "Buffer CacheDlg.h"
#include "afxdialogex.h"

#include <cstdlib>
#include <ctime>
#include "gdiplus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBufferCacheDlg 대화 상자



CBufferCacheDlg::CBufferCacheDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BUFFERCACHE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBufferCacheDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITMODNUMBER, mModNumber);
	DDX_Control(pDX, IDC_BUTTONINIT, mButton_BuffersInit);
	DDX_Control(pDX, IDC_MODNUMBER, mStatic_ModNumber);
	DDX_Control(pDX, IDC_GETBLKNUMBER, mStatic_GetBlkNumber);
	DDX_Control(pDX, IDC_EDITGETBLKNUMBER, mGetBlkNumber);
	DDX_Control(pDX, IDC_BUTTONGETBLK, mButton_GetBlk);
}

BEGIN_MESSAGE_MAP(CBufferCacheDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONINIT, &CBufferCacheDlg::OnBnClickedButtoninit)
	ON_BN_CLICKED(IDC_BUTTONGETBLK, &CBufferCacheDlg::OnBnClickedButtongetblk)
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CBufferCacheDlg 메시지 처리기

BOOL CBufferCacheDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	this->GetWindowRect(mCurWindowSize);
	ScreenToClient(mCurWindowSize);

	//초기 위치 설정
	mStatic_ModNumber.MoveWindow(mCurWindowSize.left + 30, mCurWindowSize.top + 50, 100, 30);
	mModNumber.MoveWindow(mCurWindowSize.left + 110, mCurWindowSize.top + 50, 50, 20);
	mButton_BuffersInit.MoveWindow(mCurWindowSize.left + 180, mCurWindowSize.top + 50, 80, 25);
	mStatic_GetBlkNumber.MoveWindow(mCurWindowSize.left + 300, mCurWindowSize.top + 50, 160, 30);
	mGetBlkNumber.MoveWindow(mCurWindowSize.left + 460, mCurWindowSize.top + 50, 50, 20);
	mButton_GetBlk.MoveWindow(mCurWindowSize.left + 530, mCurWindowSize.top + 50, 80, 25);

	mModnum = 0; // 초기화

	srand((unsigned int)time(NULL)); // random number seed갑 설정

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBufferCacheDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBufferCacheDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBufferCacheDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// block 전체 다시 그리는 함수
void CBufferCacheDlg::fnUpdateAllState() {

	for (int i = 0; i <= mModnum; i++) {
		// static text 프린트를 위한 초기 x,y 위치
		int xPos = mCurWindowSize.left + 30;
		int yPos = mCurWindowSize.top + 100;
		for (int j = 0; j <= mCountBlock[i]; j++) {
			if (j == 0) {
				mBlockText[i][j]->MoveWindow(xPos, yPos + i * 50, 100, 20);
				xPos += 130;
			}
			else {
				mBlockText[i][j]->MoveWindow(xPos + (j-1) * 70, yPos + i * 50, 50, 30);
			}
		}
	}

	return;
}

void CBufferCacheDlg::OnBnClickedButtoninit() // Button_Buffer Init 눌렀을 때
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Invalidate(true);

	CString temp;
	mModNumber.GetWindowText(temp); // 입력한 mod number 가져오기

	if (temp == "") // 입력된 것이 없으면 경고 창 띄우기
	{
		MessageBox(_T("입력된 것이 없습니다."), _T("경고"), MB_ICONWARNING);
		return;
	}

	mModnum = _ttoi(temp);

	if (mModnum > 10) {
		MessageBox(_T("숫자가 너무 큽니다. 10 이하로 입력해주세요."), _T("경고"), MB_ICONWARNING);
		return;
	}

	for (int i = 0; i <= mModnum; i++) { // 초기 buffer cache 세팅
		// static text 프린트를 위한 초기 x,y 위치
		int xPos = mCurWindowSize.left + 30;
		int yPos = mCurWindowSize.top + 100;

		CStatic* modText = new CStatic;
		CString text;
		if (i != mModnum) {
			text.Format(_T("blkno %d mod %d"), i, mModnum); // blkno ? mod ? 형태로 text 출력
		}
		else { // 마지막은 Free list로 설정
			text.Format(_T("Free List")); 
			mCountBlock.push_back(0); // 처음 free list 0개
		}
		modText->Create(text, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 50, 10), this);
		modText->MoveWindow(xPos, yPos + i * 50, 100, 20);
		std::vector<CStatic*> iVector; // vector에 헤더들 넣어서 관리
		iVector.push_back(modText); // 첫 block은 blkno 이름 관리
		mBlockText.push_back(iVector);

		Block* block = new Block; //Linked list 관리 용 block header 삽입
		block->next = block;
		block->previous = block;
		mBlockHeaders.push_back(block);

		if (i == mModnum) // 마지막 free list면 for문 끝내기
			break;

		// 랜덤으로 숫자를 생성하여 3~10개 사이의 블럭 할당해주기 - mCountBlock에 저장
		int blockNum = rand() % 8 + 3;
		mCountBlock.push_back(blockNum);
		Block* blockIter = block;
		xPos += 130;
		for (int j = 0; j < blockNum; j++) {
			int number;
			while (1) {
				Block* blockDuplIter = block;
				number = rand() % 100 + 1; // 임의로 100 이하의 숫자 생성하도록 함
				if (number % mModnum == i) // mod number가 일치하는지 확인
				{
					// 이미 같은 숫자가 없는지 확인
					bool duplicate = false;
					for (int temp = 0; temp < j; temp++) {
						blockDuplIter = blockDuplIter->next;
						if (number == blockDuplIter->bufferNumber) { // 같은 숫자가 있으면
							duplicate = true;
							break;
						}
					}

					if (duplicate == false) // 중복되는게 없으면 break
						break;
				}
			}

			Block* newBlock = new Block;
			newBlock->previous = blockIter;
			newBlock->next = blockIter->next;
			newBlock->bufferNumber = number;
			int status = rand() % 2; // 2가지 상태(lock, write)중 하나 랜덤 할당 - delay, unlock은 free list 내에서 랜덤 할당 예정
			switch (status)
			{
			case 0:
				newBlock->status = "write";
				break;
			case 1:
				newBlock->status = "lock";
				break;
			default:
				break;
			}
			blockIter->next = newBlock;
			newBlock->next->previous = newBlock;
			
			blockIter = blockIter->next; // blockIter 다음 block으로 넘겨주기

			//block 프린트
			CStatic* blockText = new CStatic;
			CString blockContent;
			blockContent.Format(_T("%d\n%s"), newBlock->bufferNumber, newBlock->status);
			blockText->Create(blockContent, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 50, 30), this);
			blockText->MoveWindow(xPos + j * 70, yPos + i * 50, 50, 30);
			mBlockText[i].push_back(blockText);
		}
	}

	// 랜덤으로 free list 설정해주기(임의로 mod 하나당 free list 하나로 세팅)
	Block* freeBlockIter = mBlockHeaders[mModnum]; // mBlockHeaders의 modnum 인덱스는 마지막 block이자 free list 헤더이므로 iterator로 가리킴
	CStatic* staticIter = mBlockText[mModnum][0]; // free list 텍스트 가리킴

	int xPos = mCurWindowSize.left + 160;
	int yPos = mCurWindowSize.top + 100;
	for (int i = 0; i < mModnum; i++) {
		int randomNum = rand() % mCountBlock[i] + 1;
		Block* blockIter2 = mBlockHeaders[i];
		for (int j = 0; j < randomNum; j++) {
			blockIter2 = blockIter2->next;
		}
		// free list에 연결
		blockIter2->previous->next = blockIter2->next;
		blockIter2->next->previous = blockIter2->previous;
		blockIter2->previous = freeBlockIter;
		blockIter2->next = freeBlockIter->next;
		freeBlockIter->next->previous = blockIter2;
		freeBlockIter->next = blockIter2;
		freeBlockIter = freeBlockIter->next;

		staticIter = mBlockText[i][randomNum];

		// free list static text vector 추가
		mBlockText[mModnum].push_back(staticIter);

		// 원래 mod의 hash queue에서 static text 제거
		mBlockText[i].erase(mBlockText[i].begin() + randomNum);

		// free list 갯수 업데이트
		mCountBlock[mModnum]++;

		// mCountBlock 갯수 업데이트
		mCountBlock[i]--;

		//랜덤으로 delay 표시 할지 말지 결정 - free list라는 표시 해주기(blknum - f)
		int randomDelay = rand() % 2;
		if (randomDelay == 0) // 0이면 delay로 바꿔줌
		{
			freeBlockIter->status = "delay";
		}
		else if (randomDelay == 1) // 1이면 unlock
		{
			freeBlockIter->status = "unlock";
		}

		CString changeString;
		changeString.Format(_T("%d-f\n%s"), freeBlockIter->bufferNumber, freeBlockIter->status);
		staticIter->SetWindowTextW(changeString);

		// free list 위치로 이동
		//staticIter->MoveWindow(xPos + i * 70, yPos + mModnum * 50, 50, 30);
	}
	fnUpdateAllState();
	mButton_BuffersInit.EnableWindow(FALSE);
	return;
}


void CBufferCacheDlg::OnBnClickedButtongetblk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	mGetBlkNumber.GetWindowText(temp); // 입력한 mod number 가져오기

	if (temp == "") // 입력된 것이 없으면 경고 창 띄우기
	{
		MessageBox(_T("입력된 것이 없습니다."), _T("경고"), MB_ICONWARNING);
		return;
	}

	MessageBox(_T("Getblk 시작"), _T("Buffer Cache 진행"), MB_ICONINFORMATION);
	mButton_GetBlk.EnableWindow(FALSE);

	int blknum = _ttoi(temp);
	int hashnum = blknum % mModnum;

	int xPos = mCurWindowSize.left + 160;
	int yPos = mCurWindowSize.top + 100;

	CString changeString;

	for (int j = 0; j <= mModnum; j++) {
		if (j == hashnum || j == mModnum) { // 해당 mod의 hash queue, 또는 free list에 입력한 blknum이 있는지 확인
			Block* blockIter = mBlockHeaders[j];
			Block* freeBlockIter = mBlockHeaders[mModnum]; // free list의 header 가리킴
			CStatic* staticIter;

			for (int i = 0; i < mCountBlock[j]; i++) {
				blockIter = blockIter->next;
				staticIter = mBlockText[j][i + 1];

				// 1. Hash Queue에 해당 block이 있을 때
				if (blockIter->bufferNumber == blknum) {
					if (blockIter->status == "write" || blockIter->status == "lock" || blockIter->status == "delay") { // 찾으려고 하는 버퍼가 busy일 때, (시나리오5 알고리즘 구현)
						if (blockIter->status == "delay") { // delay->write 작업
							
							//free list에서 제거 후 mod에 추가
							// 프로세스가 해당 block 가져가기
							mBlockText[hashnum].push_back(staticIter);
							mBlockText[mModnum].erase(mBlockText[mModnum].begin() + i + 1);

							// 갯수도 업데이트
							mCountBlock[mModnum]--;
							mCountBlock[hashnum]++;

							Block* modLastBlockIter = mBlockHeaders[hashnum]->previous;
							freeBlockIter = blockIter;

							freeBlockIter->previous->next = freeBlockIter->next;
							freeBlockIter->next->previous = freeBlockIter->previous;
							freeBlockIter->previous = modLastBlockIter;
							freeBlockIter->next = modLastBlockIter->next;
							modLastBlockIter->next->previous = freeBlockIter;
							modLastBlockIter->next = freeBlockIter;
							modLastBlockIter = modLastBlockIter->next;

							// write으로 바꿔주기
							modLastBlockIter->status = "write";
							changeString.Format(_T("%d\n%s"), modLastBlockIter->bufferNumber, modLastBlockIter->status);
							staticIter->SetWindowTextW(changeString);

							// 위치도 mod의 hash queue로 업데이트
							staticIter->MoveWindow(xPos + mCountBlock[hashnum] * 70, yPos + hashnum * 50, 50, 30);
							
							MessageBox(_T("delay->write"), _T("Buffer Cache 진행"), MB_ICONINFORMATION);
						}
						
						Sleep(3000); // 시나리오 5: sleep -> wake up 3초

						blockIter->status = "unlock";
						changeString.Format(_T("%d-f\n%s"), blockIter->bufferNumber, blockIter->status);
						staticIter->SetWindowTextW(changeString);

						// free list에 추가해주기
						freeBlockIter = mBlockHeaders[mModnum]->previous; // free list의 마지막 block

						blockIter->previous->next = blockIter->next;
						blockIter->next->previous = blockIter->previous;
						blockIter->previous = freeBlockIter;
						blockIter->next = freeBlockIter->next;
						freeBlockIter->next->previous = blockIter;
						freeBlockIter->next = blockIter;
						freeBlockIter = freeBlockIter->next;

						mBlockText[mModnum].push_back(staticIter);
						mBlockText[hashnum].erase(mBlockText[hashnum].begin() + i + 1); // 원래 mod의 list에서는 삭제해줌

						// 갯수도 업데이트
						mCountBlock[mModnum]++;
						mCountBlock[hashnum]--;

						// 위치도 free list로 업데이트
						staticIter->MoveWindow(xPos + (mCountBlock[mModnum] - 1) * 70, yPos + mModnum * 50, 50, 30);

						MessageBox(_T("시나리오 5: sleep -> wake up 3초"), _T("Buffer Cache 진행"), MB_ICONINFORMATION);

						// 프로세스가 해당 block 가져가기
						// free list에서 제거 후 원래 mod vector에 추가
						mBlockText[hashnum].push_back(staticIter);
						mBlockText[mModnum].pop_back(); // free list 마지막에 추가해주었으므로 마지막에서 제거

						// 갯수도 업데이트
						mCountBlock[mModnum]--;
						mCountBlock[hashnum]++;

						Block* modLastBlockIter = mBlockHeaders[hashnum]->previous;

						freeBlockIter->previous->next = freeBlockIter->next;
						freeBlockIter->next->previous = freeBlockIter->previous;
						freeBlockIter->previous = modLastBlockIter;
						freeBlockIter->next = modLastBlockIter->next;
						modLastBlockIter->next->previous = freeBlockIter;
						modLastBlockIter->next = freeBlockIter;
						modLastBlockIter = modLastBlockIter->next;

						// lock으로 바꿔주기
						modLastBlockIter->status = "lock";
						changeString.Format(_T("%d\n%s"), modLastBlockIter->bufferNumber, modLastBlockIter->status);
						staticIter->SetWindowTextW(changeString);

						// 위치도 mod의 hash queue로 업데이트
						staticIter->MoveWindow(xPos + mCountBlock[hashnum] * 70, yPos + hashnum * 50, 50, 30);

						MessageBox(_T("시나리오 done"), _T("Buffer Cache 진행"), MB_ICONINFORMATION);
						mButton_GetBlk.EnableWindow(TRUE);
						fnUpdateAllState();
						return;
					}
					else { // 찾으려고 하는 block이 busy가 아닐 때(free list)
						if (blockIter->status == "unlock") { // 상태가 unlock이면 바로 lock으로 (시나리오 1)
							// 프로세스가 해당 block 가져가기
							// free list에서 제거, 원래 mod vector에 추가
							mBlockText[hashnum].push_back(staticIter);
							mBlockText[mModnum].erase(mBlockText[mModnum].begin() + i + 1);

							// 갯수도 업데이트
							mCountBlock[mModnum]--;
							mCountBlock[hashnum]++;

							Block* modLastBlockIter = mBlockHeaders[hashnum]->previous;
							freeBlockIter = blockIter; // free list에서 해당하는 block 가리킴

							freeBlockIter->previous->next = freeBlockIter->next;
							freeBlockIter->next->previous = freeBlockIter->previous;
							freeBlockIter->previous = modLastBlockIter;
							freeBlockIter->next = modLastBlockIter->next;
							modLastBlockIter->next->previous = freeBlockIter;
							modLastBlockIter->next = freeBlockIter;
							modLastBlockIter = modLastBlockIter->next;

							// lock으로 바꿔주기
							modLastBlockIter->status = "lock";
							changeString.Format(_T("%d\n%s"), modLastBlockIter->bufferNumber, modLastBlockIter->status);
							staticIter->SetWindowTextW(changeString);

							// 위치도 mod의 hash queue로 업데이트
							staticIter->MoveWindow(xPos + (mCountBlock[hashnum] - 1) * 70, yPos + hashnum * 50, 50, 30);

							MessageBox(_T("시나리오 1: free list에서 버퍼 가져오기 \n시나리오 Done!"), _T("Buffer Cache 진행"), MB_ICONINFORMATION);
							mButton_GetBlk.EnableWindow(TRUE);
							fnUpdateAllState();
							return;
						}
						
					}
				}
			}
		}
	}
	// hash queue 안에 해당 block이 없는 경우
	Block* freeBlockIter = mBlockHeaders[mModnum]; // free list의 header 가리킴
	CStatic* staticIter;
	Block* tempBlockIter;

	while (true) { // 시나리오 4 <-> 시나리오 2,3 왔다갔다 할 수 있으므로
		if (mCountBlock[mModnum] == 0) { // free list가 비어있는 경우 (시나리오 4 알고리즘 구현)
		// busy한 process 중 랜덤으로 하나 반환
			int randomX = rand() % mModnum; // mod 중 하나 랜덤으로 선택
			int randomY = rand() % mCountBlock[randomX] + 1; // 해당 mod 중 랜덤으로 block 선택

			tempBlockIter = mBlockHeaders[randomX];
			for (int i = 0; i < randomY; i++) {
				tempBlockIter = tempBlockIter->next;
			}
			staticIter = mBlockText[randomX][randomY];

			Sleep(3000);

			tempBlockIter->status = "unlock";
			changeString.Format(_T("%d-f\n%s"), tempBlockIter->bufferNumber, tempBlockIter->status);
			staticIter->SetWindowTextW(changeString);

			// free list에 추가해주기
			freeBlockIter = mBlockHeaders[mModnum]->previous; // free list의 마지막 block

			tempBlockIter->previous->next = tempBlockIter->next;
			tempBlockIter->next->previous = tempBlockIter->previous;
			tempBlockIter->previous = freeBlockIter;
			tempBlockIter->next = freeBlockIter->next;
			freeBlockIter->next->previous = tempBlockIter;
			freeBlockIter->next = tempBlockIter;
			freeBlockIter = freeBlockIter->next;

			mBlockText[mModnum].push_back(staticIter);
			mBlockText[randomX].erase(mBlockText[randomX].begin() + randomY); // 원래 mod의 list에서는 삭제해줌

			// 갯수도 업데이트
			mCountBlock[mModnum]++;
			mCountBlock[randomX]--;

			// 위치도 free list로 업데이트
			staticIter->MoveWindow(xPos + mCountBlock[mModnum] * 70, yPos + mModnum * 50, 50, 30);

			MessageBox(_T("시나리오 4: Freelist가 비었을 때 wake up"), _T("Buffer Cache 진행"), MB_ICONINFORMATION);
			// 그 후 시나리오 2
		}

		// 시나리오 2 또는 시나리오 3
		for (int i = 0; i < mCountBlock[mModnum]; i++) { // free list 돌면서 확인
			freeBlockIter = freeBlockIter->next;
			staticIter = mBlockText[mModnum][i + 1];
			if (freeBlockIter->status == "unlock") { // 시나리오 2
				// 프로세스가 해당 block 가져가기
				// free list에서 제거, 원래 mod vector에 추가
				hashnum = blknum % mModnum;
				mBlockText[hashnum].push_back(staticIter);
				mBlockText[mModnum].erase(mBlockText[mModnum].begin() + i + 1);

				// 갯수도 업데이트
				mCountBlock[mModnum]--;
				mCountBlock[hashnum]++;

				Block* modLastBlockIter = mBlockHeaders[hashnum]->previous;

				freeBlockIter->previous->next = freeBlockIter->next;
				freeBlockIter->next->previous = freeBlockIter->previous;
				freeBlockIter->previous = modLastBlockIter;
				freeBlockIter->next = modLastBlockIter->next;
				modLastBlockIter->next->previous = freeBlockIter;
				modLastBlockIter->next = freeBlockIter;
				modLastBlockIter = modLastBlockIter->next;

				// lock으로 바꿔주기
				modLastBlockIter->bufferNumber = blknum;
				modLastBlockIter->status = "lock";
				changeString.Format(_T("%d\n%s"), modLastBlockIter->bufferNumber, modLastBlockIter->status);
				staticIter->SetWindowTextW(changeString);

				// 위치도 mod의 hash queue로 업데이트
				staticIter->MoveWindow(xPos + (mCountBlock[hashnum] - 1) * 70, yPos + hashnum * 50, 50, 30);

				MessageBox(_T("시나리오 2: free list에서 버퍼 가져오기 \n시나리오 Done!"), _T("Buffer Cache 진행"), MB_ICONINFORMATION);
				mButton_GetBlk.EnableWindow(TRUE);
				fnUpdateAllState();
				return;
			}
			else if (freeBlockIter->status == "delay") { // 시나리오 3
				//free list에서 제거 후 mod에 추가
				// 프로세스가 해당 block 가져가기
				hashnum = freeBlockIter->bufferNumber % mModnum; // 해당 block의 원래 mod를 계산
				mBlockText[hashnum].push_back(staticIter);
				mBlockText[mModnum].erase(mBlockText[mModnum].begin() + i + 1);

				// 갯수도 업데이트
				mCountBlock[mModnum]--;
				mCountBlock[hashnum]++;

				Block* modLastBlockIter = mBlockHeaders[hashnum]->previous;
				tempBlockIter = freeBlockIter;
				freeBlockIter = freeBlockIter->previous; // free list에서 하나 빠질 것이므로 바로 전으로 이터레이터 이동

				tempBlockIter->previous->next = tempBlockIter->next;
				tempBlockIter->next->previous = tempBlockIter->previous;
				tempBlockIter->previous = modLastBlockIter;
				tempBlockIter->next = modLastBlockIter->next;
				modLastBlockIter->next->previous = tempBlockIter;
				modLastBlockIter->next = tempBlockIter;
				modLastBlockIter = modLastBlockIter->next;

				// write으로 바꿔주기
				modLastBlockIter->status = "write";
				changeString.Format(_T("%d\n%s"), modLastBlockIter->bufferNumber, modLastBlockIter->status);
				staticIter->SetWindowTextW(changeString);

				// 위치도 mod의 hash queue로 업데이트
				staticIter->MoveWindow(xPos + (mCountBlock[hashnum] - 1) * 70, yPos + hashnum * 50, 50, 30);

				MessageBox(_T("시나리오 3: delay->write"), _T("Buffer Cache 진행"), MB_ICONINFORMATION);
				i--; // count 하나 줄었으므로
			}
		}
	}
	
}
