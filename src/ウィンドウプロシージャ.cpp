/**************************************************************************

	�����̃t�@�C���ł̓E�B���h�E����̃��b�Z�[�W�i�{�^�����N���b�N�����Ȃǁj
	���󂯎���āC���s����֐����Ăяo���܂��D
	
***************************************************************************/
#include "filter.h"

//�������m�ۂƃt�@�C�����̌������s�����߂̃w�b�_
#define ANSI				
#include "nrutil.h"				
#include<stdio.h>


/////////////////////////
//�E�B���h�E�v���V�[�W��
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static PAINTSTRUCT ps;	//API�̌^
	static BITMAP bitmap;	//API�̌^
	static HDC hBuffer;		//API�̌^


	switch (msg) {
		case WM_CREATE:

			//#��Ɨp(�o�b�t�@)�摜���쐬(�E�B���h�E�̉�f�l��������hBuffer�Ɋm��)
			MakeBuffDDB(hWnd, &hBuffer, &bitmap,0,50);		//���[�U��`�̊֐�							
			//�w��͈͂𔒂��h��Ԃ�
			PatBlt(hBuffer , 0 , 0 ,WIDTH ,HEIGHT , WHITENESS);//API�̊֐�
			//hBuffer�̒l����ʂɕ\��
			LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);//���[�U��`�̊֐�	
		break;


		case WM_COMMAND:
			switch (LOWORD(wp)) {

				case ID_BUTTON0://�t�B���^���Z

					PatBlt(hBuffer,	 0,  50,  WIDTH,  HEIGHT,  WHITENESS);//API�̊֐�
					Space_filter( hWnd, hBuffer, bitmap );	//���[�U��`�̊֐�//�t�B���^���Z

				break;

			}
		break;

		case WM_PAINT:
			//��ʂɕ\��
			BeginPaint(hWnd, &ps);//API�̊֐�
				//�e�L�X�g��\��
				TextOut(ps.hdc,30,10,"bmp��ǂݍ���Ńt�B���^���Z",strlen("bmp��ǂݍ���Ńt�B���^���Z"));//API�̊֐�
				LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��//���[�U��`�̊֐�	
			EndPaint(hWnd, &ps);//API�̊֐�
		break;
			
		case WM_CLOSE:

            DestroyWindow(hWnd);//Window��j��//API�̊֐�

		break;

		case WM_DESTROY:
			//�A�v���P�[�V�����̏I�����̌ォ���Â�
			if(!ExitApp(hBuffer)){return FALSE;}	//���[�U��`�̊֐�
			PostQuitMessage(0);//API�̊֐�
		break;
		default:
		return (DefWindowProc(hWnd, msg, wp, lp));//API�̊֐�
	}
    return 0;
}

