#include <stdio.h>

typedef struct student {
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} Student;

typedef struct tagged_student1 {
    int id;
    char name[100];
    int age;
    double height;
    double weight;
    char tag;
} TStudent1;

typedef struct tagged_student2 {
    char tag;
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} TStudent2;

int main (int argc, char**argv){
    Student s_begin;
    Student s1;
    TStudent1 s2;
    TStudent2 s3;
    TStudent2 s_end;
  
    /* 以下に各構造体の先頭アドレスを表示するプログラムを書く */
    /* Hint: printf で %p フォーマットを指定する*/
    /* 逆順に表示（s_end, s3, s2, s1, s_begin の順）*/
    /*printf("s_end: %p\n", &s_end);
    printf("s3: %p\n", &s3);
    printf("s2: %p\n", &s2);
    printf("s1: %p\n", &s1);
    printf("s_begin: %p\n", &s_begin);*/
    
    // do_something!!
    /*printf("address of each member in Student\n");
    printf("address of weight: %p\n", &s_begin.weight);
    printf("address of height: %p\n", &s_begin.height);
    printf("address of age: %p\n", &s_begin.age);
    printf("address of name: %p\n", &s_begin.name);
    printf("address of id: %p\n", &s_begin.id);*/

    printf("address of each member in TStudent1\n");
    printf("address of tag: %p\n", &s2.tag);
    printf("address of weight: %p\n", &s2.weight);
    printf("address of height: %p\n", &s2.height);
    printf("address of age: %p\n", &s2.age);
    printf("address of name: %p\n", &s2.name);
    printf("address of id: %p\n", &s2.id);
    
    
    /* 以下には各構造体のサイズをsizeof演算子で計算し、表示する */
    /* printf 表示には%zu を用いる*/
    /*printf("size of s_end: %zu\n", sizeof(s_end));
    printf("size of s3: %zu\n", sizeof(s3));
    printf("size of s2: %zu\n", sizeof(s2));
    printf("size of s1: %zu\n", sizeof(s1));
    printf("size of s_begin: %zu\n", sizeof(s_begin));*/
    
    // do_something!!
    /*printf("size of each member in Student\n");
    printf("size of id: %zu\n", sizeof(s_begin.id));
    printf("size of name: %zu\n", sizeof(s_begin.name));
    printf("size of age: %zu\n", sizeof(s_begin.age));
    printf("size of height: %zu\n", sizeof(s_begin.height));
    printf("size of weight: %zu\n", sizeof(s_begin.weight));*/
    
    return 0;
}
