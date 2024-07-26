#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define LEN_OF_MSG 10 //primary length of allocated memory for message
#define ERROR_INPUT 100
#define ERROR_LEN_MSG 101
#define NUM_OF_CHAR 52 //number of character [a-z][A-Z]
  
//print message with no return
void print_msg(char *msg1, int length){
  for (int i = 0; i < length; i++){
    printf("%c", msg1[i]);
  } 
  printf("\n");
}
//read encoded message, return pointer where was message allocated
char *read_input(int *msg1_len){
  //allocation for encoded message
  char *msg = malloc(sizeof(char)*LEN_OF_MSG);
  if (msg == NULL){
    free(msg);
    exit(100);
  }
//reading encoded message and writing it in allocated memory
  int i = 0; 
  int len = LEN_OF_MSG;
  int c;
  while ((c = getchar()) != EOF && c != '\n'){
    if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z')){ //wrong input 
      free(msg);
      msg = NULL;
      return msg;
    }
    i++;
    if (i == len){
      len += 1;
      char *msg_2 = realloc(msg, sizeof(char)*len); //reallocation with size of one character
      if (msg_2 == NULL){ 
        free(msg_2);
        exit(100);
      }
      msg = msg_2;
    }
    msg[i-1] = c; //writing down the scanned number into string
  }
  *msg1_len = i; //lenght of message is number of iteration in while loop
  return msg;
}
//read partly intercepted message, return pointer where was message allocated
char *read_input_2(int *msg2_len){
  char *msg = malloc(sizeof(char)*LEN_OF_MSG);
  if (msg == NULL){
    free(msg);
    exit(100);
  }
 
  int i = 0; 
  int len = LEN_OF_MSG;
  int c;
  while ((c = getchar()) != EOF && c != '\n'){
    if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z')){
      free(msg);
      msg = NULL;
      return msg;
    }
    i++;
    if (i == len){
      len += 1;
      char *msg_2 = realloc(msg, sizeof(char)*len); 
      if (msg_2 == NULL){ 
        free(msg_2);
        exit(100);
      }
      msg = msg_2;
    }
    msg[i-1] = c;
  }
  *msg2_len = i;
  return msg;
}
//comparing shifted message with partly intercepted message - Hamming distance, return number of matching letters
int compare_msg(char *shifted_msg, char *msg2, int msg1_len){
  int matching_letters = 0;
  for (int i = 0; i < msg1_len; i++){
    if(shifted_msg[i] == msg2[i]){
      matching_letters += 1;
    }
  }
  return matching_letters;
}
//shifting message when inputs have same length, return pointer on shifted message with the best result of matching letters
char *shift_msg(char *msg1, char *msg2, char *data, int msg1_len, int msg2_len){
  //allocation for shifted message 
  char *shifted_msg = malloc(sizeof(char*)*msg1_len);
    if (shifted_msg == NULL){
        free(shifted_msg);
        exit(100);
    }
 
  int best_result = 0;
  int result = 0;
  //shift through whole alphabet
  for (int q = 0; q < NUM_OF_CHAR; q++){
    //for every letter in encoded message
    for (int j = 0; j < msg1_len; j++){
      //going through our alphabet
      for (int n = 0; n < NUM_OF_CHAR; n++){
        //finding same character in alphabet to move by one character
        if (msg1[j] == data[n]){
          //if its at the end of alphabet shift to beginning 
          if (n == (NUM_OF_CHAR - 1)){
            msg1[j] = data[0];
            break;
          }else{
            msg1[j] = data[n+1];
            break;
          }
        }
      }
    }
     
    result = compare_msg(msg1, msg2, msg1_len);
    //write down 
    if (result > best_result){
      for (int i = 0; i <= msg1_len; i++){
        shifted_msg[i] = msg1[i];
      }
      best_result = result;
    }
  }
   
  return shifted_msg;
}
 
