/*
Title: FIT3143 Assignment1
Author: LIANG DIZHEN
StudentID: 31240291
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include<stdbool.h>
#include<time.h>
#include<omp.h>


// Function prototype
int* ReadFromFile(char *pFilename, int *pOutRow, int *pOutCol);
void WriteToFile(char *pFilename, int *pMatrix, int inRow, int inCol);
void print_2d_array(int **arr, int nrow, int ncol);
void print_1d_array(int *arr, int n) ;
void free_2d_array(int **arr, int nrow);
char *rev_str(char *s);
int *rev_arr(int *s);
void test();
int* ReadFileChar(FILE *wFILE, FILE *rFILE, char *pat);
int *append(int *arr, int size, int ele);
int **bad_chara(char *pattern);
int *z_array(char *s);
int *z_rev(char *s);
int *good_suffix(char *pat);
int *match_prefix(char *s);
long getFileSize(FILE *file);
char **readPat(int num_pat, char *filename);
char *readText(char *filename);
void free_2d_char_array(char **arr, int nrow);




int boyer_moore(char *t, char *p) {
    
    int **bc = bad_chara(p);
    int *gs = good_suffix(p);
    int *mp = match_prefix(p);
    int m = strlen(p);
    int n = strlen(t);
    int i = 0;

    int size = 0;
    // A dynamic array to store the pattern indices
     // The size of the dynamic array
    //int *pattern_indices = (int *)calloc(size,sizeof(int));

    //calloc and malloc would dynamically assign the memory space
    //auto-add /0 null ad the end
    // If length for the rest of text is shorter than the pattern, then no need to compare
    while (i <= n - m) {
        // Counter to record the number of matching characters
        // Reinitialize all values in case they are not updated

        int count = 0;
        int bs = 0;
        int gss = 0;
        int ms = 0;


        // Immediately stop the while if there is either a mismatched
        // Or fully matched
        //printf("%c", t[0]);
        while (count < m && t[i + m - 1 - count] == p[m - 1 - count]) {
            // Use the bind to gradually compare letter from end back
            // To start by incrementing count
            count++;
    
        }
        // If == -> fully matched
        if (count == m) {
            //pattern_indices = append(pattern_indices, size, i);
            //print result into a txt file
            size++;
            //printf("%*d", 2, i);
            //fprintf(wFile, "%*d", 2, i);
            //printf("%*d\n", y, x); // prints "   10" with y spaces before the number
            //printf("%-*d\n", y, x); // prints "10   " with y spaces after the number
            //size++;
            int ful_shift = m - mp[1]; //[0] is the length of the pattern
            i += ful_shift;
        } 
        else {
            int shift;
            // Index of mismatch character in pattern
            int mis_i = m - count - 1;
            // Corresponding mismatch character in text
            int mis_c = t[i + mis_i] - 'a';
            
            // See if there is any the same in the pattern 
            // And search it in corresponding row in the table
            if (bc[mis_c][mis_i] == 0) {
                bs = 0;
            } else { // if not, Shift whole pattern exactly bypass this letter
                // By +1
                bs = mis_i - bc[mis_c][mis_i] + 1;
            }
            
            // Good suffix shift
            // Mis_i + 1 get the first index of the matching substring
            // Which is after mis_i (mismatch index in pattern)
            
            gss = m - gs[mis_i + 1];

            if (gss == 0){
                ms = m - mp[mis_i+1];
                
                if (ms > bs){
                    shift = ms;
                }
                else{
                    shift = bs;
                }
            }
            else
            {
                if (bs > gss){
                    shift = bs;
                }
                else{
                    shift = gss;
                }
            }

            //see which give the longest shift
            i += shift;

        }
    }
    //printf("\n");
    //fprintf(file, "]\n");
    // free(gs);
    // free(mp);
    // free_2d_array(bc, 26); // Free memory allocated
    //free(pattern_indices);
    return size;
}



int main() {
    struct timespec start, end, startComp, endComp; 
	double time_taken; 
//sed 's/\n//g' lower_case_txt.txt > txt.txt
//remove all characters except lowercase letters and newline
//tr -d ' ' < txt.txt > ttxt.txt
//remove all newline letters
    clock_gettime(CLOCK_MONOTONIC, &start); 
    int size = 0;
    int pat_size;
    pat_size = 2000;
    //Cell product complete - Computational time only(s): 138.394561
    //Overall time (Including read, product and write)(s): 138.397771
  //size = boyer_moore("fefefefe", "fe");
  //printf("%d\n", size);

    //test();
    char **pat_2d_array = readPat(pat_size, "query.txt");
    char *txt_1d_array = readText("txt.txt");
//   printf("%lu",sizeof(txt_1d_array));
//   printf("\n");
//   printf("%lu",sizeof(pat_2d_array));
    //unsigned int randomSeed;

    //randomSeed = omp_get_thread_num() * time(NULL);
    clock_gettime(CLOCK_MONOTONIC, &startComp); 


    for (int i = 0; i < pat_size; i++)
    {
        //printf("%s", pat_2d_array[i]);//i > cn - certain number, but par_2d_array[cn]
        //so segmentation fault
        //size = boyer_moore("fefefefe", "fe");
        size += boyer_moore(txt_1d_array,pat_2d_array[i]);
    }
        
    
    clock_gettime(CLOCK_MONOTONIC, &endComp); 
    time_taken = (endComp.tv_sec - startComp.tv_sec) * 1e9; 
    	    time_taken = (time_taken + (endComp.tv_nsec - startComp.tv_nsec)) * 1e-9; 
    printf("Boyer Moore - Computational time only(s): %lf\n", time_taken); // portion of the computing time of ts

    //printf("%s", txt_1d_array);
    printf("%d\n", size);
    free(txt_1d_array);
    free_2d_char_array(pat_2d_array,pat_size);
    clock_gettime(CLOCK_MONOTONIC, &end); 
    
	time_taken = (end.tv_sec - start.tv_sec) * 1e9; 
    	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9; 
	printf("Overall time (Including read, product and write)(s): %lf\n", time_taken);	// ts
    return 0;
}

//from prelab2 in week4
int* ReadFileInt(char *pFilename, int *pOutRow, int *pOutCol)
{
	int i, j;
	int row, col;
	FILE *pFile = fopen(pFilename, "r");
	if(pFile == NULL)
	{
		printf("Error: Cannot open file\n");
		return 0;
	}

	fscanf(pFile, "%d%d", &row, &col);
	int *pMatrix = (int*)malloc(row * col * sizeof(int)); // Heap array

	// Reading a 2D matrix into a 1D heap array
	for(i = 0; i < row; i++){
	    for(j = 0; j < col; j++){
		fscanf(pFile, "%d", &pMatrix[(i * col) + j]);
		}
	}
	fclose(pFile);

	*pOutRow = row; // Dereferencing the pointer
	*pOutCol = col; // Dereferencing the pointer
	return pMatrix;
}


char *readText(char *filename) {
    FILE *file = fopen(filename, "r");

    long fileSize = getFileSize(file);
    // Allocate memory for text
    char *textStr = (char *) malloc(fileSize + 1 * sizeof(char)); //to put /0

    // Read the entire file into fileContent
    //fread to store a file into a array
    //string is array of characters
    //so store them in an array
    //array pointer, size of the character, number of characters to read
    
    fread(textStr, 1, fileSize, file);
    fclose(file);
    textStr[fileSize] = '\0'; // Null-terminate the string
    return textStr;
}

long getFileSize(FILE *file) {
  // address of the file start of the file stream
  long filePos = ftell(file);

  //move 0bytes from the end of the file
  //-> return end address of the file 
  fseek(file, 0L, SEEK_END);
  
  //ftell to give the bytes from the start of the file strea
  //to the file pointer (end of the file strea)
  long fileSize = ftell(file);

  //re-arrage pointer back to the start of file stream
  //to return lateron
  fseek(file, filePos, SEEK_SET);

  // Return the file size
  return fileSize;
}

char **readPat(int num_pat, char *filename){
    FILE *rfile = fopen(filename, "r");
    //buffer to store each line of string
    //each char pointer has 4 or 8 bytes dependent on the system
    //cast to char pointer, so pointer to pointers
    //filelength size of the strings (since char pointers)
    //table
    char **pat_2d_array = (char **)calloc(num_pat, sizeof(char *));
    //buffer to store the each line of pattern string
    char patStr[200];
    //fscanf by default stop at space character
    //not good at reading big block of strings
    int *size = 0;

    for (int i = 0; i < num_pat; i++)
    {
        fgets(patStr, sizeof(patStr), rfile);
        patStr[strcspn(patStr, "\n")] = '\0';
        pat_2d_array[i] = strdup(patStr);
        //boyer_moore("forfet", pat_2d_array[i], size);
        //printf("%s", pat_2d_array[i]);
        //printf("%d", *size);
    }
    fclose(rfile);
    return pat_2d_array;
}



//since only pattern pass in can just do on pattern
//for efficiency, this bad_chara function can only handle lowercase letters!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//grep -Eo '\b[a-z]+\n' query.txt > lower_case_only.txt
//grep -Eo '\b[a-z]+\s' clean_Shakespeare_dataset.txt > lower_case_txt.txt
// A function to create a table for bad character heuristic
int **bad_chara(char *pattern) {
    // Bad character that cause the mismatch found in text
    // Found bad character in the text, find 'the same character' of the left of it closest to the bad character in the pattern 
    // Then shift the pattern to the right until 'the same character' in the pattern is aligned with the bad character in the text
    int ncol = strlen(pattern);
    int nrow = 26;
    
    //create table with size of the nrow
    int **table = (int **)calloc(nrow, sizeof(int *));
    //each row of the table create an array with the side of column
    for (int i = 0; i < nrow; i++) {
        table[i] = (int *)calloc(ncol, sizeof(int));
    }
    int a = 97;
    int end = ncol - 1;
    // -1 to loop until 0
    for (int i = end; i >= 0; i--) {
        int row = pattern[i] - a;
        int col = i;
        table[row][col] = i; // 0-indexed
        // No the same letter ahead in pattern then just shift the whole pattern 
        // Past the none-repeating letter alone the text, that is why 
        while (col < ncol - 1) {
            if (table[row][col + 1] == 0) {
                // If right is 0 change into 1 continue until the end
                table[row][col + 1] = i + 1;
            }
            col++;
        }
    }
    return table; // If there is a matching letter in the pattern, shift the pattern to align the letter with
                  // The same letter in the text
}

// A function to create a Z-array for a given string
int *z_array(char *s) {
    int n = strlen(s);
    int *z = (int *)calloc(n, sizeof(int)); // Initialize Z-array with zeros
    // Loop through string
    for (int i = 0; i < n; i++) {
        // Initialize a counter for the length of matching characters
        int count = 0;
        // Compare characters until a mismatch 
        // Binding the count with the index (j index) to compare
        while (i + count < n && s[count] == s[i + count]) {
            count++;
        }
        // Update element in z-array at index of the start letter with of the len of longest matching substring
        z[i] = count;
    }
    return z;
}

// A function to create a reversed Z-array for a given string
int *z_rev(char *s) {
    int n = strlen(s);
    int *z = (int *)calloc(n, sizeof(int)); // Initialize Z-array with zeros
    // Do z-array from right to left for the longest matching pattern
    // Since z-array is heavily relies on the first character
    // Do from the end, would require to reverse
    // To put last character to the first
    char *rev_s = (char *)calloc(n, sizeof(char));
    strcpy(rev_s, s);
    rev_s = rev_str(rev_s); // Reverse the string
    //printf("Reversed S: %s\n", rev_s);
    // Loop through string
    for (int i = 0; i < n; i++) {
        // Initialize a counter for the length of matching characters
        int count = 0;
        // Compare characters until a mismatch 
        // Binding the count with the index (j index) to compare
        while (i + count < n && rev_s[count] == rev_s[i + count]) {
            count++;
        }
        // Update element in z-array at index of the start letter with of the len of longest matching substring
        z[i] = count;
    }
    free(rev_s); // Free memory allocated for reversed string
    int *z_r = (int *)calloc(n, sizeof(int));
    z_r = rev_arr(z);   // Reverse back z-array in correct order
    free(z);
    return z_r;
}


//formula from FIT3155 Lecturer notes
int *good_suffix(char *pat) {
    if (strlen(pat) == 0) {
        return NULL;
    }
    
    // Create a good suffix array
    int m = strlen(pat);
    int *gs = (int *)calloc((m + 1), sizeof(int));
    //printf("%d", m+1);
    
    //print_1d_array(gs,sizeof(gs));
    int *zr = z_rev(pat);
    //print_1d_array(zr,sizeof(zr));
    
    //fill the array with 0 #gs has longer array
    //only run until last 2 element
    for (int i = 0; i < m - 1; i++){
        int j = m - zr[i] + 1;
       // printf("%d", j-1);
        //printf("\n");
        gs[j-1] = i+1;
        //printf("%d", gs[j-1]);
    }
    free(zr);
    return gs;
    
    print_1d_array(gs,sizeof(gs));
    // Z-array[m] always = size of pattern
    // Z-array[m] which was the first character before reverse
    // Since first character only compare itself, so just store length of array for useful info
    
    for (int i = m-1; i >= 0; i--) {

        //printf("%d", i);
        int j = m - zr[i] + 1; // Len - z_array + 1 to give the index of the duplicate at the back
        //printf("%d\n",zr[i]);
        //printf("%d",i+1);
        // Adjust to start from index 0 
        gs[j - 1] = i + 1; // +1 to compensate the way of the formula

        // Gs record the v-value to deduct from length of pattern
        // M - v = positions to shift the pattern alone the text to skip other useless pattern matching
    }
    return gs;
}

// A function to create a match prefix array for a given string
int *match_prefix(char *s) { // Exact matching substring before the current substring in the pattern
    if (strlen(s) == 0) {
        return NULL;
    }
    int *z = z_array(s);
    // If z[i] = length of the string + 1
    int *len_array = (int *)calloc((strlen(s) + 1), sizeof(int)); 
    int end = strlen(s) - 1;
    //int count = 1;
    for (int i = end; i >= 0; i--) {
        if (z[i] + i == strlen(s)) { // For 0-indexed
            len_array[i] = z[i];
        } else {
            if (i != end) {
                len_array[i] = len_array[i + 1]; // 1. extra element for end element to the value at its right
            }
        }
    }
    free(z);
    return len_array;
}
char *rev_str(char *s) //pointer variable
{
    char * a = (char *)calloc(strlen(s), sizeof(char));
    int end = strlen(s) - 1;

    for (int i = 0; i < strlen(s); i++)
    {
        a[i] = s[end];
        //printf("%c", a[i]);
        end--;
        //printf("%d", end);

    }
    return a;
}

int *rev_arr(int *s) //pointer variable
{
    int * a = (int *)calloc(sizeof(s), sizeof(int));
    int end = sizeof(s) - 1;
    for (int i = 0; i < sizeof(s); i++)
    {
        a[i] = s[end];
        end--;
    }
    return a;
}
// A function to print a two-dimensional array
void print_2d_array(int **arr, int nrow, int ncol) {
    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

// A function to print a one-dimensional array
void print_1d_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// A function to free a two-dimensional array
void free_2d_array(int **arr, int nrow) {
    for (int i = 0; i < nrow; i++) {
        free(arr[i]);
    }
    free(arr);
}

void free_2d_char_array(char **arr, int nrow) {
    for (int i = 0; i < nrow; i++) {
        free(arr[i]);
    }
    free(arr);
}


