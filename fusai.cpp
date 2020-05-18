/*用前向星的方式重新建图*/
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace std;
struct NV
{
    int node;
    long int val;
    NV(int node, long int val) : node(node), val(val) {}
};

struct dict_NV
{
    int node;
    int node2;
    long int val1;
    long int val2;
    dict_NV(int node, int node2, long int val1, long int val2) : node(node), node2(node2), val1(val1), val2(val2) {}

    bool operator<(const dict_NV &a) const
    {
        return node < a.node || (node == a.node && node2 < a.node2);
    }
};

struct Out_node
{
    long int from[3]; //起点
    // int to;   //终点
    // int val;  //定义权值
    bool operator<(const Out_node &a) const
    {
        return (from[0] == a.from[0] && from[1] < a.from[1]) || from[0] < a.from[0];
    }
};

struct In_node
{
    // int to;   //起点
    long int from[3]; //终点
    string s;
    // int val;  //定义权值
    bool operator<(const In_node &a) const
    {
        return (from[1] == a.from[1] && from[0] < a.from[0]) || from[1] < a.from[1];
    }
};

struct Node
{
    int index = 0;
    int node_val = 0;
    // int in_index[2]{-1};
    // int out_index[2]{-1};
    //初始化
    //Node(int index, int node_val) : index(index), node_val(node_val) {}
};

//定义结构体
struct MultiThreadinfo
{

    int all_num = 0;
    int node_count = 0;
    int i = 0;
    int (*i_s_index)[5] = new int[50000][5]; //定义每个点在char数组中的索引值 28000/8=35000
    int node_cycle_cur[5]{0};                 //定义每个char数组现在所在的位置
    char *node_cycles7 = new char[2000000000];
    char *node_cycles6 = new char[2000000000];
    char *node_cycles5 = new char[2000000000];
    char *node_cycles4 = new char[2000000000];
    char *node_cycles3 = new char[2000000000];
};

#define add_reuslts(id, index, depth)                                         \
    for (int dc = 0; dc < depth; ++dc)                                        \
    {                                                                         \
        for (char k : MAPNode[mystack[dc]])                                   \
        {                                                                     \
            info->node_cycles##index[info->node_cycle_cur[index - 3]] = k;    \
            ++info->node_cycle_cur[index - 3];                                \
        }                                                                     \
        if ((dc + 1) % index != 0)                                            \
        {                                                                     \
            info->node_cycles##index[info->node_cycle_cur[index - 3]] = ',';  \
            ++info->node_cycle_cur[index - 3];                                \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            info->node_cycles##index[info->node_cycle_cur[index - 3]] = '\n'; \
            ++info->node_cycle_cur[index - 3];                                \
        }                                                                     \
        info->i_s_index[id / THREAD_COUNT][index - 3] = info->node_cycle_cur[index - 3];     \
    }

