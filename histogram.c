#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>



// These are the bins from which histogram would be calculated
// By default they are defined at the gap of 128, but you can change these as you wish.
int bins[33] = {0, 128, 256, 384, 512, 640, 768, 896, 1024, 1152, 1280, 1408, 1536, 1664, 1792, 1920, 2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968, 4096}; 



// Finds on which bucket a given pixel value lies
int find_bucket(int value)
{
    for(int i=0;i<33;i++){
        if(value<bins[i]){
            if(i==0){
                printf("Error calculating histogram bad Input data recieved\n");
                exit(0);
            }
            return i-1;
        }
    }
}


// Prints the histogram 
void print_histogram(int **histogram)
{
    for(int i=0; i<32;i++){
        printf("%d ", i+1);
        for(int j=0;j<4;j++){
            printf("%d ",histogram[i][j]);
        }
        printf("\n");
    }
}


// Calculates the histogram , takes the memoy mapped file pointer and the histogram as input
void calculate_histogram(unsigned char *fileinmemory, int **histogram)
{
    for(int i=0;i<3072;i++){
        for(int j=0;j<6144;j+=3){

            //Calculate the value of the two pixels either they would be R,G or G,B
            unsigned short first_pixel = fileinmemory[i*6144+j] << 4 | (fileinmemory[i*6144+j+1] & 0xF0)>>4;
            unsigned short second_pixel = (fileinmemory[i*6144+j+1] & 0x0F)<<8 | fileinmemory[i*6144+j+2];
            
            //Finds bucket and fill the give values, if the row is RGRGRG... then i%2 would be zero
            //and if the row is GBGBGB... then i%2 would be 1
            histogram[find_bucket(first_pixel)][2*(i%2)]++;
            histogram[find_bucket(second_pixel)][(2*(i%2))+1]++;
        }
    }
}

int main(int argc, char **argv)
{
    if(argc<2){
        printf("No file provided, Please provide a raw12 file in the argument\n");
        return 0;
    }
    int inputfile = open(argv[1], O_RDONLY | O_APPEND);
    if (inputfile < 0){
        printf("Error opening file %s \n Is this a valid raw12 file? \n", argv[1]);
        return 0;
    }

    // Size of a raw12 image in bytes
    int size = 1024*1024*18;

    // The Four columns of the Histogram are RGGB and 32 rows are the buckets
    int **histogram = (int **)calloc(32, sizeof(int *));
    for(int i=0;i<32;i++){
        histogram[i] = (int *)calloc(4,sizeof(int));
    }

    unsigned char *fileinmemory = mmap(NULL, size, PROT_READ, MAP_SHARED, inputfile, 0);
    close(inputfile);

    calculate_histogram(fileinmemory, histogram);
    
    print_histogram(histogram);
    munmap(fileinmemory, size);

    return 0;
}