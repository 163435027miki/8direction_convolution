/**************************************************************************

	�����̃t�@�C���ł͎g�p����E�B���h�E��Windows�̃V�X�e���ɓo�^���܂��D
	WNDCLASSEX�Őݒ肵�CRegisterClassEx()�œo�^���܂��D
	
***************************************************************************/
#include <windows.h>					//�E�B���h�E���g���v���O�����ɂ͕K�{

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//�E�B���h�E�v���V�[�W��
													 //�̐錾
/////////////////////////////
//�E�B���h�E�E�N���X�̓o�^
////
ATOM InitApp(HINSTANCE hInst, char szClassName[])
{
    WNDCLASSEX wc;	     		//�V�������E�B���h�E�N���X�p�̕ϐ�wc���`
    wc.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEX�̑傫��(�e�ʂ�)
    wc.style = CS_HREDRAW | CS_VREDRAW;				//�E�B���h�E�̓������̐ݒ�
    wc.lpfnWndProc = WndProc;						//���̃N���X�����v���V�[�W����
    wc.cbClsExtra = 0;							
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;									//�C���X�^���X�n���h��
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//�A�C�R���̎w��
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			//�����ȃA�C�R���̎w��
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//�J�[�\���̎w��
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);	//�w�i�̐F�̎w��
    wc.lpszMenuName = "MYMENU";								//���j���[��
    wc.lpszClassName = (LPCSTR)szClassName;					//�E�B���h�E�N���X�̖��O
    return (RegisterClassEx(&wc));							//�E�B���h�E�N���X�̓o�^
}
/////////////////////
