/**************************************************************************

	�����̃t�@�C���ł͍�Ɨp(�o�b�t�@)�摜���쐬���܂��D
	
***************************************************************************/


#include "filter.h"



void MakeBuffDDB(HWND hWnd,HDC *hBuffer,BITMAP *bitmap,int x,int y){

	HDC hdc;
	hdc = GetDC(hWnd);//�f�o�C�X�R���e�L�X�g�̃n���h�����擾

	HDC hBuf;
	BITMAP bitm;

	hBuf=*hBuffer;
	bitm=*bitmap;

	static HBITMAP	hBitmap;//��Ɨp�r�b�g�}�b�v�̃n���h��
	
	//----��Ɨp��DDB���쐬
	hBitmap = CreateCompatibleBitmap(hdc , WIDTH ,HEIGHT);
	//�w�肳�ꂽ�f�o�C�X�ƌ݊����̂��郁�����f�o�C�X�R���e�L�X�g���쐬
	hBuf = CreateCompatibleDC(hdc);
	//�w�肳�ꂽ�f�o�C�X�R���e�L�X�g�ɁA�w�肳�ꂽ�I�u�W�F�N�g��I��
	SelectObject(hBuf , hBitmap);
	//�y�����w��
	SelectObject(hBuf , GetStockObject(NULL_PEN));
	//�w�肳�ꂽ�����`�����݂̃u���V�Ńy�C���g
	PatBlt(hBuf , 0 +x, 0+y ,WIDTH -x,HEIGHT -y, WHITENESS);
	GetObject(//�w�肳�ꂽ�O���t�B�b�N�I�u�W�F�N�g�̏����擾
		hBitmap,//�O���t�B�b�N�I�u�W�F�N�g�̃n���h�����w��
		sizeof(BITMAP),//�o�b�t�@�̃T�C�Y���w��
		&bitm//�o�b�t�@�ւ̃|�C���^���w��,���̃o�b�t�@�ɃI�u�W�F�N�g�̏�񂪊i�[
		);
	//----

	//�r�b�g�}�b�v�̃n���h�����J��
	DeleteObject(hBitmap);

	*hBuffer=hBuf;
	*bitmap=bitm;

}

