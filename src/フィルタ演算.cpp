
#include <windows.h>					//�E�B���h�E���g���v���O�����ɂ͕K�{

//�������m�ۂ��s�����߂̃w�b�_
#define ANSI				
#include "nrutil.h"				
#include<stdio.h>
#include<math.h>

#include "filter.h"

#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //�����X�g���[��

#include<time.h>//���Ԃ�p����

#include <direct.h>//�t�H���_���쐬����


using namespace std;



void Space_filter(HWND hWnd,  HDC hBuffer,BITMAP bitmap){

	COLORREF colorref;	//�J���[���t�@�����X
	
	//���s�����̕\��
	time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
	char date[128]="";
	sprintf(date,"%2d/%2d/%2d - %d:%d:%d\n",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
	printf(date);


 ///////////////////////////////�����ݒ�1 : ���͉摜�w��//////////////////////////////////////////////////////////////////////////////////////
	
	//��x�N�g�����擾����Ƃ��͂�����
	int filter_number=315;								//�t�B���^���Z�C��x�N�g���쐬����Ƃ��̂ݎw��D������ύX����					
/*	char propety_dire_char[128];
	sprintf(propety_dire_char,"..\\property_usa\\simulation17-0203\\Rvector\\8dire_100k\\property_8dire_100k_I~%d.txt",filter_number);	
	std::ifstream propety_dire(propety_dire_char);
	*/
	//�ʏ펞�͂�����
	std::ifstream propety_dire("..\\property_usa\\simulation17-0615_circle-1\\property_10k_conv_sd0.txt");		//�����������͂���΂悢.��x�N�g���쐬�̍ۂ͏��ύX����

	

	//臒l�̐ݒ�D����property.txt�ɉ����邩��
	int Offset=128;
	double magnification=1;
	float Upper_threshold=35;
	float Under_threshold=-35;
	int use_upperthreshold_flag=1;
	int use_underthreshold_flag=1;
	int Save_image_flag[9][9];

	//�摜�쐬���f�̏�����(�f�t�H���g�ł͍쐬���Ȃ�)
	for(int i=0;i<9;++i){
	Save_image_flag[1][i]=0;
	Save_image_flag[2][i]=0;
	}

	//���l�摜���쐬����
	Save_image_flag[2][0]=1;		//���l�摜���쐬����Ƃ�1
/*
	//4�����ɉ摜���쐬����
	for(int i=1;i<=4;++i){
		int I=2*i-1;
	Save_image_flag[2][I]=1;
	}
*/
	
	//8�����ɉ摜���쐬����
	for(int i=1;i<=8;++i){
		
	Save_image_flag[2][i]=1;
	}
		
	int max_x, max_y;		//�摜�T�C�Y-1
	int image_x, image_y;		//�摜�T�C�Y
	int Rvector_create;		//1�Ȃ��x�N�g�������
	int Rvector_pointX;		//��x�N�g�����쐬����ʒu.0���X�^�[�g�ōl����
	int Rvector_pointY;		//����Ȏw�������ꍇ�͉���
	int fs;					//�t�B���^�̑傫��(�)
	int All_property[10];	//��L�̏���ǂݍ��ޗp

	char propety[255];
	char Allpropety[255];
	char outputrvector_directory[128];				//Rvector�̏ꏊ
	char inputimage_directory[128];
	char inputdate_directory1[255];
	char inputdate_directory2[255];
	char inputdate_directory3[255];
	char inputdate_directory4[255];
	char inputdate_directory5[255];
	char inputdate_directory6[255];	
	char inputdate_directory7[255];
	char inputdate_directory8[255];			//�J�Ԃ����𑝂₵����1/4
	int count_property=0;
	int count_Allproperty=0;


	//�v���p�e�Btxt�t�@�C���̓ǂݍ���
    if (propety_dire.fail())
    {
        printf("property�e�L�X�g��ǂݎ�邱�Ƃ��ł��܂���\n");
    }
    while (propety_dire.getline(propety, 256 - 1))
    {

		if(count_property==0)sprintf(Allpropety,propety);
		if(count_property==1)sprintf(outputrvector_directory,propety);
		if(count_property==2)sprintf(inputimage_directory,propety);
		if(count_property==3)sprintf(inputdate_directory1,propety);
		if(count_property==4)sprintf(inputdate_directory2,propety);
		if(count_property==5)sprintf(inputdate_directory3,propety);
		if(count_property==6)sprintf(inputdate_directory4,propety);
		if(count_property==7)sprintf(inputdate_directory5,propety);
		if(count_property==8)sprintf(inputdate_directory6,propety);
		if(count_property==9)sprintf(inputdate_directory7,propety);
		if(count_property==10)sprintf(inputdate_directory8,propety);

		++count_property;
    }

	//�v���p�e�Btxt�t�@�C���̈�s�ڂ𕶎��񂩂琔�l�ɕϊ����Ċ֐��ɑ��
	std::string tmp_Allpropety;
	std::istringstream stream_Allpropety(Allpropety);
	while(getline(stream_Allpropety,tmp_Allpropety,',')){

		int All_tmp_property=stof(tmp_Allpropety); //stof(string str) : string��int�ɕϊ�
		All_property[count_Allproperty]=All_tmp_property;


////////////////property.txt�̈�s��///////////////////////////////////////////////////////////
		if(count_Allproperty==0){image_x=All_property[count_Allproperty];max_x=image_x-1;}			//image_x:�摜�̉��T�C�Y,max_x:�摜�̉��T�C�Y-1
		if(count_Allproperty==1){image_y=All_property[count_Allproperty];max_y=image_y-1;}			//image_y:�摜�̏c�T�C�Y,max_y:�摜�̏c�T�C�Y-1
		if(count_Allproperty==2)Rvector_create=All_property[count_Allproperty];		//Rvector_create:1�Ȃ��x�N�g�����쐬�D���̏ꍇ�����ݒ�2��ݒ肷�邱��
		if(count_Allproperty==3)Rvector_pointX=All_property[count_Allproperty];		//Rvector_pointX:��x�N�g���擾X���W�D0����X�^�[�g
		if(count_Allproperty==4)Rvector_pointY=All_property[count_Allproperty];		//Rvector_pointY:��x�N�g���擾Y���W�D0����X�^�[�g
		if(count_Allproperty==5)fs=All_property[count_Allproperty];					//fs:�t�B���^�T�C�Y�i��j

		++count_Allproperty;
	}
	
	char InputImage[64];						//���͉摜
	sprintf(InputImage,inputimage_directory);	//property����ǂݎ�������͉摜������



	 
///////////////////////////////�����ݒ�2 : ��x�N�g���쐬���ۂ�(�C�������Ȃ���΂Ȃ�Ȃ��j/////////////////////////////////////////////////////////////

	
	//if(Rvector_create==1)sprintf(InputImage, "..\\bmp\\255-0\\%d.bmp", filter_number);	//Rvector���쐬����ۂ̓��͉摜�w��.
	

	char Rvector_directory[128];
	double Rvector[9];											//��x�N�g���̐�+1
	sprintf(Rvector_directory,outputrvector_directory,propety);		//Rvector��ۑ�����t�H���_�����w��.property.txt��2�s��

	
	//Rvector��ۑ�����t�H���_�̍쐬
	printf("Rvector_create=%d\n",Rvector_create);
	if(Rvector_create==1){


		if(_mkdir(Rvector_directory)==0){
			printf("Rvecor�ۑ��t�H���_ %s ���쐬���܂���\n",Rvector_directory);
		}else{
			printf("Rvecor�ۑ��t�H���_ %s �͍쐬�ς݂ł�\n",Rvector_directory);
		}
	}
//////////////////////////�����ݒ�̊m�F�p//////////////////////////////////////////////////////////////////////////////

	if(Rvector_create==1)printf("��x�N�g���擾���W�FX=%d�CY=%d\n",Rvector_pointX,Rvector_pointY);
	printf("InputImage=%s\n",InputImage);
	printf("�摜�T�C�Y�FX=%d�CY=%d\n",image_x, image_y);
	printf("fs=%d\n",fs);
	printf("Offset=%d\n",Offset);
	printf("Upper_threshold : %f, Under_threshold : %f\n",Upper_threshold,Under_threshold);
	printf("use_upperthreshold_flag : %d, use_underthreshold_flag : %d\n",use_upperthreshold_flag,use_underthreshold_flag);

////////////////////////////�����ݒ�3 : �t�B���^���w��////////////////////////////////////////////////////////////////////
	
	int hfs=(fs+1)/2;			//�t�B���^�̑傫���̔���

//property.txt�ł̎w��
	char Filter1[127],Filter2[127],Filter3[127],Filter4[127],Filter5[127],Filter6[127],Filter7[127],Filter8[127];
	sprintf(Filter1,inputdate_directory1);
	sprintf(Filter2,inputdate_directory2);
	sprintf(Filter3,inputdate_directory3);
	sprintf(Filter4,inputdate_directory4);
	sprintf(Filter5,inputdate_directory5);
	sprintf(Filter6,inputdate_directory6);
	sprintf(Filter7,inputdate_directory7);
	sprintf(Filter8,inputdate_directory8);

//�蓮�w��o�b�N�A�b�v
	/*
	int fs=15;					//�t�B���^�̑傫��(�)
	int hfs=(fs+1)/2;			//�t�B���^�̑傫���̔���
	char Filter1[]="..\\�t�B���^\\15�~15��H�J�[�l��\\4����Rs=1k\\V(0).txt";
	char Filter2[]="..\\�t�B���^\\15�~15��H�J�[�l��\\4����Rs=1k\\V(45).txt";
	char Filter3[]="..\\�t�B���^\\15�~15��H�J�[�l��\\4����Rs=1k\\V(90).txt";
	char Filter4[]="..\\�t�B���^\\15�~15��H�J�[�l��\\4����Rs=1k\\V(135).txt";
	char Filter5[]="..\\�t�B���^\\15�~15��H�J�[�l��\\4����Rs=1k\\V(180).txt";
	char Filter6[]="..\\�t�B���^\\15�~15��H�J�[�l��\\4����Rs=1k\\V(225).txt";
	char Filter7[]="..\\�t�B���^\\15�~15��H�J�[�l��\\4����Rs=1k\\V(270).txt";
	char Filter8[]="..\\�t�B���^\\15�~15��H�J�[�l��\\4����Rs=1k\\V(315).txt";
	*/
	//sobel
	/*
	int fs=3;					//�t�B���^�̑傫��(�)
	int hfs=(fs+1)/2;			//�t�B���^�̑傫���̔���
	char Filter1[]="..\\�t�B���^\\sobel\\sobel0.txt";
	char Filter2[]="..\\�t�B���^\\sobel\\sobel45.txt";
	char Filter3[]="..\\�t�B���^\\sobel\\sobel90.txt";
	char Filter4[]="..\\�t�B���^\\sobel\\sobel135.txt";
	char Filter5[]="..\\�t�B���^\\sobel\\sobel180.txt";
	char Filter6[]="..\\�t�B���^\\sobel\\sobel225.txt";
	char Filter7[]="..\\�t�B���^\\sobel\\sobel270.txt";
	char Filter8[]="..\\�t�B���^\\sobel\\sobel315.txt";
	*/

////////////////////////////�����ݒ�4 : �o�͌��ʂ̃t�@�C�����̎w��//////////////////////////////////////////////////////////////////////////////////
	
	//�o�͌��ʂ̃t�@�C�����̎w��
	char *Filename1_s = "V(0).csv";
	char *Filename2_s = "V(45).csv";
	char *Filename3_s = "V(90).csv";
	char *Filename4_s = "V(135).csv";
	char *Filename5_s = "V(180).csv";
	char *Filename6_s = "V(225).csv";
	char *Filename7_s = "V(270).csv";
	char *Filename8_s = "V(315).csv";


	//���ʂ�ۑ�����t�H���_�̐���
	//�t�H���_���͎��s�����ɂȂ�
	char date_directory[128];
	char outputbmp_directory[128];
	sprintf(date_directory,"..\\result_usa\\%2d-%02d%02d-%02d%02d%02d\\",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
	 if(_mkdir(date_directory)==0){
        printf("�t�H���_ %s ���쐬���܂���\n",date_directory);
    }else{
        printf("�t�H���_�쐬�Ɏ��s���܂����B\n");
    }
	//�o�̓t�@�C�����E�o�͐�̐ݒ�
	char Filename1[64];
	char Filename2[64];
	char Filename3[64];
	char Filename4[64];
	char Filename5[64];
	char Filename6[64];
	char Filename7[64];
	char Filename8[64];

	//������̌���
	strcpy(Filename1, date_directory);
    strcat(Filename1, Filename1_s);
	strcpy(Filename2, date_directory);
    strcat(Filename2, Filename2_s);
	strcpy(Filename3, date_directory);
    strcat(Filename3, Filename3_s);
	strcpy(Filename4, date_directory);
    strcat(Filename4, Filename4_s);
	strcpy(Filename5, date_directory);
    strcat(Filename5, Filename5_s);
	strcpy(Filename6, date_directory);
    strcat(Filename6, Filename6_s);
	strcpy(Filename7, date_directory);
    strcat(Filename7, Filename7_s);
	strcpy(Filename8, date_directory);
    strcat(Filename8, Filename8_s);

	
	printf("�g�p�����J�[�l����\n");
	printf("V0   = %s\n",Filter1);
	printf("V45  = %s\n",Filter2);
	printf("V90  = %s\n",Filter3);
	printf("V135 = %s\n",Filter4);
	printf("V180 = %s\n",Filter5);
	printf("V225 = %s\n",Filter6);
	printf("V270 = %s\n",Filter7);
	printf("V315 = %s\n",Filter8);
	

////////////////////////��x�N�g���쐬///////////////////////////////////////////////////////////////////////////////

	char Rvector_Filename[255];
	char *Rvector_Filename_s;
	sprintf(Rvector_Filename,"%s\\Rvector%d.csv",Rvector_directory,filter_number);
	

/////////////////////////��x�N�g���쐬�ʒu�̓���w��//////////////////////////////////////////////////////
	//����w��Ւn


	int Rvector_number=0;				//Rvector�̔ԍ��̃J�E���g

////////////////////////��ݍ��݉��Z�v���O�����J�n/////////////////////////////////////////////////////////////////////////////////


		//Nrutil��p�����������̊m��
	double **input_bmp=matrix(0, max_x, 0, max_y);
	double **output1=matrix(0, max_x, 0, max_y);
	double **spfil1=matrix( 1,  fs,  1,  fs);
	double **output_bmp_image=matrix(0, max_x, 0, max_y);	//�ǉ�1
	double **output_bmp_flag=matrix(0, max_x, 0, max_y);	//�ǉ�3

	int x,y,fx,fy;									
	char filter_name[256],BMP_INPUT[256],BMP_OUTPUT[256],Rvector_name[256];												
	FILE *fp;


	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
			output_bmp_image[x][y]=0;	//�ǉ�2
			output_bmp_flag[x][y]=0;	//�ǉ�3
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvector�̔ԍ��̃J�E���g
	//////////////////////////////////////���͉摜�̓ǂݍ���//////////////////////////////////////
	//���͉摜�̃t�@�C���l�[�����擾
	wsprintf(BMP_INPUT,  "%s",InputImage);	//API�̊֐�
	//BMP��hBuffer�ɓǂݍ���
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBuffer�̒l����ʂɕ\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	


	//��ʂ̐F(hBuffer)���擾���ă������Ɋi�[
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //API�̊֐�
			//printf("%d,",GetBValue(colorref));		//��̓I�ɓǂݍ���ł����f�l�̃e�X�g
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//API�̊֐�
			
		}
	}
	
	//////////////////////////////////���͉摜�̓ǂݍ��ݏI���/////////////////////////////////////
	/*
	//test16-1207

	char Filename_test[64];
	sprintf(Filename_test,"..\\inputimage.csv");
	if((fp=fopen(Filename_test,"w"))==NULL){exit(1);}

	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",input_bmp[x][y]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	
	*/

	/////////////�t�B���^�ǂݍ���/////////////////	
	wsprintf(filter_name,"%s",Filter1);	//API�̊֐�
	if((fp=fopen(filter_name,"r"))==NULL){exit(1);}
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////�t�B���^�ǂݍ��ݏI���////////////



	////////////////////�t�B���^���Z
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<=max_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<=max_y )
					{
						output1[x][y]+=spfil1[fx][fy]*input_bmp[x+(fx-hfs)][y+(fy-hfs)];
					}
				}			
			}
			output1[x][y]=output1[x][y]*magnification;
		}
	}
	////////////////////�t�B���^���Z�I���//////////////////////


	//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}


		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b0.bmp");	//API�̊֐�
	
	if(Save_image_flag[1][1]==1){
		sprintf(outputbmp_directory,"%sb0.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------�t�@�C���ւ̏�������--------------------------//
	if((fp=fopen(Filename1,"w"))==NULL){exit(1);}
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",output1[x][y]);
			

			if(Rvector_create==1){
				if(y==Rvector_pointY){
					if(x==Rvector_pointX){
						Rvector[Rvector_number]=output1[x][y];
					}
				}
			}
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	////////////////////////�t�@�C���ւ̏������ݏI���////////////////////////
	
	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvector�̔ԍ��̃J�E���g
	//////////////////////////////////////���͉摜�̓ǂݍ���//////////////////////////////////////
	//���͉摜�̃t�@�C���l�[�����擾
	wsprintf(BMP_INPUT,  "%s",InputImage);	//API�̊֐�
	//BMP��hBuffer�ɓǂݍ���
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBuffer�̒l����ʂɕ\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//��ʂ̐F(hBuffer)���擾���ă������Ɋi�[
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //API�̊֐�
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//API�̊֐�
		}
	}
	//////////////////////////////////���͉摜�̓ǂݍ��ݏI���/////////////////////////////////////


	/////////////�t�B���^�ǂݍ���/////////////////	
	wsprintf(filter_name,"%s",Filter2);	//API�̊֐�
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////�t�B���^�ǂݍ��ݏI���////////////



	////////////////////�t�B���^���Z
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<=max_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<=max_y )
					{
						output1[x][y]+=spfil1[fx][fy]*input_bmp[x+(fx-hfs)][y+(fy-hfs)];
					}
				}			
			}
			output1[x][y]=output1[x][y]*magnification;
		}
	}
	////////////////////�t�B���^���Z�I���//////////////////////


	//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b45.bmp");	//API�̊֐�
	if(Save_image_flag[1][2]==1){
	sprintf(outputbmp_directory,"%sb45.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}
	
	//------------------------�t�@�C���ւ̏�������--------------------------//
	if((fp=fopen(Filename2,"w"))==NULL){exit(1);}
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",output1[x][y]);
			
			if(Rvector_create==1){
				if(y==Rvector_pointY){
					if(x==Rvector_pointX){
						Rvector[Rvector_number]=output1[x][y];
					}
				}
			}
	
	
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	////////////////////////�t�@�C���ւ̏������ݏI���////////////////////////
	
	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvector�̔ԍ��̃J�E���g
	//////////////////////////////////////���͉摜�̓ǂݍ���//////////////////////////////////////
	//���͉摜�̃t�@�C���l�[�����擾
	wsprintf(BMP_INPUT,   "%s",InputImage);	//API�̊֐�
	//BMP��hBuffer�ɓǂݍ���
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBuffer�̒l����ʂɕ\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//��ʂ̐F(hBuffer)���擾���ă������Ɋi�[
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //API�̊֐�
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//API�̊֐�
		}
	}
	//////////////////////////////////���͉摜�̓ǂݍ��ݏI���/////////////////////////////////////


	/////////////�t�B���^�ǂݍ���/////////////////	
	wsprintf(filter_name,"%s",Filter3);	//API�̊֐�
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////�t�B���^�ǂݍ��ݏI���////////////



	////////////////////�t�B���^���Z
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<=max_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<=max_y )
					{
						output1[x][y]+=spfil1[fx][fy]*input_bmp[x+(fx-hfs)][y+(fy-hfs)];
					}
				}			
			}
			output1[x][y]=output1[x][y]*magnification;
		}
	}
	////////////////////�t�B���^���Z�I���//////////////////////


	//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b90.bmp");	//API�̊֐�
	if(Save_image_flag[1][3]==1){
	sprintf(outputbmp_directory,"%sb90.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------�t�@�C���ւ̏�������--------------------------//
	if((fp=fopen(Filename3,"w"))==NULL){exit(1);}
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",output1[x][y]);
			
			if(Rvector_create==1){
				if(y==Rvector_pointY){
					if(x==Rvector_pointX){
						Rvector[Rvector_number]=output1[x][y];
					}
				}
			}
	
	
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	////////////////////////�t�@�C���ւ̏������ݏI���////////////////////////
	
	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvector�̔ԍ��̃J�E���g
	//////////////////////////////////////���͉摜�̓ǂݍ���//////////////////////////////////////
	//���͉摜�̃t�@�C���l�[�����擾
	wsprintf(BMP_INPUT,   "%s",InputImage);	//API�̊֐�
	//BMP��hBuffer�ɓǂݍ���
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBuffer�̒l����ʂɕ\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//��ʂ̐F(hBuffer)���擾���ă������Ɋi�[
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //API�̊֐�
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//API�̊֐�
		}
	}
	//////////////////////////////////���͉摜�̓ǂݍ��ݏI���/////////////////////////////////////


	/////////////�t�B���^�ǂݍ���/////////////////	
	wsprintf(filter_name,"%s",Filter4);	//API�̊֐�
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////�t�B���^�ǂݍ��ݏI���////////////



	////////////////////�t�B���^���Z
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<=max_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<=max_y )
					{
						output1[x][y]+=spfil1[fx][fy]*input_bmp[x+(fx-hfs)][y+(fy-hfs)];
					}
				}			
			}
			output1[x][y]=output1[x][y]*magnification;
		}
	}
	////////////////////�t�B���^���Z�I���//////////////////////


	//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b135.bmp");	//API�̊֐�
	if(Save_image_flag[1][4]==1){
	sprintf(outputbmp_directory,"%sb135.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------�t�@�C���ւ̏�������--------------------------//
	if((fp=fopen(Filename4,"w"))==NULL){exit(1);}
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",output1[x][y]);

			if(Rvector_create==1){
				if(y==Rvector_pointY){
					if(x==Rvector_pointX){
						Rvector[Rvector_number]=output1[x][y];
					}
				}
			}
	
	
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	////////////////////////�t�@�C���ւ̏������ݏI���////////////////////////
	
	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvector�̔ԍ��̃J�E���g
	//////////////////////////////////////���͉摜�̓ǂݍ���//////////////////////////////////////
	//���͉摜�̃t�@�C���l�[�����擾
	wsprintf(BMP_INPUT,  "%s",InputImage);	//API�̊֐�
	//BMP��hBuffer�ɓǂݍ���
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBuffer�̒l����ʂɕ\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//��ʂ̐F(hBuffer)���擾���ă������Ɋi�[
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //API�̊֐�
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//API�̊֐�
		}
	}
	//////////////////////////////////���͉摜�̓ǂݍ��ݏI���/////////////////////////////////////


	/////////////�t�B���^�ǂݍ���/////////////////	
	wsprintf(filter_name,"%s",Filter5);	//API�̊֐�
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////�t�B���^�ǂݍ��ݏI���////////////



	////////////////////�t�B���^���Z
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<=max_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<=max_y )
					{
						output1[x][y]+=spfil1[fx][fy]*input_bmp[x+(fx-hfs)][y+(fy-hfs)];
					}
				}			
			}
			output1[x][y]=output1[x][y]*magnification;
		}
	}
	////////////////////�t�B���^���Z�I���//////////////////////


	//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b180.bmp");	//API�̊֐�
	if(Save_image_flag[1][5]==1){
	sprintf(outputbmp_directory,"%sb180.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------�t�@�C���ւ̏�������--------------------------//
	if((fp=fopen(Filename5,"w"))==NULL){exit(1);}
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",output1[x][y]);
	
			if(Rvector_create==1){
				if(y==Rvector_pointY){
					if(x==Rvector_pointX){
						Rvector[Rvector_number]=output1[x][y];
					}
				}
			}
	
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	////////////////////////�t�@�C���ւ̏������ݏI���////////////////////////
	
	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvector�̔ԍ��̃J�E���g
	//////////////////////////////////////���͉摜�̓ǂݍ���//////////////////////////////////////
	//���͉摜�̃t�@�C���l�[�����擾
	wsprintf(BMP_INPUT,  "%s",InputImage);	//API�̊֐�
	//BMP��hBuffer�ɓǂݍ���
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBuffer�̒l����ʂɕ\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//��ʂ̐F(hBuffer)���擾���ă������Ɋi�[
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //API�̊֐�
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//API�̊֐�
		}
	}
	//////////////////////////////////���͉摜�̓ǂݍ��ݏI���/////////////////////////////////////


	/////////////�t�B���^�ǂݍ���/////////////////	
	wsprintf(filter_name,"%s",Filter6);	//API�̊֐�
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////�t�B���^�ǂݍ��ݏI���////////////



	////////////////////�t�B���^���Z
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<=max_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<=max_y )
					{
						output1[x][y]+=spfil1[fx][fy]*input_bmp[x+(fx-hfs)][y+(fy-hfs)];
					}
				}			
			}
			output1[x][y]=output1[x][y]*magnification;
		}
	}
	////////////////////�t�B���^���Z�I���//////////////////////


	//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b225.bmp");	//API�̊֐�
	if(Save_image_flag[1][6]==1){
	sprintf(outputbmp_directory,"%sb225.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------�t�@�C���ւ̏�������--------------------------//
	if((fp=fopen(Filename6,"w"))==NULL){exit(1);}
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",output1[x][y]);
	
			if(Rvector_create==1){
				if(y==Rvector_pointY){
					if(x==Rvector_pointX){
						Rvector[Rvector_number]=output1[x][y];
					}
				}
			}
	
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	////////////////////////�t�@�C���ւ̏������ݏI���////////////////////////
	
	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvector�̔ԍ��̃J�E���g
	//////////////////////////////////////���͉摜�̓ǂݍ���//////////////////////////////////////
	//���͉摜�̃t�@�C���l�[�����擾
	wsprintf(BMP_INPUT,  "%s",InputImage);	//API�̊֐�
	//BMP��hBuffer�ɓǂݍ���
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBuffer�̒l����ʂɕ\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//��ʂ̐F(hBuffer)���擾���ă������Ɋi�[
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //API�̊֐�
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//API�̊֐�
		}
	}
	//////////////////////////////////���͉摜�̓ǂݍ��ݏI���/////////////////////////////////////


	/////////////�t�B���^�ǂݍ���/////////////////	
	wsprintf(filter_name,"%s",Filter7);	//API�̊֐�
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////�t�B���^�ǂݍ��ݏI���////////////



	////////////////////�t�B���^���Z
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<=max_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<=max_y )
					{
						output1[x][y]+=spfil1[fx][fy]*input_bmp[x+(fx-hfs)][y+(fy-hfs)];
					}
				}			
			}
			output1[x][y]=output1[x][y]*magnification;
		}
	}
	////////////////////�t�B���^���Z�I���//////////////////////


	//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b270.bmp");	//API�̊֐�
	if(Save_image_flag[1][7]==1){
	sprintf(outputbmp_directory,"%sb270.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------�t�@�C���ւ̏�������--------------------------//
	if((fp=fopen(Filename7,"w"))==NULL){exit(1);}
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",output1[x][y]);
	
			if(Rvector_create==1){
				if(y==Rvector_pointY){
					if(x==Rvector_pointX){
						Rvector[Rvector_number]=output1[x][y];
					}
				}
			}
	
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	////////////////////////�t�@�C���ւ̏������ݏI���////////////////////////
	
	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvector�̔ԍ��̃J�E���g
	//////////////////////////////////////���͉摜�̓ǂݍ���//////////////////////////////////////
	//���͉摜�̃t�@�C���l�[�����擾
	wsprintf(BMP_INPUT,  "%s",InputImage);	//API�̊֐�
	//BMP��hBuffer�ɓǂݍ���
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBuffer�̒l����ʂɕ\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//��ʂ̐F(hBuffer)���擾���ă������Ɋi�[
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //API�̊֐�
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//API�̊֐�
		}
	}
	//////////////////////////////////���͉摜�̓ǂݍ��ݏI���/////////////////////////////////////


	/////////////�t�B���^�ǂݍ���/////////////////	
	wsprintf(filter_name,"%s",Filter8);	//API�̊֐�
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////�t�B���^�ǂݍ��ݏI���////////////



	////////////////////�t�B���^���Z
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<=max_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<=max_y )
					{
						output1[x][y]+=spfil1[fx][fy]*input_bmp[x+(fx-hfs)][y+(fy-hfs)];
					}
				}			
			}
			output1[x][y]=output1[x][y]*magnification;
		}
	}
	////////////////////�t�B���^���Z�I���//////////////////////


	//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b315.bmp");	//API�̊֐�
	if(Save_image_flag[1][8]==1){
	sprintf(outputbmp_directory,"%sb315.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------�t�@�C���ւ̏�������--------------------------//
	if((fp=fopen(Filename8,"w"))==NULL){exit(1);}
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			fprintf(fp,"%lf,",output1[x][y]);
	
			if(Rvector_create==1){
				if(y==Rvector_pointY){
					if(x==Rvector_pointX){
						Rvector[Rvector_number]=output1[x][y];
					}
				}
			}
	
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	////////////////////////�t�@�C���ւ̏������ݏI���////////////////////////

//////////////////////�ŏI�摜�o��////////////////////////////////////////////////////////////////////
		//hBuffer�ɉ��Z���ʂ�`��
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			//if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
			//	SetPixel(hBuffer,	x+10,	y+350,
			//		RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//API�̊֐�
			//}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output_bmp_flag[x][y]==1){
				if(use_underthreshold_flag==1){
					SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//API�̊֐�
				}else{
					SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//API�̊֐�
				}
			}

		}
	}	

	//hBuffer�̉摜��\��
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBuffer�̒l����ʂɕ\��

	//hBuffer�̉摜��ۑ�
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//API�̊֐�
//	CreateDirectory(BMP_OUTPUT, NULL);			//API�̊֐�
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b315.bmp");	//API�̊֐�
	if(Save_image_flag[1][0]==1){

	if(use_upperthreshold_flag==1)sprintf(outputbmp_directory,"%sb_upperthreshold.bmp",date_directory);
	if(use_underthreshold_flag==1)sprintf(outputbmp_directory,"%sb_underthreshold.bmp",date_directory);
	if(use_upperthreshold_flag==1 && use_underthreshold_flag==1)sprintf(outputbmp_directory,"%sb_all.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//API�̊֐�
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	//�m�ۂ���������������������
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}
	
///////////////////////////////////��x�N�g���̍쐬���ʃt�@�C���̍쐬/////////////////////////////////////////////////////

	if(Rvector_create==1){
			if((fp=fopen(Rvector_Filename,"w"))==NULL){exit(1);}
			Rvector_number=0;
			for(Rvector_number=1;Rvector_number<=8;Rvector_number++){
				fprintf(fp,"%lf,",Rvector[Rvector_number]);
			}
			fclose(fp);
	}else{

	}

//////////////////////////////opencv��p������ݍ��݉摜�̍쐬///////////////////////////////////////////////////////////////////////////////////

	
	if(Save_image_flag[2][0]==1){
		for(int make_image_repeat=1;make_image_repeat<=8;++make_image_repeat){
		make_bmp(date_directory,Filename1,image_x,image_y,make_image_repeat,Save_image_flag);
		}
	}

//////////////////////////////log�̍쐬///////////////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];
	//sprintf(filename_log, "..\\log\\log-%2d-%02d%02d-%02d%02d%02d.txt",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);	//log�t�@�C���쐬�̃f�B���N�g���w��
	sprintf(filename_log, "..\\result_usa\\%2d-%02d%02d-%02d%02d%02d\\log.txt",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);	//log�t�@�C���쐬�̃f�B���N�g���w��
	if((fp_date=fopen(filename_log,"w"))==NULL){printf("log�t�@�C�����J���܂���");exit(1);}
	fprintf(fp_date,"Time : %s",date);								//����
	fprintf(fp_date,"Rvector_create=%d\n",Rvector_create);			//Rvector�̍쐬�������ǂ���
	if(Rvector_create==1)fprintf(fp_date,"Rvector_Coordinate�FX=%d�CY=%d\n",Rvector_pointX,Rvector_pointY);
	fprintf(fp_date,"ImputImage=%s\n",InputImage);					//���͉摜
	fprintf(fp_date,"ImputImage_size�FX=%d�CY=%d\n",image_x, image_y);
	fprintf(fp_date,"Upper_threshold : %f, Under_threshold : %f\n",Upper_threshold,Under_threshold);
	fprintf(fp_date,"use_upperthreshold_flag : %d, use_underthreshold_flag : %d\n",use_upperthreshold_flag,use_underthreshold_flag);
	fprintf(fp_date,"Offset : %d\n",Offset);						//��ݍ��݂̍ۂ�offset
	fprintf(fp_date,"Kernel\n");									//�g�p�����J�[�l���̋L�^
	fprintf(fp_date,"Kernel_size=%d\n",fs);
	fprintf(fp_date,"V0   = %s\n",Filter1);
	fprintf(fp_date,"V45  = %s\n",Filter2);
	fprintf(fp_date,"V90  = %s\n",Filter3);
	fprintf(fp_date,"V135 = %s\n",Filter4);
	fprintf(fp_date,"V180 = %s\n",Filter5);
	fprintf(fp_date,"V225 = %s\n",Filter6);
	fprintf(fp_date,"V270 = %s\n",Filter7);
	fprintf(fp_date,"V315 = %s\n",Filter8);
	fclose(fp_date);
	printf("log�t�@�C�� %s ��ۑ����܂���\n",filename_log);
			

	//�������̊J��
	free_matrix(input_bmp, 0, max_x, 0, max_y);
	free_matrix(output1,0, max_x, 0, max_y);
	free_matrix(spfil1,  1,  fs,  1,  fs);
	

}