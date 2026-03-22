#include "GameScene.h"
#include "../FrameworkCore/SceneManager.h"
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/MovementComponent.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "../FrameworkCore/TileMapComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"
#include "GhostAIComponent.h"
#include "../FrameworkCore/Utilitys.h"
#include "TileMap.h"
#include "PacMan.h"
#include <memory.h>
#include <string>
#include "GameOverScene.h"

GameScene::GameScene(SceneManager& sceneManager, sf::RenderWindow& window) :
	IScene(sceneManager, window)
	, mScoreText(mFont)
	, mLivesText(mFont)
{
    mGhostAISystem = std::make_unique<GhostAISystem>();
    mTileCollisionSystem = std::make_unique<TileCollisionSystem>();
    mCollisionSystem = std::make_unique<PacmanCollisionSystem>();
    mRenderSystem = std::make_unique<RenderSystem>();
    mInputSystem = std::make_unique<InputSystem>();

    mCollisionSystem->SetGhostAISystem(mGhostAISystem.get());

    mCollisionSystem->OnPacmanKilled = [this]() { KillPacman();  };
    mCollisionSystem->OnGhostEaten = [this](Node*) {
		mScore += GHOST_SCORE;
		};

    mModManager.LoadMod("SpeedBoost.dll");
}

GameScene::~GameScene()
{
	for (Node* node : mEntities) delete node;
}

void GameScene::OnEnter()
{
    mFont.openFromFile("ARCADE_N.ttf");

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(18);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setPosition({ 8.f, 4.f });

    mLivesText.setFont(mFont);
    mLivesText.setCharacterSize(18);
    mLivesText.setFillColor(sf::Color::White);
    mLivesText.setPosition({ 8.f, 24.f });

    mScore = 0;
    mLives = 3;
    mGameActive = true;
    ResetEntities();
    UpdateHUD();
}

void GameScene::OnExit()
{

    mGhostAISystem->mNodes.clear();
    mGhostAISystem->SetPacman(nullptr);
    mGhostAISystem->SetBlinky(nullptr);
    mGhostAISystem->SetTileMap(nullptr);
    mTileCollisionSystem->mNodes.clear();
    mTileCollisionSystem->SetTileMap(nullptr);
    mCollisionSystem->mNodes.clear();
    mRenderSystem->mNodes.clear();
    mInputSystem->mNodes.clear();

    mEntities.clear();
    mPacmanNode = nullptr;
    mBlinkyNode = nullptr;
    mTileMapNode = nullptr;
}

void GameScene::ResetEntities()
{

    mGhostAISystem->mNodes.clear();
    mGhostAISystem->SetPacman(nullptr);
    mGhostAISystem->SetBlinky(nullptr);
    mGhostAISystem->SetTileMap(nullptr);
    mTileCollisionSystem->mNodes.clear();
    mTileCollisionSystem->SetTileMap(nullptr);
    mCollisionSystem->mNodes.clear();
    mRenderSystem->mNodes.clear();
    mInputSystem->mNodes.clear();

    mEntities.clear();
    mPacmanNode = nullptr;
    mBlinkyNode = nullptr;
    mTileMapNode = nullptr;

    BuildMap();
    BuildPacman();
    BuildGhost(GhostType::Blinky);
    BuildGhost(GhostType::Pinky);
    BuildGhost(GhostType::Inky);
    BuildGhost(GhostType::Clyde);


    mGhostAISystem->SetPacman(mPacmanNode->GetNode().get());
    mGhostAISystem->SetBlinky(mBlinkyNode);
    mGhostAISystem->SetTileMap(mTileMapNode->GetNode()->GetComponent<TileMapComponent>());

    mPelletsLeft = 0;
    mPacmanDead = false;
    mDeathTimer = 0.f;
}


void GameScene::BuildMap()
{
    std::array<std::string, 21> m_map = {
" ################### ",
" #........#........# ",
" #o##.###.#.###.##o# ",
" #.................# ",
" #.##.#.#####.#.##.# ",
" #....#...#...#....# ",
" ####.### # ###.#### ",
"    #.#   0   #.#    ",
"#####.# ##=## #.#####",
"     .  #123#  .     ",
"#####.# ##### #.#####",
"    #.#       #.#    ",
" ####.# ##### #.#### ",
" #........#........# ",
" #.##.###.#.###.##.# ",
" #o.#.....P.....#.o# ",
" ##.#.#.#####.#.#.## ",
" #....#...#...#....# ",
" #.######.#.######.# ",
" #.................# ",
" ################### "
    };
    
    mTileMapNode = new TileMap();
    auto gameMap = mTileMapNode->ParseMapToGame(m_map);
    mTileMapNode->Initialize(gameMap);

    mEntities.push_back(mTileMapNode->GetNode().get());

    mTileCollisionSystem->SetTileMap(mTileMapNode->mTileMapComponent);
}

