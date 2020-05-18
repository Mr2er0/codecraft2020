#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
using namespace std;

#define add_reuslts(id, index, depth)                                               \
    for (int dc = 0; dc < depth; ++dc)                                              \
    {                                                                               \
        for (char k : map_node[mystack[dc]])                                        \
        {                                                                           \
            info->node_cycles##index[info->node_cycle_cur[index - 3]] = k;          \
            ++info->node_cycle_cur[index - 3];                                      \
        }                                                                           \
        if ((dc + 1) % index != 0)                                                  \
        {                                                                           \
            info->node_cycles##index[info->node_cycle_cur[index - 3]] = ',';        \
            ++info->node_cycle_cur[index - 3];                                      \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            info->node_cycles##index[info->node_cycle_cur[index - 3]] = '\n';       \
            ++info->node_cycle_cur[index - 3];                                      \
        }                                                                           \
        info->i_s_index[(id - w) / a][index - 3] = info->node_cycle_cur[index - 3]; \
    }

//定义全局变量
const int THREAD_COUNT = 8;
// vector<vector<int>> out_node;
// vector<vector<int>> in_node;
int isfindcycle1[50002]{0};
int in_node_init[50002][30];  //将第一个参数定义为入度的个数
int out_node_init[50002][30]; //将第一个参数定义为出度的个数
int in_node_num[50002];       //定义入度的个数
int out_node_num[50002];      // 定义出度的个数
int to_index[50002];          //to_index的第一个参数表示节点数
string map_node[50002];

//定义结构体
struct MultiThreadinfo
{
    int i;
    int node_num;
    int w = 1; //id起始的位置
    int all_num = 0;
    int T_C = 6;                             //线程数
    int (*i_s_index)[5] = new int[17500][5]; //定义每个点在char数组中的索引值 28000/8=35000
    int *node_cycle_cur = new int[5];        //定义每个char数组现在所在的位置
    char *node_cycles7 = new char[70000000];
    char *node_cycles6 = new char[35000000];
    char *node_cycles5 = new char[20000000];
    char *node_cycles4 = new char[10000000];
    char *node_cycles3 = new char[10000000];
};

