typedef struct
{
	int id;

	// ���±�����Ҫ��ֵ
	const int MAX_LINE_NUM = 5;
	int line_num; // ·����Ŀ�����ߡ�˫�ߣ�
	float line_intervals[MAX_LINE_NUM-1]; // ·���м����
	float line_sides[2]; // ·�����˾���

	// ���±�����Ҫ����
	Point2 draw_points[10]; //��ά���Ƶ�
	
	// ����ģ��
	st_line_point_info_t* p_line_info; //ģ��������·����Ϣ
	void* p_parent; // ���ڵ�

}st_road_surface_2d_t;

typedef struct
{
	int id; // �Զ�����

	// ���±���ͨ������õ�����������꣬�ο�����start_point
	Point3 left_edge_points[INTERPLATE_NUM]; // ���·��ͱ������ӵ�
	Point3 right_edge_points[INTERPLATE_NUM]; // �Ҳ�·��ͱ������ӵ�
	Point3 track_start_points[MAX_LINE_NUM] [INTERPLATE_NUM]; // ������ʼ��
	Point3 track_end_points[MAX_LINE_NUM] [INTERPLATE_NUM]; // �����յ�

	// ����ģ�ͣ���Ҫ��ֵ
	const st_road_surface_2d_t* p_2d_start; // 2D·��ģ��
	const st_road_surface_2d_t* p_2d_end; // 2D·��ģ��
	void* p_parent; // ���ڵ�

}st_road_surface_3d_t;
