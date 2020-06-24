#ifndef SC_SUITE_H
#define SC_SUITE_H

#include <irrlicht.h>
#include "../../graphics/hpp/graphics_embedded.h"
#include "params.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace gui;
using namespace core;

extern IrrlichtDevice* Idevice;
extern scene::ISceneManager* smgr;
extern video::IVideoDriver* drv;
extern gui::IGUIEnvironment* guienv;

struct CmdReturn
{
	bool done;
	kScorchCparameters prmt;
	float value;
};

class SuiteGui
{
public:
	SuiteGui();
	~SuiteGui();

	void SetupScene(IrrlichtDevice *dev, ISceneManager *sman, IVideoDriver *idrv, IGUIEnvironment *igman);
	void ClearScene(IrrlichtDevice *dev, ISceneManager *sman, IVideoDriver *idrv, IGUIEnvironment *igman);
	void LoadGraphicData(IrrlichtDevice *dev, ISceneManager *sman, IVideoDriver *idrv, IGUIEnvironment *igman);
	CmdReturn Run(IrrlichtDevice *dev, ISceneManager *sman, IVideoDriver *idrv, IGUIEnvironment *igman);

protected:
	video::ITexture* skyaU;
	video::ITexture* skyaD;
	video::ITexture* skyaE;
	video::ITexture* skyaW;
	video::ITexture* skyaN;
	video::ITexture* skyaS;
	scene::ICameraSceneNode* cam;

	ICursorControl* mCon;

	ISceneNode *SkyBox;

	CmdReturn CR;
};

#endif