const int THREAD_COUNT = 4;
vector<vector<NV>> In(500000, vector<NV>());
vector<vector<NV>> Out(500000, vector<NV>());
vector<string> MAPNode(500000);
//定义找环的函数
void *find_cycle(void *find_cycle_info)
{
    MultiThreadinfo *info = (MultiThreadinfo *)find_cycle_info;
    int i = info->i;
    int node_count = info->node_count;
    int all_num(0);
    int mystack[7]{0};
    bool *sort_or_not = new bool[node_count];
    bool find(true);
    int *flags = new int[node_count];
    // unordered_map<int, vector<dict_NV>> dict_in;
    // unordered_map<int, vector<dict_NV>>::iterator dict_key;
    vector<vector<dict_NV>> dict_in(50000, vector<dict_NV>());
   
    int *ifin_index = new int[node_count]; //入度的索引
    while (i < node_count)
    {
        memset(sort_or_not, true, node_count);

        // if (i == 2604)
        // {
        //     printf("已处理的节点 %d/%d\n", i, node_count);
        // }
        find = true;
        //++time;
        flags[i] = 1;
        //mystack[0] = all_node[i].node_val;
        mystack[0] = i;
        for (NV &d1 : Out[i])
        {
            //++time;
            int l1 = d1.node;
            long int v1 = d1.val;
            if (l1 < i)
            {
                continue;
            }
            //mystack[1] = all_node[l1].node_val;
            mystack[1] = l1;
            flags[l1] = 1;
            //vector<NV> testt = Out[l1];
            for (NV &d2 : Out[l1])
            {
                //++time;
                int l2 = d2.node;
                long int v2 = d2.val;
                //((v1<=boundary5 && v2<=boundary3 ) &&
                if (l2 < i || (v1 > 5 * v2 || v2 > 3 * v1))
                {
                    continue;
                }
                // if(v1>boundary5 || v2>boundary3){
                //     if(v1>boundary5){

                //     }
                // }

                if (flags[l2] == 1)
                    continue;
                //mystack[2] = all_node[l2].node_val;
                mystack[2] = l2;
                flags[l2] = 1;
                for (NV &d3 : Out[l2])
                {
                    //++time;
                    //int l3 = all_node[l3].node_val;
                    int l3 = d3.node;
                    long int v3 = d3.val;
                    if (l3 < i || (v2 > 5 * v3 || v3 > 3 * v2))
                        continue;
                    if (flags[l3] == 1)
                    {
                        if (l3 == i && (v3 <= 5 * v1 && v1 <= 3 * v3))
                        {
                            ++all_num;
                            add_reuslts(i, 3, 3);
                        }
                        continue;
                    }
                    //mystack[3] = all_node[l3].node_val;
                    mystack[3] = l3;
                    flags[l3] = 1;
                    for (NV &d4 : Out[l3])
                    {
                        //++time;
                        int l4 = d4.node;
                        long int v4 = d4.val;
                        //int l4 = all_node[l4].node_val;
                        if (l4 < i || (v3 > 5 * v4 || v4 > 3 * v3))
                            continue;

                        if (flags[l4] == 1)
                        {
                            if (l4 == i && (v4 <= 5 * v1 && v1 <= 3 * v4))
                            {
                                ++all_num;
                                add_reuslts(i, 4, 4);
                            }
                            continue;
                        }
                        //mystack[4] = all_node[l4].node_val;
                        mystack[4] = l4;
                        flags[l4] = 1;
                        if (find)
                        {
                        
                            memset(ifin_index, -1, node_count * 4);
                            int icount(0);
                            for (NV &f1 : In[i])
                            {
                                int fd1 = f1.node;
                                long int fv1 = f1.val;
                                if (fd1 > i)
                                {
                                    if (ifin_index[fd1] < 0)
                                        {
                                            
                                            ifin_index[fd1] = icount;
                                            dict_in[ifin_index[fd1]].clear();
                                            ++icount;
                                        }
                                    dict_in[ifin_index[fd1]].emplace_back(dict_NV(-1, -1, fv1, 0));
                                    for (NV &f2 : In[fd1])
                                    {
                                        //int in2 = all_node[f2].node_val;
                                        int fd2 = f2.node;
                                        long int fv2 = f2.val;
                                        if (fd2 > i && (fv2 <= 5 * fv1 && fv1 <= 3 * fv2))
                                        {
                                             if (ifin_index[fd2] < 0)
                                                {
                                                    //ifin[fd2] = 1;
                                                    ifin_index[fd2] = icount;
                                                    dict_in[ifin_index[fd2]].clear();
                                                    ++icount;
                                                }
                                            //sort_index.emplace_back(fd2);
                                            dict_in[ifin_index[fd2]].emplace_back(dict_NV(fd1, -1, fv2, fv1));
                                            //遍历第三层入度表
                                            for (NV &f3 : In[fd2])
                                            {
                                                int fd3 = f3.node;
                                                long int fv3 = f3.val;
                                                if (fd3 > i)
                                                {
                                                    if (fd3 != fd1 && ((fv3 <= 5 * fv2 && fv2 <= 3 * fv3)))
                                                    {
                                                        if (ifin_index[fd3] < 0)
                                                        {
                                                            //ifin_index[fd3] = 1;
                                                            ifin_index[fd3] = icount;
                                                            dict_in[ifin_index[fd3]].clear();
                                                            ++icount;
                                                        } 
                                                        dict_in[ifin_index[fd3]].emplace_back(dict_NV(fd2, fd1, fv3, fv1));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            find = false;
                        }
                        //++time;
                        if(ifin_index[l4]>=0)
                        {
                            int l4_index = ifin_index[l4];
                            if (sort_or_not[l4_index])
                            {
                                sort(dict_in[l4_index].begin(), dict_in[l4_index].end());
                                sort_or_not[l4_index] = false;
                            }
                            
                            for (dict_NV &w : dict_in[l4_index])
                            {
                                int l = w.node;
                                if (l > 0)
                                {
                                    int ll = w.node2;
                                    if (ll > 0)
                                    {
                                        //int n_index7 = mymap[l];
                                        long int wv1 = w.val1;
                                        long int wv2 = w.val2;
                                        if (flags[l] == 1 || flags[ll]==1||(v4 > 5 * wv1 || wv1 > 3 * v4) || (wv2 > 5 * v1 || v1 > 3 * wv2))
                                            continue;
                                        //mystack[5] = all_node[l5].node_val;
                                        //mystack[6] = all_node[l].node_val;
                                        mystack[4] = l4;
                                        mystack[5] = l;
                                        mystack[6] = ll;
                                        add_reuslts(i, 7, 7);
                                        ++all_num;
                                    }
                                    else{
                                        //int n_index7 = mymap[l];
                                        long int wv1 = w.val1;
                                        long int wv2 = w.val2;
                                        if (flags[l] == 1 || (v4 > 5 * wv1 || wv1 > 3 * v4) || (wv2 > 5 * v1 || v1 > 3 * wv2))
                                            continue;
                                        //mystack[5] = all_node[l5].node_val;
                                        //mystack[6] = all_node[l].node_val;
                                        mystack[4] = l4;
                                        mystack[5] = l;
                                        add_reuslts(i, 6, 6);
                                        ++all_num;
                                    }
                                }
                                else
                                {

                                    long int wv1 = w.val1;
                                    if ((v4 > 5 * wv1 || wv1 > 3 * v4) || (wv1 > 5 * v1 || v1 > 3 * wv1))
                                        continue;
                                    //mystack[5] = all_node[l5].node_val;
                                    ++all_num;
                                    mystack[4] = l4;
                                    add_reuslts(i, 5, 5);
                                }
                            }
                        }
                        flags[l4] = 0;
                    }
                    flags[l3] = 0;
                }
                flags[l2] = 0;
            }
            flags[l1] = 0;
        }
        i+=THREAD_COUNT;
    }
    info->all_num = all_num;
    pthread_exit(NULL);
}

// int *in_node_index_left = new int[1500000];
// int *in_node_index_right = new int[1500000];
// int *out_node_index_left = new int[1500000];
// int *out_node_index_right = new int[1500000];
// Out_node *out_edge = new Out_node[2900000];

// Out_node *out_edge = new Out_node[2000000];
// In_node *in_edge = new In_node[2000000]; //入度表
// In_node *in_edge = new In_node[2000000];    //入度表
// Out_node *out_edge = new Out_node[2000000]; //出度表
int main()
{

    //初始化各个参数
    In_node *in_edge = new In_node[2000000]; //入度表
    unordered_map<int, string> map_node;     //定义数字到字符的转换
    int edge_count(0);                       //图的边数
    // const int boundary5 = INT32_MAX / 5;
    // const int boundary3 = INT32_MAX / 3;
    // const int max_val = INT32_MAX;

    /*--------------------------使用mmap读取测试数据--------------------------*/

#ifdef TEST
    //自己测试的路径
    string testFile = "/home/lsf/桌面/fusai/复赛/test_data.N111314.E200W.A19630345.txt";
    //testFile = "/root/data/test_data1.txt";
    //testFile = "/home/lsf/桌面/fusai/复赛/test_data.txt";
    testFile = "/home/lsf/桌面/chusai/test_data1.txt";
    //string testFile = "/root/data/test_data1.txt";
    //testFile = "/home/lsf/桌面/chusai/HuaweiCodecraft-TestData2020/3512444/test_data.txt";
    string resultFile = "./myresult1.txt";
#else
    //提交代码的路径
    string testFile = "/data/test_data.txt";
    string resultFile = "/projects/student/result.txt";
#endif

    auto t1 = clock();
    int fd = open(testFile.c_str(), O_RDONLY);
    int test_data_size = lseek(fd, 0, SEEK_END);
    char *buf = (char *)mmap(NULL, test_data_size, PROT_READ, MAP_PRIVATE, fd, 0);
    const char *buf_end(buf + test_data_size);
    int to(0);
    int from(0);
    long int val(0);
    char Il1[12]{'\0'};
    char Il2[12]{'\0'};
    char Il3[12]{'\0'};
    int idx[3]{0};
    int i(0);
    int num(0);

    for (char *magic_buf(buf); magic_buf < buf_end;)
    {
        idx[0] = 0;
        idx[1] = 0;
        i = 0;
        num = 0;
        memset(Il1, 0, 12);
        memset(Il2, 0, 12);
        memset(Il3, 0, 12);
        while (*magic_buf != '\n')
        {
            // cout << *magic_buf << " ";
            if (*magic_buf == ',')
            {
                idx[i++] = num;
            }
            magic_buf++;
            num++;
        }
        // 内存拷贝
        memcpy(Il1, buf, idx[0]);
        from = atoi(Il1);
        memcpy(Il2, buf + idx[0] + 1, idx[1] - idx[0] - 1);
        to = atoi(Il2);
        memcpy(Il3, buf + idx[1] + 1, num - idx[1] - 1);
        val = atoi(Il3);
        in_edge[edge_count].from[0] = from;
        in_edge[edge_count].from[1] = to;
        in_edge[edge_count].from[2] = val;
        in_edge[edge_count].s = Il2;
        // out_edge[edge_count].from[0] = from;
        // out_edge[edge_count].from[1] = to;
        // out_edge[edge_count].from[2] = val;
        //map_node.insert({to, Il2});
        ++edge_count;
        ++magic_buf;
        buf = buf + num + 1;
    }
    //对出度表和入度表排序
    sort(in_edge, in_edge + edge_count);
    //sort(out_edge, out_edge + edge_count);

    unordered_map<int, pair<int, int>> in_node_index;  //定义节点的入度表的索引参数,表示从first到second,不包括second
    unordered_map<int, pair<int, int>> out_node_index; //定义节点的出度表的索引参数
    unordered_map<int, int> mymap;                     //定义映射
    unordered_map<int, int>::iterator mymap_key;
    // vector<vector<In_node>> In(1500000, vector<In_node>());
    // vector<vector<Out_node>> Out(1500000, vector<Out_node>());

    int node_count(0); //定义节点数
    int left(0);
    int right(0);
    int all_node[500000];
    // vector<string> MAPNode(500000);
    //对入度表做一下映射,同时建立索引表
    for (int i = 0; i < edge_count - 1; ++i)
    {
        if (in_edge[i].from[1] != in_edge[i + 1].from[1])
        {
            mymap[in_edge[i].from[1]] = node_count;
            MAPNode[node_count] = in_edge[i].s;
            all_node[node_count] = node_count;
            //all_node[node_count].node_val = in_edge[i].from[1];
            ++node_count;
        }
    }
    //最后一个边单独判断
    mymap[in_edge[edge_count - 1].from[1]] = node_count;
    MAPNode[node_count] = in_edge[edge_count - 1].s;
    all_node[node_count] = node_count;
    // int a = mymap[6050];
    // string fuck = MAPNode[a];
    //all_node[node_count].node_val = in_edge[edge_count - 1].from[1];
    ++node_count;
    //做入度表
    node_count = 0;
    for (int i = 0; i < edge_count - 1; ++i)
    {
        mymap_key = mymap.find(in_edge[i].from[0]);

        if (mymap_key != mymap.end())
        {
            In[node_count].emplace_back(NV(mymap_key->second, in_edge[i].from[2]));
        }
        if (in_edge[i].from[1] != in_edge[i + 1].from[1])
        {
            ++node_count;
        }
    }
    //最后一个边单独判断
    mymap_key = mymap.find(in_edge[edge_count - 1].from[0]);
    if (mymap_key != mymap.end())
    {
        In[node_count].emplace_back(NV(mymap_key->second, in_edge[edge_count - 1].from[2]));
    }
    ++node_count;
    //由入度表做出度表
    for (int i = 0; i < node_count; ++i)
    {
        for (NV &j : In[i])
        {
            Out[j.node].emplace_back(NV(i, j.val));
        }
    }
#ifdef TEST
    cout << "读取加建图时间:" << (double)(clock() - t1) / CLOCKS_PER_SEC << "s" << endl;
#endif

//     unordered_map<int, int> test_map;
//     unordered_map<int, int>::iterator map_key;
//     // for(int i= 0;i<500;i++){
//     //     test_map[i] = i;
//     // }
//     // t1 = clock();
//     // for (int i = 0; i < 56745486; ++i)
//     // {
//     //     int k = i / 100;
//     //     map_key =  test_map.find(k);
//     // }
//     // cout << "aaa建图时间:" << (long int)(clock() - t1) / CLOCKS_PER_SEC << "s" << endl;
#ifdef TEST
    printf("节点总数 %d\n", node_count);
#endif
    /*--------------------------使用多线程找环--------------------------*/
    pthread_attr_t attr;
    //初始化线程属性
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_t tids[THREAD_COUNT];
    //定义结构体并初始化
    MultiThreadinfo infos[THREAD_COUNT];
    //time_t t;
    //t = time(NULL);
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        infos[i].i = i;
        infos[i].node_count = node_count;
        pthread_create(&tids[i], NULL, find_cycle, (void *)&(infos[i]));
    }
    pthread_attr_destroy(&attr);
    void *status;
    for (int tc = 0; tc < THREAD_COUNT; ++tc)
    {
        int rc = pthread_join(tids[tc], &status);
        if (rc)
        {
            cerr << "Wrong!" << endl;
            exit(-1);
        }
    }
    int all_num(0);
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        all_num = all_num + infos[i].all_num;
    }
#ifdef TEST
    printf("找出的环总数 %d\n", all_num);
    //cout << "找环时间:" << (double)(time(NULL) - t) / CLOCKS_PER_SEC << "s" << endl;
#endif

    /*--------------------------写入结果--------------------------*/
    string s;
    s = to_string(all_num);
    s = s + '\n';
    FILE *pFile = fopen(resultFile.c_str(), "w");
    fwrite(s.c_str(), sizeof(char), s.size(), pFile);
    //定义最后的结果数组
    int results_size = 0;
    char *results = new char[2000000000];
    int begin[8]{0};
    int end[8]{0};

    //下面的其实是有问题的, 会少遍历最后几个节点, 所以线上10进程的时候结果出错了.但是八进程和四进程没有错, 所以暂时不管了, 说明后几个节点没有环 MultiThreadinfo infos[THREAD_COUNT];

#define reset_begin             \
    for (int i = 0; i < 8; ++i) \
    {                           \
        begin[i] = 0;           \
    }

#define add_to_char(index)                                              \
    reset_begin;                                                        \
    for (int i = 0; i <= node_count / THREAD_COUNT +1; ++i)                 \
    {                                                                   \
        for (int a = 0; a < THREAD_COUNT; ++a)                          \
        {                                                               \
            if (infos[a].i_s_index[i][index - 3] == 0)                  \
                continue;                                               \
            end[a] = infos[a].i_s_index[i][index - 3];                  \
            for (int b = begin[a]; b < end[a]; ++b)                     \
            {                                                           \
                results[results_size] = infos[a].node_cycles##index[b]; \
                ++results_size;                                         \
            }                                                           \
            begin[a] = end[a];                                          \
        }                                                               \
    }

    add_to_char(3);
    add_to_char(4);
    add_to_char(5);
    add_to_char(6);
    add_to_char(7);
    // string s;
    // for(int i=0;i<5;++i){
    //     s = s+to_string(all_num[i]);
    //     s = s+'\n';
    // }
    fwrite(&results[0], sizeof(char), results_size, pFile);

    exit(0);
}