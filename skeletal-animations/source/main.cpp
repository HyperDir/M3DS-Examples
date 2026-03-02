#include <m3ds/M3DS.hpp>

int main() {
    // Initialise systems, sets bottom screen as console (configurable)
    M3DS::Init _ {};

    // Create a root node
    M3DS::Root root {};
    // Create a viewport for the top screen
    M3DS::Viewport* topScreen = root.emplaceChild<M3DS::Viewport>(M3DS::Screen::top);

    // Create a light for the scene
    M3DS::Light3D* light = topScreen->emplaceChild<M3DS::Light3D>();
    light->setTranslation(M3DS::Vector3{-1, 2, 0});
    light->colour = { 0.5f, 0.5f, 0.5f };
    light->enable();

    // Create a camera for the scene. Camera forward direction is -z by default
    M3DS::Camera3D* camera = topScreen->emplaceChild<M3DS::Camera3D>();
    camera->enable();

    // Load a 3D model (file converted from glTF using M3DSModelConverter)
    // https://github.com/HyperDir/M3DS-Model-Converter
    std::expected mesh = M3DS::Mesh::load("romfs:/GDBot.mod3ds");
    if (!mesh)
        M3DS::Debug::terminate(mesh.error());

    // Create a MeshInstance using the mesh loaded in the previous step
    M3DS::MeshInstance* meshInstance = topScreen->emplaceChild<M3DS::MeshInstance>(mesh.value());
    meshInstance->setTranslation(M3DS::Vector3{ 0.f, -1.f, -3.f });

    const std::size_t animationCount = meshInstance->getAnimationCount();
    if (animationCount == 0) {
        M3DS::Debug::warn("Model has no animations!");
    } else {
        M3DS::Debug::log("Playing animation {}", 0);
        meshInstance->playAnimationIdx(0);
    }

    std::size_t idx {};

    M3DS::Vector3 translation {};

    float yaw {};
    float pitch {};

    // Begin main loop
    root.mainLoop([&](const M3DS::Seconds<float> delta) {
        if (M3DS::Input::isKeyPressed(M3DS::Input::Key::start))
            root.exit();

        if (M3DS::Input::isKeyPressed(M3DS::Input::Key::y) && animationCount != 0) {
            idx = (idx + 1) % animationCount;
            M3DS::Debug::log("Playing animation {}", idx);
            meshInstance->playAnimationIdx(idx);
        }

        // Get directional input from circle pad and c-stick (if available)
        const M3DS::Vector2 joy = M3DS::Input::getLeftJoy() + M3DS::Input::getRightJoy();
        yaw += joy.x * delta;
        pitch += joy.y * delta;
        
        // Set the camera rotation based on the input
        const M3DS::Quaternion lookDirection = M3DS::Quaternion::fromAxisAngle({0, 1, 0}, -yaw) * M3DS::Quaternion::fromAxisAngle({1, 0, 0}, -pitch);
        camera->setRotation(lookDirection);

        // Get directional input from d-pad
        const M3DS::Vector2 movement = M3DS::Input::getVector().rotated(yaw);
        translation.x += movement.x * delta;
        translation.z += movement.y * delta;

        const float vertical = M3DS::Input::getAxis(M3DS::Input::Key::b, M3DS::Input::Key::a);
        translation.y += vertical * delta;

        camera->setTranslation(translation);

        root.treeInput();
        root.treeUpdate(delta);
        root.treeDraw(M3DS::Draw::draw_3d);
    });
}
