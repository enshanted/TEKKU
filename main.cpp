#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Pet.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tekku");
    window.setFramerateLimit(60);

    auto loadTexture = [](sf::Texture& texture, const std::string& path) {
        if (!texture.loadFromFile(path)) {
            std::cerr << "Could not load: " << path << '\n';
            return false;
        }
        return true;
    };

    sf::Texture startScreenTexture, petSelectScreenTexture;
    sf::Texture backgroundTexture;
    sf::Texture catIdleTexture, dogIdleTexture;
    sf::Texture catSleepTexture, dogSleepTexture;
    sf::Texture catPlayTexture, dogPlayTexture;
    sf::Texture catBathTexture, dogBathTexture;
    sf::Texture catBurgerTexture, catSinigangTexture, catDubaiTexture;
    sf::Texture dogBurgerTexture, dogSinigangTexture, dogDubaiTexture;
    sf::Texture burgerButtonTexture, sinigangButtonTexture, dubaiButtonTexture;
    sf::Texture sleepButtonTexture, bathButtonTexture, tickleButtonTexture;

    if (!loadTexture(startScreenTexture, "../assets/start_screen.png") ||
        !loadTexture(petSelectScreenTexture, "../assets/pet_select_screen.png") ||
        !loadTexture(backgroundTexture, "../assets/tekku_mockup.png") ||
        !loadTexture(catIdleTexture, "../assets/pets/cat/cat_idle.png") ||
        !loadTexture(dogIdleTexture, "../assets/pets/dog/dog_idle.png") ||
        !loadTexture(catSleepTexture, "../assets/pets/cat/cat_sleep.png") ||
        !loadTexture(dogSleepTexture, "../assets/pets/dog/dog_sleep.png") ||
        !loadTexture(catPlayTexture, "../assets/pets/cat/cat_play.png") ||
        !loadTexture(dogPlayTexture, "../assets/pets/dog/dog_play.png") ||
        !loadTexture(catBathTexture, "../assets/pets/cat/cat_bath.png") ||
        !loadTexture(dogBathTexture, "../assets/pets/dog/dog_bath.png") ||
        !loadTexture(catBurgerTexture, "../assets/pets/cat/cat_burger.png") ||
        !loadTexture(catSinigangTexture, "../assets/pets/cat/cat_sinigang.png") ||
        !loadTexture(catDubaiTexture, "../assets/pets/cat/cat_dubai.png") ||
        !loadTexture(dogBurgerTexture, "../assets/pets/dog/dog_burger.png") ||
        !loadTexture(dogSinigangTexture, "../assets/pets/dog/dog_sinigang.png") ||
        !loadTexture(dogDubaiTexture, "../assets/pets/dog/dog_dubai.png") ||
        !loadTexture(burgerButtonTexture, "../assets/buttons/burger.png") ||
        !loadTexture(sinigangButtonTexture, "../assets/buttons/sinigang.png") ||
        !loadTexture(dubaiButtonTexture, "../assets/buttons/dubai_chewy.png") ||
        !loadTexture(sleepButtonTexture, "../assets/buttons/sleep.png") ||
        !loadTexture(bathButtonTexture, "../assets/buttons/bath.png") ||
        !loadTexture(tickleButtonTexture, "../assets/buttons/tickle.png")) {
        return 1;
    }

    sf::Font statusFont;
    if (!statusFont.openFromFile("../assets/fonts/PIXEARG_.TTF")) {
        std::cerr << "Could not load font: ../assets/fonts/PIXEARG_.TTF\n";
        return 1;
    }

    sf::Text petNameDisplay(statusFont);
    petNameDisplay.setCharacterSize(24);
    petNameDisplay.setFillColor(sf::Color(70, 0, 100));
    petNameDisplay.setStyle(sf::Text::Bold);

    sf::Text petSpeciesDisplay(statusFont);
    petSpeciesDisplay.setCharacterSize(18);
    petSpeciesDisplay.setFillColor(sf::Color(70, 0, 100));
    petSpeciesDisplay.setStyle(sf::Text::Bold);

    sf::RectangleShape petNameDivider({240.0f, 3.0f});
    petNameDivider.setFillColor(sf::Color(70, 0, 100));


    sf::SoundBuffer eatBuffer, sleepBuffer, bathBuffer, tickleBuffer;
    if (!eatBuffer.loadFromFile("../assets/sounds/eat.wav") ||
        !sleepBuffer.loadFromFile("../assets/sounds/sleep.wav") ||
        !bathBuffer.loadFromFile("../assets/sounds/bath.wav") ||
        !tickleBuffer.loadFromFile("../assets/sounds/tickle.wav")) {
        std::cerr << "Could not load an action sound from assets/sounds.\n";
        return 1;
    }

    sf::Sound eatSound(eatBuffer);
    sf::Sound sleepSound(sleepBuffer);
    sf::Sound bathSound(bathBuffer);
    sf::Sound tickleSound(tickleBuffer);

    eatSound.setVolume(50.0f);
    sleepSound.setVolume(50.0f);
    bathSound.setVolume(50.0f);
    tickleSound.setVolume(50.0f);

    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("../assets/sounds/background.wav")) {
        std::cerr << "Could not load: ../assets/sounds/background.wav\n";
        return 1;
    }
    backgroundMusic.setLooping(true);
    backgroundMusic.setVolume(30.0f);
    backgroundMusic.play();

    enum class ActionSound { None, Eat, Sleep, Bath, Tickle };
    ActionSound activeActionSound = ActionSound::None;

    auto stopActionSound = [&]() {
        eatSound.stop();
        sleepSound.stop();
        bathSound.stop();
        tickleSound.stop();
        activeActionSound = ActionSound::None;
    };

    auto startLoopingSound = [&](sf::Sound& sound, ActionSound type) {
        stopActionSound();
        sound.setLooping(true);
        sound.play();
        activeActionSound = type;
    };

    enum class Screen { Start, ChoosePet, SavedPets, Game };
    Screen currentScreen = Screen::Start;

    std::string enteredPetName;
    std::string selectedSpecies = "Dog";
    std::string startErrorMessage;

    bool cursorVisible = true;
    float cursorBlinkElapsed = 0.0f;

    sf::Sprite startScreen(startScreenTexture);
    startScreen.setScale({
        1280.0f / static_cast<float>(startScreenTexture.getSize().x),
        720.0f / static_cast<float>(startScreenTexture.getSize().y)
    });

    sf::Sprite petSelectScreen(petSelectScreenTexture);
    petSelectScreen.setScale({
        1280.0f / static_cast<float>(petSelectScreenTexture.getSize().x),
        720.0f / static_cast<float>(petSelectScreenTexture.getSize().y)
    });

    sf::Sprite background(backgroundTexture);
    background.setScale({
        1280.0f / static_cast<float>(backgroundTexture.getSize().x),
        720.0f / static_cast<float>(backgroundTexture.getSize().y)
    });

    sf::Sprite catIdle(catIdleTexture), dogIdle(dogIdleTexture);
    sf::Sprite catSleep(catSleepTexture), dogSleep(dogSleepTexture);
    sf::Sprite catPlay(catPlayTexture), dogPlay(dogPlayTexture);
    sf::Sprite catBath(catBathTexture), dogBath(dogBathTexture);
    sf::Sprite catBurger(catBurgerTexture), catSinigang(catSinigangTexture);
    sf::Sprite catDubai(catDubaiTexture), dogBurger(dogBurgerTexture);
    sf::Sprite dogSinigang(dogSinigangTexture), dogDubai(dogDubaiTexture);

    sf::Sprite startCat(catIdleTexture), startDog(dogIdleTexture);
    sf::Sprite chooseCat(catIdleTexture), chooseDog(dogIdleTexture);
    sf::Sprite savedCatIcon(catIdleTexture), savedDogIcon(dogIdleTexture);

    sf::Sprite burgerButton(burgerButtonTexture);
    sf::Sprite sinigangButton(sinigangButtonTexture);
    sf::Sprite dubaiButton(dubaiButtonTexture);
    sf::Sprite sleepButton(sleepButtonTexture);
    sf::Sprite bathButton(bathButtonTexture);
    sf::Sprite tickleButton(tickleButtonTexture);

    std::array<sf::Sprite*, 6> actionButtons{{
        &burgerButton, &sinigangButton, &dubaiButton,
        &sleepButton, &bathButton, &tickleButton
    }};

    const std::array<sf::Vector2f, 6> buttonPositions{{
        {106.0f, 540.0f}, {288.0f, 540.0f}, {470.0f, 540.0f},
        {652.0f, 540.0f}, {834.0f, 540.0f}, {1016.0f, 540.0f}
    }};

    constexpr sf::Vector2f buttonDisplaySize(174.0f, 147.0f);
    std::array<sf::Vector2f, 6> buttonBaseScales{};

    for (std::size_t i = 0; i < actionButtons.size(); ++i) {
        const sf::FloatRect bounds = actionButtons[i]->getLocalBounds();
        const sf::Vector2f scale{
            buttonDisplaySize.x / bounds.size.x,
            buttonDisplaySize.y / bounds.size.y
        };

        actionButtons[i]->setOrigin({
            bounds.position.x + bounds.size.x / 2.0f,
            bounds.position.y + bounds.size.y / 2.0f
        });
        actionButtons[i]->setPosition({
            buttonPositions[i].x + buttonDisplaySize.x / 2.0f,
            buttonPositions[i].y + buttonDisplaySize.y / 2.0f
        });
        actionButtons[i]->setScale(scale);
        buttonBaseScales[i] = scale;
    }

    constexpr int frameWidth = 640;
    constexpr int frameHeight = 640;
    constexpr float secondsPerFrame = 0.12f;

    const int catIdleFrameCount = std::max(
        1, static_cast<int>(catIdleTexture.getSize().x) / frameWidth
    );
    const int dogIdleFrameCount = std::max(
        1, static_cast<int>(dogIdleTexture.getSize().x) / frameWidth
    );
    const int catSleepFrameCount = std::max(
        1, static_cast<int>(catSleepTexture.getSize().x) / frameWidth
    );
    const int dogSleepFrameCount = std::max(
        1, static_cast<int>(dogSleepTexture.getSize().x) / frameWidth
    );
    const int catPlayFrameCount = std::max(
        1, static_cast<int>(catPlayTexture.getSize().x) / frameWidth
    );
    const int dogPlayFrameCount = std::max(
        1, static_cast<int>(dogPlayTexture.getSize().x) / frameWidth
    );
    const int catBathFrameCount = std::max(
        1, static_cast<int>(catBathTexture.getSize().x) / frameWidth
    );
    const int dogBathFrameCount = std::max(
        1, static_cast<int>(dogBathTexture.getSize().x) / frameWidth
    );

    const std::array<int, 6> foodFrameCounts{{
        std::max(1, static_cast<int>(catBurgerTexture.getSize().x) / frameWidth),
        std::max(1, static_cast<int>(catSinigangTexture.getSize().x) / frameWidth),
        std::max(1, static_cast<int>(catDubaiTexture.getSize().x) / frameWidth),
        std::max(1, static_cast<int>(dogBurgerTexture.getSize().x) / frameWidth),
        std::max(1, static_cast<int>(dogSinigangTexture.getSize().x) / frameWidth),
        std::max(1, static_cast<int>(dogDubaiTexture.getSize().x) / frameWidth)
    }};

    std::array<sf::Sprite*, 6> foodSprites{{
        &catBurger, &catSinigang, &catDubai,
        &dogBurger, &dogSinigang, &dogDubai
    }};

    const sf::Vector2f petCenter(480.0f, 310.0f);

    for (sf::Sprite* sprite : {
             &catIdle, &dogIdle, &catSleep, &dogSleep,
             &catPlay, &dogPlay, &catBath, &dogBath
         }) {
        sprite->setTextureRect(
            sf::IntRect({0, 0}, {frameWidth, frameHeight})
        );
        sprite->setOrigin({frameWidth / 2.0f, frameHeight / 2.0f});
        sprite->setPosition(petCenter);
        sprite->setScale({0.35f, 0.35f});
    }

    for (sf::Sprite* sprite : foodSprites) {
        sprite->setTextureRect(
            sf::IntRect({0, 0}, {frameWidth, frameHeight})
        );
        sprite->setOrigin({frameWidth / 2.0f, frameHeight / 2.0f});
        sprite->setPosition(petCenter);
        sprite->setScale({0.35f, 0.35f});
    }

    for (sf::Sprite* sprite : {&startCat, &startDog}) {
        sprite->setTextureRect(
            sf::IntRect({0, 0}, {frameWidth, frameHeight})
        );
        sprite->setOrigin({frameWidth / 2.0f, frameHeight / 2.0f});
        sprite->setScale({0.19f, 0.19f});
    }
    startCat.setPosition({310.0f, 280.0f});
    startDog.setPosition({900.0f, 110.0f});

    for (sf::Sprite* sprite : {&chooseCat, &chooseDog}) {
        sprite->setTextureRect(
            sf::IntRect({0, 0}, {frameWidth, frameHeight})
        );
        sprite->setOrigin({frameWidth / 2.0f, frameHeight / 2.0f});
        sprite->setScale({0.24f, 0.24f});
    }
    chooseCat.setPosition({433.0f, 467.0f});
    chooseDog.setPosition({817.0f, 467.0f});

    for (sf::Sprite* sprite : {&savedCatIcon, &savedDogIcon}) {
        sprite->setTextureRect(
            sf::IntRect({0, 0}, {frameWidth, frameHeight})
        );
        sprite->setOrigin({frameWidth / 2.0f, frameHeight / 2.0f});
        sprite->setScale({0.11f, 0.11f});
    }

    sf::Text nameEntryText(statusFont);
    nameEntryText.setCharacterSize(24);
    nameEntryText.setFillColor(sf::Color(70, 35, 130));

    sf::RectangleShape nameCursor({3.0f, 30.0f});
    nameCursor.setFillColor(sf::Color(70, 35, 130));

    sf::RectangleShape startButton({210.0f, 46.0f});
