#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#include <semaphore.h>

#include <bits/stdc++.h>
using namespace std;


int q1[12] = {};
int q2 = 0;
int q3[30] = {};

int m;

int q1_elements=0;

vector< int > approved_students;

map<int , string> sid_pass;

sem_t q1_sem, q2_sem, q3_sem, abc_sem;
pthread_mutex_t q2_mutex , approved_sid_mutex;

void * A_thread(void *arg);
void * B_thread(void *arg);
void * E_thread(void *arg);
void * C_thread(void *arg);

int status_of_students[50] = {};

void * student_thread(void *threadid)
{

    int *tid;
    tid=(int *)threadid;
    printf("It's thread #%d\n",*tid);

    if(q1_elements==10)
    {
        printf("Thread %d is waiting...\n", *tid);
        sem_wait(&q1_sem);
    }

    printf("Wait is over for thread %d \n", *tid);


    q1_elements++;
    q1[q1_elements] = *tid;



    //ekhn student tar request niye b er kache jabe

    pthread_mutex_lock(&q2_mutex);
    q2 = *tid;

    //now he will ask d to provide his password



    pthread_exit(NULL);
}





int main()
{
    int n;
    cin>>n;

    for(int i=1;i<=n;i++)sid_pass[i] = "u";

    int rc, res;
    void *thread_result;
    pthread_t threads[n+1];
    pthread_t a_thread, e_thread, c_thread, b_thread ;

    int x[n+1];
    for(int i=0; i<n+1; i++)x[i] = i;

    res = sem_init(&q1_sem, 0, 0); //q1 er jonno banalam ebong 1 kore rakhlam fole resource sobai porte parbe
    if (res != 0)
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    res = sem_init(&q2_sem, 0, 0); //q1 er jonno banalam ebong 1 kore rakhlam fole resource sobai porte parbe
    if (res != 0)
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_mutex_init(&q2_mutex, NULL);
    if (res != 0)
    {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&q2_mutex);

    res = pthread_mutex_init(&approved_sid_mutex, NULL);
    if (res != 0)
    {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }




    m = n + 5;

    for(int i=0; i<m; i++)
    {
        int p = rand()%n+1;


        rc=pthread_create(&threads[p],NULL,student_thread,(void *)&x[p]);
        if(rc)
        {
            printf("Error! return code from pthread_create() is %d\n",rc);
            exit(-1);
        }
    }



    res = pthread_create(&a_thread, NULL, A_thread, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&e_thread, NULL, E_thread, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&c_thread, NULL, C_thread, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

//    res = pthread_create(&b_thread, NULL, B_thread, NULL);
//    if (res != 0)
//    {
//        perror("Thread creation failed");
//        exit(EXIT_FAILURE);
//    }



    printf("\nWaiting for threads to finish...\n");


    res = pthread_join(a_thread, &thread_result);
    if (res != 0)
    {
        perror("Thread A join failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_join(e_thread, &thread_result);
    if (res != 0)
    {
        perror("Thread E join failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_join(c_thread, &thread_result);
    if (res != 0)
    {
        perror("Thread C join failed");
        exit(EXIT_FAILURE);
    }


    int tot = approved_students.size();
    cout<<"finally in approved students ...\n";
    for(int i=0; i<tot; i++)cout<<approved_students[i]<<" ";

    res = pthread_create(&b_thread, NULL, B_thread, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }



    res = pthread_join(b_thread, &thread_result);
    if (res != 0)
    {
        perror("Thread B join failed");
        exit(EXIT_FAILURE);
    }









    tot = approved_students.size();
    cout<<"finally in approved students ...\n";
    for(int i=0; i<tot; i++)cout<<approved_students[i]<<" ";








    return 0;
}




void * E_thread(void *arg)
{


    printf("It's thread E\n");


    while(q1_elements >0)
    {
        int element = q1[q1_elements];
        q1_elements--;

        printf("E choosed  %d\n", element);
        approved_students.push_back(element);
        sem_post(&q1_sem);
        Sleep(1);
    }


    pthread_exit(NULL);
}

void * C_thread(void *arg)
{


    printf("It's thread C\n");


    while(q1_elements >0)
    {
        int element = q1[q1_elements];
        q1_elements--;

        printf("C choosed  %d\n", element);

        approved_students.push_back(element);
        sem_post(&q1_sem);
        Sleep(1);
    }
    pthread_exit(NULL);
}


void * A_thread(void *arg)
{


    printf("It's thread A\n");


    while(q1_elements >0)
    {
        int element = q1[q1_elements];
        q1_elements--;
        printf("A choosed  %d\n", element);

        approved_students.push_back(element);
        sem_post(&q1_sem);
        Sleep(1);
    }


    pthread_exit(NULL);
}

void * B_thread(void *arg)
{
    printf("It's thread B\n");

    while(m-- )
    {

        while(q2==0)
        {
            pthread_mutex_unlock(&q2_mutex);
        }

        int elem = q2;
        q2 = 0;
        printf("Teacher B found a request from student %d \n" , elem);


        int loop = approved_students.size();
        sort(approved_students.begin(),approved_students.end());

        vector<int> temp;
        int flag = 1;
        for(int i=1;i<loop;i++)
        {
            if(approved_students[i]==approved_students[i-1] && approved_students[i]==elem)
            {
                flag = 0;
                break;
            }
        }

        if(flag)
        {
            printf("Student %d has no duplicate application :D \n",elem);
            status_of_students[elem] = 1;
        }
        else
        {
            printf("Student %d has duplicate application :/ so his applications will be discarded now \n",elem);
            for(int i=0;i<loop;i++)
            {
                if(approved_students[i]!=elem)
                {
                    temp.push_back(approved_students[i]);
                }
            }
            approved_students.clear();
            for(int i=0;i<temp.size();i++)
            {
                approved_students.push_back(temp[i]);
            }
        }
    }

    pthread_mutex_unlock(&q2_mutex);

    pthread_exit(NULL);
}










