#include "global_model.h"
#include "line_model.h"

typedef struct
{
	int id;

	// 以下变量需要赋值
	const int MAX_LINE_NUM = 5;
	int line_num; // 路线数目（单线、双线）
	float line_intervals[MAX_LINE_NUM-1]; // 路线中间距离
	float line_sides[2]; // 路面两端距离

	// 以下变量需要计算
	Point2 draw_points[10]; //二维绘制点
	
	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	void* p_parent; // 父节点

}st_road_surface_2d_t;

typedef struct
{
	int id; // 自动生成

	// 以下变量通过计算得到，是相对坐标，参考点是start_point
	Point3 left_edge_points[INTERPLATE_NUM]; // 左侧路面和边坡连接点
	Point3 right_edge_points[INTERPLATE_NUM]; // 右侧路面和边坡连接点
	Point3 track_start_points[MAX_LINE_NUM] [INTERPLATE_NUM]; // 铁轨起始点
	Point3 track_end_points[MAX_LINE_NUM] [INTERPLATE_NUM]; // 铁轨终点

	// 关联模型，需要赋值
	const st_road_surface_2d_t* p_2d_start; // 2D路面模型
	const st_road_surface_2d_t* p_2d_end; // 2D路面模型
	void* p_parent; // 父节点

}st_road_surface_3d_t;
