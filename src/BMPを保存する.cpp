/**************************************************************************

	�����̃t�@�C���ł�4�̊֐����쐬���܂��D

�����̃t�@�C���ł�DIB�`����BMP�f�[�^��ۑ�����֐����쐬���܂��D
�����̃t�@�C���ł�DDB�`����BMP�t�@�C����ۑ�����֐����쐬���܂��D
�����̃t�@�C���ł�DDB�`����BMP�t�@�C����ۑ����邽�߂Ƀr�b�g�}�b�v
�@�t�@�C���̃f�[�^��ǂݍ��ފ֐����쐬���܂��D
�����̃t�@�C���ł̓A�v���P�[�V�����̏I�����̌ォ���Â�������֐����쐬���܂��D
***************************************************************************/



#include <windows.h>					//�E�B���h�E���g���v���O�����ɂ͕K�{

//---�ۑ��p�r�b�g�}�b�v�\����---
BITMAPFILEHEADER bmpFileHeader_min;//BITMAPFILEHEADER�\����
BITMAPINFO *bmpInfo_min;//BITMAPINFO�\����
BYTE *bPixelBits_min;//�r�b�g�z��

int SaveDIB(LPCTSTR,BITMAPFILEHEADER*,BITMAPINFO*,BYTE*);//#�r�b�g�}�b�v��ۑ�����
int ReadDIB2(LPCTSTR);//#�r�b�g�}�b�v��ǂݍ���

//
void SaveDDB(HWND hWnd,HDC hBuffer,LPCTSTR szFileName,LPCTSTR strSaveFileName,int x, int y){

	//-----�ϐ��錾
	static HBITMAP	hBitmap_save;//�ۑ��p�r�b�g�}�b�v�̃n���h��
	static BITMAP	bitmap_save;//�ۑ��p�r�b�g�}�b�v�o�b�t�@
	static HDC		hBuffer_save;//�ۑ��p�r�b�g�}�b�v�̃������f�o�C�X�R���e�L�X�g

	//-----�f�o�C�X�R���e�L�X�g�̃n���h�����擾
	HDC hdc;
	hdc = GetDC(hWnd);
	//-----

	//----�ۑ��p�r�b�g�}�b�v��DDB���쐬
	ReadDIB2(szFileName);
	hBitmap_save = CreateDIBitmap(//�f�o�C�X�Ɨ� (DIB) ����f�o�C�X�ˑ�(DDB) ���쐬
		hdc ,//�f�o�C�X�R���e�L�X�g�̃n���h�����w��
		&bmpInfo_min->bmiHeader ,//DIB ���w�b�_�ւ̃|�C���^���w��
		CBM_INIT ,//�������̗L��,CBM_INIT�͏�����
		bPixelBits_min ,//�s�N�Z���r�b�g�ւ̃|�C���^���w��
		bmpInfo_min ,//���w�b�_�ւ̃|�C���^���w��
		DIB_RGB_COLORS//�J���[���t���O���w��
		);				
	//�w�肳�ꂽ�f�o�C�X�ƌ݊����̂��郁�����f�o�C�X�R���e�L�X�g���쐬
	hBuffer_save = CreateCompatibleDC(hdc);
	//�w�肳�ꂽ�f�o�C�X�R���e�L�X�g�ɁA�w�肳�ꂽ�I�u�W�F�N�g��I��
	SelectObject(hBuffer_save, hBitmap_save);
	GetObject(hBitmap_save,sizeof(BITMAP),&bitmap_save);	

	//��Ɨp�r�b�g�}�b�v��ۑ��p�r�b�g�}�b�v�ɃR�s�[
	BitBlt(	
			hBuffer_save,
			0,
			0,
			bitmap_save.bmWidth,
			bitmap_save.bmHeight,
			hBuffer,
			0+x,
			0+y,
			SRCCOPY//���̂܂܃R�s�[
			);
	//�ۑ��p�r�b�g�}�b�v��DDB����DIB�֕ϊ�
	GetDIBits(
			   hBuffer_save,
			   hBitmap_save,
			   0,
			   bitmap_save.bmHeight,
			   bPixelBits_min,
			   bmpInfo_min,
			   DIB_RGB_COLORS
			   );
	//�ۑ��p�r�b�g�}�b�v��ۑ�
	SaveDIB(strSaveFileName,
		&bmpFileHeader_min,
		bmpInfo_min,
		bPixelBits_min
		);//#

	//-----

	//�r�b�g�}�b�v�̃n���h�����J��
	DeleteObject(hBitmap_save);
	//�f�o�C�X�R���e�L�X�g�̃n���h�����J��
	if (hBuffer_save){DeleteDC(hBuffer_save);}
	ReleaseDC(hWnd,hdc);//�f�o�C�X�R���e�L�X�g�����

}

//////////////////////////////////

