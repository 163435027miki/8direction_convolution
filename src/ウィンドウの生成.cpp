/**************************************************************************

	�����̃t�@�C���ł͎g�p����E�B���h�E�𐶐����C�\�����܂��D
	
***************************************************************************/

#include "filter.h"

//�E�B���h�E�̐���
//
BOOL InitInstance(HINSTANCE hInst, int nCmdShow, char szClassName[])
{


	int width;
	int height;
	// �E�C���h�E���̓E�C���h�E�̊O�g�̃T�C�Y���l������
	width = WIDTH + GetSystemMetrics(SM_CXSIZEFRAME/*�g�̕�*/) * 2;
	height = HEIGHT + GetSystemMetrics(SM_CYSIZEFRAME/*�g�̍���*/) * 2 
		+ GetSystemMetrics(SM_CYCAPTION/*�^�C�g���o�[�̍���*/)
		//+ GetSystemMetrics(SM_CYMENU/*���j���[�o�[�̍���*/)
		;
    HWND hWnd = CreateWindow(szClassName,
						"�t�B���^���Z",	//�^�C�g���o�[�ɂ��̖��O���\������܂�
						WS_OVERLAPPEDWINDOW	,//�E�B���h�E�̎��
						120,		//�w���W
						0,		//�x���W
						width,				//��
						height,				//����
						HWND_DESKTOP,		//�e�E�B���h�E�̃n���h���A�e�����Ƃ���NULL
						NULL,		//���j���[�n���h���A�N���X���j���[���g���Ƃ���NULL
						hInst,		//�C���X�^���X�n���h��
						NULL);

	//�{�^��
	CreateWindow( "BUTTON","���s",WS_CHILD | WS_VISIBLE  |  BS_PUSHBUTTON,
		300, 10, 60, 20, hWnd, (HMENU)ID_BUTTON0, hInst, NULL );




    if (!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    return TRUE;
}