startButton.setPosition({535.0f, 608.0f});
startButton.setFillColor(sf::Color(137, 76, 220));
startButton.setOutlineColor(sf::Color(42, 22, 125));
startButton.setOutlineThickness(3.0f);

sf::Text startButtonText(statusFont);
startButtonText.setString("START");
startButtonText.setCharacterSize(22);
startButtonText.setStyle(sf::Text::Bold);
startButtonText.setFillColor(sf::Color::White);

sf::RectangleShape existingPetsButton({150.0f, 32.0f});
existingPetsButton.setPosition({565.0f, 660.0f});
existingPetsButton.setFillColor(sf::Color(75, 170, 190));
existingPetsButton.setOutlineColor(sf::Color(42, 22, 125));
existingPetsButton.setOutlineThickness(2.0f);

sf::Text existingPetsButtonText(statusFont);
existingPetsButtonText.setString("EXISTING PETS");
existingPetsButtonText.setCharacterSize(12);
existingPetsButtonText.setStyle(sf::Text::Bold);
existingPetsButtonText.setFillColor(sf::Color::White);

auto centerText = [](sf::Text& text, float x, float y) {
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });
    text.setPosition({x, y});
};


centerText(startButtonText, 640.0f, 631.0f);
centerText(existingPetsButtonText, 640.0f, 676.0f);

    sf::Text startError(statusFont);
    startError.setCharacterSize(15);
    startError.setFillColor(sf::Color(180, 30, 80));

    sf::Text choosePetPrompt(statusFont);
    choosePetPrompt.setCharacterSize(18);
    choosePetPrompt.setFillColor(sf::Color(70, 35, 130));
    centerText(choosePetPrompt, 640.0f, 680.0f);

    sf::Text savedPetsTitle(statusFont);
    savedPetsTitle.setString("EXISTING PETS");
    savedPetsTitle.setCharacterSize(30);
    savedPetsTitle.setStyle(sf::Text::Bold);
    savedPetsTitle.setFillColor(sf::Color(72, 13, 103));
    centerText(savedPetsTitle, 640.0f, 65.0f);

    sf::RectangleShape savedPetsTitleBackground({420.0f, 66.0f});
