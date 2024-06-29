
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define PSX
void PcsxReduxExit(short exitCode)
{
    printf("Exiting with code %d\n", exitCode);
#ifdef PSX
    (*(volatile unsigned short *)0x1f802082) = exitCode;
#else
    exit(exitCode);
#endif
}

u8 space[0x1000] = {0};

s32 SpuInitMalloc(s32 num, s8 *top);
long SpuMallocWithStartAddr(unsigned long addr, long size);

typedef struct tagSpuMalloc
{
    u32 addr;
    u32 size;
} SPU_MALLOC;

extern SPU_MALLOC *_spu_memList[];

#define ASSERT_EQ(value1, value2)                                                           \
    if ((value1) != (value2))                                                               \
    {                                                                                       \
        printf("%d != %d in %s %s:%d\n", value1, value2, __FUNCTION__, __FILE__, __LINE__); \
        PcsxReduxExit(1);                                                                   \
    }

#define ASSERT_EQ_HEX(value1, value2)                                                           \
    if ((value1) != (value2))                                                                   \
    {                                                                                           \
        printf("%08X != %08X in %s %s:%d\n", value1, value2, __FUNCTION__, __FILE__, __LINE__); \
        PcsxReduxExit(1);                                                                       \
    }

extern s32 _spu_rev_reserve_wa;
extern s32 _spu_rev_offsetaddr;
extern s32 _spu_mem_mode_plus;

s32 SpuClearReverbWorkArea(s32 rev_mode);

extern s32 _spu_rev_startaddr[];

s32 g_VabAddrs[] = {0x00001010, 0x00042CC0, 0x00050E50,
                    0x00060A40, 0x00060A40, 0x00060A40};

#define SS_IMEDIATE 0

#define LOAD_VAB(vab_id, name, addr, data, dataLen)      \
    SsVabClose(vab_id);                                  \
    while (SsVabTransCompleted(SS_IMEDIATE) != 1)        \
        ;                                                \
                                                         \
    if (SsVabOpenHeadSticky(name, vab_id, addr) < 0)     \
    {                                                    \
        return -1;                                       \
    }                                                    \
    if (SsVabTransBodyPartly(data, dataLen, vab_id) < 0) \
    {                                                    \
        return -1;                                       \
    }                                                    \
    while (SsVabTransCompleted(SS_IMEDIATE) != 1)

extern unsigned char vb_0[];
extern unsigned char vh_0[];
extern unsigned char lib_seq[];

short SsSeqOpen(char *addr, short vab_id);

typedef struct VabHdr
{
    s32 form;
    s32 ver;
    s32 id;
    u32 fsize;
    u16 reserved0;
    u16 ps;
    u16 ts;
    u16 vs;
    u8 mvol;
    u8 pan;
    u8 attr1;
    u8 attr2;
    u32 reserved1;
} VabHdr;

typedef struct ProgAtr
{ /* Program Headdings */

    unsigned char tones; /* # of tones */
    unsigned char mvol;  /* program volume */
    unsigned char prior; /* program priority */
    unsigned char mode;  /* program mode */
    unsigned char mpan;  /* program pan */
    char reserved0;      /* system reserved */
    short attr;          /* program attribute */
    unsigned long reserved1;
    unsigned short reserved2;
    unsigned short reserved3;
} ProgAtr;
typedef struct VagAtr
{ /* VAG Tone Headdings */

    unsigned char prior;     /* tone priority */
    unsigned char mode;      /* play mode */
    unsigned char vol;       /* tone volume*/
    unsigned char pan;       /* tone panning */
    unsigned char center;    /* center note */
    unsigned char shift;     /* center note fine tune */
    unsigned char min;       /* minimam note limit */
    unsigned char max;       /* maximam note limit */
    unsigned char vibW;      /* vibrate depth */
    unsigned char vibT;      /* vibrate duration */
    unsigned char porW;      /* portamento depth */
    unsigned char porT;      /* portamento duration */
    unsigned char pbmin;     /* under pitch bend max */
    unsigned char pbmax;     /* upper pitch bend max */
    unsigned char reserved1; /* system reserved */
    unsigned char reserved2; /* system reserved */
    unsigned short adsr1;    /* adsr1 */
    unsigned short adsr2;    /* adsr2 */
    short prog;              /* parent program*/
    short vag;               /* vag reference */
    short reserved[4];       /* system reserved */

} VagAtr; /* 32 byte */

extern VabHdr *_svm_vab_vh[16];  /* number: 21 */
extern ProgAtr *_svm_vab_pg[16]; /* number: 22 */
extern VagAtr *_svm_vab_tn[16];  /* number: 23 */
extern s32 _svm_vab_total[16];
extern s32 _svm_vab_start[];
extern u8 _svm_vab_used[];

#define SS_SEQ_TABSIZ 176

static char stab[SS_SEQ_TABSIZ * 4 * 5];

int frames = 0;
extern s32 _spu_rev_reserve_wa;
extern s32 _spu_rev_offsetaddr;
extern s32 _spu_mem_mode_unitM;
extern s32 _spu_mem_mode_plus;

s32 SpuInitMalloc(s32 num, s8 *top);
s32 SpuMalloc(s32 size);

void print_memlist()
{
    int i;
    printf("-----------------------------------------\n");
    for (i = 0; i < 5; i++)
    {
        printf("_spu_memList[%d] addr %08X size %d\n", i, (*_spu_memList)[i].addr, (*_spu_memList)[i].size);
    }
}

