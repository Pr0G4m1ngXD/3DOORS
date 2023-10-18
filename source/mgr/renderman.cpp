#include "renderman.h"
#include <3ds.h>
#include <citro3d.h>
#include <vshader_shbin.h>
#include <stdio.h>
#include "define.h"

#define M_PI 3.14159265358979323846

#define CLEAR_COLOR 0x110221FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))


// Called when the system is ready to initialize proper graphics
void RenderManager::readyFunction()
{
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	// Initialize the render target
	target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);


    // Load the vertex shader, create a shader program and bind it
	globalVshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&globalProgram);
	shaderProgramSetVsh(&globalProgram, &globalVshader_dvlb->DVLE[0]);

	// Get the location of the uniforms
	uLoc_projection   = shaderInstanceGetUniformLocation(globalProgram.vertexShader, "projection");
	uLoc_modelView    = shaderInstanceGetUniformLocation(globalProgram.vertexShader, "modelView");
	uLoc_lightVec     = shaderInstanceGetUniformLocation(globalProgram.vertexShader, "lightVec");
	uLoc_lightHalfVec = shaderInstanceGetUniformLocation(globalProgram.vertexShader, "lightHalfVec");
	uLoc_lightClr     = shaderInstanceGetUniformLocation(globalProgram.vertexShader, "lightClr");
	uLoc_material     = shaderInstanceGetUniformLocation(globalProgram.vertexShader, "material");


    AttrInfo_Init(&attrInf);
    AttrInfo_AddLoader(&attrInf, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(&attrInf, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(&attrInf, 2, GPU_FLOAT, 3); // v2=normal

	BufInfo_Init(&bufferInf);

    Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(70.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, false);
}

void RenderManager::render()
{
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C3D_RenderTargetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
	C3D_FrameDrawOn(target);

    consoleClear();
	frameNum++;
	printf("framecount: %i \n", frameNum);
	#ifdef MEMDEBUG
	    printf("linearfree: %lu MB\n", linearSpaceFree() / 1048576); // \x1b[10;1H
	#endif
	// Render Objects



	C3D_FrameEnd(0);
}

void RenderManager::done()
{
    // Free the texture
	//C3D_TexDelete(&kitten_tex);

	// Free the shader program
	shaderProgramFree(&globalProgram);
	DVLB_Free(globalVshader_dvlb);

    C3D_Fini();
}
