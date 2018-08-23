#include "Jx3DEnginePch.h"
#include "SchemeModel.h"

using namespace jx::data_type;

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme {
namespace model {

	SchemeInitModel::SchemeInitModel()
	{
		nRailLineNum = 0;
		fRailLineInteral = 0.0f;
		fRoadWidth = 0.0f;
		fStartLicheng = 0.0f;
		strRoadType = "Railway";
		pt3DCenter = Point3(0, 0, 0);
	}

	SchemeInitModel::~SchemeInitModel()
	{

	}

	SchemeDesignModel::SchemeDesignModel()
	{

	}

	SchemeDesignModel::~SchemeDesignModel()
	{

	}

	SchemeAnalysModel::SchemeAnalysModel()
	{

	}

	SchemeAnalysModel::~SchemeAnalysModel()
	{

	}

	SchemeRenderModel::SchemeRenderModel()
	{
		strRoadbedLeftBianpoTextureFile = "texture/bianpo.jpg";
		strRoadbedRightBianpoTextureFile = "texture/bianpo.jpg";
		strRoadbedPingtaiTextureFile = "texture/pingtai.jpg";
		strRoadbedLujiTextureFile = "texture/luji.jpg";
		strRetainingWallTexture = "texture/dtq.jpg";
		vecRoadbedTypeLeftTexIndex.resize(9);
		vecRoadbedTypeRightTexIndex.resize(9);
	}

	SchemeRenderModel::~SchemeRenderModel()
	{

	}

	SchemeModel::SchemeModel()
	{
		pInstance = this;
	}
	SchemeModel* SchemeModel::pInstance;

}}}}}