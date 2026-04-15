#include "TutorialUI.hpp"
#include "GameStateManager.hpp"
#include "EntityManager.hpp"

TutorialUI::TutorialUI(sf::Font& font, sf::Font& tutorialTextFont, sf::RenderWindow& window)
    : nextButton({100.f, 50.f}, {300.f, 500.f}, "Next", font),
      skipButton({100.f, 50.f}, {450.f, 500.f}, "Skip", font),
      tutorialText(tutorialTextFont), enemySpawned(false)
{
    screenWidth = (float)window.getSize().x;
    screenHeight = (float)window.getSize().y;

    tutorialText.setCharacterSize(24);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setStyle(sf::Text::Bold);
    auto bounds = tutorialText.getLocalBounds();
    tutorialText.setOrigin( {bounds.size.x/2, bounds.size.y/2} );
    tutorialText.setPosition( {50.f, screenHeight*2/3} );
    
    nextButton.setPosition( {screenWidth/2 - 100.f, screenHeight*7/8} );
    skipButton.setPosition( {screenWidth/2 + 100.f, screenHeight*7/8} );

    controlStep();
}

void TutorialUI::update(float dt, sf::RenderWindow& window) {
    bool nextHover = nextButton.isMouseOver(window);
    bool skipHover = skipButton.isMouseOver(window);
    nextButton.setHover(nextHover);
    skipButton.setHover(skipHover);

    if (nextButton.isClicked(window)) {
        currentStep++;
    }

    if (currentTutorialPhase == 0) {
        controlStep();
    }
    else if (currentTutorialPhase == 1) {
        buildStep();
    }
    else if (currentTutorialPhase == 2) {
        enemyStep();
    }
    else {
        finished = true;
        GameStateManager::getInstance().setState(GameStateManager::State::Playing);
    }

    if (skipButton.isClicked(window)) {
        finished = true;
        GameStateManager::getInstance().setState(GameStateManager::State::Playing);
    }
}

void TutorialUI::controlStep() {
    switch(currentStep) {
        case 0:
            // tutorialText.setString("Welcome to Onslaught!\n"
            //                         "This tutorial will teach you how to play!\n"
            //                         "Press Next for the next step, or skip to start the game.");
            tutorialText.setString(L"Onslaught!へようこそ！\n"
                                    L"このチュートリアルではこのゲームの遊び方を教えるよ！\n"
                                    L"Nextを押したら次に、Skip押したらゲームスタートできる。");
            break;
        case 1:
            // tutorialText.setString("Use WASD to move.");
            tutorialText.setString(L"まずは、WASDキーで移動できる");
            break;
        case 2:
            // tutorialText.setString("Hold Left Shift while moving to sprint.\n"
                                    // "This uses stamina, located at the top left of the screen");
            tutorialText.setString(L"移動中にLeft Shiftを押すとダッシュも可能！\n"
                                    L"ダッシュはスタミナを使用するから注意。\n"
                                    L"画面左上の緑バーが残りスタミナだよ。");
            break;
        case 3:
            // tutorialText.setString("Press Spacebar or Left Mouse Button to attack.\n"
                                    // "This also uGses some stamina.");
            tutorialText.setString(L"スペースキーか左マウスボタンで攻撃できる。\n"
                                    L"これもスタミナを消費するよ。");
            break;
        default:
            currentStep = 0;
            currentTutorialPhase = 1;
            break;
    }
}

