#include "GameScene.h"
#include "../FrameworkCore/SceneManager.h"
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/MovementComponent.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "../FrameworkCore/TileMapComponent.h"
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

    mCollisionSystem->SetGhostAISystem(mGhostAISystem.get());

    mCollisionSystem->OnPacmanKilled = [this]() { KillPacman();  };
    mCollisionSystem->OnGhostEaten = [this](Node*) {
		mScore += GHOST_SCORE;
		};
}

GameScene::~GameScene()
{
	for (Node* node : mEntities) delete node;
}

void GameScene::OnEnter()
{
    mFont.openFromFile("assets/fonts/arcade.ttf");

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
    mTileCollisionSystem->mNodes.clear();
    mCollisionSystem->mNodes.clear();


    mTileCollisionSystem->SetTileMap(nullptr);

    for (Node* n : mEntities) delete n;
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

    for (Node* n : mEntities) delete n;
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
    auto* tileMap = mTileMapNode->GetNode()->GetComponent<TileMapComponent>();
    if (tileMap)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
            for (int y = 0; y < MAP_HEIGHT; ++y)
                if (tileMap->mMap[x][y] == Cell::Pellet ||
                    tileMap->mMap[x][y] == Cell::Energizer)
                    ++mPelletsLeft;
    }

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

    TileMap tileMap;
    TileMap tileMap;
    auto gameMap = tileMap.ParseMapToGame(m_map);
    tileMap.Initialize(gameMap);

    mEntities.push_back(tileMap.GetNode().get());
    mTileMapNode = &tileMap;

    mTileCollisionSystem->SetTileMap(tileMap.mTileMapComponent);
}

void GameScene::BuildPacman()
{
    PacMan player;

    auto* transform = new TransformComponent(13.f * TILE_SIZE, 23.f * TILE_SIZE);
    auto* movement = new MovementComponent();
    movement->speed = 100.f;
    auto* collider = new ColliderComponent(1.f, 1.f, 14.f, 14.f);

    player.GetNode()->AddComponent(transform);
    player.GetNode()->AddComponent(movement);
    player.GetNode()->AddComponent(collider);


    mEntities.push_back(player.GetNode().get());
    mPacmanNode = &player;

    mTileCollisionSystem->mNodes.push_back(player.GetNode());
    mCollisionSystem->mNodes.push_back(player.GetNode());
    mRenderSystem->AddNode(player.GetNode());
}

void GameScene::BuildGhost(GhostType type)
{
    std::shared_ptr<Node> node = std::make_shared<Node>();
    auto* ai = new GhostAIComponent(type);
    auto* transform = new TransformComponent(ai->mPosition.x, ai->mPosition.y);
    auto* movement = new MovementComponent();
    movement->speed = ai->mSpeed;
    auto* collider = new ColliderComponent(1.f, 1.f, 14.f, 14.f);
    collider->isTrigger = true;

    node->AddComponent(ai);
    node->AddComponent(transform);
    node->AddComponent(movement);
    node->AddComponent(collider);

    mEntities.push_back(node.get());
    mGhostAISystem->mNodes.push_back(node);
    mCollisionSystem->mNodes.push_back(node);

    if (type == GhostType::Blinky) mBlinkyNode = node.get();
}

void GameScene::HandleEvent(const sf::Event& event)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
        if (key->code == sf::Keyboard::Key::Escape)
            mSceneManager.PushScene(SCENE_TYPE::PAUSE);
}

void GameScene::HandlePacmanInput()
{
    //if (!mPacmanNode || mPacmanDead) return;
    //auto* movement = mPacmanNode->GetComponent<MovementComponent>();
    //if (!movement) return;

    //const float spd = movement->speed;
    //sf::Vector2f vel = { 0.f, 0.f };

    //using Key = sf::Keyboard::Key;
    //if (sf::Keyboard::isKeyPressed(Key::Left) || sf::Keyboard::isKeyPressed(Key::A)) vel = { -spd, 0.f };
    //else if (sf::Keyboard::isKeyPressed(Key::Right) || sf::Keyboard::isKeyPressed(Key::D)) vel = { spd, 0.f };
    //else if (sf::Keyboard::isKeyPressed(Key::Up) || sf::Keyboard::isKeyPressed(Key::W)) vel = { 0.f, -spd };
    //else if (sf::Keyboard::isKeyPressed(Key::Down) || sf::Keyboard::isKeyPressed(Key::S)) vel = { 0.f,  spd };

    //movement->velocity = vel;
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

    HandlePacmanInput();
    mTileCollisionSystem->Update(deltaTime);
    mGhostAISystem->Update(deltaTime);
    mCollisionSystem->Update(deltaTime, mEntities);
    mRenderSystem->Update(0.f);
    CheckPacmanPickups();
    CheckWin();
    UpdateHUD();
}

void GameScene::CheckPacmanPickups()
{
    if (!mPacmanNode) return;
    auto* transform = mPacmanNode->GetNode()->GetComponent<TransformComponent>();
    auto* tileMap = mTileMapNode->GetNode()->GetComponent<TileMapComponent>();
    if (!transform || !tileMap) return;

    const sf::Vector2f center = transform->position + sf::Vector2f(7.f, 7.f);
    Cell consumed = Cell::Empty;

    if (tileMap->TryConsumePickup(center, consumed))
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
    
    //if (mTileMapNode)
    //{
    //    auto* tileMap = mTileMapNode->GetComponent<TileMapComponent>();
    //    if (tileMap && tileMap->mTexture)
    //        mWindow.draw(tileMap->mVertexArray, tileMap->mTexture);
    //}

    // TODO: draw sprites for Pac-Man and ghosts via a GraphicsComponent/RenderSystem
    
    mRenderSystem->Render(mWindow);
    mWindow.draw(mScoreText);
    mWindow.draw(mLivesText);

    mWindow.display();
}
