/*************************************************************************
* Program:                                                                 
*    Project Extra, Sudoku                                                 
*    Brother Ridges, CS124                                                 
* Author:                                                                  
*    Alexander Berryhill                                                   
* Summary:                                                                 
*    Sudoku will allow you to play a game of sudoku.                       
*                                                                          
*                                                                          
*                                                                          
*                                                                          
*    Estimated:  5.5 hrs                                                   
*    Actual:     6.5 hrs                                                   
*      The most difficult part was getting the input in interact to work   
*************************************************************************/

#include <fstream>
#include <iostream>
using namespace std;

/*************************************************************************
 * getIFile will get the input fileName                                    
 ************************************************************************/
void getIFile(char fileName[])
{
   cout << "Where is your board located? ";
   cin >> fileName;
   cin.ignore(256, '\n');
   return;
}

/*************************************************************************
 * readFile will read the input file and put it in a board.                
 ************************************************************************/
void readFile(char fileName[], char board[9][9], int plEdits[2][81])
{
   ifstream fin;
   fin.open(fileName);
   if (fin.fail())
      cout << "Could not open " << fileName << endl;
   for (int i = 0; i < 9; i++)
   {
      for (int j = 0; j < 9; j++)
      {
         fin >> board[i][j];
      }
   }
   for (int i = 0; i < 81 && fin >> plEdits[0][i]; i++)
      fin >> plEdits[1][i];
   for (int i = 0; plEdits[0][i]; i++)
   {
      cout << "(" << plEdits[1][i] << ", " << plEdits[0][i] << ")\n";
   }
   fin.close();
   return;
}

/*************************************************************************
 * getOFile will get an output fileName                                    
 ************************************************************************/
void getOFile(char fileName[])
{
       cout << "What file would you like to write your board to: ";
   cin >> fileName;
   cin.ignore(256, '\n');
   return;
}

/*************************************************************************
 * writeFile will save your game onto a file                               
 ************************************************************************/
void writeFile(char fileName[], char board[9][9], int plEdits[2][81])
{
   ofstream fout;
   fout.open(fileName);
   if (fout.fail())
      cout << "Could not open " << fileName << endl;
   for (int i = 0; i < 9; i++)
   {
      for (int j = 0; j < 9; j++)
      {
         fout << board[i][j];
      }
      fout << endl;
   }
   for (int i = 0; i < 81 && fout << plEdits[0][i]; i++)
      fout << ' ' << plEdits[1][i] << ' ';
   fout.close();
   return;
}

/*************************************************************************
 * save will save the game (used to be bigger)                             
 ************************************************************************/
 void save(char board[9][9], int plEdits[2][81], char fileName[])
 {
   getOFile(fileName);
   writeFile(fileName, board, plEdits);
   cout << "Board written successfully\n";
   return;
}

/*************************************************************************
 * empty will empty 1D arrays                                              
 ************************************************************************/
void empty(char array[])
{
   for (int i = 0; array[i]; i++)
   {
      array[i] = '\0';
   }
   return;
}

/*************************************************************************
 * isCoord will return if you can write on a coordinate                    
 ************************************************************************/
bool isCoord(char board[9][9], char x, char y)
{
   if ((y - 48) < 1 || (y - 48) > 9)
   {
      cout << "ERROR: Square \'" << x << y << "\' is invalid\n";
      return false;
   }
   else if (toupper(x) - 64 < 1 || toupper(x) - 64 > 9)
   {
      cout << "ERROR: Square \'" << x << y << "\' is invalid\n";
      return false;
   }
   else if (board[y - 49][toupper(x) - 65] != '0')
   {
      cout << "ERROR: Square \'" << x << y << "\' is filled\n";
      return false;
   }
   return true;
}

/*************************************************************************
 * getCoord will get a pair of coordinates                                 
 ************************************************************************/
bool getCoord(char board[9][9], char &x, char &y)
{
   cout << "What are the coordinates of the square: ";
   cin >> x;
   if (x > '9')
      cin >> y;
   else
   {
      y = x;
      cin >> x;
   }
   return isCoord (board, x, y);
}

/*************************************************************************
 * computeValues will compute what is allowed and display if prompted.     
 ************************************************************************/
