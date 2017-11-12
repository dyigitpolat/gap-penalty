#include "stdio.h"
#include <vector>
#include <inttypes.h>

FILE* input;
std::vector<char> seq_1;
std::vector<char> seq_2;

std::vector<uint8_t> enc1;
std::vector<uint8_t> enc2;

int sm[16] = { 4, -2, -3, -1,
              -2,  4, -1, -3, 
              -3, -1,  4, -2,
              -1, -3, -2,  4 };

int *naive_scores;
int nsh, nsw;
uint8_t *naive_moves; //from (0:diagonal 1:up 2:left)

void parse_input()
{
    char line[512];

    int seq = 0;
    while(fscanf(input, "%s", line) != EOF)
    {
        //printf("%s\n", line);
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

    for(int i = 0; seq_1[i]; i++)
    {
        switch(seq_1[i])
        {
            case 'A': enc1.push_back(0); break;
            case 'T': enc1.push_back(1); break;
            case 'C': enc1.push_back(2); break;
            case 'G': enc1.push_back(3); break;
            default: break;
        }
    }

    for(int i = 0; seq_2[i]; i++)
    {
        switch(seq_2[i])
        {
            case 'A': enc2.push_back(0); break;
            case 'T': enc2.push_back(1); break;
            case 'C': enc2.push_back(2); break;
            case 'G': enc2.push_back(3); break;
            default: break;
        }
    }
}

int sim( uint8_t a, uint8_t b)
{
    return sm[a*4 + b];
}

int naive_calc(int i, int j)
{
    int diag = naive_scores[(i-1)*nsw + (j-1)] 
               + sim(enc2[i-1],enc1[j-1]);
    int up = naive_scores[(i-1)*nsw + (j)] - 4;
    int left = naive_scores[(i)*nsw + (j-1)] - 4;

    int move = 0;
    int max = 0;
    if(diag > max){ max = diag; move = 0;}
    if(up   > max){ max = up;   move = 1;}
    if(left > max){ max = left; move = 2;}

    naive_moves[i*nsw + j] = move;
    return max;
}

void align_naive()
{
    FILE* output;
    char str1[65536]; //if this exceeds we are fucked
    char str2[65536]; //if this exceeds we are fucked
    //alloc
    nsw = enc1.size()+1;
    nsh = enc2.size()+1;
    naive_scores = new int[nsh*nsw];
    naive_moves = new uint8_t[nsh*nsw];

    //init
    for( int i = 0; i < nsh; i++)
    {
        //naive_scores[i*nsw] = i * (-4);
        naive_scores[i*nsw] = 0;
    }
    for( int j = 0; j < nsw; j++)
    {
        //naive_scores[j] = j * (-4);
        naive_scores[j] = 0;
    }

    int max = 0;
    int mind = 0;
    for( int i = 1; i < nsh; i++)
    {
        for( int j = 1; j < nsw; j++)
        {
            int score = naive_calc(i, j);
            naive_scores[i*nsw + j] = score;
            if(score > max) 
            {
                max = score;
                mind = i*nsw + j;
            }
        }      
    }

    int score = naive_scores[mind];
    int i = mind/nsw;
    int j = mind%nsw;
    int x = 0;
    int y = 0;
    while(score && i >= 0 && j >= 0)
    {
        if(naive_moves[i*nsw + j] == 0)
        {
            printf("%c %c: %d\n", seq_1[j-1], seq_2[i-1], score);
            str1[x++] = seq_1[j-1];
            str2[y++] = seq_2[i-1];
            i--;
            j--;
            score = naive_scores[i*nsw + j];
            continue;
        }

        if(naive_moves[i*nsw + j] == 1)
        {
            printf("- %c: %d\n", seq_2[i-1], score);
            str1[x++] = '-';
            str2[y++] = seq_2[i-1];
            i--;
            score = naive_scores[i*nsw + j];
            continue;
        }

        if(naive_moves[i*nsw + j] == 2)
        {
            printf("%c -: %d\n", seq_1[j-1], score);
            str1[x++] = seq_1[j-1];
            str2[y++] = '-';
            j--;
            score = naive_scores[i*nsw + j];
            continue;
        }
    }
    str1[x] = 0;
    str2[y] = 0;
    for( int i = 0; i < x; i++)
    {
        str1[i] = str1[x-i-1];
    }

    for( int i = 0; i < y; i++)
    {
        str2[i] = str2[y-i-1];
    }

    printf("%d\n", max);
    printf("%s\n", str1);
    printf("%s\n", str2);

    output = fopen("naiveGap.txt", "w");
    fprintf(output, "%d\n", max);
    fprintf(output, "%s\n", str1);
    fprintf(output, "%s\n", str2);
}

void align_affine()
{
    FILE* output;

}

int main()
{
    input = fopen("sequences.fasta", "r");
    parse_input();
    align_naive();

    return 0;
}