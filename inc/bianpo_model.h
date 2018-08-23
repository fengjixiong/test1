#include "global_model.h"
#include "line_model.h"

#ifndef _BIANPO_MODEL_H
#define _BIANPO_MODEL_H

typedef enum 
{
	BIANPO_UNKNOW = 0, // 未知
	BIANPO_LUJI, // 路基
	BIANPO_LUQIAN // 路堑
} enum_bianpo_type_t;
typedef struct
{
	int id;
	// 以下参数需要赋值
	float top_width; // 上面宽
	float bottom_width;  //底宽
	float height;  //沟深
	float inner_hudao;//内侧护道
	float outer_hudao;//外侧护道
	float inner_pingtai_width; //内侧平台宽
	float outer_pingtai_width; //外侧平台宽
	// 以下参数需要计算
	Point2 outline[8]; // 轮廓线
	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	void* p_parent; // 父节点

}st_paishuigou_2d_t;   //排水沟
typedef struct
{
	int id；
	// 以下参数需要赋值
	/******** for input ctr *****************/
	int qs; //墙式
	int qx; //墙型
	int   iflt;           //是否全露头      1-是    0-否
	int ifts; //是否设置凸榫
	int base;//俯斜式是否加设底座
	int wgh;
	/********for input size ****************/
	double b;   //顶宽
	double bj1; //一级台阶宽度
	double bj2; //二级台阶宽度
	double hj1; //一级台阶高度
	double hj2; //二级台阶高度
	double H;   //墙高
	double mx;   //胸坡
	double nb;   //背坡(俯斜式too)
	double x;   //基宽斜度
	double a;    //墙顶到路肩的高度
	double mp;   //墙顶路基坡度F
	//for 衡重式:
	double ns;  //衡重式上墙背坡
	double pt;  //衡重式平台宽度
	//for 悬臂式
	double ts_l; //凸榫距墙踵距离
	double ts_h; //凸榫高度
	double ts_b; //凸榫底宽
	/***********for drawing ******************/
	double jcdp; //基础埋深 //1998-11-8
	/********** for caculate coor ************/
	double xx[14];
	double yy[14];
	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	void* p_parent; // 父节点

}st_dangtuqiang_2d_t;
typedef struct
{
int id;
// 以下参数需要赋值
	const char MAX_LEVEL = 7; // 最多7级边坡
	char level_count; // 当前有几级边坡，级数从路线向外的方向
	enum_bianpo_type_t type; // 边坡类型（1-路基，2-路堑）
	float	width [MAX_LEVEL]; // 边坡宽度
	float	height [MAX_LEVEL]; // 边坡高度
	float	pingtai_width[MAX_LEVEL ]; // 层级之间的平台宽
	
	// 以下参数需要计算
	Point2 outline[[MAX_LEVEL][8]j // 8个点已经考虑了并入排水沟后的轮廓线

	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	const st_paishuigou_2d_t* p_cegou[MAX_LEVEL ]; // 每个平台上的侧沟（排水沟）
	const st_dangtuqiang_2d_t* p_dangtuqiang;
	void* p_parent; // 父节点

}st_bianpo_2d_t;

typedef struct
{
	int id;
	// 以下变量需要赋值
	Point3 reference_point; // 靠里的平台边缘点

	// 以下变量需要计算
	Point3 start_side_points[20]; // 起始端坡脚线点
	Point3 end_side_points[20]; // 终止端坡脚线点
    Point3 down_points[20];   // 与路面相接的点
	Point3 up_points[20]; // 与地面相接的点

	//关联模型
	const st_paishuigou_2d_t* p_2d _start;
	const st_paishuigou_2d_t* p_2d _end;
	void* p_parent; // 父节点

}st_paishuigou_3d_t;
typedef struct
{
	int id;
	// 以下变量需要赋值
	Point3 reference_point; // 靠里的平台边缘点
	
	// 以下变量需要计算
	Point3 start_side_points[20]; // 起始端坡点
	Point3 end_side_points[20]; // 终止端坡点

	//关联模型
	const st_dangtuqiang_2d_t* p_2d _start;
	const st_dangtuqiang_2d_t* p_2d _end;
	void* p_parent; // 父节点

}st_dangtuqiang_3d_t;
typedef struct
{
	int id;
	// 以下参数需要赋值
	Point3 reference_point; // 参考点世界坐标（默认为与路面接触的点）
			
	// 以下参数需要计算
	Point3 start_side_points[20]; // 起始端坡脚线点
	Point3 end_side_points[20]; // 终止端坡脚线点
    Point3 road_points[20];   // 与路面相接的点
	Point3 ground_points[20]; // 与地面相接的点

	//关联模型，须赋值
	const st_bianpo_2d_t* p_2d _start;
	const st_bianpo_2d_t* p_2d _end;
	void* p_parent; // 父节点

} st_bianpo_3d_t;

#endif
