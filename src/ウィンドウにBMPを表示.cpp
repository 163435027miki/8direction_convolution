/**************************************************************************

	�����̃t�@�C���ł�BMP����ʂɕ\������֐����쐬���܂��D

***************************************************************************/



#include <windows.h>					//�E�B���h�E���g���v���O�����ɂ͕K�{

//////////////////////////////////
//#��ʂɕ\��
//hWnd->�E�B���h�E�̃n���h��
int LookBMP(HWND hWnd,HDC hBuffer,BITMAP bitmap,int x,int y)
{
    RECT rt;
	HDC hdc;

	hdc = GetDC(hWnd);//�f�o�C�X�R���e�L�X�g�̃n���h�����擾
    GetClientRect(hWnd, &rt);//�N���C�A���g�̈�̍��W���擾

	StretchBlt(	hdc , rt.left+x , rt.top+y, rt.right , rt.bottom ,
		hBuffer , 0+x, 0+y, bitmap.bmWidth , bitmap.bmHeight , SRCCOPY);

	ReleaseDC(hWnd,hdc);//�f�o�C�X�R���e�L�X�g�����	
    return 0;
}