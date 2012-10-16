/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/


#include "HGE\hge.h"
#include "HGE\hgesprite.h"
#include "Math.h"
#include <iostream>

using namespace std;

HGE *hge = 0;


hgeQuad btnQuad;
float x,y;
hgeTriple upL,upR,downL,downR,triLeft,triRight;
float percent=0;



inline void setTiple(hgeTriple *tri,float vx0,float vy0,float vx1,float vy1,float vx2,float vy2,DWORD col=0x7f7f7f7f)
{
    (*tri).v[0].x=vx0;
    (*tri).v[0].y=vy0;
    (*tri).v[1].x=vx1;
    (*tri).v[1].y=vy1;
    (*tri).v[2].x=vx2;
    (*tri).v[2].y=vy2;
    for(int i=0;i<3;i++)
    {
        (*tri).v[i].col=col;
    }
}

//根据冷却进度百分比和所覆盖的按钮的四元数，通过三角面的方式绘制冷却遮罩
//此种方式的问题在于只能用于正方形按钮。
void RenderCoolDownEffect(hgeQuad *btnQuad,float percent)
{

    float midX=((*btnQuad).v[2].x+(*btnQuad).v[0].x)/2;
    float midY=((*btnQuad).v[2].y+(*btnQuad).v[0].y)/2;
    float triH=((*btnQuad).v[2].y-(*btnQuad).v[0].y)/2;
    float triW=((*btnQuad).v[2].x-(*btnQuad).v[0].x)/2;
    //  _ _
    // |\|/|   左边是遮罩层三角面分割后的样子
    // |/|\|   可惜有下划线没有上横线，我知道查下ASCII表就有但我懒得查，U MAD？
    //  - -
    setTiple(&upR,midX,midY,(*btnQuad).v[1].x,(*btnQuad).v[1].y,midX,(*btnQuad).v[1].y,0x7f7f0000);
    setTiple(&upL,midX,midY,midX,(*btnQuad).v[0].y,(*btnQuad).v[0].x,(*btnQuad).v[0].y,0x7f007f00);
    setTiple(&downR,midX,midY,midX,(*btnQuad).v[2].y,(*btnQuad).v[2].x,(*btnQuad).v[2].y,0x7f00007f);
    setTiple(&downL,midX,midY,(*btnQuad).v[3].x,(*btnQuad).v[3].y,midX,(*btnQuad).v[3].y);
    setTiple(&triLeft,midX,midY,(*btnQuad).v[0].x,(*btnQuad).v[0].y,(*btnQuad).v[3].x,(*btnQuad).v[3].y,0x7f7f007f);
    setTiple(&triRight,midX,midY,(*btnQuad).v[2].x,(*btnQuad).v[2].y,(*btnQuad).v[1].x,(*btnQuad).v[1].y,0x7f007f7f);

    //设置三角面的颜色和混合方式

    upR.blend=BLEND_ALPHAADD|BLEND_NOZWRITE;
    downR.blend=BLEND_ALPHAADD|BLEND_NOZWRITE;
    upL.blend=BLEND_ALPHAADD|BLEND_NOZWRITE;
    downL.blend=BLEND_ALPHAADD|BLEND_NOZWRITE;
    triLeft.blend=BLEND_ALPHAADD|BLEND_NOZWRITE;
    triRight.blend=BLEND_ALPHAADD|BLEND_NOZWRITE;
    /*
        hge->Gfx_RenderQuad(btnQuad);
        hge->Gfx_RenderTriple(&upR);
        hge->Gfx_RenderTriple(&upL);
        hge->Gfx_RenderTriple(&downR);
        hge->Gfx_RenderTriple(&downL);
        hge->Gfx_RenderTriple(&triLeft);
        hge->Gfx_RenderTriple(&triRight);
    */

    if(percent==0)
    {
        hge->Gfx_RenderQuad(btnQuad);
        hge->Gfx_RenderTriple(&upR);
        hge->Gfx_RenderTriple(&upL);
        hge->Gfx_RenderTriple(&downR);
        hge->Gfx_RenderTriple(&downL);
        hge->Gfx_RenderTriple(&triLeft);
        hge->Gfx_RenderTriple(&triRight);
    }
    if(percent>0&&percent<13)
    {
        upR.v[2].x=midX+triH*tan(2*M_PI*percent/100.0);
        hge->Gfx_RenderQuad(btnQuad);
        hge->Gfx_RenderTriple(&upR);
        hge->Gfx_RenderTriple(&upL);
        hge->Gfx_RenderTriple(&downR);
        hge->Gfx_RenderTriple(&downL);
        hge->Gfx_RenderTriple(&triLeft);
        hge->Gfx_RenderTriple(&triRight);

    }
    if(percent>=13&&percent<38)
    {
        if(percent<=25)
        {
            triRight.v[2].y=midY-triW*tan(M_PI_4-M_PI*(2*(percent/100.0-0.125)));
        }else
        {
            triRight.v[2].y=midY+triW*tan(M_PI*(2*(percent/100.0-0.25)));
        }

        hge->Gfx_RenderQuad(btnQuad);
        //hge->Gfx_RenderTriple(&upR);
        hge->Gfx_RenderTriple(&upL);
        hge->Gfx_RenderTriple(&downR);
        hge->Gfx_RenderTriple(&downL);
        hge->Gfx_RenderTriple(&triLeft);
        hge->Gfx_RenderTriple(&triRight);

    }
    if(percent>=38&&percent<50)
    {
        downR.v[2].x=midX+triH*tan(M_PI_4-M_PI*(2*(percent/100.0-0.375)));
        hge->Gfx_RenderQuad(btnQuad);
        //hge->Gfx_RenderTriple(&upR);
        hge->Gfx_RenderTriple(&upL);
        hge->Gfx_RenderTriple(&downR);
        hge->Gfx_RenderTriple(&downL);
        hge->Gfx_RenderTriple(&triLeft);
        //hge->Gfx_RenderTriple(&triRight);
    }
    if(percent>=50&&percent<63)
    {
        downL.v[2].x=midX-triH*tan(2*M_PI*(percent/100.0-0.5));
        hge->Gfx_RenderQuad(btnQuad);

        //hge->Gfx_RenderTriple(&upR);
        hge->Gfx_RenderTriple(&upL);
        //hge->Gfx_RenderTriple(&downR);
        hge->Gfx_RenderTriple(&downL);

        hge->Gfx_RenderTriple(&triLeft);
        //hge->Gfx_RenderTriple(&triRight);
    }
    if(percent>=63&&percent<88)
    {
        if(percent<=75)
        {
            triLeft.v[2].y=midY+triW*(M_PI_4-2*M_PI*(percent/100.0-0.625));
        }else
        {

            triLeft.v[2].y=midY-triW*(2*M_PI*(percent/100.0-0.75));
        }
        hge->Gfx_RenderQuad(btnQuad);
        //hge->Gfx_RenderTriple(&upR);
        hge->Gfx_RenderTriple(&upL);
        //hge->Gfx_RenderTriple(&downR);
        //hge->Gfx_RenderTriple(&downL);
        hge->Gfx_RenderTriple(&triLeft);
        //hge->Gfx_RenderTriple(&triRight);
    }
    if(percent<100&&percent>=88)
    {
        upL.v[2].x=midX-triH*tan(M_PI_4-2*M_PI*(percent/100.0-0.875));
        hge->Gfx_RenderQuad(btnQuad);
        //hge->Gfx_RenderTriple(&upR);
        hge->Gfx_RenderTriple(&upL);
        //hge->Gfx_RenderTriple(&downR);
        //hge->Gfx_RenderTriple(&downL);
        //hge->Gfx_RenderTriple(&triLeft);
        //hge->Gfx_RenderTriple(&triRight);
    }
    if(percent==100)
    {
        hge->Gfx_RenderQuad(btnQuad);
    }


}

