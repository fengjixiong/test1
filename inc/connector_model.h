#include "global_model.h"
#include "road_model.h"
#include "line_model.h"

#ifndef _CONNECTOR_MODEL_H
#define _CONNECTOR_MODEL_H

typedef enum
{
	ROAD_TYPE_UNKOWN,
	ROAD_TYPE_HIGHWAY,
	ROAD_TYPE_RAILWAY,
	ROAD_TYPE_BRIDGE,
	ROAD_TYPE_TUNNEL
}enum_road_type_t;
typedef struct
{
	int id;
	// 以下参数需要赋值
	int huanpo_width; // 缓坡宽
	enum_road_type_t start_type; // 起始端类型
	
	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	st_road_point_2d_t* p_road;
	st_bridge_point_2d_t* p_bridge;
}st_road_bridge_connect_2d_t;
typedef struct
{
	int id;
	// 以下参数需要赋值
	int huanpo_width; // 缓坡宽
	enum_road_type_t start_type; // 起始端类型

	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	st_road_point_2d_t* p_road;
	st_tunnel_point_2d_t* p_bridge;
}st_road_tunnel_connect_2d_t;

#endif
