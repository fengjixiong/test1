#ifndef _GEOMETRYMODEL_H
#define _GEOMETRYMODEL_H

#include "Util/OSG_Header.h"
#include "Scheme/Model/ModelBase.h"
#include "Util/BasicStructures.h"

namespace jx{
namespace kernel3d { 
namespace engine { 
namespace utils {
	class CDem;
	class CTin;
}}}}

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme {
namespace model{

	enum GeometryModelType
	{
		GeometryModelType_Unknown,
		GeometryModelType_Terrain,
		GeometryModelType_RoadBed,
		GeometryModelType_Tunnel,
		GeometryModelType_Bridge,
		GeometryModelType_RetainingWall,
		GeometryModelType_RailTrack,
		GeometryModelType_RailStation
	};



	class GeometryModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		GeometryModel();
	public:
		GeometryModelType m_type;
		SchemeLayerType m_layer;
	};

	class TerrainGeometry : public GeometryModel
	{
	public:
		TerrainGeometry();
	};

	class RailbedGeometry : public GeometryModel
	{
	public:
		RailbedGeometry();
		
	};

	class RetainingWallGeometry : public GeometryModel
	{
	public:
		RetainingWallGeometry();
	};

	class TunnelGeometry : public GeometryModel
	{
	public:
		TunnelGeometry();
	};

	class BridgeGeometry : public GeometryModel
	{
	public:
		BridgeGeometry();
	};

	class RailTrackGeometry : public GeometryModel
	{
	public:
		RailTrackGeometry();
	};

	class RailStationGeometry : public GeometryModel
	{
	public:
		RailStationGeometry();
	};

}}}}}

#endif