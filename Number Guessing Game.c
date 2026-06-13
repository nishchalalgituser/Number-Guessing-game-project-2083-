#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int wallet = 1000;
int totalGames = 0;
int wins = 0;
int losses = 0;

/* Function Prototypes */
void loadWallet();
void saveWallet();
void showRank();
void saveHistory(char player[], int bet, int guess, int number, char result[]);
void showStats();
void showHints(int number);

int main()
{
    char player[50];
    char choice;

     srand(10);

    loadWallet();

    printf("\nEnter Player Name: "); 
    scanf("%s", player);

    do
    {
        int level;
        int number;
        int guess;
        int attempts;
        int maxAttempts;
        int range;
        int bet;
        int win = 0;
        int hintChoice;

        printf("\n==================================");
        printf("\n      NUMBER GUESSING CASINO");
        printf("\n==================================");

        printf("\nYour current balance is: Rs.%d\n", wallet);

        if(wallet <= 0)
        {
            printf("You are out of money!\n");
            break;
        }

        /* Betting */
        do
        {
            printf("Enter your bet amount: ");
            scanf("%d", &bet);

            if(bet > wallet || bet <= 0)
                printf("You don't have enough balance!\n");

        } while(bet > wallet || bet <= 0);

        /* Difficulty */
        printf("\nChoose level:");
        printf("\n1. Easy (1-50, 10 attempts)");
        printf("\n2. Medium (1-100, 7 attempts)");
        printf("\n3. Hard (1-500, 5 attempts)");
        printf("\nEnter your choice: ");
        scanf("%d", &level);
        if(level==1)
         {
            printf("you choose easy level\n");
         }
         else if(level==2)
         {
            printf("you choose medium level\n");
         }
         else if(level==3)
         {
            printf("you choose hard level\n");
         }
         else
         {
            printf("Invalid choice! Default level (Easy) is selected.\n");
            level = 1;
         }

         switch(level)
        {
            case 1:
                range = 50;
                maxAttempts = 10;
                break;

            case 2:
                range = 100;
                maxAttempts = 7;
                break;

            case 3:
                range = 500;
                maxAttempts = 5;
                break;

            default:
                printf("Your choice is invalid! Default level (Easy) is selected.\n");
                range = 50;
                maxAttempts = 10;
        }

        number = rand() % range + 1;

        printf("\nDo you want a hint?");
        printf("\nHint Cost = Rs.50");
        printf("\n1. Yes");
        printf("\n2. No");
        printf("\nChoice: ");
        scanf("%d", &hintChoice);

        if(hintChoice == 1)
        {
            if(wallet >= 50)
            {
                wallet -= 50;
                showHints(number);
            }
            else
            {
                printf("You donot have sufficient balance for a hint.\n");
            }
        }

        printf("\nYou're requested to enter your guess between 1 and %d\n", range);

        for(attempts = 1; attempts <= maxAttempts; attempts++)
        {
            printf("\nAttempt %d/%d: ",
                   attempts,
                   maxAttempts);

            scanf("%d", &guess);
            if(guess > range || guess < 1)
            {
                printf("Please enter a valid guess between 1 and %d\n", range);
                continue;
            }
           else if(guess == number)
            {
                win = 1;
                break;
            }
            else if(guess < number)
            {
                printf("Your guessed amount is too low!\n");
            }
            else
            {
                printf("Your guessed amount is too high!\n");
            }
        }

        totalGames++;

        if(win)
        {
            int reward;

            if(attempts == 1)
                reward = bet * 5;
            else if(attempts == 2)
                reward = bet * 3;
            else
                reward = bet * 2;

            wallet += reward;
            wins++;

            printf("\nCongratulations,%s!\n", player);
            printf("You have guessed the correct number = %d\n", number);
            printf("You have won Rs.%d\n", reward);

            saveHistory(
                player,
                bet,
                guess,
                number,
                "WIN"
            );
        }
        else
        {
            wallet -= bet;
            losses++;

            printf("\nYou lose your game!\n");
            printf("The correct number was %d\n", number);

            saveHistory(
                player,
                bet,
                guess,
                number,
                "LOSS"
            );
        }

        showRank();

        printf("\nYour current balance is: Rs.%d\n", wallet);

        saveWallet();

        printf("\nDo you want to play again? (Y/N): ");
        scanf(" %c", &choice);

    } while(choice == 'Y' || choice == 'y');

    showStats();
    saveWallet();

    printf("\nThanks for enjoying our game. Hope you enjoyed a lot!\n");

    return 0;
}


/* Load Wallet */
void loadWallet()
{
    FILE *fp;

    fp = fopen("game_wallet.txt", "r");

    if(fp != NULL)
    {
        fscanf(fp, "%d", &wallet);
        fclose(fp);
    }
}

/* Save Wallet */
void saveWallet()
{
    FILE *fp;

    fp = fopen("game_wallet.txt", "w");

    if(fp != NULL)
    {
        fprintf(fp, "%d", wallet);
        fclose(fp);
    }
}

/* Rank System */
void showRank()
{
    printf("\nRank: ");

    if(wallet < 1000)
        printf("Bronze");
    else if(wallet < 5000)
        printf("Silver");
    else if(wallet < 10000)
        printf("Gold");
    else
        printf("Platinum");

    printf("\n");
}

/* Hint System */
void showHints(int number)
{
    printf("\n========== HINT ==========\n");

    if(number % 2 == 0)
        printf("Number is EVEN\n");
    else
        printf("Number is ODD\n");

    if(number > 50)
        printf("Number is greater than 50\n");
    else
        printf("Number is less than or equal to 50\n");

    if(number % 5 == 0)
        printf("Number is multiple of 5\n");
    else
        printf("Number is not multiple of 5\n");

    printf("==========================\n");
}

/* Save History */
void saveHistory(
    char player[],
    int bet,
    int guess,
    int number,
    char result[]
)
{
    FILE *fp;

    fp = fopen("game_history.txt", "a");

    if(fp != NULL)
    {
        fprintf(fp,
                "Player:%s Bet:%d Guess:%d Number:%d Result:%s\n",
                player,
                bet,
                guess,
                number,
                result);

        fclose(fp);
    }
}

/* Statistics */
void showStats()
{
    float winRate = 0;

    if(totalGames > 0)
    {
        winRate =
            ((float)wins / totalGames) * 100;
    }

    printf("\n=================================");
    printf("\n         GAME STATISTICS");
    printf("\n=================================");

    printf("\nTotal Games you had played : %d", totalGames);
    printf("\nTotal matches you have won : %d", wins);
    printf("\nTotal matches you have lost : %d", losses);
    printf("\nYour win rate is: %.2f%%", winRate);

    printf("\n=================================\n");
}