//定义找环的函数
void *find_cycle(void *find_cycle_info)
{
    MultiThreadinfo *info = (MultiThreadinfo *)find_cycle_info;
    int a = info->T_C;
    int node_num = info->node_num;
    int i = info->i;
    int w = info->w;
    int mystack[7]{0};
    vector<int>::iterator mystack_iter;
    unordered_map<int, vector<int>> two_in_node;
    unordered_map<int, vector<int>>::iterator key;
    int *flags = new int[50002];
    int all_num(0);
    //int j(1);
    bool find_or_not = true;
    while (i <= node_num)
    {
        int d0 = to_index[i];
        if (out_node_num[d0] == 0 || (isfindcycle1[d0] == in_node_num[d0]))
        {
            i += a;
            continue;
        }
        find_or_not = true;
        mystack[0] = d0;

        flags[d0] = 1;
        for (int l1 = 0; l1 < out_node_num[d0]; ++l1)
        {
            int d1 = out_node_init[d0][l1];
            if (d1 < to_index[i])
                continue;
            mystack[1] = out_node_init[d0][l1];

            ++isfindcycle1[d1];
            flags[d1] = 1;
            for (int l2 = 0; l2 < out_node_num[d1]; ++l2)
            {
                int d2 = out_node_init[d1][l2];
                if (d2 < d0)
                    continue;
                if (flags[d2] == 1)
                    continue;
                mystack[2] = d2;

                flags[d2] = 1;
                for (int l3 = 0; l3 < out_node_num[d2]; ++l3)
                {
                    int d3 = out_node_init[d2][l3];
                    if (d3 < d0)
                        continue;
                    if (flags[d3] == 1)
                    {
                        if (d3 == d0)
                        {
                            add_reuslts(i, 3, 3);
                            ++all_num;
                        }
                        continue;
                    }
                    mystack[3] = d3;

                    flags[d3] = 1;
                    for (int l4 = 0; l4 < out_node_num[d3]; ++l4)
                    {
                        int d4 = out_node_init[d3][l4];
                        if (d4 < d0)
                            continue;
                        if (flags[d4] == 1)
                        {
                            if (d4 == d0)
                            {
                                add_reuslts(i, 4, 4);
                                ++all_num;
                            }
                            continue;
                        }
                        mystack[4] = d4;

                        flags[d4] = 1;
                        for (int l5 = 0; l5 < out_node_num[d4]; ++l5)
                        {
                            int d5 = out_node_init[d4][l5];
                            if (d5 < d0)
                                continue;
                            if (flags[d5] == 1)
                            {
                                if (d5 == d0)
                                {
                                    add_reuslts(i, 5, 5);
                                    ++all_num;
                                }
                                continue;
                            }
                            //判断二次入度表是否已经创建过,创建过的话直接跳过
                            if (find_or_not)
                            {
                                two_in_node.clear();
                                for (int &j : in_node_init[d0])
                                {
                                    if (j > d0)
                                    {
                                        two_in_node[j].emplace_back(-1);
                                        for (int &k : in_node_init[j])
                                        {
                                            if (k > d0)
                                            {
                                                two_in_node[k].emplace_back(j);
                                            }
                                        }
                                    }
                                }
                                find_or_not = false;
                            }
                            key = two_in_node.find(d5);
                            if (key != two_in_node.end())
                            {
                                for (int &l : two_in_node[d5])
                                {
                                    if (flags[l] == 1)
                                        continue;
                                    if (l > 0)
                                    {
                                        //写入mystack中的元素
                                        mystack[5] = d5;
                                        mystack[6] = l;
                                        add_reuslts(i, 7, 7);

                                        ++all_num;
                                    }
                                    else
                                    {
                                        //写入mystack中的元素
                                        mystack[5] = d5;
                                        add_reuslts(i, 6, 6);

                                        ++all_num;
                                    }
                                }
                            }
                        }

                        flags[d4] = 0;
                    }

                    flags[d3] = 0;
                }

                flags[d2] = 0;
            }

            flags[d1] = 0;
        }
        i += a;
    }
    info->all_num = all_num;
    pthread_exit(NULL);
}
int main()
{
    /*--------------------------使用mmap读取测试数据--------------------------*/
#ifdef TEST
    //自己测试的路径
    string testFile = "/home/lsf/桌面/chusai/test_data.txt";
    //testFile = "/home/lsf/桌面/chusai/HuaweiCodecraft-TestData2020/3512444/test_data.txt";
    string resultFile = "./myresult.txt";
#else
    //提交代码的路径
    string testFile = "/data/test_data.txt";
    string resultFile = "/projects/student/result.txt";
#endif
    //auto t1 = clock();
    int fd = open(testFile.c_str(), O_RDONLY);
    int test_data_size = lseek(fd, 0, SEEK_END);
    char *buf = (char *)mmap(NULL, test_data_size, PROT_READ, MAP_PRIVATE, fd, 0);
    const char *buf_end(buf + test_data_size);
    //auto t = clock();
    // int(*in_node_init)[50] = new int[260000][50];  //将第一个参数定义为入度的个数
    // int(*out_node_init)[50] = new int[260000][50]; //将第一个参数定义为出度的个数

    int *sort_count = new int[50002]; //使用计数排序来做ID重映射
    int to = 0;
    int from = 0;
    int all_edge(0);
    char Id1[10]{'\0'};
    char Id2[10]{'\0'};
    int idx[2]{0};
    int i(0);
    int num(0);

    unordered_map<int, string> mapNode;
    for (char *magic_buf(buf); magic_buf < buf_end;)
    {
        idx[0] = 0;
        idx[1] = 0;
        i = 0;
        num = 0;
        memset(Id1, 0, 10);
        memset(Id2, 0, 10);
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
        // cout << endl;
        // 内存拷贝
        memcpy(Id1, buf, idx[0]);
        from = atoi(Id1);
        memcpy(Id2, buf + idx[0] + 1, idx[1] - idx[0] - 1);
        to = atoi(Id2);
        //if (to <= 50000 && to > 11)

        ++magic_buf;
        buf = buf + num + 1;
        //if (to > 50000)
        if (to > 50000 || to <= 11) //这个是群里大佬给的trick,有环的节点数在11到5w之间
        {
            continue;
        }
        in_node_init[to][in_node_num[to]] = from;
        ++in_node_num[to];
        map_node[to] = Id2;
        ++sort_count[to]; //使用计数排序的方法
        out_node_init[from][out_node_num[from]] = to;
        ++out_node_num[from];
    }

    int j(0);
    int first = 1;
    for (int i = 0; i <= 50000; ++i)
    {
        if (sort_count[i] > 0)
        {
            to_index[to_index[0] + 1] = i;
            ++to_index[0];
            if (first && i > 15000)
            {
                j = to_index[0];
                first = 0;
            }
        }
    }
    for (int i = 0; i <= to_index[0]; ++i)
    {
        //注意这个排序,sort(a+1,a+4),从索引1开始排,排到索引3,不包括4
        to = to_index[i + 1];
        sort(in_node_init[to], in_node_init[to] + in_node_num[to]);
    }
    for (int i = 0; i <= to_index[0]; ++i)
    {
        //注意这个排序,sort(a+1,a+4),从索引1开始排,排到索引3,不包括4
        from = to_index[i + 1];
        sort(out_node_init[from], out_node_init[from] + out_node_num[from]);
    }
    munmap(buf, test_data_size);
    //cout << "读取加建图时间:" << (double)(clock() - t1) / CLOCKS_PER_SEC << "s" << endl;
#ifdef TEST
    printf("节点总数 %d\n", to_index[0]);
#endif
    int node_num = to_index[0];
    //使用多线程去找环
    //定义线程属性
    pthread_attr_t attr;
    //初始化线程属性
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_t tids[THREAD_COUNT];
    //定义结构体并初始化
    /*  这里是群里大佬给的,线上大部分环在15000之内,15000到50000只有小部分环
        所以定义6个线程找15000之内的环,2个找15000到50000的环              */
    MultiThreadinfo infos[THREAD_COUNT];
    for (int i = 0; i < 6; ++i)
    {
        infos[i].i = i + 1;
        infos[i].node_num = j;
        // infos[i].T_C = 6;
        // infos[7].w = 1;
        pthread_create(&tids[i], NULL, find_cycle, (void *)&(infos[i]));
    }
    infos[6].i = j + 1;
    infos[6].node_num = to_index[0];
    infos[6].T_C = 2;
    infos[6].w = j + 1;
    pthread_create(&tids[6], NULL, find_cycle, (void *)&(infos[6]));
    infos[7].i = j + 2;
    infos[7].node_num = to_index[0];
    infos[7].T_C = 2;
    infos[7].w = j + 1;
    pthread_create(&tids[7], NULL, find_cycle, (void *)&(infos[7]));

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
    //求出找出的环总数
    int all_num = 0;
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        all_num = all_num + infos[i].all_num;
    }