savedPetsTitleBackground.setOrigin({210.0f, 33.0f});
savedPetsTitleBackground.setPosition({640.0f, 65.0f});
savedPetsTitleBackground.setFillColor(sf::Color(255, 235, 250, 245));
savedPetsTitleBackground.setOutlineColor(sf::Color(72, 13, 103));
savedPetsTitleBackground.setOutlineThickness(3.0f);

    sf::RectangleShape savedPetsBackButton({100.0f, 40.0f});
    savedPetsBackButton.setPosition({40.0f, 40.0f});
    savedPetsBackButton.setFillColor(sf::Color(210, 170, 235));
    savedPetsBackButton.setOutlineColor(sf::Color(42, 22, 125));
    savedPetsBackButton.setOutlineThickness(2.0f);

    sf::Text savedPetsBackText(statusFont);
    savedPetsBackText.setString("BACK");
    savedPetsBackText.setCharacterSize(15);
    savedPetsBackText.setStyle(sf::Text::Bold);
    savedPetsBackText.setFillColor(sf::Color(42, 22, 125));
    centerText(savedPetsBackText, 90.0f, 60.0f);

    sf::Text noSavedPetsText(statusFont);
    noSavedPetsText.setString("No saved pets yet.");
    noSavedPetsText.setCharacterSize(20);
    noSavedPetsText.setFillColor(sf::Color(72, 13, 103));
    centerText(noSavedPetsText, 640.0f, 300.0f);

    sf::Text savedPetsHint(statusFont);
    savedPetsHint.setString("Use the Up and Down arrow keys to browse.");
    savedPetsHint.setCharacterSize(13);
    savedPetsHint.setFillColor(sf::Color(72, 13, 103));
    centerText(savedPetsHint, 640.0f, 690.0f);

    sf::Text savedRowName(statusFont);
    savedRowName.setCharacterSize(18);
    savedRowName.setStyle(sf::Text::Bold);
    savedRowName.setFillColor(sf::Color(72, 13, 103));

    sf::Text savedRowDetails(statusFont);
    savedRowDetails.setCharacterSize(13);
    savedRowDetails.setFillColor(sf::Color(72, 13, 103));

    sf::RectangleShape savedRowCard({900.0f, 82.0f});
    savedRowCard.setFillColor(sf::Color(255, 235, 250));
    savedRowCard.setOutlineColor(sf::Color(80, 55, 150));
    savedRowCard.setOutlineThickness(2.0f);

    sf::RectangleShape loadPetButton({85.0f, 46.0f});
    loadPetButton.setFillColor(sf::Color(110, 175, 110));
    loadPetButton.setOutlineColor(sf::Color(42, 22, 125));
    loadPetButton.setOutlineThickness(2.0f);

    sf::Text loadPetButtonText(statusFont);
    loadPetButtonText.setString("LOAD");
    loadPetButtonText.setCharacterSize(14);
    loadPetButtonText.setStyle(sf::Text::Bold);
    loadPetButtonText.setFillColor(sf::Color::White);

    sf::RectangleShape deletePetButton({85.0f, 46.0f});
    deletePetButton.setFillColor(sf::Color(210, 90, 125));
    deletePetButton.setOutlineColor(sf::Color(42, 22, 125));
    deletePetButton.setOutlineThickness(2.0f);

    sf::Text deletePetButtonText(statusFont);
    deletePetButtonText.setString("DELETE");
    deletePetButtonText.setCharacterSize(13);
    deletePetButtonText.setStyle(sf::Text::Bold);
    deletePetButtonText.setFillColor(sf::Color::White);

    sf::RectangleShape modalDimmer({1280.0f, 720.0f});
    modalDimmer.setFillColor(sf::Color(20, 10, 50, 170));

    sf::RectangleShape modalPanel({560.0f, 230.0f});
    modalPanel.setPosition({360.0f, 245.0f});
    modalPanel.setFillColor(sf::Color(255, 235, 250));
    modalPanel.setOutlineColor(sf::Color(42, 22, 125));
    modalPanel.setOutlineThickness(4.0f);

    sf::Text modalText(statusFont);
    modalText.setCharacterSize(20);
    modalText.setFillColor(sf::Color(42, 22, 125));

    sf::RectangleShape modalYesButton({150.0f, 48.0f});
    modalYesButton.setFillColor(sf::Color(210, 90, 125));
    modalYesButton.setOutlineColor(sf::Color(42, 22, 125));
    modalYesButton.setOutlineThickness(2.0f);

    sf::Text modalYesText(statusFont);
    modalYesText.setString("DELETE");
    modalYesText.setCharacterSize(15);
    modalYesText.setStyle(sf::Text::Bold);
    modalYesText.setFillColor(sf::Color::White);

    sf::RectangleShape modalNoButton({150.0f, 48.0f});
    modalNoButton.setFillColor(sf::Color(110, 175, 110));
    modalNoButton.setOutlineColor(sf::Color(42, 22, 125));
    modalNoButton.setOutlineThickness(2.0f);

    sf::Text modalNoText(statusFont);
    modalNoText.setString("CANCEL");
    modalNoText.setCharacterSize(15);
    modalNoText.setStyle(sf::Text::Bold);
    modalNoText.setFillColor(sf::Color::White);

    sf::RectangleShape exitSaveButton({175.0f, 48.0f});
    exitSaveButton.setFillColor(sf::Color(110, 175, 110));
    exitSaveButton.setOutlineColor(sf::Color(42, 22, 125));
    exitSaveButton.setOutlineThickness(2.0f);

    sf::Text exitSaveText(statusFont);
    exitSaveText.setString("SAVE & EXIT");
    exitSaveText.setCharacterSize(13);
    exitSaveText.setStyle(sf::Text::Bold);
    exitSaveText.setFillColor(sf::Color::White);

    sf::RectangleShape exitNoSaveButton({175.0f, 48.0f});
    exitNoSaveButton.setFillColor(sf::Color(210, 90, 125));
    exitNoSaveButton.setOutlineColor(sf::Color(42, 22, 125));
    exitNoSaveButton.setOutlineThickness(2.0f);

    sf::Text exitNoSaveText(statusFont);
    exitNoSaveText.setString("EXIT WITHOUT SAVE");
    exitNoSaveText.setCharacterSize(10);
    exitNoSaveText.setStyle(sf::Text::Bold);
    exitNoSaveText.setFillColor(sf::Color::White);

    sf::RectangleShape exitCancelButton({120.0f, 48.0f});
    exitCancelButton.setFillColor(sf::Color(155, 135, 190));
    exitCancelButton.setOutlineColor(sf::Color(42, 22, 125));
    exitCancelButton.setOutlineThickness(2.0f);

    sf::Text exitCancelText(statusFont);
    exitCancelText.setString("CANCEL");
    exitCancelText.setCharacterSize(13);
    exitCancelText.setStyle(sf::Text::Bold);
    exitCancelText.setFillColor(sf::Color::White);

    const std::array<sf::Vector2f, 4> barPositions{{
        {870.0f, 193.0f}, {870.0f, 265.0f},
        {870.0f, 337.0f}, {870.0f, 409.0f}
    }};

    const std::array<sf::Color, 4> barColors{{
        sf::Color(245, 95, 155), sf::Color(255, 190, 55),
        sf::Color(225, 45, 190), sf::Color(30, 175, 235)
    }};

    std::array<sf::RectangleShape, 4> barTracks;
    std::array<sf::RectangleShape, 4> barFills;

    sf::Text nourishmentLabel(statusFont), energyLabel(statusFont);
    sf::Text funLabel(statusFont), cleanlinessLabel(statusFont);

    std::array<sf::Text*, 4> barLabels{{
        &nourishmentLabel, &energyLabel, &funLabel, &cleanlinessLabel
    }};

    for (std::size_t i = 0; i < barTracks.size(); ++i) {
        barTracks[i].setSize({240.0f, 22.0f});
        barTracks[i].setPosition(barPositions[i]);
        barTracks[i].setFillColor(sf::Color(205, 195, 255));
        barTracks[i].setOutlineColor(sf::Color(42, 22, 125));
        barTracks[i].setOutlineThickness(3.0f);

        barFills[i].setSize({0.0f, 14.0f});
        barFills[i].setPosition({
            barPositions[i].x + 4.0f,
            barPositions[i].y + 4.0f
        });
        barFills[i].setFillColor(barColors[i]);

        barLabels[i]->setCharacterSize(16);
        barLabels[i]->setFillColor(sf::Color(42, 22, 125));
        barLabels[i]->setOutlineColor(sf::Color::White);
        barLabels[i]->setOutlineThickness(1.0f);
    }

    constexpr float barFillMaxWidth = 232.0f;
    constexpr float barFillHeight = 14.0f;

    sf::CircleShape helpButton(19.0f);
