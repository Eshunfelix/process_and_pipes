// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child back to parent 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char input_str[100]; 
    char second_input[100]; 
    pid_t p; 
  
    if (pipe(fd1) == -1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2) == -1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate: "); 
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        close(fd1[0]);  // Close reading end of first pipe 
        close(fd2[1]);  // Close writing end of second pipe 
  
        // Write input string and close writing end of first pipe 
        write(fd1[1], input_str, strlen(input_str) + 1); 
        close(fd1[1]); 
  
        // Wait for child to send a concatenated string 
        wait(NULL); 
  
        // Read concatenated string from child and print 
        char concat_str[200]; 
        read(fd2[0], concat_str, 200); 
        printf("Concatenated string from child: %s\n", concat_str); 
        close(fd2[0]); 
  
        // Parent receives second input from user
        printf("Enter another string to concatenate: "); 
        scanf("%s", second_input); 
  
        strcat(concat_str, second_input); 
        strcat(concat_str, "gobison.org"); 
  
        printf("Final concatenated string: %s\n", concat_str); 
    } 
  
    // Child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipe 
        close(fd2[0]);  // Close reading end of second pipe 
  
        // Read string from parent 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
        close(fd1[0]); 
  
        // Concatenate fixed string "howard.edu" 
        strcat(concat_str, fixed_str); 
  
        // Write concatenated string and close writing end of second pipe 
        write(fd2[1], concat_str, strlen(concat_str) + 1); 
        close(fd2[1]); 
  
        exit(0); 
    } 
} 
