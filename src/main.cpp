#include "Engine.cpp"
#include "Graphics.cpp"
#include "GameState.cpp"
#include "Transform.cpp"

class Platform : public Aspen::Object::Object
{
 Aspen::Graphics::Animation *sprite; 
public:
  Platform(Object *parent = nullptr, std::string name = "Platform") : Aspen::Object::Object(parent, name)
  {
    sprite = new Aspen::Graphics::Animation(
      new Aspen::Graphics::UniformSpritesheet("./resources/platform.png", 800, 50, 6, nullptr, "BaloonSpritesheet"),
      1.0f / 0.0f, this, "Baloon");
    AddChild(sprite);
    CreateChild<Aspen::Transform::Transform>();
    GetTransform()->SetPosition(300,300);
    CreateChild<Aspen::Physics::AABBCollider>()->SetSize(800, 30);

  }


};

class Player : public Aspen::Object::Object
{
  Aspen::Graphics::Animation *sprite1;
  Aspen::Graphics::Animation *sprite2;
 
  bool onGround = false;

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
  
  void OnUpdate()
  {
    double xv = GetRigidbody()->GetVelocityX();
    double yv = GetRigidbody()->GetVelocityY();

    if (Aspen::Input::KeyHeld(SDLK_d) && !Aspen::Input::KeyHeld(SDLK_a))
    {
      Aspen::Log::Info("D is held");
      sprite1->Deactivate();
      sprite2->Activate();
      sprite2->GetTransform()->SetXScale(1);
      if (onGround == false)
      { 
        xv = 7;
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
        xv = -7;
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
    
    
    if (GetTransform()->GetYPosition() > 400)
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

class MainMenu : public Aspen::GameState::GameState
{
  Aspen::Graphics::UI::Text *title;
  Player character; 
  
public:
  MainMenu(Object *parent = nullptr, std::string name = "MainMenu") : GameState(parent, name)
  {
    title = new Aspen::Graphics::UI::Text("Aweseom gmae!", "default" , 64, this, "Title");
    AddChild(title);
    title->GetTransform()->SetPosition(310, 140);
    title->GetTransform()->SetRotation(-0.069f);
    title->GetTransform()->SetScale(1, 1);

    CreateChild<Player>();
    CreateChild<Platform>();
  }
};

int main(int argc, char **argv)
{
  Aspen::Log::Log::SetFile("./Aspen.log");

  Aspen::Engine::Engine engine(Aspen::Engine::START_FLAGS::ALL);
  engine.FindChildOfType<Aspen::Physics::Physics>()->SetGravityStrength(1);
  engine.FindChildOfType<Aspen::Physics::Physics>()->SetDrag(0.1);
  engine.FindChildOfType<Aspen::Time::Time>()->TargetFramerate(60);
  engine.FindChildOfType<Aspen::Graphics::Graphics>()->FindChildOfType<Aspen::Graphics::FontCache>()->LoadFont("resources/ABeeZee-Regular.ttf", "default");

  engine.FindChildOfType<Aspen::GameState::GameStateManager>()->LoadState<MainMenu>(true);
  //engine.FindChildOfType<GameStateManager>()->LoadState<Game>(false);

    while (engine)
        engine();
    return 0;
}
