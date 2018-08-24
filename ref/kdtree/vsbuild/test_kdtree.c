/*! gcc -Wall -g -o test test.c libkdtree.a */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "kdtree.h"

// 计算两个向量的距离
double dist_sq( double *a1, double *a2, int dims )
{
    double dist = 0;
    while( --dims >= 0 )
        dist += (a1[dims] - a2[dims])*(a1[dims] - a2[dims]);
    return sqrt(dist);
}

// 生成[-10, 10]的随机点
double rd( void )
{
    return ((double)rand()/RAND_MAX * 20.0 - 10.0);
}

// kd-tree测试示例，以3维点为例
int main(int argc, char **argv)
{
    int i;
    int num_pts = 8;
    void *kd; // kdtree结构体
    void *presults; // kdtree查询返回结构体
    double pos[3]; // kdtree查询返回
    double dist; // 距离
    double pt[3] = { 0, 0, 0 }; // 搜索点
    double radius = 5; // 搜索半径
    char *data; // 每个插入点的tag
    char *data_find; // 查找结果的插入点tag
    
    printf("[使用方法]\n");
    printf("test_kdtree.exe [number] [radius]\n");
    printf("生成number（默认为8）个半径为10的随机点，并且返回半径radius（默认5）的点\n\n");
    
	if(argc > 1 && isdigit(argv[1][0]))
		num_pts = atoi(argv[1]);
    if(argc > 2 && isdigit(argv[2][0]))
        radius = atoi(argv[2]);
    
	printf("inserting %d random vectors... \n", num_pts);
	fflush(stdout);

    if(!(data = malloc(num_pts))) 
    {
        printf("** malloc failed **\n");
        return 1;
    }
    
	kd = kd_create(3); // 创建3维KD-Tree
	for(i=0; i<num_pts; i++)  // 插入随机点和tag数据
    {
        data[i] = 'a'+i;
		assert(kd_insert3(kd, rd(), rd(), rd(), &data[i]) == 0);
	}
    presults = kd_nearest_range( kd, pt, radius );/* 根据距离radius查找最近的点 */

    printf("range query returned %d items\n", kd_res_size(presults));
    while( !kd_res_end( presults ) ) 
    {
        data_find = (char*)kd_res_item( presults, pos ); // 检索点的位置和tag数据
        dist = dist_sq( pt, pos, 3 ); // 计算距离
        printf( "node at (%.3f, %.3f, %.3f) is %.3lf away and has tag data=\"%c\"\n", 
            pos[0], pos[1], pos[2], dist, *data_find );
        kd_res_next( presults ); // 获取下一个
    }

    free(data);
	kd_res_free(presults);
	kd_free(kd);
	return 0;
}