//////////////////////////
//#�r�b�g�}�b�v��ۑ�����
//FileName<-�ۑ�����t�@�C����
//*bmpFileHeader<-�ۑ�����BITMAPFILEHEADER�\���̂̃|�C���^
//*bmpInfo<-�ۑ�����BITMAPINFO�\���̂̃|�C���^
//*bPixelBits<-�ۑ�����r�b�g�z��̃|�C���^
int SaveDIB(LPCTSTR FileName,
			BITMAPFILEHEADER *bmpFileHeader,BITMAPINFO *bmpInfo,BYTE *bPixelBits)
{

    HANDLE hFile;
	DWORD dwBytes;//���ۂɏ������܂��o�C�g�����i�[�����DWORD �^�̕ϐ�
		hFile = CreateFile(//�I�u�W�F�N�g���쐬�܂��̓I�[�v��
			FileName,//�쐬����I�u�W�F�N�g�̖��O���������|�C���^���w��
			GENERIC_WRITE,//GENERIC_WRITE ->�������݃A�N�Z�X 
			0,//�I�u�W�F�N�g�̋��L���@���w��,'�O'���w�肷��Ƌ��L����Ȃ�
			NULL,//�擾�����n���h���̎q�v���Z�X�ւ̌p��������,NULL�͌p���Ȃ�
			CREATE_ALWAYS,//�V�����t�@�C�����쐬,�㏑��
			FILE_ATTRIBUTE_NORMAL,//�t�@�C���̑�������уt���O���w��
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE){//�֐������s�����
			return 0;
		}

	    //BITMAPFILEHEADER�\���̂���������
		WriteFile(//�t�@�C���Ƀf�[�^����������
			hFile,//�t�@�C���̃n���h�����w��
			bmpFileHeader,//�������ރf�[�^���������o�b�t�@�ւ̃|�C���^���w��
			sizeof(BITMAPFILEHEADER),//�������ރo�C�g�����w��
			&dwBytes,//���ۂɏ������܂ꂽ�o�C�g�����i�[
			NULL
			);
		//BITMAPINFO�\���̂���������
		WriteFile(
			hFile,
			bmpInfo,
			bmpFileHeader_min.bfOffBits - sizeof(BITMAPFILEHEADER),
			&dwBytes,
			NULL
			);
		//�r�b�g�z�����������
		WriteFile(
			hFile,
			bPixelBits,
			bmpFileHeader_min.bfSize - bmpFileHeader_min.bfOffBits,
			&dwBytes,
			NULL);
		CloseHandle(hFile);//CreateFile �֐����Ԃ����n���h�����N���[�Y
    return 0;
}

////////////////////////////////////////////////////////////

//////////////////////////
//#�r�b�g�}�b�v��ǂݍ���
//FileName<-�ǂݍ��ރt�@�C����
int ReadDIB2(LPCTSTR FileName)
{
    HANDLE hFile;//�t�@�C���p�n���h��
	DWORD dwBytes;//���ۂɓǂݎ��ꂽ�o�C�g�����i�[�����DWORD �^�̕ϐ�
	//----�ۑ��p�r�b�g�}�b�v�̍\����ǂݍ���
	hFile = CreateFile(//�I�u�W�F�N�g���쐬�܂��̓I�[�v��
		FileName,//�쐬����I�u�W�F�N�g�̖��O���������|�C���^���w��
		GENERIC_READ,//GENERIC_READ ->�ǂݎ��A�N�Z�X
		0,//�I�u�W�F�N�g�̋��L���@���w��,'�O'���w�肷��Ƌ��L����Ȃ�
		NULL,//�擾�����n���h���̎q�v���Z�X�ւ̌p��������,NULL�͌p���Ȃ�
		OPEN_EXISTING,//OPEN_EXISTING ->�t�@�C�����I�[�v�� 
		FILE_ATTRIBUTE_NORMAL,//�t�@�C���̑�������уt���O���w��
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE){//�֐������s�����
		MessageBox(NULL , TEXT("�摜�t�@�C�����J���܂���") , NULL , MB_OK);
		return 1;
	}
	//+BITMAPFILEHEADER�\���̂�ǂݍ���
	ReadFile(//�t�@�C������f�[�^��ǂݎ��
		hFile,//�t�@�C���̃n���h�����w��
		&(bmpFileHeader_min),//�t�@�C������ǂݎ�����f�[�^���i�[�����
		sizeof(BITMAPFILEHEADER),//�ǂݎ��o�C�g�����w��
		&dwBytes,//���ۂɓǂݎ��ꂽ�o�C�g�����i�[
		NULL);
	if ((bmpFileHeader_min).bfType != 0x4D42) {//0x4D42��"BM"
		MessageBox(NULL , TEXT("�r�b�g�}�b�v�t�@�C���ł͂���܂���") , NULL , MB_OK);
		return 1;
	}
	//+
	//++BITMAPINFO�\���̂�ǂݍ���
	//�������[���m��
	(bmpInfo_min) = (BITMAPINFO *) malloc ((bmpFileHeader_min).bfOffBits - dwBytes);
	//�f�[�^��ǂݎ��
	ReadFile(hFile,(bmpInfo_min),
		(bmpFileHeader_min).bfOffBits - dwBytes,&dwBytes,NULL);
	//++
	//+++BITMAPINFO�\���̂�ǂݍ���
	//�������[���m��
	(bPixelBits_min) = (BYTE *) malloc ((bmpFileHeader_min).bfSize - (bmpFileHeader_min).bfOffBits);
	//�f�[�^��ǂݎ��
	ReadFile(hFile,(bPixelBits_min),
		(bmpFileHeader_min).bfSize - (bmpFileHeader_min).bfOffBits,&dwBytes,NULL);
	//+++
	CloseHandle(hFile);//CreateFile �֐����Ԃ����n���h�����N���[�Y
	//----
    return 0;
}


//#�A�v���P�[�V�����̏I�����̌ォ���Â�������֐�
//
bool ExitApp(HDC hBuffer){

	free(bPixelBits_min);
	free(bmpInfo_min);

	if (hBuffer) {DeleteDC(hBuffer);}

	return TRUE;
}


/////////////////////////////////////////////

