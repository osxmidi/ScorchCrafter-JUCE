#include "suite.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace gui;
using namespace core;

SuiteGui::SuiteGui()
{
	CR.done = true;
	CR.prmt = kMasterVol;
	CR.value = 0.f;
}
SuiteGui::~SuiteGui()
{
	// nothing
}
void SuiteGui::LoadGraphicData(IrrlichtDevice *dev, ISceneManager *sman, IVideoDriver *idrv, IGUIEnvironment *igman)
{
	io::IReadFile *gfskyaN = dev->getFileSystem()->createMemoryReadFile(skya_n_jpg, sizeof(skya_n_jpg), "skyaN");
	io::IReadFile *gfskyaS = dev->getFileSystem()->createMemoryReadFile(skya_s_jpg, sizeof(skya_s_jpg), "skyaS");
	io::IReadFile *gfskyaE = dev->getFileSystem()->createMemoryReadFile(skya_e_jpg, sizeof(skya_e_jpg), "skyaE");
	io::IReadFile *gfskyaW = dev->getFileSystem()->createMemoryReadFile(skya_w_jpg, sizeof(skya_w_jpg), "skyaW");
	io::IReadFile *gfskyaU = dev->getFileSystem()->createMemoryReadFile(skya_u_jpg, sizeof(skya_u_jpg), "skyaU");
	io::IReadFile *gfskyaD = dev->getFileSystem()->createMemoryReadFile(skya_d_jpg, sizeof(skya_d_jpg), "skyaD");

	video::IImage *giskyaN = idrv->createImageFromFile(gfskyaN);
	video::IImage *giskyaS = idrv->createImageFromFile(gfskyaS);
	video::IImage *giskyaE = idrv->createImageFromFile(gfskyaE);
	video::IImage *giskyaW = idrv->createImageFromFile(gfskyaW);
	video::IImage *giskyaU = idrv->createImageFromFile(gfskyaU);
	video::IImage *giskyaD = idrv->createImageFromFile(gfskyaD);

	skyaN = idrv->addTexture("skyaN", giskyaN);
	skyaS = idrv->addTexture("skyaS", giskyaS);
	skyaE = idrv->addTexture("skyaE", giskyaE);
	skyaW = idrv->addTexture("skyaW", giskyaW);
	skyaU = idrv->addTexture("skyaU", giskyaU);
	skyaD = idrv->addTexture("skyaD", giskyaD);

	giskyaN->drop();
	giskyaS->drop();
	giskyaE->drop();
	giskyaW->drop();
	giskyaU->drop();
	giskyaD->drop();

	gfskyaN->drop();
	gfskyaS->drop();
	gfskyaE->drop();
	gfskyaW->drop();
	gfskyaU->drop();
	gfskyaD->drop();

	mCon = dev->getCursorControl();
}
void SuiteGui::ClearScene(IrrlichtDevice *dev, ISceneManager *sman, IVideoDriver *idrv, IGUIEnvironment *igman)
{
	sman->clear();
}
void SuiteGui::SetupScene(IrrlichtDevice *dev, ISceneManager *sman, IVideoDriver *idrv, IGUIEnvironment *igman)
{
	cam = sman->addCameraSceneNode();
	cam->setTarget(core::vector3df(0.f,0.f,10.f));

	SkyBox = sman->addSkyBoxSceneNode(skyaU, skyaD, skyaW, skyaE, skyaN, skyaS);
}
CmdReturn SuiteGui::Run(IrrlichtDevice *dev, ISceneManager *sman, IVideoDriver *idrv, IGUIEnvironment *igman)
{
	CR.done = true;

	if (dev->isWindowFocused() == true)
	{
		f32 x = (f32) mCon->getPosition().X;
		f32 y = (f32) mCon->getPosition().Y;

		cam->setTarget(core::vector3df(x, y, 2000.f));
	}

	return CR;
}
