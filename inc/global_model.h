#ifndef _GLOBAL_MODEL_H
#define _GLOBAL_MODEL_H

typedef struct 
{
	float x;
	float y;
} Point2; Point2f;

typedef struct 
{
	float x;
	float y;
	float z;
} Point3; Point3f;

#define LICHENG_MAX_POINTS (10000)

typedef struct
{
	int licheng; // 里程
	Point3 center_point; // 路线中心点（采样点）
	int road_surface_id; // 路面ID
	int bianpo_id; // 边坡ID
	int dangtuqiang_id // 挡土墙ID
} st_licheng_info_t;
st_licheng_info_t table_licheng_info[LICHENG_MAX_POINTS];

typedef struct
{
	int id; // ID
	int type; // ID对应的类型
	void* pointer; // ID对应的指针
}st_id_info_t;
st_id_info_t table_id_info[LICHENG_MAX_POINTS*10];

//查找里程对应的资源	
int find_licheng_info(st_licheng_info_t** info_table, int licheng);	//返回info_table的行，-1表示失败
//查找ID对应的指针	
int find_id_info(st_id_info_t** info_table, int id);	//返回info_table的行，-1表示失败

#endif