void computeValues(char board[9][9], char canVal[], char x, char y,
                   bool promp)
{
   char notVal[3][9];
   if (promp)
   {
      getCoord(board, x, y);
   }
   int ix = toupper(x) - 65;
   int iy = y - 49;                //to int form int x and int y           

   for (int i = 0; i < 9; i++)     //column as 0                           
   {
      notVal[0][i] = board[i][ix];
   }
   for (int i = 0; i < 9; i++)     //row as 1                              
   {
      notVal[1][i] = board[iy][i];
   }
   int blockX = 0;                 //block as 2                            
   for (int i = ((iy / 3) * 3); i < ((iy / 3) * 3) + 3; i++)
   {
      for (int j = ((ix / 3) * 3); j < ((ix / 3) * 3) + 3; j++)
      {
         notVal[2][blockX++ ] = board[i][j];
      }
   }

   int canX = 0;
   empty(canVal);
   for (int n = 1; n <= 9; n++)
   {
      bool isN = true;
        char cn = '0' + n;
      for (int i = 0; i < 3; i++)
      {
         for (int j = 0; j < 9; j++)
         {
            if (notVal[i][j] == cn)
            {
               isN = false;
            }
            for (int k = 0; canVal[k]; k++)
            {
               if (notVal[i][j] == canVal[k])
                  isN = false;
            }
         }
      }
      if (isN)
         canVal[ canX++ ] = cn;
   }

   if (promp)
   {
      cout << "The possible values for \'" << x << y << "\' are: ";
      for (int i = 0; canVal[i]; i++)
      {
         cout << canVal[i];
         if (canVal[i + 1])
            cout << ", ";
      }
      cout << endl;
   }
   return;
}

/*************************************************************************
 * isValidNew will check if the new value is valid                         
 ************************************************************************/
bool isValidNew(char board[9][9], char n, char x, char y)
{
   char canVal[9];
   computeValues(board, canVal, x, y, false);

   if (n < '1' || n > '9')
   {
      cout << "ERROR: Value \'" << n << "\' in square \'" << x << y <<
         "\' is invalid\n";
      return false;
   }
   for (int i = 0; canVal[i]; i++)
   {
      if (canVal[i] == n)
      {
         return true;
      }
   }
   cout << "ERROR: Value \'" << n << "\' in square \'" << x << y <<
      "\' is invalid\n";
   return false;
}

/*************************************************************************
 * getNew will get a new value                                             
 ************************************************************************/
bool getNew(char board[9][9], char &n, char x, char y)
{
      cout << "What is the value at \'" << x << y << "\': ";
   cin >> n;
   return isValidNew (board, n, x, y);
}

/*************************************************************************
 * edit will edit one spot                                                 
 ************************************************************************/
void edit(char board[9][9], int plEdits[2][81])
{
   char x;
   char y;
   char n;
   int i = 0;

   if (!getCoord(board, x, y))
      return;
   if (!getNew(board, n, x, y))
      return;

   board[y - 49][toupper(x) - 65] = n;

   for (i = 0; plEdits[0][i]; i++); //to next open spot                    
   plEdits[0][i] = (y - 49);
   plEdits[1][i] = (toupper(x) - 65);

   return;
}

/*************************************************************************
 * display will display the board      
 ************************************************************************/
void display(char board[9][9], int plEdits[2][81])
{
   cout << "   A B C D E F G H I\n";
   for (int i = 0; i < 9; i++)
   {
      if ((i) % 3 == 0 && i != 0)
         cout << "   -----+-----+-----\n";
      cout << i + 1 << "  ";
      for (int j = 0; j < 9; j++)
      {
         char d;
         bool green = true;

         if (board[i][j] == '0')
            d = ' ';
         else
            d = board[i][j];

         for (int k = 0; plEdits[0][k]; k++)
         {
            if (plEdits[0][k] == i && plEdits[1][k] == j)
               green = false;
         }

         if (green)
            cout << "\E[22;32m";
         if (((j + 1) % 3) == 0 && j != 8)
            cout << d << "\E[0m|";
         else if (j == 8)
            cout << d << "\E[0m";
         else
            cout << d << "\E[0m ";
      }
      cout << endl;
   }
   return;
}

/*************************************************************************
 * options will display the options                                        
 ************************************************************************/
void options()
{

   cout << "Options:\n   ?  Show these instructions\n   D  Display the ";
   cout << "board\n   E  Edit one square\n   S  Show the possible values";
   cout << " for a square\n   Q  Save and Quit\n\n";
   return;
}

/*************************************************************************
 * interact will loop around taking input and reacting to it, when it      
 * ends the game ends.                                                     
 ************************************************************************/
void interact(char board[9][9], int plEdits[2][81], char fileName[])
{
   char choice;
   bool play = true;

   options();
   display(board, plEdits);

   while (play)
   {
            cout << "\n> ";
      cin >> choice;
      cin.ignore(256, '\n');
      switch (choice)
      {
         case '?':
            options();
            break;
         case 'D':
            display(board, plEdits);
            break;
         case 'E':
            edit(board, plEdits);
            break;
         case 'S':
            char canVal[9];                          //this and            
            computeValues(board, canVal, 0, 0, true);//0, 0, true so I can
            break;                                //use this in isValidNew
         case 'Q':
            save(board, plEdits, fileName);
            play = false;
            break;
         default:
            cout << "ERROR: Invalid command\n";
      }
   }
   return;
}

/*************************************************************************
 * main will initialize fileName and board, then fill them. Then push the 
 * rest of the work onto interact.  
 ************************************************************************/
int main()
{
   char fileName[256];
   char board[9][9];
   int plEdits[2][81];

   getIFile(fileName);
   readFile(fileName, board, plEdits);
   interact(board, plEdits, fileName);

   return 0;
}