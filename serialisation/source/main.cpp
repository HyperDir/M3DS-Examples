#include <m3ds/M3DS.hpp>

// Create a custom node to connect signal to
class MyNode : public M3DS::Node {
    M_CLASS(MyNode, M3DS::Node)

    int count {};
public:
    // Create a method to attach the signal to
    void print() noexcept {
        M3DS::Debug::log("Signal connection retained!");
        getRoot()->printTree();
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

    {
        // Set up a 2 second repeating timer
        M3DS::Timer timer {};
        timer.oneShot = false;
        timer.duration = 2.f;
        timer.start();

        // Create an instance of MyNode as a child of timer
        [[maybe_unused]] MyNode* myNode = timer.emplaceChild<MyNode>();

        if (const M3DS::Failure failure = timer.timeout.connect(*myNode, "print"))
            M3DS::Debug::terminate(failure);

        // Serialisation includes signal connections
        // Fails if the node is not reachable within the same tree
        if (const M3DS::Failure failure = M3DS::Registry::serialise(timer, "sdmc:/3ds/TimerAndMyNode.mscn"))
            M3DS::Debug::terminate(failure);
    }

    // Deserialisation will restore state of all nodes, including signal connections
    std::expected exp = M3DS::Registry::deserialise<M3DS::Timer>("sdmc:/3ds/TimerAndMyNode.mscn");
    if (!exp) 
        M3DS::Debug::terminate(exp.error());

    // Add the deserialised scene to the scene tree
    root.emplaceChild(std::move(exp.value()));
    
    root.mainLoop();
}
