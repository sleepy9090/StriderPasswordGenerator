/**
 ************************************************************************************************************************
 *
 *  @file                   StriderPassGen.cpp
 *  @brief                  Strider Password Generator
 *  @copyright              2019 Shawn M. Crawford
 *  @date                   March 20th, 2019
 *
 *  @remark Author:         Shawn M. Crawford
 *
 *  @note                   For use with the N.E.S. Version
 *
 ************************************************************************************************************************
 */

/* Includes */
#include <iostream>
#include <sstream>
#include <stdint.h>

using namespace std;

/**
 ************************************************************************************************************************
 *
 * @brief   Get user input
 *
 * @param   acquiredText The item to get input for
 *
 * @return  1 if acquired, 0 if not acquired
 *
 * @note    N/A
 *
 ************************************************************************************************************************
 */
int getUserInput(string acquiredText)
{
    string input = "";
    int acquired = 0;
    while (true) {
        cout << acquiredText << "? (0 for no, 1 for yes): ";
        getline(cin, input);

        stringstream myStream(input);
        if (myStream >> acquired)
        {
            if (acquired >= 0 && acquired <= 1)
            {
                break;
            }
        }
        
        cout << "Invalid number, please try again." << endl;
    }
    return acquired;
}

/**
 ************************************************************************************************************************
 *
 * @brief   Runs the program
 *
 * @param   N/A
 *
 * @return  N/A
 *
 * @note    N/A
 *
 ************************************************************************************************************************
 */
int main (int argc, char *argv[])
{
    int passwordData[12];
    int tempArray[8];
    int tempArray2[8];
    int tempArray3[6];
    int tempArray4[6];

    int data1 = 0;
    int data2 = 0;
    int data3 = 0;
    int data4 = 0;
    int level = 1;
    int shiftBit = 0;
    int x = 0;

    string input = "";

    while (true) {
        cout << "Level? (1..10) ";
        getline(cin, input);

        stringstream myStream(input);
        if (myStream >> level)
        {
            if (level >= 1 && level <= 10)
            {
                break;
            }
        }
        
        cout << "Invalid number, please try again." << endl;
    }

    while (true) {
        cout << "Shift Bit? (0..15): ";
        getline(cin, input);

        stringstream myStream(input);
        if (myStream >> shiftBit)
        {
            if (shiftBit >= 0 && shiftBit <= 15)
            {
                break;
            }
        }
        
        cout << "Invalid number, please try again." << endl;
    }

    /** Set data */

    /* level and shift*/
    level = level - 1;
    passwordData[0] = shiftBit;
    passwordData[1] = level + shiftBit & 15;

    /* Key 1 is given at the beginning of the game and is required. */
    tempArray[0] = 1; // key1

    /* keys, red dragon, unknown 1, unknown 2 (8 total) */
    tempArray[1] = getUserInput("Key 2");
    tempArray[2] = getUserInput("Key 3");
    tempArray[3] = getUserInput("Key 4");
    tempArray[4] = getUserInput("Key 5");
    tempArray[5] = getUserInput("Red Dragon Open");
    tempArray[6] = getUserInput("Unknown 1 (Fought Kain?)");
    tempArray[7] = getUserInput("Unknown 2 (Fought Kain?)"); 
    
    for (int y = 0; y < 8; y++)
    {
        if (tempArray[y] == 1)
        {
    	    x += 1 << 7 - y;
    	}
    }
    data1 = x;

    /* boots, plasma arrow, unknown 3 (5 total) */
    tempArray2[0] = getUserInput("Aqua Boots");
    tempArray2[1] = getUserInput("Magnetic Boots");
    tempArray2[2] = getUserInput("Attack Boots");
    tempArray2[3] = 1; // set to 1, unknown
    tempArray2[4] = 1; // set to 1, unknown
    tempArray2[5] = 1; // set to 1, unknown
    tempArray2[6] = getUserInput("Plasma Arrow");
    tempArray2[7] = getUserInput("Unknown 3 (Fought Kain?)"); 

    x = 0;
    for (int y = 0; y < 3; y++)
    {
        if (tempArray2[y] == 1)
    	{
    	    x += 1 << 7 - y;
    	}
    }

    for (int y = 6; y < 8; y++)
    {
        if (tempArray2[(y & 15) - 3] == 1)
    	{
    	    x += 1 << 7 - y;
    	}
    }

    data2 = x;

    /* data files (6 total) */
    tempArray3[0] = getUserInput("Data File 1");
    tempArray3[1] = getUserInput("Data File 2");
    tempArray3[2] = getUserInput("Data File 3");
    tempArray3[3] = getUserInput("Data File 4");
    tempArray3[4] = getUserInput("Data File 5");
    tempArray3[5] = getUserInput("Data File 6"); 

    x = 0;
    for(int y = 0; y < 6; y++)
    {
        if (tempArray3[y] == 1)
    	{
            x += 1 << 7 - y;
    	}
    }

    data3=x;

    /* analyzed data files (6 total) */
    tempArray4[0] = getUserInput("Data File 1 Analyzed");
    tempArray4[1] = getUserInput("Data File 2 Analyzed");
    tempArray4[2] = getUserInput("Data File 3 Analyzed");
    tempArray4[3] = getUserInput("Data File 4 Analyzed");
    tempArray4[4] = getUserInput("Data File 5 Analyzed");
    tempArray4[5] = getUserInput("Data File 6 Analyzed"); 

    x = 0;
    for(int y = 0; y < 6; y++)
    {
        if (tempArray4[y] == 1)
    	{
    	    x += 1 << 7 - y;
    	}
    }

    data4 = x;

    /** Calculate Checksum */
    int  checksumData = (level & 255) + data1 + data2 + data3 + data4 & 255;
    passwordData[10] = checksumData >> 4 & 15;
    passwordData[11] = checksumData & 15;

    /** Create Password Data */
    passwordData[2] = (data1 >> 4) + shiftBit & 15;
    passwordData[3] = (data1 & 15) + shiftBit & 15;
    passwordData[4] = (data2 >> 4) + shiftBit & 15;
    passwordData[5] = (data2 & 15) + shiftBit & 15;
    passwordData[6] = (data3 >> 4) + shiftBit & 15;
    passwordData[7] = (data3 & 15) + shiftBit & 15;
    passwordData[8] = (data4 >> 4) + shiftBit & 15;
    passwordData[9] = (data4 & 15) + shiftBit & 15;

    /** Generate Password */
	string password = "";
    int i = 0;
    int j = 0;
    
	while(i < 12)
    {
        if (i == 4 || i == 8)
        {
            password += ' ';
        }
        password += "ABCDEFGHIJKLMNOP"[passwordData[i++]];
    }
    cout << endl;
    cout << "Generated password: " << password << endl;

    return(0);
}

