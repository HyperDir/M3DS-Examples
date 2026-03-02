#include <m3ds/M3DS.hpp>

// Create a custom script based on Node
class MyScript : public M3DS::Script<M3DS::Node> {
public:
    // Called when script is attached to Node
    void ready() override {
        // Base node can be accessed with base()
        M3DS::Debug::log("Attached MyScript to Node: {}!", base()->getClass());
    }

    // Called every frame
    void update(M3DS::Seconds<float> delta) override {
        M3DS::Debug::log("Frame Time: {}", delta);
    }

    // Draw calls
    void draw([[maybe_unused]] const M3DS::RenderTarget2D& target) override {}
    void draw([[maybe_unused]] const M3DS::RenderTarget3D& target) override {}

    // Called on input events
    void input(M3DS::Input::InputFrame& inputFrame) override {
        M3DS::Debug::log("Left Joy: {:.4}", inputFrame.getLeftJoy());
    }

    // Custom functions can be added
    void myFunc() const {
        M3DS::Debug::log("myFunc()");
    }
};

int main() {
    M3DS::Init _ {};

    M3DS::Root root {};

    // Instantiate script (creates Node child and attaches script automatically)
    MyScript* script = root.emplaceChild<MyScript>();
    script->myFunc();

    root.mainLoop();
}
