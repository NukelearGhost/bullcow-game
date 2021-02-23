// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/isograms.txt");
    FFileHelper::LoadFileToStringArray(AllIsograms, *WordListPath);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &PlayerInput) // When the player hits enter
{
    CheckGuess(PlayerInput);

    // Regardless of how game ends, prompt the player to play again
    // If player wants to play again, begin a new game
    // If player does not want to play again, exit the game.
}

void UBullCowCartridge::SetupGame()
{

    Isograms = GetValidIsograms(AllIsograms);
    // Set hidden word and player lives
    //HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    //PlayerLives = HiddenWord.Len();

    // Display welcome message
    PrintLine(TEXT("Welcome to the Bull/Cow Game!"));
    PrintLine(TEXT("Try to guess the %i letter isogram"), HiddenWord.Len());
    PrintLine(TEXT("You have %i tries"), PlayerLives);
    PrintLine(TEXT("Type your guess and press enter."));

    // Debugging
    PrintLine(TEXT("DEBUG: Total possible words: %i"), Isograms.Num());
    PrintLine(TEXT("DEBUG: The HiddenWord is: %s\n"), *HiddenWord);
}

void UBullCowCartridge::CheckGuess(const FString &PlayerGuess)
{

    if (PlayerGuess == HiddenWord)
    {
        EndGame(true);
        return;
    }

    if (PlayerGuess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Your guess was %i characters long,\nthe hidden word is %i.\nTRY AGAIN!"), PlayerGuess.Len(), HiddenWord.Len());
        return;
    }

    if (!IsIsogram(PlayerGuess))
    {
        PrintLine(TEXT("Your guess was not an isogram. TRY AGAIN!"));
        return;
    }

    // Deduct a life
    --PlayerLives;

    if (PlayerLives <= 0)
    {
        EndGame(false);
        return;
    }

    FBullCowCount Score = GetBullCows(PlayerGuess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Incorrect! You have %i tries remaining"), PlayerLives);
}

bool UBullCowCartridge::IsIsogram(const FString &Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparator = Index + 1; Comparator < Word.Len(); Comparator++)
        {
            if (Word[Index] == Word[Comparator])
            {
                return false;
            }
        }
    }

    return true;
}

void UBullCowCartridge::EndGame(const bool &bWinGame)
{
    if (bWinGame == true)
    {
        ClearScreen();
        PrintLine(TEXT("Congratulations! You guessed correctly!"));
    }
    else
    {
        ClearScreen();
        PrintLine(TEXT("GAME OVER! YOU LOSE!"));
    }

    PrintLine(TEXT("Press enter to play again."));
    SetupGame();
}

TArray<FString> UBullCowCartridge::GetValidIsograms(const TArray<FString> &WordList) const
{
    TArray<FString> ValidIsograms;

    for (FString Isogram : WordList)
    {
        if (Isogram.Len() >= 4 && Isogram.Len() <= 8)
        {
            if (IsIsogram(Isogram))
            {
                ValidIsograms.Emplace(Isogram);
            }
        }
    }
    return ValidIsograms;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{

    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < Guess.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}