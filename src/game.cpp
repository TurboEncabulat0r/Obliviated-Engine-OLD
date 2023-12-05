#include "game.h"



namespace game {


    class Player : public engine::GameObject {
    public:
        engine::Rigidbody* rb;
        Player(float x, float y, float size, std::string name, engine::Texture t) : engine::GameObject(x, y, size, name, t) {
            //engine::Rigidbody* rb = new engine::Rigidbody(this);
            rb = addComponent<engine::Rigidbody>();
        }

        void move(float x, float y) {
            

        }

        void jump(float j) {
            rb->vely = -j;

        }

        void Update() {
            if (input::getKeyDown('D')) {
                rb->addVelocity(20, 0);
            }
            if (input::getKeyDown('A')) {
                rb->addVelocity(-20, 0);
            }
        }
    };

    engine::GameObject* background;
    Player* p;

    void update() {
        if (input::getKeyDown('D'))
            p->move(10 * engine::getDeltaTime(), 0);
        if (input::getKeyDown('A'))
            p->move(-10 * engine::getDeltaTime(), 0);


        p->update();
    }

    // called on key press
    void keyPress(char k) {
        if (k == VK_SPACE) {    
            p->jump(-5);
        }
    }

    void init() {
        input::onKeyPress(*keyPress);
        engine::onUpdate(*update);
        
        background = new engine::GameObject(350, -350, 700, "Background", engine::Texture());
        p = new Player(0, 0, 200, "Fred", engine::Texture());
        engine::loadTexture("fred_texture", L"textures/cat.png", &p->texture);
        engine::loadTexture("shitty_trees", L"textures/tree.png", &background->texture);
        //engine::loadTexture("fred_eye_closed", L"textures/cat_closed.png", &p->t1);



    }

}
