//Max Elgart
//Student ID: 9624143167
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>

int main()
{
    std::string action[6][40][40];
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            for (int m = 0; m < 40; m++)
            {
                if(i > 0 && j + m >= 40 && m > 0)
                {
                    action[i][j][m] = "Yes";
                }
                else
                {
                    action[i][j][m] = "No";
                }
            }
        }
    }
    
    double probabilities[6][40][40];
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            for (int m = 0; m < 40; m++)
            {
                if(j + m >= 40 && i > 0)
                {
                    probabilities[i][j][m] = 1;
                }
                else
                {
                    probabilities[i][j][m] = 0;
                }
            }
        }
    }
    
    for (int l = 0; l < 10000; l++)
    {
        for (int i = 1; i < 6; i++)
        {
            for (int j = 39; j >= 0; j--)
            {
                for (int m = 39; m >= 0; m--)
                {
                    double hold = 0.0;
                    double no_hold = 0.0;
                    for (int g = 1; g < 6; g++)
                    {
                        if (j + m + g >= 40)
                        {
                            no_hold += 1.0/6.0;
                        }
                        else
                        {
                            no_hold += (1.0/6.0)*(probabilities[i][j][m + g]);
                        }
                    
                        if (j + m + g >= 40)
                        {
                            hold += 1.0/6.0;
                        }
                        else
                        {
                            hold += (1.0/6.0)*(probabilities[i - 1][j + m + g][0]);
                        }
                    }
                
                    no_hold += (1.0/6.0)*(probabilities[i - 1][j][0]);
                    hold += (1.0/6.0)*(probabilities[i - 1][j][0]);
                    if (m == 0 || i == 1)
                    {
                        probabilities[i][j][m] = no_hold;
                    }
                    else if (no_hold > hold)
                    {
                        probabilities[i][j][m] = no_hold;
                        action[i][j][m] = "No";
                    }
                    else
                    {
                        probabilities[i][j][m] = hold;
                        action[i][j][m] = "Yes";
                    }
                }
            }
        }
    }
    
    std::ofstream First_File("output.txt");
    double total_expected_win = 0;
    
    //Gets the output.txt with everything except for values with total + scratch >= 40
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            for (int m = 0; m < 40; m++)
            {
                if (j + m < 40)
                {
                    First_File << i << " " << j << " " << m << " " << action[i][j][m] << " " << probabilities[i][j][m] << std::endl;
                }
                // This is for getting the expected value for question2 of how much to pay to play
                total_expected_win += 500*(probabilities[i][j][m]);
            }
        }
    }
    
    // This is for getting the expected value for question2 of how much to pay to play
    total_expected_win = total_expected_win/(40*40*6);
    
    std::ofstream Second_File("part1.txt");
    // This is for outputtting the states to hold when total = 20 and scratch_score < 20 (AKA for question 1)
    for (int i = 1; i < 6; i++)
    {
        for (int m = 0; m < 20; m++)
        {
            if (20 + m < 40 && action[i][20][m] == "Yes")
            {
                Second_File << i << " " << 20 << " " << m << " " << action[i][20][m] << " " << probabilities[i][20][m] << std::endl;
            }
        }
    }
    
    // Puts resulting expected value and how much we should pay to play (q2)
    Second_File << std::endl << "2. The total expected profit if we play the game many times is: $" << total_expected_win << std::endl << "Therefore, I am willing to pay anything less than $" << total_expected_win;
    
    First_File.close();
    Second_File.close();
    
    return 0;
}
