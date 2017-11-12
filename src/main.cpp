#include "stdio.h"
#include <vector>

FILE* input;
std::vector<char> seq_1;
std::vector<char> seq_2;

void parse_input()
{
    char line[512];

    int seq = 0;
    while(fscanf(input, "%s", line) != EOF)
    {
        printf("%s\n", line);
        if(line[0] == '>')
        {
            seq++; 
            continue;
        }
        
        if(seq == 1)
        {
            int i = 0;
            while( line[i])
            {
                seq_1.push_back( line[i]);
                i++;
            }
        }   
        else
        {
            int i = 0;
            while( line[i])
            {
                seq_2.push_back( line[i]);
                i++;
            }
        }
    }

    seq_1.push_back(0);
    seq_2.push_back(0);
    printf("seq1 : %s\n", &seq_1[0]);
    printf("seq2 : %s\n", &seq_2[0]);
}

void align_naive()
{
    FILE* output;

}

void align_affine()
{
    FILE* output;

}

int main()
{
    input = fopen("sequences.fasta", "r");
    parse_input();

    return 0;
}