#include<time.h>
#include<omp.h>
#include<stdio.h>
static long num_steps =2000000000;
double step;
clock_t reg_end;
clock_t reg_start;
double speedup = 1.0;
void regularCode(double pi,double step)
{
    double pi1=pi;
    double pi2=pi;
    double pi3=pi;
    reg_start=clock();
    int i ;
    double x,y,z,final_ans;
    double sum1=0.0,sum2=0.0,sum3=0.0;
    step = 1.0/(double)num_steps;
    for (i=0;i<num_steps;i++){
        x = (i+0.5)*step;
        sum1 = sum1 + 4.0/(1+x*x);
    }
    pi1 = step*sum1;
    printf("\n pi1->%.2f",pi1);
    for(i=0;i<num_steps;i++){
        y=(i+0.5)*step;
        sum2=sum2+4.0/(1+y*y);
    }
     pi2 = step*sum2;
    for(i=0;i<num_steps;i++){
        z=(i+0.5)*step;
        sum3=sum3+4.0/(1+z*z);
    }
    pi3 = step*sum3;
    final_ans = pi1*pi2*pi3;
    printf("\n integration result->%.2f",final_ans);
    reg_end=clock();
    printf("\n Time taken in regular code->%.2ld sec",(reg_end-reg_start)/CLOCKS_PER_SEC);
    speedup = speedup*((reg_end-reg_start)/CLOCKS_PER_SEC);
}
void parallelizedCode(double pi,double step)
{
    int i,id;
    double x,sum;
    omp_set_num_threads(8);
    reg_start=clock();
    #pragma omp parallel                                                              
    {
        id = omp_get_thread_num();
        printf("\n id->%d",id);
        int numThreads = omp_get_num_threads();
        sum=0.0;
        #pragma omp parallel for schedule(static,10000) reduction(+:sum) reduction(*:x)
        for(i=id;i<num_steps;i=i+numThreads)
        {
           x=(i+0.5)*step;
           sum=sum+4.0/(1+x*x);
        }
        pi+=sum*step;
    }
    printf("\n Integration result->%.2f",pi*pi*pi);
    reg_end=clock();
    printf("\n Time taken in || code->%.2ld sec",(reg_end-reg_start)/CLOCKS_PER_SEC);
    speedup = speedup/((reg_end-reg_start)/CLOCKS_PER_SEC);
}
int main()
{
    double pi=0.0,step=1/(double)num_steps;
    regularCode(pi,step);
    printf("\n");
    parallelizedCode(pi,step);
    printf("\n speedup->%.2f",speedup);
}
