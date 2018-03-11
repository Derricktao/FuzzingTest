#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct {
        char* name;
        int index;
        int frequency;
}tweeter;

//get the text field of the tweet
char* getfield(char* line, int Users) {
        char* tok;
        for (tok = strtok(line, ","); tok!=NULL; tok = strtok(NULL, ",\n")) {
                if (!Users--) {
                        return tok;
                }
        }
        return NULL;
}
/*
   "A valid input csv file has columns separated with “,”’s, but you cannot assume the location of the
   tweeter column will be fixed to a particular location (like 8 for instance)."

   so we have to manually get the name index.
 */
int get_name_index(char* line){
        char* tok;
        int Users  = 0;

        for (tok = strtok(line, ","); tok!=NULL; tok = strtok(NULL, ",\n")) {
                while (tok != NULL) {
                        if (strcmp(tok, "\"name\"") == 0) {
                                return Users;
                        }
                        Users++;
                        tok = strtok(NULL, ",\n");
                }
        }
        return -1;
}

int alphabetically(const void *string1, const void *string2) {
        tweeter* tweeter1 = (tweeter*)string1;
        tweeter* tweeter2 = (tweeter*)string2;
        return strcoll(tweeter1->name, tweeter2->name);
}

int frequencily(const void *string1, const void *string2) {
        tweeter* tweeter1 = (tweeter*)string1;
        tweeter* tweeter2 = (tweeter*)string2;
        if (tweeter1->frequency < tweeter2->frequency) return -1;
        else if(tweeter1->frequency == tweeter2->frequency) return 0;
        else return 1;
}

void sort_by_frequency(char** names, int size){
        tweeter Users[size];
        for(int i = 0; i<size; ++i)  {
                Users[i].index = i;
                Users[i].frequency = 0;
                Users[i].name = names[i];
        }
        //first sort those names alphabetically
        qsort(Users, size, sizeof(tweeter), alphabetically);
        //then count the frequency of each name
        Users[0].frequency = 1;
        for (int i = 1; i<size; ++i)  {
                if(strcmp(Users[i].name,Users[i-1].name) == 0) {
                        Users[i].frequency += Users[i-1].frequency + 1;
                        Users[i-1].frequency = -1;
                        Users[i].index = Users[i-1].index;
                }
                else Users[i].frequency = 1;
        }
        //then sort those names based on their frequency
        qsort(Users, size, sizeof(tweeter), frequencily);
        //filter those who has frequency -1
        for(int i = size-1, index = 0; i >= 0; --i) {
                if(Users[i].frequency != -1)
                        for(int j=0; j<Users[i].frequency; ++j) {
                                names[index++] = Users[i].name;
                        }
        }
        //print final answer
        for(int i = size-1; i >=size-10 ; i--) {
                        printf("%s %d\n", Users[i].name, Users[i].frequency);
        }
}

int main(int argc, char** argv)
{
        char** names = malloc(sizeof(char*)*20000);
        int array_size;
        if(argc <= 1) {
                fprintf(stderr, "Invalid Input Format\n");
                exit(1);
        }
        //printf("Input file: %s\n", argv[1]);
        FILE* stream = fopen(argv[1], "r");
        char firstLine[1024];
        fgets(firstLine, 1024, stream);
        char* temp = strdup(firstLine);
        int name_index = get_name_index(temp);
        char line[1024];
        int index = 0;
        while (fgets(line, 1024, stream)){
                char* tmp = strdup(line);
                char* name = getfield(tmp, name_index);
                memmove(&name[0], &name[1], strlen(name));
                name[strlen(name)-1] = '\0';
                names[index] = (char *) malloc(100);
                strcpy(names[index], name);
                index++;
        }
        array_size = index;
        sort_by_frequency(names, array_size);
}