//temporary function for finding the lowest number from replacement, delection, insertion
int findmin(int r, int d, int i){
  int min1, min2, min;
  min1 = r < d ? r : d;
  min2 = d < i ? d : i;
  min = min1 < min2 ? min1 : min2;
 
  return min;
}
//wagner-fisher algorithm, return lenvenshtein distance
int levenshtein_distance(char *msg1, char *msg2, int msg1_len, int msg2_len){
  int len_distance = 0;
  //board should be one character bigger to add empty character in the beginning
  int rows = msg1_len + 1;
  int columns = msg2_len + 1;
  int **board = (int **)malloc(rows*sizeof(int *)); //allocation of 1D array
    if (board == NULL){
      free(board);
      exit(100);
    }
  //allocating another dimension to 1D array
  for (int i = 0; i < rows; i++){
    board[i] = (int *)malloc(columns*sizeof(int));
    if (board[i] == NULL){
      free(board);
      exit(100);
    }
  }
  //board[0][0] = 0 - we are making empty character from empty character - do not to make anything
  //row and column with index 0 are just about replacing one character and adding another
  for (int i = 0; i <= msg1_len; i++){
    board[i][0] = i;
  }
  for (int j = 0; j <= msg2_len; j++){
    board[0][j] = j;
  }
  //changes we can do with our shifted string and partly intercepted message with different lenght
  int replacement, deletion, insertion, minimal;
 
  for (int r = 1; r <= msg1_len; r++){
    for (int c = 1; c <= msg2_len; c++){
      //when the characters are the same we solve the problem from diagonal result (do not need to change anything)
      if (msg1[r-1] == msg2[c-1]){
        board[r][c] = board[r-1][c-1];
      }else{
        //searching for the best change to reach the shifted message
        replacement = board[r-1][c-1];
        deletion = board[r-1][c]; 
        insertion = board[r][c-1];
        minimal = findmin(replacement, deletion, insertion) + 1;
        board[r][c] = minimal;
 
      }
    }
  }
   
  len_distance = board[msg1_len][msg2_len];
  for (int i = 0; i < rows; i++){
    free(board[i]);
  }
  free(board);
 
  return len_distance;
 
}
//shifting message when inputs do not have same length, return pointer on shifted message with the lowest Levenshtein distance
char *shift_msg_Lev(char *msg1, char *msg2, char *data, int msg1_len, int msg2_len){
   
  char *shifted_msg = malloc(sizeof(char*)*msg1_len);
    if (shifted_msg == NULL){
        free(shifted_msg);
        exit(100);
    }
 
  int best_result = msg2_len;
  int result = 0;
  for (int q = 0; q < NUM_OF_CHAR; q++){
    for (int j = 0; j < msg1_len; j++){
      for (int n = 0; n < NUM_OF_CHAR; n++){
       
        if (msg1[j] == data[n]){
         
          if (n == (NUM_OF_CHAR - 1)){
            msg1[j] = data[0];
            break;
          }else{
            msg1[j] = data[n+1];
            break;
          }
        }
      }
    }
    //updating the shifted message with the lowest Levenshtein distance
    result = levenshtein_distance(msg1, msg2, msg1_len, msg2_len);
    if (result < best_result){
      for (int i = 0; i <= msg1_len; i++){
        shifted_msg[i] = msg1[i];
      }
      best_result = result;
    }
  }
   
  return shifted_msg;
}
 
 
 
int main(int argc, char *argv[])
{
  int prp_opt;
  char expected_arg[] = "-prp-optional"; 
  //controlling argument -prp-optional
  if (argc > 1){
    prp_opt = strcmp(argv[1], expected_arg);
  }else{
    prp_opt = 1;
  }
   
  char *msg1, *msg2;
  msg1 = msg2 = NULL;
  int msg1_len, msg2_len;
  msg2_len = msg1_len = 0;
 
  msg1 = read_input(&msg1_len);
 
  if (msg1 != NULL){
    msg2 = read_input_2(&msg2_len);
  }
   
  if (msg1 == NULL || msg2 == NULL){
    free(msg1);
    free(msg2);
    fprintf (stderr, "Error: Chybny vstup!\n");
    return ERROR_INPUT;
  }
 
  if (msg1_len != msg2_len && prp_opt == 1){
    free(msg1);
    free(msg2);
    fprintf (stderr, "Error: Chybna delka vstupu!\n");
    return ERROR_LEN_MSG;  
  }
  //creating string with our alphabet [a-z][A-Z]
  char *data = malloc(sizeof(char)*NUM_OF_CHAR);
    if (data == NULL){
      exit(100);
    }
 
  int i = 0;
  char c;
  for (c = 'a'; c <= 'z'; c++){
    data[i] = c;
    i++;
  }
  for (c = 'A'; c <= 'Z'; c++){
    data[i] = c;
    i++;
  }
   
  char *decoded_msg; 
  if (prp_opt == 1){
    decoded_msg = shift_msg(msg1, msg2, data, msg1_len, msg2_len);
    print_msg(decoded_msg, msg1_len);
  }else{
    decoded_msg = shift_msg_Lev(msg1, msg2, data, msg1_len, msg2_len);
    print_msg(decoded_msg, msg1_len);
  }
 
 
  free(msg1);
  free(msg2);
  free(decoded_msg);
  free(data);
   
  return 0;
}
