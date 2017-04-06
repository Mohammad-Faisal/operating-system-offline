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
int q3=0;

int n,m;

int q1_elements=0;

vector< int > approved_students;

map<int, string> sid_pass;

sem_t q1_sem, q2_sem, q3_sem, abc_sem;
pthread_mutex_t q2_mutex, q3_mutex,approved_sid_mutex;

void * A_thread(void *arg);
void * B_thread(void *arg);
void * E_thread(void *arg);
void * C_thread(void *arg);


int rc, res;
void *thread_result;
pthread_t threads[50];
int x[50];
int status_of_students[50] = {};

int check()
{
    for(int i=1; i<=n; i++)
    {
        if(status_of_students[i]==0)return 0;
    }
    return 1;
}

void * student_thread(void *threadid)
{

    int *tid;
    tid=(int *)threadid;
    printf("It's student #%d\n",*tid);

    if(q1_elements==10)
    {
        printf("student %d is waiting...\n\n", *tid);
        sem_wait(&q1_sem);
    }


    printf("Wait is over for student %d \n\n", *tid);


    q1_elements++;
    q1[q1_elements] = *tid;

    //Sleep(1);

    //ekhn student tar request niye b er kache jabe

    pthread_mutex_lock(&q2_mutex);
    q2 = *tid;

    //Sleep(1);

    //now he will ask d to provide his password


    while(status_of_students[*tid] != 1)
    {
        sem_wait(&q3_sem);
        q3 = *tid;
        sem_post(&q3_sem);
    }

    pthread_exit(NULL);
}


void * D_thread(void *arg)
{


    printf("It's Teacher D\n\n\n");

    int temp= 3;

    while(check()!=1 && temp--)
    {
        sem_wait(&q3_sem);

        int now = q3;



        if(status_of_students[now] == 1)
        {
            printf("Student no %d gets his password\n",now);
            cout<<"his password is   "<<sid_pass[now]<<endl;
        }
        else
        {
            printf("Student no %d didnt  get his password\n\n",now);
        }

        sem_post(&q3_sem);
       // Sleep(1);
    }


    pthread_exit(NULL);
}

int main()
{

    //for(int i=1;i<=n;i++)cout<<i<<" "<<sid_pass[i]<<endl;

    cin>>n;

    for(int i=1; i<=n; i++)sid_pass[i] = "u";


    pthread_t a_thread, e_thread, c_thread, b_thread,d_thread ;


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

    res = sem_init(&q3_sem, 0, 0); //q1 er jonno banalam ebong 1 kore rakhlam fole resource sobai porte parbe
    if (res != 0)
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    sem_post(&q3_sem);

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


    res = pthread_mutex_init(&q3_mutex, NULL);
    if (res != 0)
    {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }






    m = n + 5;

    for(int i=1; i<=n; i++)
    {
        int p = i;
        rc=pthread_create(&threads[p],NULL,student_thread,(void *)&x[p]);
        if(rc)
        {
            printf("Error! return code from pthread_create() is %d\n",rc);
            exit(-1);
        }
    }


    for(int i=0; i<5; i++)
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


    res = pthread_create(&b_thread, NULL, B_thread, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&d_thread, NULL, D_thread, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }



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




//    int tot = approved_students.size();
//    cout<<"finally in approved students ...\n";
//    for(int i=0; i<tot; i++)cout<<approved_students[i]<<" ";






    res = pthread_join(b_thread, &thread_result);
    if (res != 0)
    {
        perror("Thread B join failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(d_thread, &thread_result);
    if (res != 0)
    {
        perror("Thread D join failed");
        exit(EXIT_FAILURE);
    }



//    tot = approved_students.size();
//    cout<<"finally in approved students ...\n";
//    for(int i=0; i<tot; i++)cout<<approved_students[i]<<" ";

    for(int i=1;i<=n;i++)cout<<i<<" "<<sid_pass[i]<<endl;


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
        pthread_mutex_lock(&approved_sid_mutex);
        approved_students.push_back(element);
        pthread_mutex_unlock(&approved_sid_mutex);
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

        pthread_mutex_lock(&approved_sid_mutex);
        approved_students.push_back(element);
        pthread_mutex_unlock(&approved_sid_mutex);

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

        pthread_mutex_lock(&approved_sid_mutex);
        approved_students.push_back(element);
        pthread_mutex_unlock(&approved_sid_mutex);

        sem_post(&q1_sem);
        Sleep(1);
    }


    pthread_exit(NULL);
}





void * B_thread(void *arg)
{
    printf("It's Teacher B\n");

    while(m-- )
    {
        while(q2==0)
        {
            pthread_mutex_unlock(&q2_mutex);
        }

        int elem = q2;
        q2 = 0;
        printf("Teacher B found a request from student %d \n", elem);


        //b will try to lock approved students vector and perform copy check
        pthread_mutex_lock(&approved_sid_mutex);

        int loop = approved_students.size();
        sort(approved_students.begin(),approved_students.end());

        vector<int> temp;
        int flag = 1;
        int ase_naki = 0;
        for(int i=1; i<loop; i++)
        {
            if(approved_students[i]==approved_students[i-1] && approved_students[i]==elem)
            {
                flag = 0;
                break;
            }
            if(approved_students[i] == elem)ase_naki = 1;
        }
        if(flag && ase_naki)
        {
            printf("Student %d has no duplicate application :D \n",elem);
            status_of_students[elem] = 1;
            sid_pass[elem] = "random";
        }
        else if(!ase_naki)
        {
            printf("Student %d dont have his application  in the queue right now:D \n",elem);
        }
        else
        {
            printf("Student %d has duplicate application :/ so his applications will be discarded now \n",elem);
            for(int i=0; i<loop; i++)
            {
                if(approved_students[i]!=elem)
                {
                    temp.push_back(approved_students[i]);
                }
            }
            approved_students.clear();
            for(int i=0; i<temp.size(); i++)
            {
                approved_students.push_back(temp[i]);
            }

            int p = elem;

            int rc=pthread_create(&threads[p],NULL,student_thread,(void *)&x[p]);
            if(rc)
            {
                printf("Error! return code from pthread_create() is %d\n",rc);
                exit(-1);
            }

        }
        pthread_mutex_unlock(&approved_sid_mutex);
        pthread_mutex_unlock(&q2_mutex);

    }



    pthread_exit(NULL);
}




