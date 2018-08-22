typedef enum
{
	INTERPLATE_UNKOWN = 0, // δ֪
	INTERPLATE_LINEAR, // ����
	INTERPLATE_QUADRIC, // ����
	INTERPLATE_CARDINAL // ����
}enum_interplate_type_t;
typedef struct
{
	// ���³�Ա������Ҫ�ⲿ��ֵ
	const int MAX_POINTS=10000; // ���֧�ֵĲ�������
	const int INTERPLATE_NUM = 10; // ÿ2��������֮��Ĳ�ֵ����
	enum_interplate_type_t INTERPLATE_TYPE = INTERPLATE_CARDINAL; // ��ֵ��ʽ
	Point3 center_points[MAX_POINTS]; // ���������ĵ�
	int center_licheng[MAX_POINTS]; // ���ĵ����

	// ���³�Ա������ͨ������ó�
	int id; // �Զ�����
	Point3 directions[MAX_POINTS]; // ����
	Point3 normals[MAX_POINTS]; // ������
	Point3 interplate_points[MAX_POINTS * INTERPLATE_NUM]; // ��ֵ��
	Point3 interplate_direction[MAX_POINTS* INTERPLATE_NUM]; // ��ֵ�㷽��
	Point3 interplate_normal[MAX_POINTS* INTERPLATE_NUM]; // ��ֵ�㷨����

} st_road_line_t;
typedef struct
{
	Point3 position; // ��������
	Point3 direction; // ·�߷���
	Point3 norm; // ���߷���
	int licheng;
}st_line_point_info_t;

int compute_direction_norm(st_road_line_t* line);	//����0��ʾ��ȷ
int compute_spline(const Point3* src, int src_count, Point3* dst, int dst_count, enum_interplate_type_t type);	//����0��ʾ��ȷ�������һ���Ѿ�����ô�С������
int compute_interplate(st_road_line_t* line);	//����0��ʾ��ȷ
Point3 find_point_at_licheng (const st_road_line_t* line, int licheng);	//����·�ߵ㡣���ö��ֲ���ȷ�������2�������㣬Ȼ���ֵ���ɡ�
int find_licheng_at_point(const st_road_line_t* line, Point3 point);	//������̣������Ψһ��ȡ����·������������ĵ����̡���Ҫ�Բ����㽨��kd����Ȼ���ҵ������2������ͶӰ
