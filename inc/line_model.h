#include "global_model.h"

#ifndef _LINE_MODEL_H
#define _LINE_MODEL_H

typedef enum
{
	INTERPLATE_UNKOWN = 0, // 未知
	INTERPLATE_LINEAR, // 线性
	INTERPLATE_QUADRIC, // 二次
	INTERPLATE_CARDINAL // 三次
}enum_interplate_type_t;

#define MAX_POINTS (10000) // 最多支持的采样点数
#define INTERPLATE_NUM (10) // 每2个采样点之间的插值个数

typedef struct
{
	// 以下成员变量需要外部赋值
	enum_interplate_type_t INTERPLATE_TYPE; // 插值方式
	Point3 center_points[MAX_POINTS]; // 采样的中心点
	int center_licheng[MAX_POINTS]; // 中心点里程

	// 以下成员变量是通过计算得出
	int id; // 自动生成
	Point3 directions[MAX_POINTS]; // 方向
	Point3 normals[MAX_POINTS]; // 法向量
	Point3 interplate_points[MAX_POINTS * INTERPLATE_NUM]; // 插值点
	Point3 interplate_direction[MAX_POINTS* INTERPLATE_NUM]; // 插值点方向
	Point3 interplate_normal[MAX_POINTS* INTERPLATE_NUM]; // 插值点法向量

} st_road_line_t;
typedef struct
{
	Point3 position; // 世界坐标
	Point3 direction; // 路线方向
	Point3 norm; // 法线方向
	int licheng;
}st_line_point_info_t;

int compute_direction_norm(st_road_line_t* line);	//返回0表示正确
int compute_spline(const Point3* src, int src_count, Point3* dst, int dst_count, enum_interplate_type_t type);	//返回0表示正确，输出是一个已经分配好大小的数组
int compute_interplate(st_road_line_t* line);	//返回0表示正确
Point3 find_point_at_licheng (const st_road_line_t* line, int licheng);	//返回路线点。先用二分查找确定最近的2个采样点，然后插值生成。
int find_licheng_at_point(const st_road_line_t* line, Point3 point);	//返回里程，结果不唯一，取距离路线中心线最近的点的里程。需要对采样点建立kd树，然后找到最近的2个，作投影

#endif