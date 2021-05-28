#include "cgraph/cgraph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/jrb.h"

typedef struct _Dinh
{
	int id;
	char ten[30];
}Dinh;

char* idtoten(int id, Dinh *dsdinh, int sodinh)
{
	for(int i=0;i<sodinh;i++)
	{
		if(dsdinh[i].id == id) return dsdinh[i].ten;
	}
}

int tentoid(char *ten, Dinh *dsdinh, int m)
{
	for(int i=0;i<m;i++)
	{
		if(strcmp(dsdinh[i].ten,ten)==0) return dsdinh[i].id;
	}
}

int kiemtrabaccau(cgraph_t g,int dinh1, int dinh2,Dinh *dsdinh)
{
	//kiem tra 2 dinh co canh truc tiep khong
	cgraph_ivec_t neis = cgraph_ivec_create();
	cgraph_neighbors(g,&neis,(CGRAPH_INTEGER)dinh1,CGRAPH_ALL);
	int kiemtra=1;
	for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(neis); ++i)
	{
    	if(dinh2==neis[i]) kiemtra=0;
    }
    if(kiemtra==0) return -1;
    else
    {
    	cgraph_ivec_t father = cgraph_ivec_create(),
			              dist = cgraph_ivec_create();
		cgraph_simple_bfs(g, (CGRAPH_INTEGER)dinh1, CGRAPH_ALL, false, &father, &dist);
		for(int i = 0; i < cgraph_ivec_size(dist); i++) 
		{
			if(dist[i] > 0 && dsdinh[i].id==dinh2) 
			{
				return 1;
			}
		}
		cgraph_ivec_free(&father);
  		cgraph_ivec_free(&dist);
    }
  	cgraph_ivec_free(&neis);
}

void danhsachdinh(cgraph_t g) //doc tu do thi
{
	printf("Danh sach cac dinh: ");
	JRB danhsach = make_jrb();
	for(int i=0 ; i<cgraph_ivec_size(g->from) ; i++)
	{
		if(jrb_find_int(danhsach, g->from[i])==NULL) //khong tim thay -> insert
		{
			jrb_insert_int(danhsach,g->from[i],(Jval)g->from[i]);
			printf("%d ",g->from[i]);
		}
		if(jrb_find_int(danhsach, g->to[i])==NULL) //khong tim thay -> insert
		{
			jrb_insert_int(danhsach,g->to[i],(Jval)g->to[i]);
			printf("%d ",g->to[i]);
		}
	}
	printf("\n");
	jrb_free_tree(danhsach);
}

