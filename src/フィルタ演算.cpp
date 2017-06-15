
#include <windows.h>					//ウィンドウを使うプログラムには必須

//メモリ確保を行うためのヘッダ
#define ANSI				
#include "nrutil.h"				
#include<stdio.h>
#include<math.h>

#include "filter.h"

#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //文字ストリーム

#include<time.h>//時間を用いる

#include <direct.h>//フォルダを作成する


using namespace std;



void Space_filter(HWND hWnd,  HDC hBuffer,BITMAP bitmap){

	COLORREF colorref;	//カラーリファレンス
	
	//実行日時の表示
	time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
	char date[128]="";
	sprintf(date,"%2d/%2d/%2d - %d:%d:%d\n",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
	printf(date);


 ///////////////////////////////初期設定1 : 入力画像指定//////////////////////////////////////////////////////////////////////////////////////
	
	//基準ベクトルを取得するときはこちら
	int filter_number=315;								//フィルタ演算，基準ベクトル作成するときのみ指定．ここを変更する					
/*	char propety_dire_char[128];
	sprintf(propety_dire_char,"..\\property_usa\\simulation17-0203\\Rvector\\8dire_100k\\property_8dire_100k_I~%d.txt",filter_number);	
	std::ifstream propety_dire(propety_dire_char);
	*/
	//通常時はこちら
	std::ifstream propety_dire("..\\property_usa\\simulation17-0615_circle-1\\property_10k_conv_sd0.txt");		//ここだけ入力すればよい.基準ベクトル作成の際は上を変更する

	

	//閾値の設定．今後property.txtに加えるかも
	int Offset=128;
	double magnification=1;
	float Upper_threshold=35;
	float Under_threshold=-35;
	int use_upperthreshold_flag=1;
	int use_underthreshold_flag=1;
	int Save_image_flag[9][9];

	//画像作成判断の初期化(デフォルトでは作成しない)
	for(int i=0;i<9;++i){
	Save_image_flag[1][i]=0;
	Save_image_flag[2][i]=0;
	}

	//多値画像を作成する
	Save_image_flag[2][0]=1;		//多値画像を作成するとき1
/*
	//4方向に画像を作成する
	for(int i=1;i<=4;++i){
		int I=2*i-1;
	Save_image_flag[2][I]=1;
	}
*/
	
	//8方向に画像を作成する
	for(int i=1;i<=8;++i){
		
	Save_image_flag[2][i]=1;
	}
		
	int max_x, max_y;		//画像サイズ-1
	int image_x, image_y;		//画像サイズ
	int Rvector_create;		//1なら基準ベクトルを作る
	int Rvector_pointX;		//基準ベクトルを作成する位置.0をスタートで考える
	int Rvector_pointY;		//特殊な指定をする場合は下へ
	int fs;					//フィルタの大きさ(奇数)
	int All_property[10];	//上記の情報を読み込む用

	char propety[255];
	char Allpropety[255];
	char outputrvector_directory[128];				//Rvectorの場所
	char inputimage_directory[128];
	char inputdate_directory1[255];
	char inputdate_directory2[255];
	char inputdate_directory3[255];
	char inputdate_directory4[255];
	char inputdate_directory5[255];
	char inputdate_directory6[255];	
	char inputdate_directory7[255];
	char inputdate_directory8[255];			//繰返し数を増やしたい1/4
	int count_property=0;
	int count_Allproperty=0;


	//プロパティtxtファイルの読み込み
    if (propety_dire.fail())
    {
        printf("propertyテキストを読み取ることができません\n");
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

	//プロパティtxtファイルの一行目を文字列から数値に変換して関数に代入
	std::string tmp_Allpropety;
	std::istringstream stream_Allpropety(Allpropety);
	while(getline(stream_Allpropety,tmp_Allpropety,',')){

		int All_tmp_property=stof(tmp_Allpropety); //stof(string str) : stringをintに変換
		All_property[count_Allproperty]=All_tmp_property;


////////////////property.txtの一行目///////////////////////////////////////////////////////////
		if(count_Allproperty==0){image_x=All_property[count_Allproperty];max_x=image_x-1;}			//image_x:画像の横サイズ,max_x:画像の横サイズ-1
		if(count_Allproperty==1){image_y=All_property[count_Allproperty];max_y=image_y-1;}			//image_y:画像の縦サイズ,max_y:画像の縦サイズ-1
		if(count_Allproperty==2)Rvector_create=All_property[count_Allproperty];		//Rvector_create:1なら基準ベクトルを作成．この場合初期設定2を設定すること
		if(count_Allproperty==3)Rvector_pointX=All_property[count_Allproperty];		//Rvector_pointX:基準ベクトル取得X座標．0からスタート
		if(count_Allproperty==4)Rvector_pointY=All_property[count_Allproperty];		//Rvector_pointY:基準ベクトル取得Y座標．0からスタート
		if(count_Allproperty==5)fs=All_property[count_Allproperty];					//fs:フィルタサイズ（奇数）

		++count_Allproperty;
	}
	
	char InputImage[64];						//入力画像
	sprintf(InputImage,inputimage_directory);	//propertyから読み取った入力画像情報を代入



	 
///////////////////////////////初期設定2 : 基準ベクトル作成か否か(修正をしなければならない）/////////////////////////////////////////////////////////////

	
	//if(Rvector_create==1)sprintf(InputImage, "..\\bmp\\255-0\\%d.bmp", filter_number);	//Rvectorを作成する際の入力画像指定.
	

	char Rvector_directory[128];
	double Rvector[9];											//基準ベクトルの数+1
	sprintf(Rvector_directory,outputrvector_directory,propety);		//Rvectorを保存するフォルダ名を指定.property.txtの2行目

	
	//Rvectorを保存するフォルダの作成
	printf("Rvector_create=%d\n",Rvector_create);
	if(Rvector_create==1){


		if(_mkdir(Rvector_directory)==0){
			printf("Rvecor保存フォルダ %s を作成しました\n",Rvector_directory);
		}else{
			printf("Rvecor保存フォルダ %s は作成済みです\n",Rvector_directory);
		}
	}
//////////////////////////初期設定の確認用//////////////////////////////////////////////////////////////////////////////

	if(Rvector_create==1)printf("基準ベクトル取得座標：X=%d，Y=%d\n",Rvector_pointX,Rvector_pointY);
	printf("InputImage=%s\n",InputImage);
	printf("画像サイズ：X=%d，Y=%d\n",image_x, image_y);
	printf("fs=%d\n",fs);
	printf("Offset=%d\n",Offset);
	printf("Upper_threshold : %f, Under_threshold : %f\n",Upper_threshold,Under_threshold);
	printf("use_upperthreshold_flag : %d, use_underthreshold_flag : %d\n",use_upperthreshold_flag,use_underthreshold_flag);

////////////////////////////初期設定3 : フィルタ名指定////////////////////////////////////////////////////////////////////
	
	int hfs=(fs+1)/2;			//フィルタの大きさの半分

//property.txtでの指定
	char Filter1[127],Filter2[127],Filter3[127],Filter4[127],Filter5[127],Filter6[127],Filter7[127],Filter8[127];
	sprintf(Filter1,inputdate_directory1);
	sprintf(Filter2,inputdate_directory2);
	sprintf(Filter3,inputdate_directory3);
	sprintf(Filter4,inputdate_directory4);
	sprintf(Filter5,inputdate_directory5);
	sprintf(Filter6,inputdate_directory6);
	sprintf(Filter7,inputdate_directory7);
	sprintf(Filter8,inputdate_directory8);

//手動指定バックアップ
	/*
	int fs=15;					//フィルタの大きさ(奇数)
	int hfs=(fs+1)/2;			//フィルタの大きさの半分
	char Filter1[]="..\\フィルタ\\15×15回路カーネル\\4方向Rs=1k\\V(0).txt";
	char Filter2[]="..\\フィルタ\\15×15回路カーネル\\4方向Rs=1k\\V(45).txt";
	char Filter3[]="..\\フィルタ\\15×15回路カーネル\\4方向Rs=1k\\V(90).txt";
	char Filter4[]="..\\フィルタ\\15×15回路カーネル\\4方向Rs=1k\\V(135).txt";
	char Filter5[]="..\\フィルタ\\15×15回路カーネル\\4方向Rs=1k\\V(180).txt";
	char Filter6[]="..\\フィルタ\\15×15回路カーネル\\4方向Rs=1k\\V(225).txt";
	char Filter7[]="..\\フィルタ\\15×15回路カーネル\\4方向Rs=1k\\V(270).txt";
	char Filter8[]="..\\フィルタ\\15×15回路カーネル\\4方向Rs=1k\\V(315).txt";
	*/
	//sobel
	/*
	int fs=3;					//フィルタの大きさ(奇数)
	int hfs=(fs+1)/2;			//フィルタの大きさの半分
	char Filter1[]="..\\フィルタ\\sobel\\sobel0.txt";
	char Filter2[]="..\\フィルタ\\sobel\\sobel45.txt";
	char Filter3[]="..\\フィルタ\\sobel\\sobel90.txt";
	char Filter4[]="..\\フィルタ\\sobel\\sobel135.txt";
	char Filter5[]="..\\フィルタ\\sobel\\sobel180.txt";
	char Filter6[]="..\\フィルタ\\sobel\\sobel225.txt";
	char Filter7[]="..\\フィルタ\\sobel\\sobel270.txt";
	char Filter8[]="..\\フィルタ\\sobel\\sobel315.txt";
	*/

////////////////////////////初期設定4 : 出力結果のファイル名の指定//////////////////////////////////////////////////////////////////////////////////
	
	//出力結果のファイル名の指定
	char *Filename1_s = "V(0).csv";
	char *Filename2_s = "V(45).csv";
	char *Filename3_s = "V(90).csv";
	char *Filename4_s = "V(135).csv";
	char *Filename5_s = "V(180).csv";
	char *Filename6_s = "V(225).csv";
	char *Filename7_s = "V(270).csv";
	char *Filename8_s = "V(315).csv";


	//結果を保存するフォルダの生成
	//フォルダ名は実行日時になる
	char date_directory[128];
	char outputbmp_directory[128];
	sprintf(date_directory,"..\\result_usa\\%2d-%02d%02d-%02d%02d%02d\\",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
	 if(_mkdir(date_directory)==0){
        printf("フォルダ %s を作成しました\n",date_directory);
    }else{
        printf("フォルダ作成に失敗しました。\n");
    }
	//出力ファイル名・出力先の設定
	char Filename1[64];
	char Filename2[64];
	char Filename3[64];
	char Filename4[64];
	char Filename5[64];
	char Filename6[64];
	char Filename7[64];
	char Filename8[64];

	//文字列の結合
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

	
	printf("使用したカーネルは\n");
	printf("V0   = %s\n",Filter1);
	printf("V45  = %s\n",Filter2);
	printf("V90  = %s\n",Filter3);
	printf("V135 = %s\n",Filter4);
	printf("V180 = %s\n",Filter5);
	printf("V225 = %s\n",Filter6);
	printf("V270 = %s\n",Filter7);
	printf("V315 = %s\n",Filter8);
	

////////////////////////基準ベクトル作成///////////////////////////////////////////////////////////////////////////////

	char Rvector_Filename[255];
	char *Rvector_Filename_s;
	sprintf(Rvector_Filename,"%s\\Rvector%d.csv",Rvector_directory,filter_number);
	

/////////////////////////基準ベクトル作成位置の特殊指定//////////////////////////////////////////////////////
	//特殊指定跡地


	int Rvector_number=0;				//Rvectorの番号のカウント

////////////////////////畳み込み演算プログラム開始/////////////////////////////////////////////////////////////////////////////////


		//Nrutilを用いたメモリの確保
	double **input_bmp=matrix(0, max_x, 0, max_y);
	double **output1=matrix(0, max_x, 0, max_y);
	double **spfil1=matrix( 1,  fs,  1,  fs);
	double **output_bmp_image=matrix(0, max_x, 0, max_y);	//追加1
	double **output_bmp_flag=matrix(0, max_x, 0, max_y);	//追加3

	int x,y,fx,fy;									
	char filter_name[256],BMP_INPUT[256],BMP_OUTPUT[256],Rvector_name[256];												
	FILE *fp;


	//確保したメモリを初期化する
	for(y=0;y<=max_y;y++){
		for(x=0;x<=max_x;x++){
			input_bmp[x][y]=0;
			output1[x][y]=0;
			output_bmp_image[x][y]=0;	//追加2
			output_bmp_flag[x][y]=0;	//追加3
		}
	}
	for(fy=1;fy<=fs;fy++){
		for(fx=1;fx<=fs;fx++){
			spfil1[fx][fy]=0;
		}
	}


Rvector_number++;					//Rvectorの番号のカウント
	//////////////////////////////////////入力画像の読み込み//////////////////////////////////////
	//入力画像のファイルネームを取得
	wsprintf(BMP_INPUT,  "%s",InputImage);	//APIの関数
	//BMPをhBufferに読み込む
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBufferの値を画面に表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	


	//画面の色(hBuffer)を取得してメモリに格納
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //APIの関数
			//printf("%d,",GetBValue(colorref));		//具体的に読み込んでいる画素値のテスト
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//APIの関数
			
		}
	}
	
	//////////////////////////////////入力画像の読み込み終わり/////////////////////////////////////
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

	/////////////フィルタ読み込み/////////////////	
	wsprintf(filter_name,"%s",Filter1);	//APIの関数
	if((fp=fopen(filter_name,"r"))==NULL){exit(1);}
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////フィルタ読み込み終わり////////////



	////////////////////フィルタ演算
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
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
	////////////////////フィルタ演算終わり//////////////////////


	//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}


		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b0.bmp");	//APIの関数
	
	if(Save_image_flag[1][1]==1){
		sprintf(outputbmp_directory,"%sb0.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------ファイルへの書き込み--------------------------//
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
	////////////////////////ファイルへの書き込み終わり////////////////////////
	
	//確保したメモリを初期化する
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


Rvector_number++;					//Rvectorの番号のカウント
	//////////////////////////////////////入力画像の読み込み//////////////////////////////////////
	//入力画像のファイルネームを取得
	wsprintf(BMP_INPUT,  "%s",InputImage);	//APIの関数
	//BMPをhBufferに読み込む
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBufferの値を画面に表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//画面の色(hBuffer)を取得してメモリに格納
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //APIの関数
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//APIの関数
		}
	}
	//////////////////////////////////入力画像の読み込み終わり/////////////////////////////////////


	/////////////フィルタ読み込み/////////////////	
	wsprintf(filter_name,"%s",Filter2);	//APIの関数
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////フィルタ読み込み終わり////////////



	////////////////////フィルタ演算
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
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
	////////////////////フィルタ演算終わり//////////////////////


	//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b45.bmp");	//APIの関数
	if(Save_image_flag[1][2]==1){
	sprintf(outputbmp_directory,"%sb45.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}
	
	//------------------------ファイルへの書き込み--------------------------//
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
	////////////////////////ファイルへの書き込み終わり////////////////////////
	
	//確保したメモリを初期化する
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


Rvector_number++;					//Rvectorの番号のカウント
	//////////////////////////////////////入力画像の読み込み//////////////////////////////////////
	//入力画像のファイルネームを取得
	wsprintf(BMP_INPUT,   "%s",InputImage);	//APIの関数
	//BMPをhBufferに読み込む
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBufferの値を画面に表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//画面の色(hBuffer)を取得してメモリに格納
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //APIの関数
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//APIの関数
		}
	}
	//////////////////////////////////入力画像の読み込み終わり/////////////////////////////////////


	/////////////フィルタ読み込み/////////////////	
	wsprintf(filter_name,"%s",Filter3);	//APIの関数
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////フィルタ読み込み終わり////////////



	////////////////////フィルタ演算
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
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
	////////////////////フィルタ演算終わり//////////////////////


	//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b90.bmp");	//APIの関数
	if(Save_image_flag[1][3]==1){
	sprintf(outputbmp_directory,"%sb90.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------ファイルへの書き込み--------------------------//
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
	////////////////////////ファイルへの書き込み終わり////////////////////////
	
	//確保したメモリを初期化する
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


Rvector_number++;					//Rvectorの番号のカウント
	//////////////////////////////////////入力画像の読み込み//////////////////////////////////////
	//入力画像のファイルネームを取得
	wsprintf(BMP_INPUT,   "%s",InputImage);	//APIの関数
	//BMPをhBufferに読み込む
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBufferの値を画面に表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//画面の色(hBuffer)を取得してメモリに格納
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //APIの関数
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//APIの関数
		}
	}
	//////////////////////////////////入力画像の読み込み終わり/////////////////////////////////////


	/////////////フィルタ読み込み/////////////////	
	wsprintf(filter_name,"%s",Filter4);	//APIの関数
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////フィルタ読み込み終わり////////////



	////////////////////フィルタ演算
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
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
	////////////////////フィルタ演算終わり//////////////////////


	//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b135.bmp");	//APIの関数
	if(Save_image_flag[1][4]==1){
	sprintf(outputbmp_directory,"%sb135.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------ファイルへの書き込み--------------------------//
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
	////////////////////////ファイルへの書き込み終わり////////////////////////
	
	//確保したメモリを初期化する
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


Rvector_number++;					//Rvectorの番号のカウント
	//////////////////////////////////////入力画像の読み込み//////////////////////////////////////
	//入力画像のファイルネームを取得
	wsprintf(BMP_INPUT,  "%s",InputImage);	//APIの関数
	//BMPをhBufferに読み込む
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBufferの値を画面に表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//画面の色(hBuffer)を取得してメモリに格納
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //APIの関数
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//APIの関数
		}
	}
	//////////////////////////////////入力画像の読み込み終わり/////////////////////////////////////


	/////////////フィルタ読み込み/////////////////	
	wsprintf(filter_name,"%s",Filter5);	//APIの関数
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////フィルタ読み込み終わり////////////



	////////////////////フィルタ演算
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
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
	////////////////////フィルタ演算終わり//////////////////////


	//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b180.bmp");	//APIの関数
	if(Save_image_flag[1][5]==1){
	sprintf(outputbmp_directory,"%sb180.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------ファイルへの書き込み--------------------------//
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
	////////////////////////ファイルへの書き込み終わり////////////////////////
	
	//確保したメモリを初期化する
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


Rvector_number++;					//Rvectorの番号のカウント
	//////////////////////////////////////入力画像の読み込み//////////////////////////////////////
	//入力画像のファイルネームを取得
	wsprintf(BMP_INPUT,  "%s",InputImage);	//APIの関数
	//BMPをhBufferに読み込む
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBufferの値を画面に表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//画面の色(hBuffer)を取得してメモリに格納
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //APIの関数
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//APIの関数
		}
	}
	//////////////////////////////////入力画像の読み込み終わり/////////////////////////////////////


	/////////////フィルタ読み込み/////////////////	
	wsprintf(filter_name,"%s",Filter6);	//APIの関数
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////フィルタ読み込み終わり////////////



	////////////////////フィルタ演算
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
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
	////////////////////フィルタ演算終わり//////////////////////


	//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b225.bmp");	//APIの関数
	if(Save_image_flag[1][6]==1){
	sprintf(outputbmp_directory,"%sb225.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------ファイルへの書き込み--------------------------//
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
	////////////////////////ファイルへの書き込み終わり////////////////////////
	
	//確保したメモリを初期化する
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


Rvector_number++;					//Rvectorの番号のカウント
	//////////////////////////////////////入力画像の読み込み//////////////////////////////////////
	//入力画像のファイルネームを取得
	wsprintf(BMP_INPUT,  "%s",InputImage);	//APIの関数
	//BMPをhBufferに読み込む
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBufferの値を画面に表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//画面の色(hBuffer)を取得してメモリに格納
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //APIの関数
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//APIの関数
		}
	}
	//////////////////////////////////入力画像の読み込み終わり/////////////////////////////////////


	/////////////フィルタ読み込み/////////////////	
	wsprintf(filter_name,"%s",Filter7);	//APIの関数
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////フィルタ読み込み終わり////////////



	////////////////////フィルタ演算
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
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
	////////////////////フィルタ演算終わり//////////////////////


	//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b270.bmp");	//APIの関数
	if(Save_image_flag[1][7]==1){
	sprintf(outputbmp_directory,"%sb270.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------ファイルへの書き込み--------------------------//
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
	////////////////////////ファイルへの書き込み終わり////////////////////////
	
	//確保したメモリを初期化する
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


Rvector_number++;					//Rvectorの番号のカウント
	//////////////////////////////////////入力画像の読み込み//////////////////////////////////////
	//入力画像のファイルネームを取得
	wsprintf(BMP_INPUT,  "%s",InputImage);	//APIの関数
	//BMPをhBufferに読み込む
	ComposeDDB(hWnd,  hBuffer,  BMP_INPUT,  10,  100);
	//hBufferの値を画面に表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);
	
	//画面の色(hBuffer)を取得してメモリに格納
	for(y=0;y<=max_y;y++){	
		for(x=0;x<=max_x;x++){	
			colorref = GetPixel(hBuffer ,x+10,y+100); //APIの関数
			input_bmp[x][y]= ((GetGValue(colorref)+GetRValue(colorref)+GetBValue(colorref))/3-Offset);//APIの関数
		}
	}
	//////////////////////////////////入力画像の読み込み終わり/////////////////////////////////////


	/////////////フィルタ読み込み/////////////////	
	wsprintf(filter_name,"%s",Filter8);	//APIの関数
	if((fp=fopen(filter_name,"r"))==NULL)	exit(1);
		for(fy=1 ; fy<=fs ; fy++){
			for(fx=1 ; fx<=fs ; fx++){
				fscanf(fp, "%lf ", &spfil1[fx][fy]);
			}
		}
	fclose(fp);							
	/////////////フィルタ読み込み終わり////////////



	////////////////////フィルタ演算
	
	//////////////////////
	for(y=0 ; y<=max_y; y++){
		for(x=0; x<=max_x; x++){
			for(fy=1 ; fy<=fs ; fy++){
				for(fx=1 ; fx<=fs ; fx++){
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
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
	////////////////////フィルタ演算終わり//////////////////////


	//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
				SetPixel(hBuffer,	x+10,	y+350,
					RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output1[x][y]>Upper_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				if(use_upperthreshold_flag==1)output_bmp_flag[x][y]=1;
			}
			if(output1[x][y]<Under_threshold ){
				SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				if(use_underthreshold_flag==1)output_bmp_flag[x][y]=1;
			}

		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b315.bmp");	//APIの関数
	if(Save_image_flag[1][8]==1){
	sprintf(outputbmp_directory,"%sb315.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	
	//------------------------ファイルへの書き込み--------------------------//
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
	////////////////////////ファイルへの書き込み終わり////////////////////////

//////////////////////最終画像出力////////////////////////////////////////////////////////////////////
		//hBufferに演算結果を描画
	for(y=0;y<=max_y;	y=y+1){	
		for(x=0; x<=max_x; x=x+1){	
			//if(0<=int(output1[x][y]+Offset) && int(output1[x][y]+Offset)<=255){
			//	SetPixel(hBuffer,	x+10,	y+350,
			//		RGB(int(output1[x][y])+Offset,int(output1[x][y])+Offset,int(output1[x][y])+Offset));//APIの関数
			//}
			
	/*		if(int(output1[x][y]+Offset)>255 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y]+Offset)<0 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
	/*		SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(int(output1[x][y])>20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
			if(int(output1[x][y])<-20 )	SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
	*/
			SetPixel(hBuffer,	x+10,	y+350,	RGB(128,128,128));
			if(output_bmp_flag[x][y]==1){
				if(use_underthreshold_flag==1){
					SetPixel(hBuffer,	x+10,	y+350,	RGB(0,0,0));//APIの関数
				}else{
					SetPixel(hBuffer,	x+10,	y+350,	RGB(255,255,255));//APIの関数
				}
			}

		}
	}	

	//hBufferの画像を表示
	LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示

	//hBufferの画像を保存
//	wsprintf(BMP_OUTPUT,  "..\\bmp");			//APIの関数
//	CreateDirectory(BMP_OUTPUT, NULL);			//APIの関数
	//wsprintf(BMP_OUTPUT,  "..\\bmp\\b315.bmp");	//APIの関数
	if(Save_image_flag[1][0]==1){

	if(use_upperthreshold_flag==1)sprintf(outputbmp_directory,"%sb_upperthreshold.bmp",date_directory);
	if(use_underthreshold_flag==1)sprintf(outputbmp_directory,"%sb_underthreshold.bmp",date_directory);
	if(use_upperthreshold_flag==1 && use_underthreshold_flag==1)sprintf(outputbmp_directory,"%sb_all.bmp",date_directory);
	wsprintf(BMP_OUTPUT,outputbmp_directory);	//APIの関数
	SaveDDB(hWnd,hBuffer, BMP_INPUT,  BMP_OUTPUT,  10,  350);
	}

	//確保したメモリを初期化する
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
	
///////////////////////////////////基準ベクトルの作成結果ファイルの作成/////////////////////////////////////////////////////

	if(Rvector_create==1){
			if((fp=fopen(Rvector_Filename,"w"))==NULL){exit(1);}
			Rvector_number=0;
			for(Rvector_number=1;Rvector_number<=8;Rvector_number++){
				fprintf(fp,"%lf,",Rvector[Rvector_number]);
			}
			fclose(fp);
	}else{

	}

//////////////////////////////opencvを用いた畳み込み画像の作成///////////////////////////////////////////////////////////////////////////////////

	
	if(Save_image_flag[2][0]==1){
		for(int make_image_repeat=1;make_image_repeat<=8;++make_image_repeat){
		make_bmp(date_directory,Filename1,image_x,image_y,make_image_repeat,Save_image_flag);
		}
	}

//////////////////////////////logの作成///////////////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];
	//sprintf(filename_log, "..\\log\\log-%2d-%02d%02d-%02d%02d%02d.txt",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);	//logファイル作成のディレクトリ指定
	sprintf(filename_log, "..\\result_usa\\%2d-%02d%02d-%02d%02d%02d\\log.txt",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);	//logファイル作成のディレクトリ指定
	if((fp_date=fopen(filename_log,"w"))==NULL){printf("logファイルが開けません");exit(1);}
	fprintf(fp_date,"Time : %s",date);								//時間
	fprintf(fp_date,"Rvector_create=%d\n",Rvector_create);			//Rvectorの作成したかどうか
	if(Rvector_create==1)fprintf(fp_date,"Rvector_Coordinate：X=%d，Y=%d\n",Rvector_pointX,Rvector_pointY);
	fprintf(fp_date,"ImputImage=%s\n",InputImage);					//入力画像
	fprintf(fp_date,"ImputImage_size：X=%d，Y=%d\n",image_x, image_y);
	fprintf(fp_date,"Upper_threshold : %f, Under_threshold : %f\n",Upper_threshold,Under_threshold);
	fprintf(fp_date,"use_upperthreshold_flag : %d, use_underthreshold_flag : %d\n",use_upperthreshold_flag,use_underthreshold_flag);
	fprintf(fp_date,"Offset : %d\n",Offset);						//畳み込みの際のoffset
	fprintf(fp_date,"Kernel\n");									//使用したカーネルの記録
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
	printf("logファイル %s を保存しました\n",filename_log);
			

	//メモリの開放
	free_matrix(input_bmp, 0, max_x, 0, max_y);
	free_matrix(output1,0, max_x, 0, max_y);
	free_matrix(spfil1,  1,  fs,  1,  fs);
	

}