// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Kirill Sedow (GitHub: hexn2764, contact.kirill@proton.me)
// Copyright (c) 2025 Lucas Schnack (GitHub: 0Zeta, lucas@schnack.dev)

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>

#define EXIT_STEAKHOUSE 1337
#define STEAK_PRICE 1000000000
#define GUESTBOOK_SIZE 10
#define ENTRY_SIZE 0x90
#define STORAGE_SIZE 0x20

int order_filled = 0;
int game_opened = 99;
unsigned long long credits = 10;

char guestbook[GUESTBOOK_SIZE][ENTRY_SIZE] = {"5/5 can totally recommend", "great place", "I love the steaks here. They are so safe and tasty.", "won a fortune playing card games", "You definitely have to try the desserts. They taste great!"};
char names[STORAGE_SIZE][STORAGE_SIZE];
char newspaper[STORAGE_SIZE][STORAGE_SIZE * 5];

void input(char *what, void *where)
{
    if (!scanf(what, where))
        exit(1);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void proceed_steaks(unsigned stks)
{
    if (stks > 0)
    {
        unsigned long long steak[0x10] = {[0 ... 0xf] = 0xdeadbeef};
        proceed_steaks(stks - 1);
    }
}

void order_steak()
{
    unsigned stks;
    puts("\x1b[33m'Shadow Steaks Die Twice', sir. But our steaks are safe. How many would you like to order?\x1b[0m");
    input("%u", &stks);

    if (stks == 0)
    {
        puts("\x1b[33mWonderful joke, sir! I’ll come back later.\x1b[0m");
        return;
    }
    if (stks > credits / STEAK_PRICE)
    {
        puts("\x1b[33mSorry sir, it looks like you don’t have enough credits for that many steaks!\x1b[0m");
        return;
    }
    else
    {
        credits -= STEAK_PRICE * stks;
        order_filled = 1;
        proceed_steaks(stks);
    }

    puts("\x1b[33mThank you, sir. Please enjoy your time while your order is processing!\x1b[0m");
}

void print_main_menu()
{
    puts("\x1b[34m1: Order a steak\x1b[0m");
    puts("\x1b[34m2: Read a newspaper\x1b[0m");
    puts("\x1b[34m3: Play a game\x1b[0m");
    puts("\x1b[34m4: Leave the steakhouse\x1b[0m");
}

void print_game_menu()
{
    puts("\x1b[33m1: Play a round\x1b[0m");
    puts("\x1b[33m2: Leave the game\x1b[0m");
    puts("\x1b[33m3: Check the balance\x1b[0m");
    puts("\x1b[33m4: Read the rules\x1b[0m");
}

void pad(int *buf, char *s)
{
    for (int i = 1; buf[i] != 0; i++)
    {
        printf("%d", buf[i]);
        if (buf[i + 1] == 0)
            printf("%s", s);
        else
            printf("%s", ", ");
    }
}

int check(size_t ptr)
{
    unsigned char vec[32];
    return mincore((void *)(ptr & ~(0xfff)), 0x1000, vec);
}

int get_card()
{
    return rand() % 10 + 2;
}

void play_game()
{
    const char *player = names[rand() % 32];
    printf("\x1b[31;52;1m\nTry your luck with the \"Magic Number\" game! You can play against %s tonight!\n\x1b[0m", player);
    printf("\x1b[33mYour balance is: %llu credits.\x1b[0m", credits);
    int choice;
    int user_turn;
    int sum_user;
    int sum_computer;
    int lost;

    while (1)
    {
        puts("\x1b[33m\nWhat would you like to do?\x1b[0m");
        print_game_menu();
        input("%d", &choice);
        switch (choice)
        {
        case 1:
            if (game_opened)
            {
                printf("\x1b[33mRound %d!\n\x1b[0m", 100 - game_opened--);
                user_turn = 1;
                sum_computer = 0;
                sum_user = 0;
                lost = 0;

                while (user_turn) // user is accumulating cards
                {
                    sum_user += get_card();
                    if (sum_user > 21) // lost the round, because his sum is greater than 21
                    {
                        lost = 1;
                        break;
                    }
                    if (sum_user == 21) // won the round, because the sum is exactly 21
                        break;

                    printf("\x1b[33mYour sum is %d. Would you like another card?\n\x1b[0m", sum_user);

                    char ans[4] = {0};
                    ans[read(0, ans, 4) - 1] = 0;
                    if (strcmp(ans, "yes"))
                    {
                        user_turn = 0;
                        break;
                    }
                }
                if (lost) // At this point, the user either lost (sum > 21), got 21, or chose to stop.
                {
                    credits /= 2;
                    printf("\x1b[31mYou lost the round with a sum of %d!\n\x1b[0m", sum_user);
                    printf("\x1b[31mYour new balance is: %llu credits.\n\x1b[0m", credits);
                    break;
                }
                if (sum_user == 21)
                {
                    credits *= 2;
                    puts("\x1b[32mYou won the round with the magic number of 21!\x1b[0m");
                    printf("\x1b[32mYour new balance is: %llu credits.\n\x1b[0m", credits);
                    break;
                }

                int nums[10] = {0};
                int count = 1;
                int magic = 0;

                // here the user holds some sum < 21. The turn goes to the opponent.
                while (!user_turn)
                {
                    int num = get_card();
                    sum_computer += num;
                    nums[count - 1] = num;

                    if (sum_computer > 21)
                    {
                        printf("\x1b[32m%s got %d, then decided to take numbers ", player, nums[0]);
                        pad(nums, " ");
                        printf("and lost the round with a sum of %d.\n\x1b[0m", sum_computer);
                        lost = 1;
                        break;
                    }
                    if (sum_computer == 21)
                    {
                        magic = 1;
                        printf("\x1b[31m%s got %d, then decided to take numbers ", player, nums[0]);
                        pad(nums, " ");
                        printf("and won with the magic number %d!\n\x1b[0m", sum_computer);
                        break;
                    }

                    if (sum_computer > 16)
                        break;
                    count++;
                }
                // here the opponent either has sum > 21, or sum == 21, or some sum
                if (lost)
                {
                    credits *= 2;
                    printf("\x1b[32mYour new balance is: %llu credits.\n\x1b[0m", credits);
                }
                else
                {
                    if (sum_user == sum_computer)
                    {
                        printf("\x1b[32m%s got %d, then continued with ", player, nums[0]);
                        pad(nums, ". ");
                        printf("Nobody won the round! You both have %d!\n\x1b[0m", sum_user);
                        printf("\x1b[32mYour credits stay the same.\n\x1b[0m");
                    }
                    else if (sum_user > sum_computer)
                    {
                        printf("\x1b[32m%s got %d, then continued with ", player, nums[0]);
                        pad(nums, ". ");
                        credits *= 2;
                        printf("\x1b[32m%s accumulated %d, but you win with %d!\n\x1b[0m", player, sum_computer, sum_user);
                        printf("\x1b[32mYour new balance is: %llu credits.\n\x1b[0m", credits);
                    }
                    else
                    {
                        credits /= 2;
                        if (!magic)
                            printf("\x1b[31mNice try with %d, but %s did even better with %d!\n\x1b[0m", sum_user, player, sum_computer);
                        printf("\x1b[31mYour new balance is: %llu credits.\n\x1b[0m", credits);
                    }
                }
                break;
            }
            else
            {
                puts("\x1b[31mIt was late and the game hall closed.\x1b[0m");
                break;
            }

        case 2:
            printf("\x1b[33mYou are leaving the game with %llu credits. Nice job!\n\x1b[0m", credits);
            return;
        case 3:
            printf("\x1b[33mYou have %llu credits by now.\n\x1b[0m", credits);
            break;
        case 4:
            puts("\x1b[33m\n---RULES---------------------------------------------------------------------------------------------------\x1b[0m");
            puts("\x1b[33m| \"Magic Number 21\" is a game consisting of rounds. Let’s say you’re playing against Alice.             |\x1b[0m");
            puts("\x1b[33m| In each round you and Alice build a stack of numbers. The possible values are from 2 to 11.             |\x1b[0m");
            puts("\x1b[33m| The round starts with you holding one number, chosen at random.                                         |\x1b[0m");
            puts("\x1b[33m| Then you decide if you want to keep drawing numbers to increase your total.                             |\x1b[0m");
            puts("\x1b[33m| If you draw again, a random number from 2 to 11 is added to your stack.                                 |\x1b[0m");
            puts("\x1b[33m| If you stop, Alice takes her turn and plays by the same rules.                                          |\x1b[0m");
            puts("\x1b[33m| Your goal is to beat Alice's total without going over 21.                                               |\x1b[0m");
            puts("\x1b[33m| If your total goes over 21, you lose. Same applies to Alice.                                            |\x1b[0m");
            puts("\x1b[33m| If either of you hits exactly 21, that player wins instantly.                                           |\x1b[0m");
            puts("\x1b[33m| If both players stop under 21, the one with the higher total wins.                                      |\x1b[0m");
            puts("\x1b[33m| Your credits will be doubled or halved depending on whether you win or lose.                            |\x1b[0m");
            puts("\x1b[33m|                                                                                                         |\x1b[0m");
            puts("\x1b[33m| Example 1: You draw 8 + 7 = 15 and stop. Alice draws 6 + 8 = 14 and stops. You win the round.            |\x1b[0m");
            puts("\x1b[33m| Example 2: You draw 10 + 9 + 4 = 23. Your total exceeds 21. You lose the round.                         |\x1b[0m");
            puts("\x1b[33m| Example 3: You draw 10 + 5 + 6 = 21. You hit the magic number. You win the round.                       |\x1b[0m");
            puts("\x1b[33m-----------------------------------------------------------------------------------------------------------\x1b[0m");
            break;
        default:
            break;
        }
    }
}

void enjoy_time()
{
    int choice;

    while (1)
    {
        puts("\x1b[34m\nWhat would you like to do?\x1b[0m");
        print_main_menu();
        input("%1d", &choice);
        switch (choice)
        {
        case 1:
            order_steak();
            break;
        case 2:
            puts("\x1b[33mWhich page would you like to read?\x1b[0m");
            size_t page;
            input("%zx", &page);
            if (!check(page) && (*((unsigned *)page) == 0xdeadbeef))
                puts("\x1b[33mHere is your steak, sir!\x1b[0m");
            else
                printf("\x1b[33m%s\x1b[0m", newspaper[rand() % 32]);
            break;
        case 3:
            play_game();
            break;
        case 4:
            if (!order_filled) {
                puts("\x1b[33mThank you for visiting us!\x1b[0m");
                return;
            }
            puts("\x1b[33mThanks for dining with us! Would you like to leave some feedback?\x1b[0m");

            char ans[12] = {0};
            ans[read(0, ans, sizeof(ans)) - 1] = 0;
            if (!strcmp(ans, "yes"))
            {
                short index = 0;
                char (*feedback)[ENTRY_SIZE];
                if (check((size_t) &feedback))
                    return;
                printf("\x1b[33mOn which page do you want to write your feedback? [0 - %d]\n\x1b[0m", GUESTBOOK_SIZE - 1);
                input("%llu", &index);
                if (index >= 0 && index < GUESTBOOK_SIZE) {
                    feedback = &guestbook[index];
                }
                puts("\x1b[33mPlease enter your comment:\x1b[0m");
                read(0, feedback, ENTRY_SIZE);
            }
            return;
        default:
            break;
        }
    }
}

void init()
{
    setbuf(stdout, NULL);
    srand(time(NULL));

    FILE *fp;
    if (!(fp = fopen("newspaper.txt", "r")))
        exit(1);
    int i = 0;
    size_t len = 0;
    while (fgets(newspaper[i++], STORAGE_SIZE * 5, fp)) ;
    fclose(fp);

    if (!(fp = fopen("names.txt", "r")))
        exit(1);

    i = 0;
    while (fgets(names[i], STORAGE_SIZE, fp))
    {
        names[i][strlen(names[i]) - 1] = '\0';
        i++;
    }
    fclose(fp);
}

int main()
{
    init();
    puts("\x1b[31;1mWelcome to \"Sea Lounge Safe Steak House\"!\x1b[0m");
    enjoy_time();
    return EXIT_STEAKHOUSE;
}   