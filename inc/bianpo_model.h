#ifndef _BIANPO_MODEL_H
#define _BIANPO_MODEL_H

typedef enum 
{
	BIANPO_UNKNOW = 0, // δ֪
	BIANPO_LUJI, // ·��
	BIANPO_LUQIAN // ·ǵ
} enum_bianpo_type_t;
typedef struct
{
	int id;
	// ���²�����Ҫ��ֵ
	float top_width; // �����
	float bottom_width;  //�׿�
	float height;  //����
	float inner_hudao;//�ڲ໤��
	float outer_hudao;//��໤��
	float inner_pingtai_width; //�ڲ�ƽ̨��
	float outer_pingtai_width; //���ƽ̨��
	// ���²�����Ҫ����
	Point2 outline[8]; // ������
	// ����ģ��
	st_line_point_info_t* p_line_info; //ģ��������·����Ϣ
	void* p_parent; // ���ڵ�

}st_paishuigou_2d_t;   //��ˮ��
typedef struct
{
	int id��
	// ���²�����Ҫ��ֵ
	/******** for input ctr *****************/
	int qs; //ǽʽ
	int qx; //ǽ��
	int   iflt;           //�Ƿ�ȫ¶ͷ      1-��    0-��
	int ifts; //�Ƿ�����͹�
	int base;//��бʽ�Ƿ�������
	int wgh;
	/********for input size ****************/
	double b;   //����
	double bj1; //һ��̨�׿��
	double bj2; //����̨�׿��
	double hj1; //һ��̨�׸߶�
	double hj2; //����̨�׸߶�
	double H;   //ǽ��
	double mx;   //����
	double nb;   //����(��бʽtoo)
	double x;   //����б��
	double a;    //ǽ����·��ĸ߶�
	double mp;   //ǽ��·���¶�F
	//for ����ʽ:
	double ns;  //����ʽ��ǽ����
	double pt;  //����ʽƽ̨���
	//for ����ʽ
	double ts_l; //͹龾�ǽ�����
	double ts_h; //͹龸߶�
	double ts_b; //͹龵׿�
	/***********for drawing ******************/
	double jcdp; //�������� //1998-11-8
	/********** for caculate coor ************/
	double xx[14];
	double yy[14];
	// ����ģ��
	st_line_point_info_t* p_line_info; //ģ��������·����Ϣ
	void* p_parent; // ���ڵ�

}st_dangtuqiang_2d_t;
typedef struct
{
int id;
// ���²�����Ҫ��ֵ
	const char MAX_LEVEL = 7; // ���7������
	char level_count; // ��ǰ�м������£�������·������ķ���
	enum_bianpo_type_t type; // �������ͣ�1-·����2-·ǵ��
	float	width [MAX_LEVEL]; // ���¿��
	float	height [MAX_LEVEL]; // ���¸߶�
	float	pingtai_width[MAX_LEVEL ]; // �㼶֮���ƽ̨��
	
	// ���²�����Ҫ����
	Point2 outline[[MAX_LEVEL][8]j // 8�����Ѿ������˲�����ˮ�����������

	// ����ģ��
	st_line_point_info_t* p_line_info; //ģ��������·����Ϣ
	const st_paishuigou_2d_t* p_cegou[MAX_LEVEL ]; // ÿ��ƽ̨�ϵĲ๵����ˮ����
	const st_dangtuqiang_2d_t* p_dangtuqiang;
	void* p_parent; // ���ڵ�

}st_bianpo_2d_t;

typedef struct
{
	int id;
	// ���±�����Ҫ��ֵ
	Point3 reference_point; // �����ƽ̨��Ե��

	// ���±�����Ҫ����
	Point3 start_side_points[20]; // ��ʼ���½��ߵ�
	Point3 end_side_points[20]; // ��ֹ���½��ߵ�
    Point3 down_points[20];   // ��·����ӵĵ�
	Point3 up_points[20]; // �������ӵĵ�

	//����ģ��
	const st_paishuigou_2d_t* p_2d _start;
	const st_paishuigou_2d_t* p_2d _end;
	void* p_parent; // ���ڵ�

}st_paishuigou_3d_t;
typedef struct
{
	int id;
	// ���±�����Ҫ��ֵ
	Point3 reference_point; // �����ƽ̨��Ե��
	
	// ���±�����Ҫ����
	Point3 start_side_points[20]; // ��ʼ���µ�
	Point3 end_side_points[20]; // ��ֹ���µ�

	//����ģ��
	const st_dangtuqiang_2d_t* p_2d _start;
	const st_dangtuqiang_2d_t* p_2d _end;
	void* p_parent; // ���ڵ�

}st_dangtuqiang_3d_t;
typedef struct
{
	int id;
	// ���²�����Ҫ��ֵ
	Point3 reference_point; // �ο����������꣨Ĭ��Ϊ��·��Ӵ��ĵ㣩
			
	// ���²�����Ҫ����
	Point3 start_side_points[20]; // ��ʼ���½��ߵ�
	Point3 end_side_points[20]; // ��ֹ���½��ߵ�
    Point3 road_points[20];   // ��·����ӵĵ�
	Point3 ground_points[20]; // �������ӵĵ�

	//����ģ�ͣ��븳ֵ
	const st_bianpo_2d_t* p_2d _start;
	const st_bianpo_2d_t* p_2d _end;
	void* p_parent; // ���ڵ�

} st_bianpo_3d_t;

#endif
