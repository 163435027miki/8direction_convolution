/**************************************************************************
	�����ӁF���̃v���O�������g���v���W�F�N�g�́CWin32 Console Application
	�ł͂Ȃ�Win32 Application �ō��D
***************************************************************************/

#include <windows.h>					//�E�B���h�E���g���v���O�����ɂ͕K�{
#include <stdio.h>

ATOM InitApp(HINSTANCE, char szClassName[]);	//�E�B���h�E�E�N���X��o�^��
												//��֐��̐錾

BOOL InitInstance(HINSTANCE, int, char szClassName[]);//�E�B���h�E�𐶐�����
													  //�֐��̐錾

///////////////////////////////////////////////////////////
//WINAPI���C��
//
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
                   LPSTR lpsCmdLine, int nCmdShow)
{
	char szClassName[1024] = "�t�B���^���Z";        //�E�B���h�E�N���X

	::AllocConsole();								//�R�}���h�v�����g��ʂ̕\��
	freopen( "CON", "r", stdin  );  // �W�����͂����蓖�Ă�
	freopen( "CON", "w", stdout );  // �W���o�͂����蓖�Ă�

    MSG msg;

    if (!InitApp(hCurInst,szClassName))
        return FALSE;
    if (!InitInstance(hCurInst, nCmdShow,szClassName)) 
        return FALSE;


    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