void test_ss_vab()
{
    int i;
    int g_SeqAccessNum;
    ResetGraph(0);
    SsInitHot();
    SsSetTickMode(1);
    SpuMallocWithStartAddr(0x1010, 0x10000);
    SsSetTableSize(stab, 4, 5);
    SsStart();

    printf("before loading ================================================\n");

    printf("_spu_rev_reserve_wa %d\n", _spu_rev_reserve_wa);
    printf("_spu_rev_offsetaddr %d\n", _spu_rev_offsetaddr);
    printf("_spu_mem_mode_unitM %d\n", _spu_mem_mode_unitM);
    printf("_spu_mem_mode_plus %d\n", _spu_mem_mode_plus);

    print_memlist();
/*
    ASSERT_EQ(_spu_memList[0].addr, 0x00001010);
    ASSERT_EQ(_spu_memList[1].addr, 0x40011010);
    ASSERT_EQ(_spu_memList[2].addr, 0);

    ASSERT_EQ(_spu_memList[0].size, 65536);
    ASSERT_EQ(_spu_memList[1].size, 454640);
    ASSERT_EQ(_spu_memList[2].size, 0);
*/
#if 1
    LOAD_VAB(0, vh_0, g_VabAddrs[0], vb_0, 269488);
#endif

    printf("_svm_vab_pg[vabId] %08X diff %d\n", _svm_vab_pg[0], (int)_svm_vab_pg[0] - (int)vh_0);
    printf("_svm_vab_vh[vabId] %08X diff %d\n", _svm_vab_vh[0], (int)_svm_vab_vh[0] - (int)vh_0);
    printf("_svm_vab_tn[vabId] %08X diff %d\n", _svm_vab_tn[0], (int)_svm_vab_tn[0] - (int)vh_0);

    ASSERT_EQ((int)_svm_vab_pg[0] - (int)vh_0, 32);
    ASSERT_EQ((int)_svm_vab_vh[0] - (int)vh_0, 0);
    ASSERT_EQ((int)_svm_vab_tn[0] - (int)vh_0, 2080);

    printf("  _svm_vab_tn[0].prior %08X\n", _svm_vab_tn[0]->prior);

    ASSERT_EQ(_svm_vab_tn[0]->prior, 8);

    ASSERT_EQ(_svm_vab_start[0], 0x00001010);
    ASSERT_EQ(_svm_vab_total[0], 269488);
    ASSERT_EQ(_svm_vab_used[0], 1);

    printf(" _svm_vab_start[0] %08X\n", _svm_vab_start[0]);

    printf("_svm_vab_total[0] %d\n", _svm_vab_total[0]);

    printf("_svm_vab_used[0] %d\n", _svm_vab_used[0]);

    print_memlist();
#if 0
    ASSERT_EQ(_spu_memList[0].addr, 0x00001010);
    ASSERT_EQ(_spu_memList[1].addr, 0x40011010);
    ASSERT_EQ(_spu_memList[2].addr, 0);

    ASSERT_EQ(_spu_memList[0].size, 65536);
    ASSERT_EQ(_spu_memList[1].size, 454640);
    ASSERT_EQ(_spu_memList[2].size, 0);
#endif
#if 1
    g_SeqAccessNum = SsSeqOpen(lib_seq, 0);
    SsSetMVol(127, 127); /* set main volume */
    SsSeqSetVol(g_SeqAccessNum, 64, 64);
    SsSeqPlay(g_SeqAccessNum, 1, 1);

    while (frames++ < 60 * 10)
    {
        VSync(0);
    }
#endif
}

void test_spu_malloc()
{
    s8 temp[0x1000] = {0};
    s32 result;
    SpuInitMalloc(32, temp);

    ASSERT_EQ((*_spu_memList)[0].addr, 0x40001010);
    ASSERT_EQ((*_spu_memList)[0].size, 520176);
    ASSERT_EQ((*_spu_memList)[1].addr, 0);
    ASSERT_EQ((*_spu_memList)[1].size, 0);

    print_memlist();

    SpuMallocWithStartAddr(0x1010, 0x10000);

    print_memlist();

    ASSERT_EQ((*_spu_memList)[0].addr, 0x00001010);
    ASSERT_EQ((*_spu_memList)[0].size, 65536);
    ASSERT_EQ((*_spu_memList)[1].addr, 0x40011010);
    ASSERT_EQ((*_spu_memList)[1].size, 454640);
    ASSERT_EQ((*_spu_memList)[2].addr, 0);
    ASSERT_EQ((*_spu_memList)[2].size, 0);

    result = SpuMalloc(269488);
    ASSERT_EQ(result, 0x11010);

    print_memlist();

    ASSERT_EQ((*_spu_memList)[0].addr, 0x00001010);
    ASSERT_EQ((*_spu_memList)[0].size, 65536);
    ASSERT_EQ((*_spu_memList)[1].addr, 0x00011010);
    ASSERT_EQ((*_spu_memList)[1].size, 269488);
    ASSERT_EQ((*_spu_memList)[2].addr, 0x40052CC0);
    ASSERT_EQ((*_spu_memList)[2].size, 185152);
    ASSERT_EQ((*_spu_memList)[3].addr, 0);
    ASSERT_EQ((*_spu_memList)[3].size, 0);

    PcsxReduxExit(0);
}

int main()
{
    test_spu_malloc();

    PcsxReduxExit(0);
    return 0;
}