helpButton.setOrigin({19.0f, 19.0f});
helpButton.setPosition({122.0f, 132.0f});
helpButton.setFillColor(sf::Color(205, 215, 225));
helpButton.setOutlineColor(sf::Color(42, 22, 125));
helpButton.setOutlineThickness(1.0f);

sf::Text helpQuestionMark(statusFont);
helpQuestionMark.setString("?");
helpQuestionMark.setCharacterSize(23);
helpQuestionMark.setFillColor(sf::Color(42, 22, 125));
centerText(helpQuestionMark, 122.0f, 132.0f);

sf::RectangleShape homeButton({70.0f, 38.0f});
homeButton.setPosition({153.0f, 113.0f});
homeButton.setFillColor(sf::Color(155, 135, 190));
homeButton.setOutlineColor(sf::Color(42, 22, 125));
homeButton.setOutlineThickness(2.0f);

sf::Text homeButtonText(statusFont);
homeButtonText.setString("HOME");
homeButtonText.setCharacterSize(13);
homeButtonText.setStyle(sf::Text::Bold);
homeButtonText.setFillColor(sf::Color::White);
centerText(homeButtonText, 188.0f, 132.0f);

sf::RectangleShape saveButton({70.0f, 38.0f});
saveButton.setPosition({233.0f, 113.0f});
saveButton.setFillColor(sf::Color(105, 175, 125));
saveButton.setOutlineColor(sf::Color(42, 22, 125));
saveButton.setOutlineThickness(2.0f);

