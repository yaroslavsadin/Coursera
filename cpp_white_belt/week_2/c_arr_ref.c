#include <stdio.h>

const char name[] = "Yaroslav";

struct Person {
    char name[10];
    char surname[10];
    int age;
};

__attribute__((noinline)) void print_name(struct Person person) {
    printf("%s\n",person.name);
}

int main(void) {
    struct Person person = {"Yaroslav", "Sadin", 28};
    print_name(person);
    printf("%s\n",person.name);
    return 0;
}