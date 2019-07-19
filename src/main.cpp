#include "Engine.cpp"
#include "Graphics.cpp"
#include "GameState.cpp"
#include "Transform.cpp"
#include <algorithm>

class Background : public Aspen::Object::Object
{
 Aspen::Graphics::Sprite *sprite; 
public:
  Background(Object *parent = nullptr, std::string name = "Background") : Aspen::Object::Object(parent, name)
  {
    sprite = new Aspen::Graphics::Sprite("./resources/background.png", this, "Background");
    AddChild(sprite);
    CreateChild<Aspen::Transform::Transform>();
    GetTransform()->SetPosition(640,310);
  }


};

class Platform : public Aspen::Object::Object
{
 Aspen::Graphics::Sprite *sprite; 
public:
  Platform(Object *parent = nullptr, std::string name = "Platform") : Aspen::Object::Object(parent, name)
  {
    sprite = new Aspen::Graphics::Sprite("./resources/platform.png", this, "Platform");
    AddChild(sprite);
    CreateChild<Aspen::Transform::Transform>();
    GetTransform()->SetPosition(640,850);
    CreateChild<Aspen::Physics::AABBCollider>()->SetSize(540, 30);

  }


};

class Platform2 : public Aspen::Object::Object
{
 Aspen::Graphics::Sprite *sprite; 
public:
  Platform2(Object *parent = nullptr, std::string name = "Platform2") : Aspen::Object::Object(parent, name)
  {
    sprite = new Aspen::Graphics::Sprite("./resources/platform.png", this, "Platform2");
    AddChild(sprite);
    CreateChild<Aspen::Transform::Transform>();
    GetTransform()->SetPosition(800,1000);
    CreateChild<Aspen::Physics::AABBCollider>()->SetSize(1280, 30);

  }


};

class Wall : public Aspen::Object::Object
{
 Aspen::Graphics::Sprite *sprite; 
public:
  Wall(Object *parent = nullptr, std::string name = "Wall") : Aspen::Object::Object(parent, name)
  {
    sprite = new Aspen::Graphics::Sprite("./resources/wall.png", this, "Platform");
    AddChild(sprite);
    CreateChild<Aspen::Transform::Transform>();
    GetTransform()->SetPosition(640,630);
    CreateChild<Aspen::Physics::AABBCollider>()->SetSize(16, 160);

  }


};

class Player : public Aspen::Object::Object
{
  Aspen::Graphics::Animation *sprite1;
  Aspen::Graphics::Animation *sprite2;
 
