#include <m3ds/M3DS.hpp>

// Create a custom node to connect signal to
class MyNode : public M3DS::Node {
    M_CLASS(MyNode, M3DS::Node)

    int count {};
public:
    // Create a method to attach the signal to
    void print() noexcept {
        M3DS::Debug::log("Run {} times.", ++count);
    }
};

// Default serialisation
M3DS::Failure MyNode::serialise(M3DS::Serialiser& serialiser) const noexcept {
    return SuperType::serialise(serialiser);
}

M3DS::Failure MyNode::deserialise(M3DS::Deserialiser& deserialiser) noexcept {
    return SuperType::deserialise(deserialiser);
}

REGISTER_NO_MEMBERS(MyNode)

// Register method so it can be called by signals
// Currently clangd complains about this, but it compiles properly.
REGISTER_METHODS(
    MyNode,

    MUTABLE_METHOD(print)
)

int main() {
    M3DS::Init _ {};

    // Register MyNode to the Registry (perform *after* Init, as Init clears Registry)
    M3DS::Registry::registerType<MyNode>();

    M3DS::Root root {};

    // Set up a 2 second repeating timer
    M3DS::Timer* timer = root.emplaceChild<M3DS::Timer>();
    timer->oneShot = false;
    timer->duration = 2.f;
    timer->start();

    // Create an instance of MyNode
    [[maybe_unused]] MyNode* myNode = root.emplaceChild<MyNode>();

    // Connect the timer's 'timeout' signal to MyNode::print()
    // Signal connections can be formed between any two nodes
    // However if a connected node is unreachable in the scene tree from another, serialisation will fail if attempted
    if (const M3DS::Failure failure = timer->timeout.connect(*myNode, "print"))
        M3DS::Debug::terminate(failure);
    
    root.mainLoop();
}
