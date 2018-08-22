typedef struct 
{
	float x,
	float y
} Point2, Point2f;

typedef struct 
{
	float x,
	float y,
	float z
} Point3, Point3f;

typedef struct
{
	int licheng, // ���
	Point3 center_point, // ·�����ĵ㣨�����㣩
	int road_surface_id, // ·��ID
	int bianpo_id, // ����ID
	int dangtuqiang_id // ����ǽID
} st_licheng_info_t;
st_licheng_info_t table_licheng_info[MAX_POINTS];

typedef struct
{
	int id, // ID
	enum_struct_type type, // ID��Ӧ������
	void* pointer // ID��Ӧ��ָ��
}st_id_info_t;
st_id_info_t table_id_info[MAX_POINTS*10];

//������̶�Ӧ����Դ	
int find_licheng_info(st_licheng_info** info_table, int licheng);	//����info_table���У�-1��ʾʧ��
//����ID��Ӧ��ָ��	
int find_id_info(st_id_info** info_table, int id);	//����info_table���У�-1��ʾʧ��
