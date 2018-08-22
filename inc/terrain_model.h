typedef struct
{
	int point_count;
	Point3* p_points;
}st_point3_array_t;
typedef struct
{
	int id;
	// ���±�����Ҫ��ֵ
	st_line_point_info_t* p_line_info; //ģ��������·����Ϣ
	Point2 point_x [20]; // ��Բο��������(x, y)
} st_ground_2d

typedef struct
{
	int id;
	// �����ɼ���ó�
	int p1_index;
	int p2_index;
	// ����ģ��
	st_point3_array_t* p_point3_array;
}st_edge_t;
typedef struct
{
	int id;
	// �����ɼ���ó�
	int p1_index;
	int p2_index;
	int p3_index;
	st_Edge_t edge1;
	st_Edge_t edge2;
	st_Edge_t edge3;
	// ����ģ��
	st_point3_array_t* p_point3_array;
}st_triangle_t;
typedef struct
{
	int id;
	// ���²������븳ֵ
	st_point3_array_t* p_point3_array;
	// ���²����ɼ�������
	int triangle_num;
	st_triangle_t* p_triangles;
	KDTree* p_tree; // kd-treeģ�ͣ�����ռ����
}st_delaunay_t;
typedef struct
{
	int id;
	int delaunay_num;
	st_delaunay_t* p_delaunays;
}st_delaunay_strip_t;

float GetHeight(st_delaunay_t* p_delaunay, Point2 pt);
float GetHeight(st_delaunay_strip_t* p_delaunay, Point2 pt);
