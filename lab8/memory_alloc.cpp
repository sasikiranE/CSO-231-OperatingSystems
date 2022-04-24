#include <iostream>
#include <stdlib.h>

using namespace std;

int *memory;
void print(int n)
{
    printf("State of memory block:\n");
    for(int i=0;i<n;i++)
        printf("%d ",memory[i]);
  
    printf("\n");
}
void swap(int *x,int *y)
{
    int temp=*x;
    *x=*y;
    *y=temp;
}
void first_fit(int pid,int mem,int n)
{
    int start=0,end=0,i,j,k;
    for(i=0;i<n;i++)
    {
        if(memory[i]==-1)
        {
            start=i;
            for(j=i+1;j<n;j++)
            {
                if(memory[j]!=-1 || ( j==n-1))
                {
                    end=j-1;
                    if(j==n-1)
                        end=j;
                    if(end-start+1>=mem)
                    {
                        for(k=start;k<start+mem;k++)
                            memory[k]=pid;
                        printf("Memory allocation request ACCEPTED\n");
                        print(n);
                        return;
                    }
                    else
                    {
                        i=j;
                        break;
                    }
                }
            }
        }
    }
    printf("Memory allocation request REJECTED\n");
    print(n);
}
void best_fit(int pid,int mem,int n)
{
    int i,j,start=0,end=0,small=n+1,x=-1,y=-1;
    for(i=0;i<n;i++)
    {
        if(memory[i]==-1)
        {
            start=i;
            for(j=i+1;j<n;j++)
            {
                if(memory[j]!=-1 || ( j==n-1))
                {
                    end=j-1;
                    if( j==n-1)
                        end=j;
                    if(end-start+1<small && end-start+1>=mem)
                    {
                        small=end-start+1;
                        x=start;
                        y=end;
                    }
                    i=j;
                    break;
                }
            }
        }
    }
    if(y-x+1>=mem && x!=-1 && y!=-1)
    {
        for(i=x;i<x+mem;i++)
            memory[i]=pid;
        printf("Memory allocation request ACCEPTED\n");
    }
    else
        printf("Memory allocation request REJECTED\n");
    print(n);
}
void worst_fit(int pid,int mem,int n)
{
    int i,j,k,start=0,end=0,large=0,x=-1,y=-1;
    for(i=0;i<n;i++)
    {
        if(memory[i]==-1)
        {
            start=i;
            for(j=i+1;j<n;j++)
            {
                if(memory[j]!=-1 || (j==n-1))
                {
                    end=j-1;
                    if(j==n-1)
                        end=j;
                    if(end-start+1>large)
                    {
                        large=end-start+1;
                        x=start;
                        y=end;
                    }
                    i=j;
                    break;
                }
            }
        }
    }
    if(y-x+1>=mem && x!=-1 && y!=-1)
    {
        for(i=x;i<x+mem;i++)
            memory[i]=pid;
        printf("Memory allocation request ACCEPTED\n");
    }
    else
        printf("Memory allocation request REJECTED\n");
    print(n);
}
void release(int pid,int n)
{
    int i;
    for(i=0;i<n;i++)
        if(memory[i]==pid)
            memory[i]=-1;
    print(n);
}
void status(int n)
{
    int i,j,pid;
    for(i=0;i<n;i++)
    {
        if(memory[i]==-1)
        {
            printf("Unused Addresses [%p : ",&memory[i]);
          int start =i,end;
            for(j=i+1;j<n;j++)
            {
                if(memory[j]!=-1 || ( j==n-1))
                {
                    if( j==n-1)
                    {
                        printf("%p]\n",&memory[j]);
                        i=j;
                      end=j;
                    }
                    else
                    {
                        printf("%p]\n",&memory[j-1]);
                        i=j-1;
                      end=j-1;
                    }
                  printf("Unused Space Range : %d %d\n" , start , end);
                  break;
                }
            }
        }
        else
        {
            pid=memory[i]; 
          int start=i;
            printf("Addresses [%p : ",&memory[i]);
            for(j=i+1;j<n;j++)
            {
                if(memory[j]!=pid || (j==n-1))
                {
                  int end;
                    if(j==n-1)
                    {
                        printf("%p] Process P%d\n",&memory[j],pid);
                        i=j;
                        end=j;
                    }
                    else
                    {
                        printf("%p] Process P%d\n",&memory[j-1],pid);
                        i=j-1;
                        end=j-1;
                    }

                  printf("Process P%d Space Range : %d %d\n" , pid,start , end);
                    break;
                }
            }
        }
    }
    print(n);
}
void compact(int n)
{
    int i,j,k,start;
    for(i=0;i<n;i++)
        if(memory[i]==-1)
        {
            start=i;
            break;
        }
    for(j=i+1;j<n;j++)
        if(memory[j]!=-1)
            swap(&memory[start++],&memory[j]);
    print(n);
}
int main(int argc, char* argv[])
{
    int i,n=atoi(argv[1]),mem,pid;
    memory=(int*)malloc(n*sizeof(int));
    if(memory==NULL)
    {
    	printf("Specified memory could not be allocated\n");
    	exit(0);
    }
    for(i=0;i<n;i++)
        memory[i]=-1;

    print(n);
    char type;
    string s,p;
    do
    {
        printf("allocator>");
        cin>>s;
        if(s=="RQ")
        {
            cin>>p>>mem>>type;
            pid=(int)p[1]-48;
            if(type=='F')
                first_fit(pid,mem,n);
            else if(type=='B')
                best_fit(pid,mem,n);
            else if(type=='W')
                worst_fit(pid,mem,n);
        }
        else if(s=="RL")
        {
            cin>>p;
            pid=p[1]-48;
            release(pid,n);
        }
        else if(s=="C")
            compact(n);
        else if(s=="STAT")
            status(n);
    }while(s!="X");
    return 0;
}