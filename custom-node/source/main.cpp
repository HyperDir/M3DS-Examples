#include <m3ds/M3DS.hpp>

class MyNode : public M3DS::Node {
    M_CLASS(MyNode, M3DS::Node);
};

M3DS::Failure MyNode::serialise(M3DS::Serialiser& serialiser) const noexcept {
    return SuperType::serialise(serialiser);
}

M3DS::Failure MyNode::deserialise(M3DS::Deserialiser& deserialiser) noexcept {
    return SuperType::deserialise(deserialiser);
}

const M3DS::GenericMember* MyNode::getMemberStatic(const std::string_view name) noexcept {
    return SuperType::getMemberStatic(name);
}

M3DS::BoundMethodPair MyNode::getMethodStatic(const std::string_view name) noexcept {
    return SuperType::getMethodStatic(name);
}

int main() {
    M3DS::Init _ {};
    // Register MyNode in the Registry.
    M3DS::Registry::registerType<MyNode>();

    M3DS::Root root {};
    root.emplaceChild<MyNode>();

    root.mainLoop();
}
