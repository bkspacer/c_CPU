#ifndef ASM_COMMANDS_H_INCLUDED
#define ASM_COMMANDS_H_INCLUDED

#define PUSH( val ) Stack_push(STACK, val)
#define POP( val ) Stack_pop(STACK, &val)

#define CMD_PUSH( ) ({Stack_push(CALL_STACK, PC - PC0 + 1);})
#define CMD_POP( val ) ({Stack_pop(CMD_STACK, &val);})

//====================================================

DEF_CMD(ADD,  0x01, 2,
{
    GET(Rd);
    GET(Rr);
    REG[Rd] += REG[Rr];
})
DEF_CMD(ADD_STACK,  0x0A, 0,
{
    POP(K);
    POP(k);
    PUSH(K + k);
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
    POP(K);
    POP(k);
    PUSH(K - k);
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
    REG[Rd] = 0;
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
    POP(K);
    POP(k);
    PUSH(K * k);
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
    POP(K);
    POP(k);
    PUSH(K / k);
})

//====================================================

DEF_CMD(RET,  0x00, 0,
{
    CMD_POP(LBL);
    PC = PC0 + LBL;
})
DEF_CMD(JMP,  0x11, 1,
{
    GET(LBL);
    PC = PC0 + LBL;
})
DEF_CMD(CALL, 0x12, 1,
{
    CMD_PUSH;
    GET(LBL);
    PC = PC0 + LBL;
})

//====================================================

DEF_CMD(CP,   0x13, 2,
{
    GET(Rd);
    GET(Rr);
    CP_RES = REG[Rd] - REG[Rr];
})
DEF_CMD(CP_STACK,   0x14, 0,
{
    POP(K);
    POP(k);
    CP_RES = K - k;
    PUSH(k);
    PUSH(K);
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
    scanf("%i", REG + Rd);
})
DEF_CMD(IN_STACK,   0x3F, 0,
{
    scanf("%i", &K);
    PUSH(K);
})

//====================================================

DEF_CMD(OUT,  0x3A, 1,
{
    GET(Rr);
    printf("\n%i", REG[Rr]);
})
DEF_CMD(OUT_STACK,  0x3E, 0,
{
    POP(K);
    printf("\n%i", K);
})

//====================================================

DEF_CMD(PUSH, 0x41, 1,
{
    GET(Rr);
    PUSH(REG[Rr]);
})
DEF_CMD(PUSH_NUM, 0x4A, 1,
{
    GET(K);
    PUSH(K);
})
DEF_CMD(PUSH_S_NUM, 0x4B, 1,
{
    GET(k);
    PUSH(SRAM[k]);
})
DEF_CMD(PUSH_S_REG, 0x4C, 1,
{
    GET(Rr);
    PUSH(SRAM[REG[Rr]]);
})
DEF_CMD(PUSH_S_REG_PLUS_NUM, 0x4D, 2,
{
    GET(Rr);
    GET(k);
    PUSH(SRAM[REG[Rr] + k]);
})

//====================================================

DEF_CMD(POP,  0x42, 1,
{
    GET(Rd);
    POP(REG + Rd);
})
DEF_CMD(POP_STACK,  0x4E, 0,
{
    POP(K);
})

//====================================================

DEF_CMD(NOP,  0xF0, 0,
{

})
DEF_CMD(END,  0xFF, 0,
{
    processing = 0;
})

#endif // ASM_COMMANDS_H_INCLUDED
