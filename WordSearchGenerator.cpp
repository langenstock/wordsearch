
#include <iostream>
#include "grid.h"
#include <cstring>


int main()
{
    // Will work with any grid format
    Grid g{ 25, 25 };
    
    std::vector<const char*> words;
    std::vector<const char*> wordsUsed;

    // Add or remove any number of words

    words.push_back("ROSE");
    words.push_back("TULIP");
    words.push_back("DAISY");
    words.push_back("LILY");
    words.push_back("ORCHID");
    words.push_back("SUNFLOWER");
    words.push_back("LAVENDER");
    words.push_back("JASMINE");
    words.push_back("MARIGOLD");
    words.push_back("DAFFODIL");
    words.push_back("POPPY");
    words.push_back("PEONY");
    words.push_back("VIOLET");
    words.push_back("IRIS");
    words.push_back("HIBISCUS");
    words.push_back("MAGNOLIA");
    words.push_back("GARDENIA");
    words.push_back("CHRYSANTHEMUM");
    words.push_back("SNAPDRAGON");
    words.push_back("PETUNIA");
    words.push_back("AZALEA");
    words.push_back("HYDRANGEA");
    words.push_back("GERANIUM");
    words.push_back("LOTUS");
    words.push_back("DAHLIA");
    words.push_back("FREESIA");


    for (const char* word : words) {
        if (g.TryInsertWord(word) == EResult::Success) {
            wordsUsed.push_back(word);
        }
    }

    // This print out will show the locations of the words more clearly
    bool formatWithSpaces = true;
    g.DebugConsolePrint(formatWithSpaces);

    g.FillInBlankSpots();

    // This print out will be the one given to players
    g.DebugConsolePrint(formatWithSpaces);

    // Print out the list of words to be found
    std::cout << std::endl;
    for (const char* word : wordsUsed) {
        std::cout << word << std::endl;
    }
}