void TutorialUI::buildStep() {
    switch(currentStep) {
        case 0:
            // tutorialText.setString("The objective is to survive the onslaught of enemies.\n"
                                    // "You must also protect the town.");
            tutorialText.setString(L"このゲームの目標は敵の猛攻撃に生き残ること。\n"
                                    L"同時に町も敵から守らないといけない。");
            break;
        case 1:
            // tutorialText.setString("To help the defence, you can build towers.");
            tutorialText.setString(L"防御を強化するためにタワーを建設することができるよ！");
            break;
        case 2:
            // tutorialText.setString("Switch to build mode by clicking 'E'.");
            tutorialText.setString(L"Eキーを押したら建築モードに切り替えられる。\n"
                                    L"一回やってみよう。（ビルドモードに切り替え）");
            nextButton.setActive(false);

            if(EntityManager::getInstance().isBuildMode()) currentStep = 3;
            break;
        case 3:
            // tutorialText.setString("Point the mouse to an open area.\n"
                                    // "A green rectangle should appear at the pointer.\n"
                                    // "Click the Left Mouse Button to build an Archer Tower.");
            tutorialText.setString(L"カーソルを空いているエリアに移動しよう。\n"
                                    L"カーソルの位置に緑の四角形が表示されるはず。\n"
                                    L"空いてる所に左マウスボタンをクリックしてみよう。");
            if(EntityManager::getInstance().doesBuildingExist()) {
                currentStep = 4;
            }
            break;
        case 4:
            // tutorialText.setString("Great! Switch back to normal mode by clicking 'E' again.");
            tutorialText.setString(L"これでアーチャータワーを建築できた！\n"
                                    L"Eキーをもう一度押して通常モードに戻ろう。");
            if(!EntityManager::getInstance().isBuildMode()) 
            {
                nextButton.setActive(true);
                currentStep = 5;
            }
            break;
        case 5:
            // tutorialText.setString("The tower will attack nearby enemies for you!");
            tutorialText.setString(L"アーチャータワーは近くの敵を攻撃するよ。");
            break;
        case 6:
            // tutorialText.setString("Building towers requires materials.");
            tutorialText.setString(L"ちなみにタワーの建設には素材が必要。");
            break;
        case 7:
            // tutorialText.setString("Materials can be gathered by destroying objects.");
            tutorialText.setString(L"素材はフィールド上のオブジェクトを\n"
                                    L"破壊することで入手できる。");
            break;
        case 8:
            // tutorialText.setString("Objects can be destroyed using your attack.\n"
                                    // "Spacebar/Left Mouse Button -> Attack");
            tutorialText.setString(L"オブジェクトを攻撃をすることで破壊できるよ！\n"
                                    L"スペースキー/マウスの左ボタン -> 攻撃");
            break;
        case 9:
            // tutorialText.setString("You can check the materials you currently have\n"
                                    // "by clicking Open Inventory at the top left of the screen.");
            tutorialText.setString(L"画面の左上にある\"Open Inventory\"をクリックしよう。\n"
                                    L"現在持っている素材を確認できる。");
            break;
        case 10:
            // tutorialText.setString("For reference, Archer Tower requires 2 wood and 2 stone.");
            tutorialText.setString(L"ちなみに、アーチャータワーの建築は木2個、石2個が必要。");
            break;
        default:
            currentStep = 0;
            currentTutorialPhase = 2;
            break;
    }
}

void TutorialUI::enemyStep() {
    EntityManager& entityManager = EntityManager::getInstance();
    switch(currentStep) {
        case 0:
            // tutorialText.setString("Next, lets try some combat!");
            tutorialText.setString(L"次に、戦闘を試してみよう！");
            break;
        case 1:
            // tutorialText.setString("Enemies will spawn randomly and head\n"
                                    // "towards you when nearby or the town.\n"
                                    // "You take damage when they collide!");
            tutorialText.setString(L"敵はランダムにスポーンする。\n"
                                    L"プレイヤーか町に向かって移動してくるよ。\n"
                                    L"当たるとダメージを喰らうので注意！");
            break;
        case 2:
            // tutorialText.setString("You can see the your health at the top left\n"
                                    // "of the screen! (Red bar above stamina)"
                                    // "You lose when the bar becomes empty!");
            tutorialText.setString(L"プレイヤーのHPは画面の左上に表示されている。\n"
                                    L"上に体力（赤）、その下にスタミナ（緑）だよ！\n"
                                    L"HPが空になるとゲームオーバー。");
            break;
        case 3:
            // tutorialText.setString("An enemy has spawned! Try defeating it!\n"
                                    // "Spacebar/Left Mouse Button -> Attack");
            tutorialText.setString(L"敵がスポーンした。倒してみよう！\n"
                                    L"スペースキー/マウスの左ボタン -> 攻撃");
            nextButton.setActive(false);

            if(!enemySpawned && !entityManager.doesEnemyExist()) {
                sf::Vector2f pos = entityManager.getPlayerPos();
                pos.y += 200.f;
                entityManager.spawnEnemy( pos );
                enemySpawned = true;
            }
            else if( !entityManager.doesEnemyExist() ) {
                currentStep = 4;
            }
            break;

        case 4:
            // tutorialText.setString("The defeated enemy has dropped experience!\n"
                                    // "Approach it to pick it up! ");
            tutorialText.setString(L"倒した敵が経験値をドロップした。\n"
                                    L"落ちた経験値に近づくと拾える。\n"
                                    L" -> 経験値に近づく");

            if(!entityManager.doesExperienceExist()) {
                nextButton.setActive(true);
                currentStep = 5;
            }
            break;

        case 5:
            // tutorialText.setString("Picking enough experience up allows you to level up.\n"
                                    // "Defeat enemies to become stronger!");
            tutorialText.setString(L"経験値を一定以上集めるとレベルアップできる。\n"
                                    L"積極的に敵を倒してレベルアップを目指そう！");
            break;
        case 6:
            // tutorialText.setString("That ends the tutorial. GLHF!");
            tutorialText.setString(L"これでチュートリアルは終了だよ。\n"
                                    L"それでは、Good luck have fun！");
            break;
        default:
            currentStep = 0;
            currentTutorialPhase = 3;
            break;
    }
}



void TutorialUI::render(sf::RenderWindow& window) {
    window.draw(tutorialText);
    nextButton.render(window);
    skipButton.render(window);
}

bool TutorialUI::isFinished() const {
    return finished;
}