// This function will be called by HGE once per frame.
// Put your game loop code here. In this example we
// just check whether ESC key has been pressed.
bool FrameFunc()
{
	// By returning "true" we tell HGE
	// to stop running the application.
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
    if (hge->Input_GetKeyState(HGEK_SPACE)) percent=0;


        if((((DWORD)(hge->Timer_GetTime()*10000))%100)==0)
        {
            if(percent<100.0)
            {
                percent+=1;
                cout<<""<<percent<<","<<hge->Timer_GetTime()*10000<<endl;
            }
        }


	// Continue execution
	return false;
}

bool RenderFunc()
{
    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);
    if(percent>=0&&percent<=100)
    {
        RenderCoolDownEffect(&btnQuad,percent);
    }
    //hge->Gfx_RenderQuad(&btnQuad);
    //hge->Gfx_RenderTriple(&upL);
    hge->Gfx_EndScene();
    return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Here we use global pointer to HGE interface.
	// Instead you may use hgeCreate() every
	// time you need access to HGE. Just be sure to
	// have a corresponding hge->Release()
	// for each call to hgeCreate()
	hge = hgeCreate(HGE_VERSION);

	// Set our frame function
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	// Set render function
	hge->System_SetState(HGE_RENDERFUNC,RenderFunc);

	// Set the window title
	hge->System_SetState(HGE_TITLE, "The Effects and Elements of an RPG Game");

	// Run in windowed mode
	// Default window size is 800x600
	hge->System_SetState(HGE_WINDOWED, true);

	// Don't use BASS for sound
	hge->System_SetState(HGE_USESOUND, false);

	// Tries to initiate HGE with the states set.
	// If something goes wrong, "false" is returned
	// and more specific description of what have
	// happened can be read with System_GetErrorMessage().
	if(hge->System_Initiate())
	{
        //初始化按钮和遮罩
        //btnSpr=new hgeSprite(hge->Texture_Load("btn.png"),0,0,64,64);
        //btnSpr->SetHotSpot(32,32);
        //btnSpr->SetColor(SETA(btnSpr->GetColor(),255));
        //btnSpr->SetBlendMode(BLEND_COLORADD|BLEND_ALPHAADD|BLEND_NOZWRITE);
        //btnMask=new hgeSprite(hge->Texture_Load("Mask.png"),0,0,64,64);
        //btnMask->SetHotSpot(32,32);
        //btnMask->SetBlendMode(BLEND_ALPHAADD|BLEND_NOZWRITE);
        //btnMask->SetColor(ARGB(64,128,128,128));


        x=300.0;
        y=300.0;
        btnQuad.tex=hge->Texture_Load("btn.png");

        btnQuad.v[0].x=x-32; btnQuad.v[0].y=y-32;
        btnQuad.v[1].x=x+32; btnQuad.v[1].y=y-32;
        btnQuad.v[2].x=x+32; btnQuad.v[2].y=y+32;
        btnQuad.v[3].x=x-32; btnQuad.v[3].y=y+32;

        btnQuad.blend=BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;

        for(int i=0;i<4;i++)
		{
			// Set up z-coordinate of vertices
			btnQuad.v[i].z=0.5f;
			// Set up color. The format of DWORD col is 0xAARRGGBB
			btnQuad.v[i].col=0xFFFFFFFF;
		}

        btnQuad.v[0].tx=0.0; btnQuad.v[0].ty=0.0;
        btnQuad.v[1].tx=1.0;btnQuad.v[1].ty=0.0;
        btnQuad.v[2].tx=1.0;btnQuad.v[2].ty=1.0;
        btnQuad.v[3].tx=0.0; btnQuad.v[3].ty=1.0;

        //无论我给出的顶点数据是顺时针还是逆时针，都能画出三角形来
        //setTiple(&upL,300,300,300,290,310,300);
		// Starts running FrameFunc().
		// Note that the execution "stops" here
		// until "true" is returned from FrameFunc().
		hge->System_Start();
		hge->Texture_Free(btnQuad.tex);
	}
	else
	{
		// If HGE initialization failed show error message
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	// Now ESC has been pressed or the user
	// has closed the window by other means.

	// Restore video mode and free
	// all allocated resources
	hge->System_Shutdown();

	// Release the HGE interface.
	// If there are no more references,
	// the HGE object will be deleted.
	hge->Release();

	return 0;
}
