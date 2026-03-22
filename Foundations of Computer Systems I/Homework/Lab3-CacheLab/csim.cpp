#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE *output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}

void printHelp(const char *name)
{
    printf(
        "Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n"
        "Options:\n"
        "  -h         Print this help message.\n"
        "  -v         Optional verbose flag.\n"
        "  -s <num>   Number of set index bits.\n"
        "  -E <num>   Number of lines per set.\n"
        "  -b <num>   Number of block offset bits.\n"
        "  -t <file>  Trace file.\n\n"
        "Examples:\n"
        "  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n"
        "  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n",
        name, name, name);
}

size_t s, E, b;
char *trace_file;
int verbose;
int help;
int hits, misses, evictions;

struct line
{
    int valid;
    int dirty;
    unsigned long tag;
    line(unsigned long t) : valid(1), dirty(0), tag(t) {}
};

struct set
{
    list<line> lines;
    unordered_map<unsigned long, list<line>::iterator> table;
};

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            help = 1;
            break;
        case 'v':
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        }
    }
    if (help)
    {
        printHelp(argv[0]);
    }
    FILE *fp = fopen(trace_file, "r");
    vector<set> cache(1 << s);

    char op;
    unsigned long add;
    int siz;
    int reg;

    while (fscanf(fp, " %c %lx,%d %d", &op, &add, &siz, &reg) == 4)
    {
        int set_idx = (add >> b) & ((1 << s) - 1);
        unsigned long tag = add >> (s + b);

        set &current_set = cache[set_idx];
        auto &table = current_set.table;
        auto &lines = current_set.lines;
        auto map_it = table.find(tag);

        if (map_it != table.end())
        {
            // HIT
            hits++;
            auto list_it = map_it->second;
            // Write-Back
            if (op == 'S')
                list_it->dirty = 1;
            // LRU 更新，移动到头部
            lines.splice(lines.begin(), lines, list_it);
            if (verbose)
                printf("%c %lx,%d hit\n", op, add, siz);
        }
        else
        {
            // MISS
            misses++;
            if (lines.size() == E)
            {
                // EVICTION
                evictions++;
                if (verbose)
                    printf("%c %lx,%d miss eviction\n", op, add, siz);
                // 尾部行作为 victim
                line &victim = lines.back();
                // 更新哈希表
                table.erase(victim.tag);
                // 复用节点更新数据并移到头部
                lines.splice(lines.begin(), lines, prev(lines.end()));
                lines.front().tag = tag;
                lines.front().dirty = (op == 'S' ? 1 : 0);
                // 插入哈希表
                table[tag] = lines.begin();
            }
            else
            {
                // 组未满
                if (verbose)
                    printf("%c %lx,%d miss\n", op, add, siz);
                // 创建新行插入头部并记录到哈希表
                lines.emplace_front(tag);
                lines.front().dirty = (op == 'S' ? 1 : 0);
                table[tag] = lines.begin();
            }
        }
    }

    printSummary(hits, misses, evictions);
    fclose(fp);
    return 0;
}