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
    M3DS::Sprite2D* sprite = viewport->emplaceChild<M3DS::Sprite2D>();

    // Create a spritesheet with 2 columns and 2 rows
    sprite->spritesheet = M3DS::SpriteSheet{ texture.value(), { 2, 2 } };

    // Create a camera centred on (0, 0)
    M3DS::Camera2D* camera = viewport->emplaceChild<M3DS::Camera2D>();
    camera->enable();

    std::uint32_t counter {};

    root.mainLoop([&](const float delta) {
        if (M3DS::Input::isKeyPressed(M3DS::Input::Key::start))
            root.exit();

        // Change the sprite's frame every second
        if (++counter == 60) {
            counter = 0;
            sprite->frame = (sprite->frame + 1) % 4;
        }

        root.treeInput();
        root.treeUpdate(delta);
        root.treeDraw(M3DS::Draw::draw_2d);
    });
}
