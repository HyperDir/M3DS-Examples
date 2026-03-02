#include <m3ds/M3DS.hpp>

int main() {
    // Initialises systems, sets bottom screen as console (configurable)
    M3DS::Init _ {};

    // Create a root node
    M3DS::Root root {};
    // Create a viewport for the top screen
    M3DS::Viewport* viewport = root.emplaceChild<M3DS::Viewport>(M3DS::Screen::top);

    
    // Load a texture, and terminate on failure
    // Textures from the gfx folder are built into romfs:/gfx/*.t3x at compile-time
    std::expected texture = M3DS::Texture::load("romfs:/gfx/texture.png.t3x");
    if (!texture)
        M3DS::Debug::terminate(texture.error());

    // Create a sprite to display the texture
    M3DS::Sprite3D* sprite = viewport->emplaceChild<M3DS::Sprite3D>();

    // Create a spritesheet with 2 columns and 2 rows
    sprite->spritesheet = M3DS::SpriteSheet{ texture.value(), { 2, 2 } };
    sprite->setTranslation({ 0.f, 0.f, -5.f });
    sprite->billboard = true;

    M3DS::Light3D* light = viewport->emplaceChild<M3DS::Light3D>();
    light->setTranslation({ 0.f, 0.f, -4.f });
    light->enable();

    // Create a camera centred on (0, 0)
    M3DS::Camera3D* camera = viewport->emplaceChild<M3DS::Camera3D>();
    camera->enable();
    camera->setRotation(M3DS::Quaternion::fromAxisAngle({1.f, 0.f, 0.f}, std::numbers::pi_v<float> / -3.f)); 

    std::uint32_t counter {};

    M3DS::Vector3 translation {};

    float yaw {};
    float pitch {};

    // Begin main loop
    root.mainLoop([&](const float delta) {
        if (M3DS::Input::isKeyPressed(M3DS::Input::Key::start))
            root.exit();

        // Change the sprite's frame every second
        if (++counter == 60) {
            counter = 0;
            sprite->frame = (sprite->frame + 1) % 4;
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
