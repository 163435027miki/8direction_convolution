#include <windows.h>			//�E�B���h�E���g���v���O�����ɂ͕K�{


#define WIDTH 1300
#define HEIGHT 700

//�{�^��
#define ID_BUTTON0 100


//#��Ɨp(�o�b�t�@)�摜���쐬
void MakeBuffDDB(HWND hWnd,  HDC *hBuffer,  BITMAP *bitmap,  int x,  int y);
//#�摜���o�b�t�@�摜�ɍ���
void ComposeDDB(HWND hWnd,  HDC hBuffer,  LPCTSTR szFileName,  int x,  int y);
//#��ʂɕ\������
int LookBMP(HWND hWnd,HDC hBuffer,BITMAP bitmap,int x,int y);
//�摜���r�b�g�}�b�v�ɕۑ�
void SaveDDB(HWND hWnd,  HDC hBuffer,  LPCTSTR szFileName,  LPCTSTR strSaveFileName,  int x,  int y);
//#�J������
bool ExitApp(HDC hBuffer);

//�t�B���^���Z
void Space_filter(HWND hWnd,  HDC hBuffer,BITMAP bitmap);

