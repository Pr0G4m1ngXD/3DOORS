#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>
#include <string.h>
#include <vector>
#include "vshader_shbin.h"
#include "kitten_t3x.h"

#include "define.h"

#include "types.h"

#include "player.h"
#include "gmobj.h"
#include "objman.h"

#define M_PI 3.14159265358979323846

#define CLEAR_COLOR 0x110221FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

static DVLB_s* vshader_dvlb;
static shaderProgram_s program;
static int uLoc_projection, uLoc_modelView;
static int uLoc_lightVec, uLoc_lightHalfVec, uLoc_lightClr, uLoc_material;
static C3D_Mtx projection;
static C3D_Mtx material =
{
	{
	{ { 0.0f, 0.2f, 0.2f, 0.2f } }, // Ambient
	{ { 0.0f, 0.4f, 0.4f, 0.4f } }, // Diffuse
	{ { 0.0f, 0.8f, 0.8f, 0.8f } }, // Specular
	{ { 1.0f, 0.0f, 0.0f, 0.0f } }, // Emission
	}
};

static int frameNum = 0;
static C3D_Tex kitten_tex;
static float viewX = 0.0, viewY = 0.0;
static float sensitivity = 10.0f;
static float sensAccel = 0.00325f;
static float deadzoneThreshold = 0.15f;
static float camStoppingPower = 1.2f;
static int maxInputRange = 0x9C;
static Player player;
static ObjectManager objman;



float clip(float n, float lower, float upper) {
  n = ( n > lower ) * n + !( n > lower ) * lower;
    return ( n < upper ) * n + !( n < upper ) * upper;
}

// Helper function for loading a texture from memory
static bool loadTextureFromMem(C3D_Tex* tex, C3D_TexCube* cube, const void* data, size_t size)
{
	Tex3DS_Texture t3x = Tex3DS_TextureImport(data, size, tex, cube, false);
	if (!t3x)
		return false;

	// Delete the t3x object since we don't need it
	Tex3DS_TextureFree(t3x);
	return true;
}

static void sceneInit(void)
{
	/*
	// Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection   = shaderInstanceGetUniformLocation(program.vertexShader, "projection");
	uLoc_modelView    = shaderInstanceGetUniformLocation(program.vertexShader, "modelView");
	uLoc_lightVec     = shaderInstanceGetUniformLocation(program.vertexShader, "lightVec");
	uLoc_lightHalfVec = shaderInstanceGetUniformLocation(program.vertexShader, "lightHalfVec");
	uLoc_lightClr     = shaderInstanceGetUniformLocation(program.vertexShader, "lightClr");
	uLoc_material     = shaderInstanceGetUniformLocation(program.vertexShader, "material");

	
	// Configure attributes for use with the vertex shader 
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

	// Compute the projection matrix
	
	// Load the texture and bind it to the first texture unit
	
	if (!loadTextureFromMem(&kitten_tex, NULL, kitten_t3x, kitten_t3x_size))
		svcBreak(USERBREAK_PANIC);
	C3D_TexSetFilter(&kitten_tex, GPU_LINEAR, GPU_NEAREST);
	C3D_TexBind(0, &kitten_tex);

	*/
	Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(70.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, false);
}

static void sceneRender(void)
{
	consoleClear();
	frameNum++;
	printf("framecount: %i \n", frameNum);

	#ifdef MEMDEBUG
	printf("linearfree: %lu MB\n", linearSpaceFree() / 1048576); // \x1b[10;1H
	#endif
	/*
			
	
	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView,  &modelView);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_material,   &material);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec,     0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr,     1.0f, 1.0f,  1.0f, 1.0f);


	int offset = 0, objcount = 0;
	for (size_t i = 0; i < objects.size(); i++) { // for loop through vector
    	int objVcount = objects[i].mesh.vertexList.size();
    	if (objVcount > 0) {
			objects[i].mesh.renderBuffer();


			C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, objects[i].mesh.uLoc_projection, &projection);
			C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, objects[i].mesh.uLoc_modelView,  &modelView);
			
        	C3D_DrawArrays(GPU_TRIANGLES, offset, objVcount);
        	offset += objVcount;
    	}
		objcount++;
	}*/

	C3D_Mtx modelView;
	objman.renderAll(&projection,&modelView);

	
	
		
}

