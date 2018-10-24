#define PUSH_( val ) ({Stack_push(STACK,  val);})
#define POP_( val )  ({Stack_pop (STACK, &val);})

#define CALL_PUSH( ) ({Stack_push(CALL_STACK, PC - PC0 + 1);})
#define CALL_POP( val ) ({Stack_pop(CALL_STACK, &st_1); val = (int) st_1;})

//====================================================

DEF_CMD(dump,  0xF1, 2,
{
    Stack_dump(STACK);
})

DEF_CMD(ADD,  0x01, 2,
{
    GET(Rd);
    GET(Rr);
    REG[Rd] += REG[Rr];
})
DEF_CMD(ADD_STACK,  0x0A, 0,
{
    POP_(st_1);
    POP_(st_2);
    PUSH_(st_1 + st_2);
})

//====================================================

DEF_CMD(SUB,  0x02, 2,
{
    GET(Rd);
    GET(Rr);
    REG[Rd] -= REG[Rr];
})
DEF_CMD(SUB_STACK,  0x0B, 0,
{
    POP_(st_1);
    POP_(st_2);
    PUSH_(st_1 - st_2);
})

//====================================================

DEF_CMD(INC,  0x03, 1,
{
    GET(Rd);
    REG[Rd]++;
})
DEF_CMD(DEC,  0x04, 1,
{
    GET(Rd);
    REG[Rd]--;
})
DEF_CMD(CLR,  0x05, 1,
{
    GET(Rd);
    REG[Rd] = 0;
})
DEF_CMD(SER,  0x06, 1,
{
    GET(Rd);
    REG[Rd] = 1;
})

//====================================================

DEF_CMD(MUL,  0x07, 2,
{
    GET(Rd);
    GET(Rr);
    REG[Rd] *= REG[Rr];
})
DEF_CMD(MUL_STACK,  0x0C, 0,
{
    POP_(st_1);
    POP_(st_2);
    PUSH_(st_1 * st_2);
})

//====================================================

DEF_CMD(DIV,  0x08, 2,
{
    GET(Rd);
    GET(Rr);
    REG[Rd] /= REG[Rr];
})
DEF_CMD(DIV_STACK,  0x0D, 0,
{
    POP_(st_1);
    POP_(st_2);
    PUSH_(st_1 / st_2);
})

//====================================================

DEF_CMD(SQRT_STACK,  0x0E, 0,
{
    POP_(st_1);
    PUSH_(sqrt(st_1));
})

//====================================================

DEF_CMD(RET,  0x00, 0,
{
    CALL_POP(LBL);
    PC = PC0 + LBL;
})
DEF_CMD(JMP,  0x11, 1,
{
    GET(LBL);
    PC = PC0 + LBL;
})
DEF_CMD(CALL, 0x12, 1,
{
    CALL_PUSH();
    GET(LBL);
    PC = PC0 + LBL;
})

//====================================================

DEF_CMD(CP,   0x13, 2,
{
    GET(Rd);
    GET(Rr);
    cp_res = REG[Rd] - REG[Rr];
})
DEF_CMD(CP_STACK,   0x14, 0,
{
    POP_(st_1);
    POP_(st_2);
    cp_res = st_1 - st_2;
    PUSH_(st_2);
    PUSH_(st_1);
})

//====================================================

DEF_CMD(BREQ, 0x15, 1,
{
    GET(LBL);
    if(cp_res == 0)
        PC = PC0 + LBL;
})
DEF_CMD(BRNE, 0x16, 1,
{
    GET(LBL);
    if(cp_res != 0)
        PC = PC0 + LBL;
})
DEF_CMD(BRPL, 0x17, 1,
{
    GET(LBL);
    if(cp_res > 0)
        PC = PC0 + LBL;
})
DEF_CMD(BRMI, 0x18, 1,
{
    GET(LBL);
    if(cp_res < 0)
        PC = PC0 + LBL;
})

//====================================================

DEF_CMD(MOV,  0x31, 2,
{
    GET(Rd);
    GET(Rr);
    REG[Rd] = REG[Rr];
})

//====================================================

DEF_CMD(LDI,  0x32, 2,
{
    GET(Rd);
    GET(K);
    REG[Rd] = K;
})
DEF_CMD(LD,   0x33, 2,
{
    GET(Rd);
    GET(Rr);
    REG[Rd] = SRAM[REG[Rr]];
})
DEF_CMD(LDD,  0x34, 3,
{
    GET(Rd);
    GET(Rr);
    GET(k);
    REG[Rd] = SRAM[REG[Rr] + k];
})
DEF_CMD(LDS,  0x35, 2,
{
    GET(Rd);
    GET(k);
    REG[Rd] = SRAM[k];
})

//====================================================

DEF_CMD(ST,   0x36, 2,
{
    GET(Rd);
    GET(Rr);
    SRAM[REG[Rd]] = REG[Rr];
})
DEF_CMD(STD,  0x37, 3,
{
    GET(Rd);
    GET(k);
    GET(Rr);
    SRAM[REG[Rd] + k] = REG[Rr];
})
DEF_CMD(STS,  0x38, 2,
{
    GET(k);
    GET(Rr);
    SRAM[k] = REG[Rr];
})

//====================================================

DEF_CMD(IN,   0x39, 1,
{
    GET(Rd);
    printf("\n");
    scanf("%i", REG + Rd);
})
DEF_CMD(IN_STACK,   0x3F, 0,
{
    printf("\n");
    scanf("%lg", &st_1);
    PUSH_(st_1);
})

//====================================================

DEF_CMD(OUT,  0x3A, 1,
{
    GET(Rr);
    printf("\n%i", REG[Rr]);
})
DEF_CMD(OUT_STACK,  0x3E, 0,
{
    POP_(st_1);
    printf("\n%lg", st_1);
    PUSH_(st_1);
})

//====================================================

DEF_CMD(PUSH, 0x41, 1,
{
    GET(Rr);
    PUSH_(REG[Rr]);
})
DEF_CMD(PUSH_NUM, 0x4A, 1,
{
    GET(K);
    PUSH_(K);
})
DEF_CMD(PUSH_S_NUM, 0x4B, 1,
{
    GET(k);
    PUSH_(SRAM[k]);
})
DEF_CMD(PUSH_S_REG, 0x4C, 1,
{
    GET(Rr);
    PUSH_(SRAM[REG[Rr]]);
})
DEF_CMD(PUSH_S_REG_PLUS_NUM, 0x4D, 2,
{
    GET(Rr);
    GET(k);
    PUSH_(SRAM[REG[Rr] + k]);
})

//====================================================

DEF_CMD(POP,  0x42, 1,
{
    GET(Rd);
    POP_(st_1);
    REG[Rd] = (int) st_1;
})
DEF_CMD(POP_STACK,  0x4E, 0,
{
    POP_(st_1);
})

//====================================================

DEF_CMD(COPY_STACK,  0x43, 0,
{
    POP_(st_1);
    PUSH_(st_1);
    PUSH_(st_1);
})

//====================================================

DEF_CMD(SWAP_STACK,  0x44, 0,
{
    POP_(st_1);
    POP_(st_2);
    PUSH_(st_1);
    PUSH_(st_2);
})

//====================================================

DEF_CMD(NOP,  0xF0, 0,
{

})
DEF_CMD(END,  0xFF, 0,
{
    processing = 0;
})