sf::Text saveButtonText(statusFont);
saveButtonText.setString("SAVE");
saveButtonText.setCharacterSize(13);
saveButtonText.setStyle(sf::Text::Bold);
saveButtonText.setFillColor(sf::Color::White);
centerText(saveButtonText, 268.0f, 132.0f);

    sf::RectangleShape guideDimmer({1280.0f, 720.0f});
    guideDimmer.setFillColor(sf::Color(20, 10, 50, 150));

    sf::RectangleShape guidePanel({900.0f, 600.0f});
    guidePanel.setPosition({190.0f, 60.0f});
    guidePanel.setFillColor(sf::Color(255, 235, 250));
    guidePanel.setOutlineColor(sf::Color(42, 22, 125));
    guidePanel.setOutlineThickness(5.0f);

    sf::Text guideTitle(statusFont);
    guideTitle.setString("TEKKU USER GUIDE");
    guideTitle.setCharacterSize(30);
    guideTitle.setFillColor(sf::Color(42, 22, 125));
    guideTitle.setPosition({235.0f, 82.0f});

    sf::Text nourishmentHeading(statusFont);
    nourishmentHeading.setString("NOURISHMENT");
    nourishmentHeading.setCharacterSize(18);
    nourishmentHeading.setStyle(sf::Text::Bold);
    nourishmentHeading.setFillColor(sf::Color(42, 22, 125));
    nourishmentHeading.setPosition({240.0f, 140.0f});

    sf::Text nourishmentInfo(statusFont);
    nourishmentInfo.setString(
        "Click Burger / Press B\n"
        "    +30% nourishment\n"
        "Click Sinigang / Press S\n"
        "    +70% nourishment\n"
        "Click Dubai Chewy / Press D\n"
        "    +10% nourishment\n"
        "    +50% fun\n"
        "    -5% cleanliness"
    );
    nourishmentInfo.setCharacterSize(16);
    nourishmentInfo.setFillColor(sf::Color(42, 22, 125));
    nourishmentInfo.setPosition({240.0f, 164.0f});

    sf::Text energyHeading(statusFont);
    energyHeading.setString("ENERGY");
    energyHeading.setCharacterSize(18);
    energyHeading.setStyle(sf::Text::Bold);
    energyHeading.setFillColor(sf::Color(42, 22, 125));
    energyHeading.setPosition({240.0f, 370.0f});

    sf::Text energyInfo(statusFont);
    energyInfo.setString(
        "Click Sleep / Press Z:\n"
        "    +1% energy per second"
    );
    energyInfo.setCharacterSize(16);
    energyInfo.setFillColor(sf::Color(42, 22, 125));
    energyInfo.setPosition({240.0f, 394.0f});

    sf::Text funHeading(statusFont);
    funHeading.setString("FUN");
    funHeading.setCharacterSize(18);
    funHeading.setStyle(sf::Text::Bold);
    funHeading.setFillColor(sf::Color(42, 22, 125));
    funHeading.setPosition({650.0f, 140.0f});

    sf::Text funInfo(statusFont);
    funInfo.setString(
        "Click Tickle / Press T\n"
        "    +20% fun\n"
        "    -5% energy and cleanliness"
    );
    funInfo.setCharacterSize(16);
    funInfo.setFillColor(sf::Color(42, 22, 125));
    funInfo.setPosition({650.0f, 164.0f});

    sf::Text cleanlinessHeading(statusFont);
    cleanlinessHeading.setString("CLEANLINESS");
    cleanlinessHeading.setCharacterSize(18);
    cleanlinessHeading.setStyle(sf::Text::Bold);
    cleanlinessHeading.setFillColor(sf::Color(42, 22, 125));
    cleanlinessHeading.setPosition({650.0f, 260.0f});

    sf::Text cleanlinessInfo(statusFont);
    cleanlinessInfo.setString(
        "Click Bath / Press W\n"
        "    +100% cleanliness"
    );
    cleanlinessInfo.setCharacterSize(16);
    cleanlinessInfo.setFillColor(sf::Color(42, 22, 125));
    cleanlinessInfo.setPosition({650.0f, 284.0f});

    sf::RectangleShape guideCloseButton({42.0f, 38.0f});
    guideCloseButton.setPosition({1030.0f, 78.0f});
    guideCloseButton.setFillColor(sf::Color(255, 190, 220));
    guideCloseButton.setOutlineColor(sf::Color(42, 22, 125));
    guideCloseButton.setOutlineThickness(2.0f);

    sf::Text guideCloseText(statusFont);
    guideCloseText.setString("X");
    guideCloseText.setCharacterSize(20);
    guideCloseText.setFillColor(sf::Color(42, 22, 125));
    centerText(guideCloseText, 1051.0f, 97.0f);

    bool guideOpen = false;
    bool deleteConfirmOpen = false;
    bool exitPromptOpen = false;
    std::size_t deletePetIndex = 0;
    int savedListOffset = 0;

    sf::RectangleShape notificationBackground({1080.0f, 70.0f});
    notificationBackground.setPosition({100.0f, 471.0f});
    notificationBackground.setFillColor(sf::Color(255, 235, 250, 230));
    notificationBackground.setOutlineColor(sf::Color(42, 22, 125));
    notificationBackground.setOutlineThickness(2.0f);

    sf::Text notificationText(statusFont);
    notificationText.setCharacterSize(18);
    notificationText.setFillColor(sf::Color(42, 22, 125));
    notificationText.setOutlineColor(sf::Color::White);
    notificationText.setOutlineThickness(1.0f);

    bool notificationVisible = false;
    float notificationElapsed = 0.0f;

    auto showNotification = [&](const std::string& message) {
        notificationText.setString(message);
        centerText(notificationText, 640.0f, 506.0f);
        notificationElapsed = 0.0f;
        notificationVisible = true;
    };

    Pet pet;
    initializePet(pet, "Dog", "Tekku");

    std::vector<Pet> savedPets;
    const std::string saveFilePath = "../tekku_pets.txt";

    auto reloadSavedPets = [&]() {
        savedPets.clear();
        std::ifstream file(saveFilePath);

        std::size_t count = 0;
        if (!(file >> count)) {
            return;
        }

        for (std::size_t i = 0; i < count; ++i) {
            Pet savedPet;

            if (!(file >> std::quoted(savedPet.name)
                      >> std::quoted(savedPet.species)
                      >> savedPet.nourishment
                      >> savedPet.energy
                      >> savedPet.fun
                      >> savedPet.cleanliness)) {
                break;
            }

            if (savedPet.species != "Cat" && savedPet.species != "Dog") {
                continue;
            }

            savedPet.nourishment =
                std::clamp(savedPet.nourishment, 0.0f, 100.0f);
            savedPet.energy = std::clamp(savedPet.energy, 0.0f, 100.0f);
            savedPet.fun = std::clamp(savedPet.fun, 0.0f, 100.0f);
            savedPet.cleanliness =
                std::clamp(savedPet.cleanliness, 0.0f, 100.0f);
            savedPet.sleeping = false;

            savedPets.push_back(savedPet);
        }
    };

    auto writeSavedPets = [&]() {
        std::ofstream file(saveFilePath, std::ios::trunc);
        if (!file) {
            return false;
        }

        file << savedPets.size() << '\n' << std::setprecision(7);
        for (const Pet& savedPet : savedPets) {
            file << std::quoted(savedPet.name) << ' '
                 << std::quoted(savedPet.species) << '\n'
                 << savedPet.nourishment << ' '
                 << savedPet.energy << ' '
                 << savedPet.fun << ' '
                 << savedPet.cleanliness << '\n';
        }
        return static_cast<bool>(file);
    };

    auto saveCurrentPet = [&]() {
        auto found = std::find_if(
            savedPets.begin(),
            savedPets.end(),
            [&](const Pet& savedPet) {
                return savedPet.name == pet.name &&
                       savedPet.species == pet.species;
            }
        );

        Pet progress = pet;
        progress.sleeping = false;

        if (found == savedPets.end()) {
            savedPets.push_back(progress);
        } else {
            *found = progress;
        }

        return writeSavedPets();
    };

    auto centerStartError = [&](const std::string& message) {
        startErrorMessage = message;
        startError.setString(message);
        centerText(startError, 640.0f, 695.0f);
    };

    auto startNameEntry = [&]() {
        if (enteredPetName.empty()) {
            centerStartError("Please enter a pet name first.");
        } else {
            startErrorMessage.clear();
            currentScreen = Screen::ChoosePet;
        }
    };

    sf::Clock deltaClock;

    int idleFrame = 0;
    float idleFrameElapsed = 0.0f;
    int sleepFrame = 0;
    float sleepFrameElapsed = 0.0f;
    bool sleepAnimationPlaying = false;
    bool playingAnimation = false;
    int playFrame = 0;
    float playFrameElapsed = 0.0f;
    bool bathingAnimation = false;
    int bathFrame = 0;
    float bathFrameElapsed = 0.0f;
    bool foodAnimationPlaying = false;
    int foodAnimationSpriteIndex = 0;
    int foodFrame = 0;
    float foodFrameElapsed = 0.0f;

    int menuCatFrame = 0;
    int menuDogFrame = 0;
    float menuFrameElapsed = 0.0f;

    std::array<float, 6> buttonPopElapsed{};
    std::array<bool, 6> buttonPopActive{};

    const std::array<std::string, 4> warningMessages{{
        "I am hungry!", "I am tired!", "I am bored!", "I am dirty!"
    }};
    std::array<float, 4> warningTimers{};

    auto loadSelectedSavedPet = [&](std::size_t index) {
        if (index >= savedPets.size()) {
            return;
        }

        pet = savedPets[index];
        pet.sleeping = false;
        selectedSpecies = pet.species;
        enteredPetName = pet.name;

        sleepAnimationPlaying = false;
        playingAnimation = false;
        bathingAnimation = false;
        foodAnimationPlaying = false;

        idleFrame = 0;
        idleFrameElapsed = 0.0f;
        sleepFrame = 0;
        sleepFrameElapsed = 0.0f;
        playFrame = 0;
        playFrameElapsed = 0.0f;
        bathFrame = 0;
        bathFrameElapsed = 0.0f;
        foodFrame = 0;
        foodFrameElapsed = 0.0f;

        currentScreen = Screen::Game;
    };

    auto activateButton = [&](std::size_t index) {
        stopActionSound();
        buttonPopElapsed[index] = 0.0f;
        buttonPopActive[index] = true;

        if (!pet.sleeping) {
            sleepAnimationPlaying = false;
            foodAnimationPlaying = false;
            bathingAnimation = false;
            playingAnimation = false;
        }

        if (index < 3) {
            const float nourishmentBefore = pet.nourishment;
            showNotification(feed(pet, static_cast<int>(index) + 1));

            if (pet.nourishment > nourishmentBefore) {
                const int speciesOffset = (pet.species == "Cat") ? 0 : 3;
                foodAnimationSpriteIndex =
                    speciesOffset + static_cast<int>(index);

                foodAnimationPlaying = true;
                sleepAnimationPlaying = false;
                bathingAnimation = false;
                playingAnimation = false;
                foodFrame = 0;
                foodFrameElapsed = 0.0f;

                startLoopingSound(eatSound, ActionSound::Eat);
            }
            return;
        }

        if (index == 3) {
            const bool wasSleeping = pet.sleeping;
            toggleSleep(pet);

            if (pet.sleeping) {
                showNotification(pet.name + " is sleeping.");
                sleepAnimationPlaying = true;
                foodAnimationPlaying = false;
                bathingAnimation = false;
                playingAnimation = false;
                sleepFrame = 0;
                sleepFrameElapsed = 0.0f;
                startLoopingSound(sleepSound, ActionSound::Sleep);
            } else if (wasSleeping) {
                sleepAnimationPlaying = false;
                showNotification(pet.name + " woke up.");
            } else if (pet.energy >= 100.0f) {
                sleepAnimationPlaying = false;
                showNotification(pet.name + " is fully rested.");
            }
            return;
        }

        if (index == 4) {
            if (pet.sleeping) {
                showNotification(
                    pet.name + " is sleeping. Wake them before a bath."
                );
            } else if (pet.cleanliness >= 100.0f) {
                showNotification(pet.name + " is already clean!");
            } else {
                shower(pet);
                showNotification(pet.name + " took a bath!");
                bathingAnimation = true;
                sleepAnimationPlaying = false;
                foodAnimationPlaying = false;
                playingAnimation = false;
                bathFrame = 0;
                bathFrameElapsed = 0.0f;
                startLoopingSound(bathSound, ActionSound::Bath);
            }
            return;
        }

        if (index == 5) {
            if (pet.sleeping) {
                showNotification(
                    pet.name + " is sleeping. Wake them before playing."
                );
            } else {
                play(pet);
                showNotification(pet.name + " is playing!");
                playingAnimation = true;
                sleepAnimationPlaying = false;
                foodAnimationPlaying = false;
                bathingAnimation = false;
                playFrame = 0;
                playFrameElapsed = 0.0f;
                startLoopingSound(tickleSound, ActionSound::Tickle);
            }
        }
    };

    while (window.isOpen()) {
        const float elapsedSeconds = deltaClock.restart().asSeconds();

        if (currentScreen == Screen::Start) {
            cursorBlinkElapsed += elapsedSeconds;
            if (cursorBlinkElapsed >= 0.5f) {
                cursorVisible = !cursorVisible;
                cursorBlinkElapsed = 0.0f;
            }
        }

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                if (currentScreen == Screen::Game) {
                    exitPromptOpen = true;
                } else {
                    window.close();
                }
                continue;
            }

            if (exitPromptOpen) {
                if (const auto* key =
                        event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Escape) {
                        exitPromptOpen = false;
                    }
                    continue;
                }

                if (const auto* mouse =
                        event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        const sf::Vector2f point =
                            window.mapPixelToCoords(mouse->position);

                        if (exitSaveButton.getGlobalBounds().contains(point)) {
                            if (saveCurrentPet()) {
                                window.close();
                            } else {
                                exitPromptOpen = false;
                                showNotification("Could not save progress.");
                            }
                        } else if (
                            exitNoSaveButton.getGlobalBounds().contains(point)
                        ) {
                            window.close();
                        } else if (
                            exitCancelButton.getGlobalBounds().contains(point)
                        ) {
                            exitPromptOpen = false;
                        }
                    }
                    continue;
                }
                continue;
            }

            if (deleteConfirmOpen) {
                if (const auto* key =
                        event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Escape) {
                        deleteConfirmOpen = false;
                    }
                    continue;
                }

                if (const auto* mouse =
                        event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        const sf::Vector2f point =
                            window.mapPixelToCoords(mouse->position);

                        if (modalYesButton.getGlobalBounds().contains(point)) {
                            if (deletePetIndex < savedPets.size()) {
                                savedPets.erase(
                                    savedPets.begin() +
                                    static_cast<std::ptrdiff_t>(deletePetIndex)
                                );
                                writeSavedPets();
                                if (savedListOffset >=
                                    static_cast<int>(savedPets.size())) {
                                    savedListOffset = std::max(
                                        0,
                                        static_cast<int>(savedPets.size()) - 1
                                    );
                                }
                            }
                            deleteConfirmOpen = false;
                        } else if (
                            modalNoButton.getGlobalBounds().contains(point)
                        ) {
                            deleteConfirmOpen = false;
                        }
                    }
                    continue;
                }
                continue;
            }

            if (currentScreen == Screen::Start) {
                if (const auto* textEvent =
                        event->getIf<sf::Event::TextEntered>()) {
                    if (textEvent->unicode >= 32 &&
                        textEvent->unicode <= 126 &&
                        enteredPetName.size() < 16) {
                        enteredPetName +=
                            static_cast<char>(textEvent->unicode);
                        startErrorMessage.clear();
                        cursorVisible = true;
                        cursorBlinkElapsed = 0.0f;
                    }
                    continue;
                }

                if (const auto* key =
                        event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Backspace) {
                        if (!enteredPetName.empty()) {
                            enteredPetName.pop_back();
                            startErrorMessage.clear();
                            cursorVisible = true;
                            cursorBlinkElapsed = 0.0f;
                        }
                    } else if (key->code == sf::Keyboard::Key::Enter) {
                        startNameEntry();
                    }
                    continue;
                }

                if (const auto* mouse =
                        event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        const sf::Vector2f point =
                            window.mapPixelToCoords(mouse->position);

                        if (startButton.getGlobalBounds().contains(point)) {
                            startNameEntry();
                        } else if (
                            existingPetsButton.getGlobalBounds().contains(point)
                        ) {
                            reloadSavedPets();
                            savedListOffset = 0;
                            currentScreen = Screen::SavedPets;
                        }
                    }
                    continue;
                }
            }

            if (currentScreen == Screen::ChoosePet) {
                if (const auto* key =
                        event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Escape) {
                        currentScreen = Screen::Start;
                    }
                    continue;
                }

                if (const auto* mouse =
                        event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        const sf::Vector2f point =
                            window.mapPixelToCoords(mouse->position);

                        if (chooseCat.getGlobalBounds().contains(point)) {
                            selectedSpecies = "Cat";
                            initializePet(pet, selectedSpecies, enteredPetName);
                            currentScreen = Screen::Game;
                        } else if (chooseDog.getGlobalBounds().contains(point)) {
                            selectedSpecies = "Dog";
                            initializePet(pet, selectedSpecies, enteredPetName);
                            currentScreen = Screen::Game;
                        }
                    }
                    continue;
                }
            }

            if (currentScreen == Screen::SavedPets) {
                if (const auto* key =
                        event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Escape) {
                        currentScreen = Screen::Start;
                    } else if (key->code == sf::Keyboard::Key::Down) {
                        const int maxOffset = std::max(
                            0, static_cast<int>(savedPets.size()) - 5
                        );
                        savedListOffset =
                            std::min(savedListOffset + 1, maxOffset);
                    } else if (key->code == sf::Keyboard::Key::Up) {
                        savedListOffset = std::max(0, savedListOffset - 1);
                    }
                    continue;
                }

                if (const auto* mouse =
                        event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        const sf::Vector2f point =
                            window.mapPixelToCoords(mouse->position);

                        if (savedPetsBackButton.getGlobalBounds().contains(point)) {
                            currentScreen = Screen::Start;
                            continue;
                        }

                        for (int row = 0; row < 5; ++row) {
                            const int index = savedListOffset + row;
                            if (index >= static_cast<int>(savedPets.size())) {
                                break;
                            }

                            const float y = 125.0f + row * 94.0f;
                            const sf::FloatRect loadBounds(
                                {870.0f, y + 18.0f}, {85.0f, 46.0f}
                            );
                            const sf::FloatRect deleteBounds(
                                {970.0f, y + 18.0f}, {85.0f, 46.0f}
                            );

                            if (loadBounds.contains(point)) {
                                loadSelectedSavedPet(
                                    static_cast<std::size_t>(index)
                                );
                                break;
                            }

                            if (deleteBounds.contains(point)) {
                                deletePetIndex =
                                    static_cast<std::size_t>(index);
                                modalText.setString(
                                    "Delete " + savedPets[index].name + "?"
                                );
                                centerText(modalText, 640.0f, 320.0f);
                                deleteConfirmOpen = true;
                                break;
                            }
                        }
                    }
                    continue;
                }
            }

            if (currentScreen != Screen::Game) {
                continue;
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    if (guideOpen) {
                        guideOpen = false;
                        stopActionSound();
                    } else {
                        exitPromptOpen = true;
                    }
                    continue;
                }

                if (guideOpen) {
                    continue;
                }

                if (key->code == sf::Keyboard::Key::B) {
                    activateButton(0);
                } else if (key->code == sf::Keyboard::Key::S) {
                    activateButton(1);
                } else if (key->code == sf::Keyboard::Key::D) {
                    activateButton(2);
                } else if (key->code == sf::Keyboard::Key::Z) {
                    activateButton(3);
                } else if (key->code == sf::Keyboard::Key::W) {
                    activateButton(4);
                } else if (key->code == sf::Keyboard::Key::T) {
                    activateButton(5);
                }
            }

            if (const auto* mouse =
                    event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button != sf::Mouse::Button::Left) {
                    continue;
                }

                const sf::Vector2f point =
                    window.mapPixelToCoords(mouse->position);

                if (guideOpen) {
                    if (guideCloseButton.getGlobalBounds().contains(point)) {
                        guideOpen = false;
                        stopActionSound();
                    }
                    continue;
                }

                if (helpButton.getGlobalBounds().contains(point)) {
                    guideOpen = true;
                    stopActionSound();
                    continue;
                }

                if (saveButton.getGlobalBounds().contains(point)) {
                    if (saveCurrentPet()) {
                        showNotification("Progress saved!");
                    } else {
                        showNotification("Could not save progress.");
                    }
                    continue;
                }

                if (homeButton.getGlobalBounds().contains(point)) {
    if (saveCurrentPet()) {
        stopActionSound();

        guideOpen = false;
        sleepAnimationPlaying = false;
        playingAnimation = false;
        bathingAnimation = false;
        foodAnimationPlaying = false;

        enteredPetName.clear();
        startErrorMessage.clear();
        currentScreen = Screen::Start;
    } else {
        showNotification("Could not save progress.");
    }
    continue;
}

                sf::Sprite* visiblePet = nullptr;

                if (pet.species == "Cat") {
                    if (pet.sleeping || sleepAnimationPlaying) {
                        visiblePet = &catSleep;
                    } else if (bathingAnimation) {
                        visiblePet = &catBath;
                    } else if (foodAnimationPlaying) {
                        visiblePet = foodSprites[foodAnimationSpriteIndex];
                    } else if (playingAnimation) {
                        visiblePet = &catPlay;
                    } else {
                        visiblePet = &catIdle;
                    }
                } else {
                    if (pet.sleeping || sleepAnimationPlaying) {
                        visiblePet = &dogSleep;
                    } else if (bathingAnimation) {
                        visiblePet = &dogBath;
                    } else if (foodAnimationPlaying) {
                        visiblePet = foodSprites[foodAnimationSpriteIndex];
                    } else if (playingAnimation) {
                        visiblePet = &dogPlay;
                    } else {
                        visiblePet = &dogIdle;
                    }
                }

                if (visiblePet != nullptr &&
                    visiblePet->getGlobalBounds().contains(point)) {
                    pet.sleeping = false;
                    sleepAnimationPlaying = false;
                    foodAnimationPlaying = false;
                    bathingAnimation = false;
                    playingAnimation = false;

                    sleepFrame = 0;
                    sleepFrameElapsed = 0.0f;
                    foodFrame = 0;
                    foodFrameElapsed = 0.0f;
                    bathFrame = 0;
                    bathFrameElapsed = 0.0f;
                    playFrame = 0;
                    playFrameElapsed = 0.0f;
                    idleFrame = 0;
                    idleFrameElapsed = 0.0f;

                    stopActionSound();
                    continue;
                }

                for (std::size_t i = 0; i < actionButtons.size(); ++i) {
                    if (actionButtons[i]->getGlobalBounds().contains(point)) {
                        activateButton(i);
                        break;
                    }
                }
            }
        }

        if (currentScreen == Screen::Start ||
            currentScreen == Screen::ChoosePet ||
            currentScreen == Screen::SavedPets) {
            menuFrameElapsed += elapsedSeconds;

            while (menuFrameElapsed >= secondsPerFrame) {
                menuCatFrame = (menuCatFrame + 1) % catIdleFrameCount;
                menuDogFrame = (menuDogFrame + 1) % dogIdleFrameCount;
                menuFrameElapsed -= secondsPerFrame;
            }

            const sf::IntRect catMenuRect(
                {menuCatFrame * frameWidth, 0},
                {frameWidth, frameHeight}
            );
            const sf::IntRect dogMenuRect(
                {menuDogFrame * frameWidth, 0},
                {frameWidth, frameHeight}
            );

            startCat.setTextureRect(catMenuRect);
            chooseCat.setTextureRect(catMenuRect);
            savedCatIcon.setTextureRect(catMenuRect);
            startDog.setTextureRect(dogMenuRect);
            chooseDog.setTextureRect(dogMenuRect);
            savedDogIcon.setTextureRect(dogMenuRect);

            window.clear();

            if (currentScreen == Screen::Start) {
                window.draw(startScreen);
                window.draw(startCat);
                window.draw(startDog);

                if (enteredPetName.empty()) {
                    nameEntryText.setString("Type your pet name here");
                    nameEntryText.setFillColor(sf::Color(120, 100, 160));
                } else {
                    nameEntryText.setString(enteredPetName);
                    nameEntryText.setFillColor(sf::Color(70, 35, 130));
                }

                const sf::FloatRect nameBounds =
                    nameEntryText.getLocalBounds();
                nameEntryText.setOrigin({
                    nameBounds.position.x + nameBounds.size.x / 2.0f,
                    nameBounds.position.y + nameBounds.size.y / 2.0f
                });
                nameEntryText.setPosition({640.0f, 397.0f});
                window.draw(nameEntryText);

                if (cursorVisible) {
                    nameCursor.setPosition({
                        640.0f + nameBounds.size.x / 2.0f + 5.0f,
                        382.0f
                    });
                    window.draw(nameCursor);
                }

                window.draw(startButton);
                window.draw(startButtonText);
                window.draw(existingPetsButton);
                window.draw(existingPetsButtonText);

                if (!startErrorMessage.empty()) {
                    window.draw(startError);
                }
            } else if (currentScreen == Screen::ChoosePet) {
                window.draw(petSelectScreen);
                window.draw(chooseCat);
                window.draw(chooseDog);
                window.draw(choosePetPrompt);
            } else {
                window.draw(startScreen);
                window.draw(savedPetsTitleBackground);
                window.draw(savedPetsTitle);
                window.draw(savedPetsBackButton);
                window.draw(savedPetsBackText);

                if (savedPets.empty()) {
                    window.draw(noSavedPetsText);
                } else {
                    const int endIndex = std::min(
                        static_cast<int>(savedPets.size()),
                        savedListOffset + 5
                    );

                    for (int index = savedListOffset;
                         index < endIndex;
                         ++index) {
                        const int row = index - savedListOffset;
                        const float y = 125.0f + row * 94.0f;

                        savedRowCard.setPosition({190.0f, y});
                        window.draw(savedRowCard);

                        Pet& savedPet = savedPets[index];
                        sf::Sprite& icon =
                            savedPet.species == "Cat"
                                ? savedCatIcon
                                : savedDogIcon;
                        icon.setPosition({245.0f, y + 41.0f});
                        window.draw(icon);

                        savedRowName.setString(savedPet.name);
                        savedRowName.setPosition({310.0f, y + 12.0f});
                        window.draw(savedRowName);

                        const std::string details =
                            savedPet.species + "   N:" +
                            std::to_string(static_cast<int>(savedPet.nourishment)) +
                            "%  E:" +
                            std::to_string(static_cast<int>(savedPet.energy)) +
                            "%  F:" +
                            std::to_string(static_cast<int>(savedPet.fun)) +
                            "%  C:" +
                            std::to_string(static_cast<int>(savedPet.cleanliness)) +
                            "%";
                        savedRowDetails.setString(details);
                        savedRowDetails.setPosition({310.0f, y + 46.0f});
                        window.draw(savedRowDetails);

                        loadPetButton.setPosition({870.0f, y + 18.0f});
                        window.draw(loadPetButton);
                        centerText(loadPetButtonText, 912.5f, y + 41.0f);
                        window.draw(loadPetButtonText);

                        deletePetButton.setPosition({970.0f, y + 18.0f});
                        window.draw(deletePetButton);
                        centerText(deletePetButtonText, 1012.5f, y + 41.0f);
                        window.draw(deletePetButtonText);
                    }

                    if (savedPets.size() > 5) {
                        window.draw(savedPetsHint);
                    }
                }
            }

            if (deleteConfirmOpen) {
                window.draw(modalDimmer);
                window.draw(modalPanel);
                window.draw(modalText);

                modalYesButton.setPosition({445.0f, 385.0f});
                modalNoButton.setPosition({685.0f, 385.0f});
                window.draw(modalYesButton);
                window.draw(modalNoButton);
                centerText(modalYesText, 520.0f, 409.0f);
                centerText(modalNoText, 760.0f, 409.0f);
                window.draw(modalYesText);
                window.draw(modalNoText);
            }

            window.display();
            continue;
        }

        const bool wasSleepingBeforeUpdate = pet.sleeping;
        updatePet(pet, elapsedSeconds);
        updateSleep(pet, elapsedSeconds);

        if (wasSleepingBeforeUpdate && !pet.sleeping) {
            showNotification(pet.name + " is fully rested!");
        }

        const std::array<float, 4> statValues{{
            pet.nourishment, pet.energy, pet.fun, pet.cleanliness
        }};

        for (std::size_t i = 0; i < barFills.size(); ++i) {
            const float value = std::clamp(statValues[i], 0.0f, 100.0f);

            barFills[i].setSize({
                barFillMaxWidth * (value / 100.0f),
                barFillHeight
            });

            barLabels[i]->setString(
                std::to_string(static_cast<int>(std::lround(value))) + "%"
            );

            const sf::FloatRect textBounds = barLabels[i]->getLocalBounds();
            barLabels[i]->setOrigin({
                textBounds.position.x + textBounds.size.x,
                textBounds.position.y + textBounds.size.y / 2.0f
            });
            barLabels[i]->setPosition({
                barPositions[i].x + 232.0f,
                barPositions[i].y + 11.0f
            });

            if (value <= 0.0f) {
                warningTimers[i] += elapsedSeconds;
                if (warningTimers[i] >= 60.0f) {
                    showNotification(
                        pet.name + ": " + warningMessages[i]
                    );
                    warningTimers[i] -= 60.0f;
                }
            } else {
                warningTimers[i] = 0.0f;
            }
        }

        if (sleepAnimationPlaying) {
            sleepFrameElapsed += elapsedSeconds;
            const int count = pet.species == "Cat"
                ? catSleepFrameCount
                : dogSleepFrameCount;
            while (sleepFrameElapsed >= secondsPerFrame) {
                sleepFrame = (sleepFrame + 1) % count;
                sleepFrameElapsed -= secondsPerFrame;
            }
        } else if (bathingAnimation) {
            bathFrameElapsed += elapsedSeconds;
            const int count = pet.species == "Cat"
                ? catBathFrameCount
                : dogBathFrameCount;
            while (bathFrameElapsed >= secondsPerFrame) {
                bathFrame = (bathFrame + 1) % count;
                bathFrameElapsed -= secondsPerFrame;
            }
        } else if (foodAnimationPlaying) {
            foodFrameElapsed += elapsedSeconds;
            const int count = foodFrameCounts[foodAnimationSpriteIndex];
            while (foodFrameElapsed >= secondsPerFrame) {
                foodFrame = (foodFrame + 1) % count;
                foodFrameElapsed -= secondsPerFrame;
            }
        } else if (playingAnimation) {
            playFrameElapsed += elapsedSeconds;
            const int count = pet.species == "Cat"
                ? catPlayFrameCount
                : dogPlayFrameCount;
            while (playFrameElapsed >= secondsPerFrame) {
                playFrame = (playFrame + 1) % count;
                playFrameElapsed -= secondsPerFrame;
            }
        } else {
            idleFrameElapsed += elapsedSeconds;
            const int count = pet.species == "Cat"
                ? catIdleFrameCount
                : dogIdleFrameCount;
            while (idleFrameElapsed >= secondsPerFrame) {
                idleFrame = (idleFrame + 1) % count;
                idleFrameElapsed -= secondsPerFrame;
            }
        }

        catIdle.setTextureRect(sf::IntRect(
            {(idleFrame % catIdleFrameCount) * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
        dogIdle.setTextureRect(sf::IntRect(
            {(idleFrame % dogIdleFrameCount) * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
        catSleep.setTextureRect(sf::IntRect(
            {(sleepFrame % catSleepFrameCount) * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
        dogSleep.setTextureRect(sf::IntRect(
            {(sleepFrame % dogSleepFrameCount) * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
        catPlay.setTextureRect(sf::IntRect(
            {(playFrame % catPlayFrameCount) * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
        dogPlay.setTextureRect(sf::IntRect(
            {(playFrame % dogPlayFrameCount) * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
        catBath.setTextureRect(sf::IntRect(
            {bathFrame * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
        dogBath.setTextureRect(sf::IntRect(
            {bathFrame * frameWidth, 0},
            {frameWidth, frameHeight}
        ));

        for (std::size_t i = 0; i < foodSprites.size(); ++i) {
            const int frame =
                foodAnimationPlaying &&
                static_cast<int>(i) == foodAnimationSpriteIndex
                    ? foodFrame
                    : 0;

            foodSprites[i]->setTextureRect(sf::IntRect(
                {frame * frameWidth, 0},
                {frameWidth, frameHeight}
            ));
        }

        constexpr float buttonPopDuration = 0.20f;
        for (std::size_t i = 0; i < actionButtons.size(); ++i) {
            if (buttonPopActive[i]) {
                buttonPopElapsed[i] += elapsedSeconds;

                if (buttonPopElapsed[i] >= buttonPopDuration) {
                    buttonPopActive[i] = false;
                    actionButtons[i]->setScale(buttonBaseScales[i]);
                } else {
                    const float progress =
                        buttonPopElapsed[i] / buttonPopDuration;
                    const float popScale =
                        1.0f + 0.12f * std::sin(3.14159f * progress);

                    actionButtons[i]->setScale({
                        buttonBaseScales[i].x * popScale,
                        buttonBaseScales[i].y * popScale
                    });
                }
            }
        }

        if (notificationVisible) {
            notificationElapsed += elapsedSeconds;
            constexpr float notificationDuration = 3.0f;
            constexpr float fadeStartTime = 2.2f;

            if (notificationElapsed >= notificationDuration) {
                notificationVisible = false;
            } else {
                float opacity = 1.0f;
                if (notificationElapsed > fadeStartTime) {
                    opacity =
                        (notificationDuration - notificationElapsed) /
                        (notificationDuration - fadeStartTime);
                }

                const auto textAlpha = static_cast<std::uint8_t>(
                    255.0f * opacity
                );
                const auto backgroundAlpha = static_cast<std::uint8_t>(
                    230.0f * opacity
                );

                notificationText.setFillColor(
                    sf::Color(42, 22, 125, textAlpha)
                );
                notificationText.setOutlineColor(
                    sf::Color(255, 255, 255, textAlpha)
                );
                notificationBackground.setFillColor(
                    sf::Color(255, 235, 250, backgroundAlpha)
                );
                notificationBackground.setOutlineColor(
                    sf::Color(42, 22, 125, textAlpha)
                );
            }
        }

petNameDisplay.setString(pet.name);
petNameDisplay.setCharacterSize(18);

sf::FloatRect petNameBounds = petNameDisplay.getLocalBounds();
while (petNameBounds.size.x > 205.0f &&
       petNameDisplay.getCharacterSize() > 12) {
    petNameDisplay.setCharacterSize(
        petNameDisplay.getCharacterSize() - 1
    );
    petNameBounds = petNameDisplay.getLocalBounds();
}

petNameDisplay.setOrigin({
    petNameBounds.position.x + petNameBounds.size.x / 2.0f,
    petNameBounds.position.y + petNameBounds.size.y / 2.0f
});
petNameDisplay.setPosition({274.0f, 462.0f});

const float dividerWidth =
    std::clamp(petNameBounds.size.x + 12.0f, 60.0f, 190.0f);
petNameDivider.setSize({dividerWidth, 2.0f});
petNameDivider.setPosition({
    274.0f - dividerWidth / 2.0f,
    474.0f
});

petSpeciesDisplay.setString(
    pet.species == "Cat" ? "CAT NAME" : "DOG NAME"
);
petSpeciesDisplay.setCharacterSize(11);

const sf::FloatRect petSpeciesBounds =
    petSpeciesDisplay.getLocalBounds();
petSpeciesDisplay.setOrigin({
    petSpeciesBounds.position.x + petSpeciesBounds.size.x / 2.0f,
    petSpeciesBounds.position.y + petSpeciesBounds.size.y / 2.0f
});
petSpeciesDisplay.setPosition({274.0f, 486.0f});

        window.clear();
        window.draw(background);
        window.draw(petNameDisplay);
        window.draw(petNameDivider);
        window.draw(petSpeciesDisplay);

        for (sf::Sprite* button : actionButtons) {
            window.draw(*button);
        }

        for (std::size_t i = 0; i < barTracks.size(); ++i) {
            window.draw(barTracks[i]);
            window.draw(barFills[i]);
            window.draw(*barLabels[i]);
        }

        if (pet.species == "Cat") {
            if (pet.sleeping || sleepAnimationPlaying) {
                window.draw(catSleep);
            } else if (bathingAnimation) {
                window.draw(catBath);
            } else if (foodAnimationPlaying) {
                window.draw(*foodSprites[foodAnimationSpriteIndex]);
            } else if (playingAnimation) {
                window.draw(catPlay);
            } else {
                window.draw(catIdle);
            }
        } else {
            if (pet.sleeping || sleepAnimationPlaying) {
                window.draw(dogSleep);
            } else if (bathingAnimation) {
                window.draw(dogBath);
            } else if (foodAnimationPlaying) {
                window.draw(*foodSprites[foodAnimationSpriteIndex]);
            } else if (playingAnimation) {
                window.draw(dogPlay);
            } else {
                window.draw(dogIdle);
            }
        }

        window.draw(helpButton);
window.draw(helpQuestionMark);
window.draw(homeButton);
window.draw(homeButtonText);
window.draw(saveButton);
window.draw(saveButtonText);

        if (notificationVisible) {
            window.draw(notificationBackground);
            window.draw(notificationText);
        }

        if (guideOpen) {
            window.draw(guideDimmer);
            window.draw(guidePanel);
            window.draw(guideTitle);
            window.draw(nourishmentHeading);
            window.draw(nourishmentInfo);
            window.draw(energyHeading);
            window.draw(energyInfo);
            window.draw(funHeading);
            window.draw(funInfo);
            window.draw(cleanlinessHeading);
            window.draw(cleanlinessInfo);
            window.draw(guideCloseButton);
            window.draw(guideCloseText);
        }

        if (exitPromptOpen) {
            window.draw(modalDimmer);
            window.draw(modalPanel);
            modalText.setString("Save your progress before exiting?");
            centerText(modalText, 640.0f, 320.0f);
            window.draw(modalText);

            exitSaveButton.setPosition({385.0f, 390.0f});
            exitNoSaveButton.setPosition({575.0f, 390.0f});
            exitCancelButton.setPosition({765.0f, 390.0f});

            window.draw(exitSaveButton);
            window.draw(exitNoSaveButton);
            window.draw(exitCancelButton);

            centerText(exitSaveText, 472.5f, 414.0f);
            centerText(exitNoSaveText, 662.5f, 414.0f);
            centerText(exitCancelText, 825.0f, 414.0f);

            window.draw(exitSaveText);
            window.draw(exitNoSaveText);
            window.draw(exitCancelText);
        }

        window.display();
    }

    stopActionSound();
    backgroundMusic.stop();
    return 0;
}