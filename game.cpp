#include "game.h"
#include "engine.h"
#include "input.h"
#include "renderer.h"
#include "gameObject.h"
using namespace engine;

class engine::GameObject;
namespace game {


    

    class Player : public GameObject{
    public:
        float size = 200;
        Texture tex;

        float yvel = 0;
        float xvel = 0;
        void move(float _x, float _y) {
            this->xvel += _x;
            this->yvel += _y;

            if (xvel > 100) {
                xvel = 100;
            }
            else if(xvel < -100) {
                xvel = -100;
            }
        }

        void jump(float j) {
            yvel = j;
        }

        void update() {
            yvel += 9.81f * engine::getDeltaTime();


            y += yvel;
            x += xvel;

            if (y > 500) {
                y = 500;
                yvel = -yvel * 0.9;
            }

            yvel *= 0.999;
            xvel *= 0.984;
        }

        Player(float x, float y, float size, std::string name, Texture tex) : GameObject(x, y, size, name, tex) {
            this->x = x;
            this->y = y;
            this->size = size;
            this->name = name;
            this->tex = tex;
            this->serializeValue("vel x", &xvel);
        }

        void draw() {
            engine::drawTexturedSquare(x, y, size, 0xffffff, tex);
        }
    };

    GameObject* background;
    Player p = Player(0, 0, 200, "Fred", NULL);

    void update() {
        if (input::getKeyDown('D'))
            p.move(10 * engine::getDeltaTime(), 0);
        if (input::getKeyDown('A'))
            p.move(-10 * engine::getDeltaTime(), 0);


        p.update();
    }

    // called every frame
    void render() {
        p.draw();
        
    }

    // called on key press
    void keyPress(char k) {
        if (k == VK_SPACE) {    
            p.jump(-4);
        }
    }

    void init() {
        input::onKeyPress(*keyPress);
        engine::onRender(*render);
        engine::onUpdate(*update);
        
        background = new GameObject(350, 350, 700, "Background", NULL);
        engine::loadTexture(L"textures/cat.png", &p.tex);
        loadTexture(L"textures/tree.png", &background->tex);




    }

}
