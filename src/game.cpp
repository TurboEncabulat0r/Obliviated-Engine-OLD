#include "game.h"



namespace game {


    class TPlayer : public engine::GameObject {
    public:
        engine::Rigidbody* rb;
        TPlayer(float x, float y, float size, std::string name, engine::Texture t) : engine::GameObject(x, y, size, name, t) {
            //engine::Rigidbody* rb = new engine::Rigidbody(this);
            rb = addComponent<engine::Rigidbody>();
        }

        void move(float x, float y) {
            

        }

        void jump(float j) {
            rb->vely = j * 3;

        }

        void Update() {
            if (input::getKeyDown('D')) {
                rb->addVelocity(1, 0);
            }
            if (input::getKeyDown('A')) {
                rb->addVelocity(-1, 0);
            }
        }




    };

    class Player : public engine::GameObject{
    public:
        float yvel = 0;
        float xvel = 0;
        engine::Texture t1;
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

        float swapTs = 0;
        bool swapped = false;
        void update() {
            yvel += 9.81f * engine::getDeltaTime();


            y += yvel;
            x += xvel;

            if (y > 500) {
                y = 500;
                if (yvel > 4) {
                    if (!swapped) {
                        swapped = true;
                        engine::Texture temp;
                        temp = texture;

                        texture = t1;
                        t1 = temp;
                        swapTs = engine::getTime() + 0.5f;
                    }
                }

                yvel = -yvel * 0.9;

            }

            if (swapped && swapTs < engine::getTime()) {
                engine::Texture temp;
                temp = texture;
                
                texture = t1;
                t1 = temp;
                swapped = false;
                
            }

            yvel *= 0.999;
            xvel *= 0.984;
        }

        Player(float x, float y, float size, std::string name, engine::Texture t) : engine::GameObject(x, y, size, name, t) {
            this->x = x;
            this->y = y;
            this->size = size;
            this->name = name;
            this->texture = t;
            this->serializeValue("vel x", &xvel);
            this->serializeValue("vel y", &yvel);
        }
    };

    engine::GameObject* background;
    TPlayer* p;

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
            p->jump(-20);
        }
    }

    void init() {
        input::onKeyPress(*keyPress);
        engine::onUpdate(*update);
        
        background = new engine::GameObject(350, 350, 700, "Background", engine::Texture());
        p = new TPlayer(0, 0, 200, "Fred", engine::Texture());
        engine::loadTexture("fred_texture", L"textures/cat.png", &p->texture);
        engine::loadTexture("shitty_trees", L"textures/tree.png", &background->texture);
        //engine::loadTexture("fred_eye_closed", L"textures/cat_closed.png", &p->t1);



    }

}
