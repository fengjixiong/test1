#include "global_model.h"
#include "line_model.h"
#include "kdtree.h"

#ifndef _TERRAIN_MODEL_H
#define _TERRAIN_MODEL_H

typedef struct
{
	int point_count;
	Point3* p_points;
}st_point3_array_t;
typedef struct
{
	int id;
	// 以下变量需要赋值
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	Point2 point_x [20]; // 相对参考点的坐标(x, y)
} st_ground_2d_t;

typedef struct
{
	int id;
	// 以下由计算得出
	int p1_index;
	int p2_index;
	// 关联模型
	st_point3_array_t* p_point3_array;
}st_edge_t;
typedef struct
{
	int id;
	// 以下由计算得出
	int p1_index;
	int p2_index;
	int p3_index;
	st_edge_t edge1;
	st_edge_t edge2;
	st_edge_t edge3;
	// 关联模型
	st_point3_array_t* p_point3_array;
}st_triangle_t;
typedef struct
{
	int id;
	// 以下参数必须赋值
	st_point3_array_t* p_point3_array;
	// 以下参数由计算生成
	int triangle_num;
	st_triangle_t* p_triangles;
	void* p_tree; // kd-tree模型，方便空间查找
}st_delaunay_t;
typedef struct
{
	int id;
	int delaunay_num;
	st_delaunay_t* p_delaunays;
}st_delaunay_strip_t;

float GetHeight(st_delaunay_t* p_delaunay, Point2 pt);
float GetHeight_strip(st_delaunay_strip_t* p_delaunay, Point2 pt);

#endif
