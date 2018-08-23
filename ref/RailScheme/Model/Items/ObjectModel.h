#ifndef _OBJECTMODEL_H
#define _OBJECTMODEL_H

#include "Scheme/Model/ModelBase.h"

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme {
namespace model{

	enum ObjectModelType
	{
		ObjectModelType_Unknown,
		ObjectModelType_Terrain,
		ObjectModelType_RoadBed,
		ObjectModelType_Tunnel,
		ObjectModelType_Bridge,
		ObjectModelType_RetainingWall
	};

	enum RailSectionType
	{
		RailSectionType_Unknown,
		RailSectionType_Railway,
		RailSectionType_Tunnel,
		RailSectionType_Bridge
	};

	class ObjectModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		ObjectModelType m_type;
		RailSectionType m_section;
		int m_nSectionIndex; // ËùÊôµÄsection index
	public:
		ObjectModel();
	};

	class TerrainModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		TerrainModel();
	};

	class RailbedModel : public ObjectModel
	{
	public:
		RailbedModel();
	};

	class RetainingWallModel : public ObjectModel
	{
	public:
		RetainingWallModel();
	};


}}}}}

#endif