static void camRotate(circlePosition cam){
	// Accelerate rotation speed
	viewX += sensAccel * cam.dx;
	viewY += sensAccel * -cam.dy;

	// Smoothly stop Camera if no movement
	if (-deadzoneThreshold < cam.dx && cam.dx < deadzoneThreshold) { viewX = viewX / camStoppingPower; }
	if (-deadzoneThreshold < cam.dy && cam.dy < deadzoneThreshold) { viewY = viewY / camStoppingPower; }

	// Clip rotation speed
	viewX = clip(viewX, -sensitivity, sensitivity);
	viewY = clip(viewY, -sensitivity, sensitivity);

	// Rotate Camera
	player.playerVerticRotation += viewY;

	// Ensure that the vertical rotation is capped between -80.0 and 80.0 degrees
	if (player.playerVerticRotation < -80.0f) {
	    viewY += -80.0f - player.playerVerticRotation;
	    player.playerVerticRotation = -80.0f;
	} else if (player.playerVerticRotation > 80.0f) {
	    viewY += 80.0f - player.playerVerticRotation;
	    player.playerVerticRotation = 80.0f;
	}

	// Actually rotate Camera
	Mtx_RotateY(&projection, C3D_AngleFromDegrees(-player.playerHorizRotation), true); //horizontal
	Mtx_RotateX(&projection, C3D_AngleFromDegrees(viewY), true); //vertical
	player.playerHorizRotation += viewX;
	Mtx_RotateY(&projection, C3D_AngleFromDegrees(player.playerHorizRotation), true); //horizontal
}

static int handleInput(void) {
	// Get Inputs
	u32 kDown = hidKeysHeld(); u32 kDownInstant = hidKeysDown(); // u32 kDownRelease = hidKeysUp();

	if (kDown & KEY_START)
		return 2; // Return to hbmenu
	
	if (kDownInstant & KEY_B)
		player.isCrouching = !player.isCrouching; // Toggle crouching


	#ifdef DEBUGMODE
		if (kDownInstant & KEY_Y){
			Gmobj test;
			test.mesh.objPath = "romfs:/suz2.obj";
			test.init();
			//objects.push_back(std::move(test));


		}
		if (kDownInstant & KEY_A){
			Gmobj test2;
			test2.mesh.objPath = "romfs:/r6doors.obj";
			test2.init();
			//objects.push_back(test2);
		}
		if (kDownInstant & KEY_X) {
			//objects.clear();
		}
	#endif


	// Handle Camera input
	circlePosition camPos;
	hidCstickRead(&camPos);
	camRotate(camPos);

	// Read Circle Pad
	circlePosition cirPos;
	hidCircleRead(&cirPos);

	// Calculate the magnitude of the joystick input
	float normalizedX = cirPos.dx / (float)maxInputRange;
	float normalizedY = cirPos.dy / (float)maxInputRange;
	float magnitude = sqrt(normalizedX * normalizedX + normalizedY * normalizedY);

	// Check if the magnitude exceeds the deadzone threshold
	if (!(magnitude > deadzoneThreshold)) {
	    // Apply the movement only if it's outside the deadzone
		normalizedX = 0; normalizedY = 0;
	}

	// Apply Player Movement
	player.Move(-normalizedX, normalizedY);
	player.Crouch(player.isCrouching);

	// Return 0 (No incident)
	return 0;
}

static void sceneExit(void)
{
	// Free the texture
	C3D_TexDelete(&kitten_tex);

	// Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
}

int main()
{
	// Initialize graphics
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	// Initialize the render target
	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	// Initialize the scene
	sceneInit();

	consoleInit(GFX_BOTTOM, NULL);

	#ifdef DEBUGMODE
	printf("*********************\n*                   *\n*  DEBUGMODE BUILD  *\n*                   *\n*********************\n");
	#endif

	Result rc = romfsInit();
	if (rc) {
		printf("romfsInit: %08lX\n", rc);
		printf("RomFS error: This application cannot    continue as it requires romfs!\n");
		system("PAUSE");
		sceneExit();
		C3D_Fini();
		gfxExit();
		return 1;
	} else {
		printf("romfs Init Successful!\n");
	}

	static Gmobj test5;
	test5.mesh.objPath = "romfs:/suz2.obj";
	objman.addObject(test5);
	
	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		
		if (handleInput() == 2) { break; } // Return to hbmenu if returned 2 (Close App No Function Error)
		
		
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_RenderTargetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
			C3D_FrameDrawOn(target);
			sceneRender();
		C3D_FrameEnd(0);
	}
	//test5.~Gmobj();

	// Deinitialize the scene
	sceneExit();

	// Deinitialize graphics
	C3D_Fini();
	romfsExit();
	gfxExit();
	return 0;
}