#ifdef TEST
    printf("找出的环总数 %d\n", all_num);
#endif

    string s;
    s = to_string(all_num);
    s = s + '\n';
    FILE *pFile = fopen(resultFile.c_str(), "w");
    fwrite(s.c_str(), sizeof(char), s.size(), pFile);
    //定义最后的结果数组
    int results_size = 0;
    char *results = new char[150000000];
    int begin[8]{0};
    int end[8]{0};
#define reset_begin             \
    for (int i = 0; i < 8; ++i) \
    {                           \
        begin[i] = 0;           \
    }

#define add_to_char(index)                                              \
    reset_begin;                                                        \
    for (int i = 0; i <= j / 6; ++i)                                    \
    {                                                                   \
        for (int a = 0; a < 6; ++a)                                     \
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

#define add_to_char2(index)                                             \
    reset_begin;                                                        \
    for (int i = 0; i < (node_num - j) / 2; ++i)                        \
    {                                                                   \
        for (int a = 6; a < 8; ++a)                                     \
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
    add_to_char2(3);

    add_to_char(4);
    add_to_char2(4);

    add_to_char(5);
    add_to_char2(5);

    add_to_char(6);
    add_to_char2(6);

    add_to_char(7);
    add_to_char2(7);
    fwrite(&results[0], sizeof(char), results_size, pFile);

    return 0;
}