int main() {
	FILE *inp = fopen("../data.txt", "r");
	int m,n;
	// m=so dinh   n=so canh
	fscanf(inp,"%d %d\n",&m,&n);
	Dinh dsdinh[100];
	for(int i=0;i<m;i++)
	{
		fscanf(inp,"%d %s",&(dsdinh[i].id),dsdinh[i].ten);
	}
	cgraph_rvec_t weights = cgraph_rvec_create();
	cgraph_ivec_t edges = cgraph_ivec_create();
	int value1,value2;
	double weight;
	for(int i = 0; i < n; i++) 
	{
		fscanf(inp, "%d %d %lf\n", &value1,&value2,&weight);
		cgraph_ivec_push_back(&edges, value1);
		cgraph_ivec_push_back(&edges, value2);
		cgraph_rvec_push_back(&weights, weight);
	}
	cgraph_ivec_push_back(&edges, 10);
	cgraph_ivec_push_back(&edges, 10);
	cgraph_t g = cgraph_create(edges, 0, CGRAPH_UNDIRECTED);
	fclose(inp);
	
	int lenh;
	do
	{
		printf("============ MENU ============\n");
		printf("1. In danh sách đỉnh\n");
		printf("2. Tìm bạn thân thiết nhất\n");
		printf("3. Kiểm tra quan hệ bắc cầu\n");
		printf("4. Tìm bạn chung\n");
		printf("5. Quit\n");
		printf("==============================\n");
		printf("Nhap lenh: ");
		scanf("%d",&lenh);
		if(lenh<1||lenh>5)
		{
			printf("LENH KHONG HOP LE!\n");
			continue;
		}
		if(lenh==1)
		{
			danhsachdinh(g);
		}
		if(lenh==2)
		{
			int id;
			printf("Nhap id: ");
			scanf("%d",&id);
			cgraph_ivec_t neis = cgraph_ivec_create();
			cgraph_neighbors(g,&neis,(CGRAPH_INTEGER)id,CGRAPH_OUT);
			double maxweight = 0.0;
			for(int i=0; i<cgraph_ivec_size(neis);i++)
			{
				int idcanh;
				cgraph_get_eid(g, (CGRAPH_INTEGER*)&idcanh,(CGRAPH_INTEGER)id, (CGRAPH_INTEGER)neis[i],false);
				if(weights[idcanh]>maxweight) maxweight=weights[idcanh];
			}
			printf("%.1lf\n",maxweight);
			for(int i=0; i<cgraph_ivec_size(neis);i++)
			{
				if(maxweight!=0.0)
				{
					int idcanh;
					cgraph_get_eid(g, (CGRAPH_INTEGER*)&idcanh,(CGRAPH_INTEGER)id, (CGRAPH_INTEGER)neis[i],false);
					if(weights[idcanh]==maxweight)
					{
						printf("%d %s\n",g->to[idcanh], idtoten(g->to[idcanh], dsdinh,m));
					}
				}
			}

		}
		if(lenh==3)
		{
			int dinh1,dinh2;
			printf("Nhap 2 dinh: ");scanf("%d %d",&dinh1, &dinh2);
			printf("%d\n",kiemtrabaccau(g,dinh1,dinh2,dsdinh));
		}
		if(lenh==4)
		{
			int dinh1,dinh2;
			printf("Nhap 2 dinh: ");scanf("%d %d",&dinh1, &dinh2);
			cgraph_ivec_t neis1 = cgraph_ivec_create();
			cgraph_neighbors(g,&neis1,(CGRAPH_INTEGER)dinh1,CGRAPH_OUT);
			cgraph_ivec_t neis2 = cgraph_ivec_create();
			cgraph_neighbors(g,&neis2,(CGRAPH_INTEGER)dinh2,CGRAPH_OUT);
			int dem=0;
			printf("Ban chung cua %s va %s\n", idtoten(dinh1, dsdinh, m),idtoten(dinh2, dsdinh, m));
			for(int i=0;i<cgraph_ivec_size(neis1);i++)
			{
				for(int j=0;j<cgraph_ivec_size(neis2);j++)
				{
					if(neis1[i]==neis2[j])
					{
						dem++;
						printf("%d %s\n",neis1[i],idtoten(neis1[i], dsdinh, m));
					}
				}
			}
		}
	}while(lenh!=5);
	// cgraph_destroy(&g);
	return 0;
	// int value1, value2, weight;
	// fscanf(inp, "%d %d\n", &n,&cohuong);
	// cgraph_rvec_t weights = cgraph_rvec_create();
	// cgraph_ivec_t edges = cgraph_ivec_create();
	// for(int i = 0; i < n; i++) {
	// 	fscanf(inp, "%d %d %d\n", &value1,&value2,&weight);
	// 	cgraph_ivec_push_back(&edges, value1);
	// 	cgraph_ivec_push_back(&edges, value2);
	// 	cgraph_rvec_push_back(&weights, weight);
	// }
	// fclose(inp);
	// cgraph_ivec_print(edges);

	// cgraph_t g;
	// cgraph_create(&g, edges, 0, cohuong);

	// cgraph_ivec_t vpath = cgraph_ivec_create(),
 //                epath = cgraph_ivec_create();
	// int from,to;
	// printf("From = ");
	// scanf("%d", &from);
	// printf("To = ");
	// scanf("%d", &to);
	// cgraph_get_shortest_path_dijkstra(&g, &vpath, &epath, from, to, weights, CGRAPH_OUT);

	// printf("Duong di tu %d den %d:\n",from,to);
	// cgraph_ivec_print(vpath);
	// cgraph_ivec_print(epath);
	
	// cgraph_destroy(&g);
}