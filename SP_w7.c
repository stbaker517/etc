#include<stdio.h>

typedef union{
    float f;
    unsigned u;

}bit_float_t;

float bit2float(unsigned u)
{
    bit_float_t arg;
    arg.u=u;
    return arg.f;
}

unsigned float2bit(float f)
{
     bit_float_t arg;
    arg.f=f;
    return arg.u;
}
int main(){

    float f=7.0;
    unsigned u=7;
    printf("b2f %f %f\n",bit2float(u),(float)u);
    printf("f2b %u %u\n",float2bit(f),(unsigned)f);

    //1.b2f,f2b 구조 설명
    //왜 그게 가능한가?



}