  bool onGround = false;
  Aspen::Graphics::Camera *cam;

public:
  Player(Object *parent = nullptr, std::string name = "Player") : Aspen::Object::Object(parent, name)
  {
    //sprites/animation
     sprite1 = new Aspen::Graphics::Animation(
      new Aspen::Graphics::UniformSpritesheet("./resources/baloonidiot.png", 160, 160, 7, nullptr, "BaloonSpritesheet"),
      1.0f / 7.0f, this, "Baloon");
    AddChild(sprite1);
      sprite2 = new Aspen::Graphics::Animation(
      new Aspen::Graphics::UniformSpritesheet("./resources/baloonmoving.png", 160, 160, 5, nullptr, "BaloonSpritesheet2"),
      1.0f / 5.0f, this, "Baloon2");
    AddChild(sprite2);
    
    sprite2->Deactivate();

    CreateChild<Aspen::Transform::Transform>();
    CreateChild<Aspen::Physics::Rigidbody>();
    CreateChild<Aspen::Physics::AABBCollider>()->SetSize(160,160);
    GetTransform()->SetPosition(300,300);

  }
  void OnStart()
  {
    cam = FindAncestorOfType<Aspen::GameState::GameState>()->CreateChild<Aspen::Graphics::Camera>();
    cam->SelectCamera();
  }
  void OnUpdate()
  { 
    float cx = GetTransform()->GetXPosition()
      - 1280/ 2.0f;

    float cy = GetTransform()->GetYPosition()
      - 720/ 2.0f;

    double xv = GetRigidbody()->GetVelocityX();
    double yv = GetRigidbody()->GetVelocityY();
    cam->GetTransform()->SetPosition(cx, cy);
    
    if (Aspen::Input::KeyHeld(SDLK_d) && !Aspen::Input::KeyHeld(SDLK_a))
    {
      Aspen::Log::Info("D is held");
      sprite1->Deactivate();
      sprite2->Activate();
      sprite2->GetTransform()->SetXScale(1);
      if (onGround == false)
      { 
        xv = 10;
      }
      else
      {
        xv = 9;
      } 
    }
    else if (Aspen::Input::KeyHeld(SDLK_a) && !Aspen::Input::KeyHeld(SDLK_d))
    {
      Aspen::Log::Info("A is held");
      sprite1->Deactivate();
      sprite2->Activate();
      sprite2->GetTransform()->SetXScale(-1);
      if (onGround == false)
      {
        xv = -10;
      }
      else
      {
        xv = -9;
      }
    }
    else
    {
      sprite1->Activate();
      sprite2->Deactivate();
    }
    
    
    if (GetTransform()->GetYPosition() > 1500)
    {
      GetTransform()->SetYPosition(400); 
      yv = 0;
      onGround = true;
    }
    if (Aspen::Input::KeyHeld(SDLK_SPACE) && onGround == true) 
    {
      Aspen::Log::Info("Space is held");
      yv = -40;
    }

    GetRigidbody()->SetCartesianVelocity(xv, yv);
    onGround = false;
  }

  void OnCollision(Aspen::Physics::Collision c)
  {
    if (c.collisionAngle < M_PI / 2.0f + 0.1f && c.collisionAngle > M_PI / 2.0f - 0.1f)
    {
      onGround = true;
    }
  }
};

class State : public Aspen::GameState::GameState
{
  Aspen::Graphics::UI::Text *title;
  Player character; 
  
public:
  State(Object *parent = nullptr, std::string name = "State") : GameState(parent, name)
  {
    /*title = new Aspen::Graphics::UI::Text("Aweseom gmae!", "default" , 64, this, "Title");
    AddChild(title);
    title->GetTransform()->SetPosition(310, 140);
    title->GetTransform()->SetRotation(-0.069f);
    title->GetTransform()->SetScale(1, 1);*/

    CreateChild<Background>();
    CreateChild<Player>();
    CreateChild<Platform>();
    CreateChild<Platform2>();
    CreateChild<Wall>();
  }
};

int main(int argc, char **argv)
{
  Aspen::Log::Log::SetFile("./Aspen.log");

  Aspen::Engine::Engine engine(Aspen::Engine::START_FLAGS::ALL ^ (
    Aspen::Engine::START_FLAGS::CREATE_GRAPHICS |
    Aspen::Engine::START_FLAGS::CREATE_GRAPHICS_DEBUGGER |
    Aspen::Engine::START_FLAGS::CREATE_GRAPHICS_FONTCACHE
  ));
  Aspen::Graphics::Graphics *gfx = new Aspen::Graphics::Graphics(1280, 720, &engine, "Graphics");
  gfx->CreateChild<Aspen::Debug::Debug>();
  gfx->CreateChild<Aspen::Graphics::FontCache>();
  engine.AddChild(gfx);

  engine.FindChildOfType<Aspen::Physics::Physics>()->SetGravityStrength(0.80f);
  engine.FindChildOfType<Aspen::Physics::Physics>()->SetDrag(0.1);
  engine.FindChildOfType<Aspen::Time::Time>()->TargetFramerate(60);
  engine.FindChildOfType<Aspen::Graphics::Graphics>()->FindChildOfType<Aspen::Graphics::FontCache>()->LoadFont("resources/ABeeZee-Regular.ttf", "default");

  engine.FindChildOfType<Aspen::GameState::GameStateManager>()->LoadState<State>(true);
  //engine.FindChildOfType<GameStateManager>()->LoadState<Game>(false);

    while (engine)
        engine();
    return 0;
}