void GameScene::BuildPacman()
{
    mPacmanNode = new PacMan();
    mEntities.push_back(mPacmanNode->GetNode().get());
    auto startPosition = mTileMapNode->mPlayerStartposition;
    mPacmanNode->GetNode()->AddComponent(mTileMapNode->GetNode()->GetComponent<TileMapComponent>());
    mPacmanNode->SetPosition(startPosition.x, startPosition.y);

    mTileCollisionSystem->AddNode(mPacmanNode->GetNode());
    mCollisionSystem->AddNode(mPacmanNode->GetNode());
    mRenderSystem->AddNode(mPacmanNode->GetNode());
    mInputSystem->AddNode(mPacmanNode->GetNode());

    mCtx.pacmanMovement = mPacmanNode->GetNode()->GetComponent<MovementComponent>();
    mCtx.ghostAISystem = mGhostAISystem.get();
    mCtx.lives = &mLives;
    mCtx.score = &mScore;
}

void GameScene::BuildGhost(GhostType type)
{
    std::shared_ptr<Node> node = std::make_shared<Node>();
    auto* ai = new GhostAIComponent(type);
    auto* transform = new TransformComponent(ai->mPosition.x, ai->mPosition.y);
    auto* movement = new MovementComponent();
    auto* graphics = new GraphicsComponent();
    movement->speed = ai->mSpeed;
    auto* collider = new ColliderComponent(1.f, 1.f, 14.f, 14.f);
    collider->isTrigger = true;

    graphics->mShape.setRadius(8.f);
    switch (type)
    {
    case GhostType::Blinky:
        graphics->mShape.setFillColor(sf::Color::Red);
        break;
    case GhostType::Pinky:
        graphics->mShape.setFillColor(sf::Color::Magenta);
        break;
    case GhostType::Inky:
        graphics->mShape.setFillColor(sf::Color::Blue);
        break;
    case GhostType::Clyde:
        graphics->mShape.setFillColor(sf::Color::Green);
        break;
    default:
        break;
    }
    

    node->AddComponent(graphics);
    node->AddComponent(ai);
    node->AddComponent(transform);
    node->AddComponent(movement);
    node->AddComponent(collider);

    mEntities.push_back(node.get());
    mRenderSystem->mNodes.push_back(node);
    mGhostAISystem->mNodes.push_back(node);
    mCollisionSystem->mNodes.push_back(node);

    if (type == GhostType::Blinky) mBlinkyNode = node.get();
}

void GameScene::HandleEvent(const sf::Event& event)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Escape)
        {
            mSceneManager.PushScene(SCENE_TYPE::PAUSE);
        }
        switch (key->code)
        {
        case sf::Keyboard::Key::F1 : 
            mModManager.ToogleMod(0, mCtx); 
            break;
        default:
            break;
        }

        mPacmanNode->InputEvent(key->code, true);
    }
    else if (event.is<sf::Event::Closed>())
    {
        mWindow.close();
    }

    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        mPacmanNode->InputEvent(keyReleased->code, false);
    }
}

void GameScene::Update(float deltaTime)
{
    if (!mGameActive) return;

    if (mPacmanDead)
    {
        mDeathTimer -= deltaTime;
        if (mDeathTimer <= 0.f)
        {
            if (mLives <= 0)
            {
                auto* goScene = dynamic_cast<GameOverScene*>(
                    mSceneManager.GetScene(SCENE_TYPE::GAMEOVER));
                if (goScene) { goScene->SetScore(mScore); goScene->SetWon(false); }
                mSceneManager.ChangeScene(SCENE_TYPE::GAMEOVER);
                return;
            }
            ResetEntities();
            UpdateHUD();
        }
        return;
    }

    mInputSystem->Update(deltaTime);
    mGhostAISystem->Update(deltaTime);
    mCollisionSystem->Update(deltaTime, mEntities);
    mRenderSystem->Update(deltaTime);
    mTileCollisionSystem->Update(deltaTime);
    mModManager.Update(deltaTime, mCtx);
    CheckPacmanPickups();
    CheckWin();
    UpdateHUD();
}

void GameScene::CheckPacmanPickups()
{
    if (!mPacmanNode) return;
    auto* transform = mPacmanNode->GetNode()->GetComponent<TransformComponent>();
    if (!transform) return;

    const sf::Vector2f center = transform->position + sf::Vector2f(7.f, 7.f);
    Cell consumed = Cell::Empty;

    if (mTileMapNode->TryConsumePickup(center, consumed))
    {
        switch (consumed)
        {
        case Cell::Pellet:
            mScore += PELLET_SCORE;
            --mPelletsLeft;
            break;
        case Cell::Energizer:
            mScore += ENERGIZER_SCORE;
            --mPelletsLeft;
            mGhostAISystem->TriggerFrightened(FRIGHTENED_DUR);
            break;
        default: break;
        }
    }
}

void GameScene::KillPacman()
{
    if (mPacmanDead) return;
    mPacmanDead = true;
    mDeathTimer = DEATH_DELAY;
    --mLives;
    if (auto* m = mPacmanNode->GetNode()->GetComponent<MovementComponent>())
        m->velocity = { 0.f, 0.f };
}

void GameScene::CheckWin()
{
    if (mPelletsLeft > 0) return;
}

void GameScene::UpdateHUD()
{
    mScoreText.setString("SCORE: " + std::to_string(mScore));
    mLivesText.setString("LIVES: " + std::to_string(mLives));
}

void GameScene::Render()
{
    mWindow.clear(sf::Color::Black);

    if (mTileMapNode)
    {
        mTileMapNode->Draw(mWindow);
    }
    
    mRenderSystem->Render(mWindow);
    mWindow.draw(mScoreText);
    mWindow.draw(mLivesText);

    mWindow.display();
}
