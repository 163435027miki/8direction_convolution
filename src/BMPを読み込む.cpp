/**************************************************************************

	�����̃t�@�C���ł�2�̊֐����`���܂��D

�����̃t�@�C���ł͂��Ɖ摜�̃r�b�g�}�b�v�̓ǂݍ���DDB�֐����쐬���܂��D
�����̃t�@�C���ł͂��Ɖ摜����Ɨp(�o�b�t�@)�摜�ɍ�������֐����쐬���܂��D

***************************************************************************/

#include <windows.h>					//�E�B���h�E���g���v���O�����ɂ͕K�{


int ReadDIB_MakeDDB(HDC, LPCTSTR,BITMAP *,HDC *);//#

//hdc<-�f�o�C�X�R���e�L�X�g
//FileName<-�ǂݍ��ރt�@�C����
//*bitmap_original->���Ɖ摜�p�r�b�g�}�b�v�o�b�t�@
//*hBuffer_original->���Ɖ摜�p�r�b�g�}�b�v�̃������f�o�C�X�R���e�L�X�g
int ReadDIB_MakeDDB(HDC hdc, LPCTSTR FileName, 
					BITMAP *bitmap_original,HDC *hBuffer_original)
{

	HBITMAP hBitmap_original;			//���Ɖ摜�r�b�g�}�b�v�̃n���h��
	//---�ۑ��p�r�b�g�}�b�v�\����---
	BITMAPFILEHEADER bmpFileHeader;//BITMAPFILEHEADER�\����
	BITMAPINFO *bmpInfo;//BITMAPINFO�\����
	BYTE *bPixelBits;//�r�b�g�z��

    HANDLE hFile;//�t�@�C���p�n���h��
	DWORD dwBytes;//���ۂɓǂݎ��ꂽ�o�C�g�����i�[�����DWORD �^�̕ϐ�

	//----�ۑ��p�r�b�g�}�b�v�̍\����ǂݍ���
	hFile = CreateFile(							//�I�u�W�F�N�g���쐬�܂��̓I�[�v��
		FileName,								//�쐬����I�u�W�F�N�g�̖��O���������|�C���^���w��
		GENERIC_READ,							//GENERIC_READ ->�ǂݎ��A�N�Z�X
		0,										//�I�u�W�F�N�g�̋��L���@���w��,'�O'���w�肷��Ƌ��L����Ȃ�
		NULL,									//�擾�����n���h���̎q�v���Z�X�ւ̌p��������,NULL�͌p���Ȃ�
		OPEN_EXISTING,							//OPEN_EXISTING ->�t�@�C�����I�[�v�� 
		FILE_ATTRIBUTE_NORMAL,					//�t�@�C���̑�������уt���O���w��
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE){			//�֐������s�����
//		MessageBox(NULL , TEXT("�t�@�C�����J���܂���") , NULL , MB_OK);
		return 1;
	}
	//+BITMAPFILEHEADER�\���̂�ǂݍ���
	ReadFile(									//�t�@�C������f�[�^��ǂݎ��
		hFile,									//�t�@�C���̃n���h�����w��
		&(bmpFileHeader),						//�t�@�C������ǂݎ�����f�[�^���i�[�����
		sizeof(BITMAPFILEHEADER),				//�ǂݎ��o�C�g�����w��
		&dwBytes,								//���ۂɓǂݎ��ꂽ�o�C�g�����i�[
		NULL);
	if ((bmpFileHeader).bfType != 0x4D42) {		//0x4D42��"BM"
		MessageBox(NULL , TEXT("�r�b�g�}�b�v�t�@�C���ł͂���܂���") , NULL , MB_OK);
		return 1;
	}
	//+
	//++BITMAPINFO�\���̂�ǂݍ���
	//�������[���m��
	(bmpInfo) = (BITMAPINFO *) malloc ((bmpFileHeader).bfOffBits - dwBytes);
	//�f�[�^��ǂݎ��
	ReadFile(hFile,(bmpInfo),
		(bmpFileHeader).bfOffBits - dwBytes,&dwBytes,NULL);
	//++
	//+++BITMAPINFO�\���̂�ǂݍ���
	//�������[���m��
	(bPixelBits) = (BYTE *) malloc ((bmpFileHeader).bfSize - (bmpFileHeader).bfOffBits);
	//�f�[�^��ǂݎ��
	ReadFile(hFile,(bPixelBits),
		(bmpFileHeader).bfSize - (bmpFileHeader).bfOffBits,&dwBytes,NULL);
	//+++
	CloseHandle(hFile);//CreateFile �֐����Ԃ����n���h�����N���[�Y
	//----

	hBitmap_original = CreateDIBitmap(hdc ,&bmpInfo->bmiHeader,CBM_INIT,
		bPixelBits ,bmpInfo ,DIB_RGB_COLORS);

	//�w�肳�ꂽ�f�o�C�X�ƌ݊����̂��郁�����f�o�C�X�R���e�L�X�g���쐬
	(*hBuffer_original) = CreateCompatibleDC(hdc);
	//�w�肳�ꂽ�f�o�C�X�R���e�L�X�g�ɁA�w�肳�ꂽ�I�u�W�F�N�g��I��
	SelectObject((*hBuffer_original) , hBitmap_original);
	GetObject(hBitmap_original,sizeof(BITMAP),&(*bitmap_original));

	//-----�J��
	free(bPixelBits);//�r�b�g�z��
	free(bmpInfo);//BITMAPINFO�\����
	DeleteObject(hBitmap_original);//�r�b�g�}�b�v�̃n���h��
	//-----

    return 0;
}


//////////////////////////////////
//#���Ɖ摜����Ɨp(�o�b�t�@)�摜�ɍ���
//
void ComposeDDB(HWND hWnd,HDC hBuffer,LPCTSTR szFileName,int x, int y){

	HDC hdc;
	hdc = GetDC(hWnd);//�f�o�C�X�R���e�L�X�g�̃n���h�����擾

	static BITMAP	bitmap_original;		//���Ɖ摜�p�r�b�g�}�b�v�o�b�t�@
	static HDC		hBuffer_original;		//���Ɖ摜�p�r�b�g�}�b�v�̃������f�o�C�X�R���e�L�X�g


	ReadDIB_MakeDDB(hdc,szFileName,&bitmap_original,&hBuffer_original);
	//----���Ɖ摜�p�r�b�g�}�b�v����Ɨp�r�b�g�}�b�v�ɍ���
	BitBlt(	
		hBuffer,							//�R�s�[��f�o�C�X�R���e�L�X�g�̃n���h��
		x,									//�R�s�[��� X ���W
		y,									//�R�s�[���Y ���W
		bitmap_original.bmWidth,			//��
		bitmap_original.bmHeight,			//����
		hBuffer_original,					//�R�s�[���̃f�o�C�X�R���e�L�X�g�̃n���h��
		0,									//�R�s�[���� X ���W
		0,									//�R�s�[���� Y ���W
		//NOTSRCCOPY
		//SRCAND//���Z����
		SRCCOPY//���̂܂܃R�s�[
		);
	//-----

	//�f�o�C�X�R���e�L�X�g�̃n���h�����J��
	if (hBuffer_original){DeleteDC(hBuffer_original);}
	ReleaseDC(hWnd,hdc);					//�f�o�C�X�R���e�L�X�g�����

}