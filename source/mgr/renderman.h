// Implementing render functions
class RenderManager 
{
protected:
    /* data */
    int uLoc_projection, uLoc_modelView;
    int uLoc_lightVec, uLoc_lightHalfVec, uLoc_lightClr, uLoc_material;
    DVLB_s* globalVshader_dvlb;
    shaderProgram_s globalProgram;
    C3D_BufInfo bufferInf;
    C3D_AttrInfo attrInf;
    C3D_Mtx projection;
    C3D_RenderTarget* target;
    int frameNum = 0;
public:
    void readyFunction();
    void render();
    void done();
    RenderManager(/* args */);
    ~RenderManager();
};