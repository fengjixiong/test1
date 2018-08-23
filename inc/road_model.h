typedef struct
{
	int id;
	// 以下参数需要赋值

	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	st_road_surface_2d_t* p_surface; // 路面
	st_bianpo_2d_t* p_bianpo_left;
	st_bianpo_2d_t* p_bianpo_right;
	void* p_parent; // 父节点

}st_road_point_2d_ t;
typedef struct
{
	int id;
	// 以下参数需要赋值
	Point3 ground_point; // 地面点
	int width; // 桥宽
	int pier_interval; // 桥墩间隔

	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	st_road_surface_2d_t* p_surface; // 路面
	void* p_parent; // 父节点

}st_road_point_2d_ t;
typedef struct
{
	int id;
	// 以下参数需要赋值
	Point3 ground_point; // 地面点
	int width; // 隧道宽
	int height; // 隧道高

	// 关联模型
	st_line_point_info_t* p_line_info; //模型所处线路点信息
	st_road_surface_2d_t* p_surface; // 路面
	void* p_parent; // 父节点

}st_tunnel_point_2d_ t;

typedef struct
{
	int id;
	// 以下参数需要赋值
	// 关联模型
	st_road_point_2d_t* p_2d; //模型所处线路点信息
	st_road_surface_3d_t* p_surface; // 路面
	st_bianpo_3d_t* p_bianpo_left;
	st_bianpo_3d_t* p_bianpo_right;
	st_bridge_3d_t* p_bridge;
	st_tunnel_cave_3d_t* p_tunnel;
} st_road_point_3d_t;
typedef struct
{
	int id;
	// 以下参数需要赋值

	// 关联模型
	st_bridge_point_2d_t* p_2d_start; 
	st_bridge_point_2d_t* p_2d_end;
	void* p_parent; // 父节点

}st_bridge_point_3d_ t;
typedef struct
{
	int id;
	// 以下参数需要赋值

	// 关联模型
	st_tunnel_point_2d_t* p_2d_start; 
	st_tunnel_point_2d_t* p_2d_end;
	void* p_parent; // 父节点

}st_tunnel_point_3d_ t;
